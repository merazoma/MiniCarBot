#include "sonar.h"
#include "sci.h"
#include "iodefine.h"
#include <machine.h>

//! タイマカウントから距離への変換用定数 340 m/s * 2 us/cnt / 2 = 340 um/cnt
#define CntToDistance 340
//! トリガパルスとエコーパルスの立ち上がりエッジ間隔に相当
#define OffsetPulse 270

//! 左側超音波センサのエコーON時間
static unsigned short tcnt_echo_on_l = 0;
//! 前側超音波センサのエコーON時間
static unsigned short tcnt_echo_on_f = 0;
//! 右側超音波センサのエコーON時間
static unsigned short tcnt_echo_on_r = 0;

static void init_mtu3();
static void init_irq();

void init_sonar() {
    init_irq();
    init_mtu3();
}

int get_sonar_distance(sonar_port_t port) {
    int distance;
    switch (port)
    {
    case SONAR_LEFT:
        distance = (tcnt_echo_on_l - OffsetPulse) * CntToDistance / 1000;
        break;
    case SONAR_FRONT:
        distance = (tcnt_echo_on_f - OffsetPulse) * CntToDistance / 1000;
        break;
    case SONAR_RIGHT:
        distance = (tcnt_echo_on_r - OffsetPulse) * CntToDistance / 1000 ;
        break;    
    default:
        break;
    }
    return distance;
}

void excep_sonar_echo_left(void) {
    tcnt_echo_on_l = MTU3.TCNT;
}

void excep_sonar_echo_front(void) {
    tcnt_echo_on_f = MTU3.TCNT;
}

void excep_sonar_echo_right(void) {
    tcnt_echo_on_r = MTU3.TCNT;
}


static void init_irq() {
    //割込み禁止
    IEN(ICU, IRQ0) = 0;
    IEN(ICU, IRQ1) = 0;
    IEN(ICU, IRQ5) = 0;
    clrpsw_i();

    //入力設定
    PORTE.PDR.BIT.B5 = 0x0;     //PE5 入力
    PORTH.PDR.BIT.B1 = 0x0;     //PH1 入力
    PORTH.PDR.BIT.B2 = 0x0;     //PH2 入力

    //MPUレジスタ設定
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
    MPC.PE5PFS.BIT.ISEL = 1;  //PE5をIRQ5として使用
    MPC.PH1PFS.BIT.ISEL = 1;  //PH1をIRQ0として使用
    MPC.PH2PFS.BIT.ISEL = 1;  //PH2をIRQ1として使用

    //ディジタルフィルタ禁止
    ICU.IRQFLTE0.BIT.FLTEN0 = 0;
    ICU.IRQFLTE0.BIT.FLTEN1 = 0;
    ICU.IRQFLTE0.BIT.FLTEN5 = 0;

    //ディジタルフィルタサンプリングクロック PCLK/64
    ICU.IRQFLTC0.BIT.FCLKSEL0 = 0x3;
    ICU.IRQFLTC0.BIT.FCLKSEL1 = 0x3;
    ICU.IRQFLTC0.BIT.FCLKSEL5 = 0x3;

    //割込み優先
    IPR(ICU, IRQ0) = 0xff;
    IPR(ICU, IRQ1) = 0xff;
    IPR(ICU, IRQ5) = 0xff;

    //エッジ設定
    ICU.IRQCR[0].BIT.IRQMD = 0x1;   //立ち下がりエッジ
    ICU.IRQCR[1].BIT.IRQMD = 0x1;   //立ち下がりエッジ
    ICU.IRQCR[5].BIT.IRQMD = 0x1;   //立ち下がりエッジ

    //フラグ下げ
    IR(ICU, IRQ0) = 0;
    IR(ICU, IRQ1) = 0;
    IR(ICU, IRQ5) = 0;
 
    //ディジタルフィルタ許可
    ICU.IRQFLTE0.BIT.FLTEN0 = 1;
    ICU.IRQFLTE0.BIT.FLTEN1 = 1;
    ICU.IRQFLTE0.BIT.FLTEN5 = 1;

    //割込み許可
    IEN(ICU, IRQ0) = 1;
    IEN(ICU, IRQ1) = 1;
    IEN(ICU, IRQ5) = 1;
    setpsw_i();
}

static void init_mtu3() {
    PORT1.PMR.BIT.B7 = 0x1;     //P17 を周辺機能に設定

    //MPUレジスタ設定
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
    MPC.P17PFS.BYTE = 0x1;      //MPC設定 P17 MTIOC3A

    MSTP_MTU            = 0x0;
    MSTP_MTU3           = 0x0;          //消費電力低減機能をOFF
    MTU.TSYR.BIT.SYNC3  = 0x0;          //MTU3.TCNTは独立動作
    MTU3.TCR.BIT.TPSC   = 0x3;          //PCLK/64
    MTU3.TCR.BIT.CCLR   = 0x2;          //TGRBのコンペアマッチでTCNTクリア
    MTU3.TMDR.BIT.MD    = 0x2;          //PWMモード1
    MTU.TOER.BIT.OE3B   = 0;            //MTIOC3B出力禁止
    MTU.TOER.BIT.OE3D   = 0;            //MTIOC3D出力禁止
    MTU3.TIORH.BYTE     = 0x12;         //初期出力Low, コンペアマッチでHigh（ハイサイド駆動で反転するため）
    MTU3.TGRB           = 32000 - 1;    //周期 64 ms
    MTU3.TGRA           = 5 - 1;        //10 us のHighトリガ
    // MTU3.TIER.BIT.TGIEB = 0x1;          //割込み要求（TGIB）を許可
    MTU.TSTR.BIT.CST3   = 0x1;          //MTU3タイマ起動
}