#include "pwm_drive.h"
#include "iodefine.h"
#include "sci.h"

#define TGR1kHz 320
#define Maxppm (10000)

static void init_mtu0();

void init_pwm_drive() {
    PORTB.PMR.BIT.B1    = 0x1;      //PB1 を周辺機能に設定
    PORTB.PMR.BIT.B3    = 0x1;      //PB3 を周辺機能に設定
    //MPUレジスタ設定
	MPC.PWPR.BIT.B0WI	= 0;
	MPC.PWPR.BIT.PFSWE	= 1;
	MPC.PB1PFS.BYTE		= 0x1; 		//MPC設定 PB1 MTIOC0C
	MPC.PB3PFS.BYTE		= 0x1; 		//MPC設定 PB3 MTIOC0A
    init_mtu0();
}

static void init_mtu0() {
    MSTP_MTU0           = 0x0;          //消費電力低減機能をOFF
    MTU0.TCR.BIT.TPSC   = 0x0;          //PCLK/1
    MTU0.TCR.BIT.CCLR   = 0x2;          //TGRBのコンペアマッチでTCNTクリア
    MTU0.TMDR.BIT.MD    = 0x3;          //PWMモード2
    MTU0.TIORH.BYTE     = 0x12;         //MTIOC0A 初期出力Low, コンペアマッチでHigh
    MTU0.TIORL.BYTE     = 0x12;         //MTIOC0C 初期出力Low, コンペアマッチでHigh
    MTU0.TGRB           = TGR1kHz - 1;  //PWM周期 1 kHz
    MTU0.TGRA           = TGR1kHz;      //MTIOC0A 初期Duty 0%
    MTU0.TGRC           = TGR1kHz;      //MTIOC0C 初期Duty 0%
    // MTU0.TIER.BIT.TGIEB = 0x1;          //割込み要求（TGIB）を許可
    MTU.TSTR.BIT.CST0   = 0x1;          //MTU0タイマ起動
}

void drive_pwm(pwm_port_t pwm_port, unsigned short duty) {
    unsigned short tgr;
    if (duty < Maxppm) {
        tgr = (unsigned short)(((Maxppm - duty) * TGR1kHz) / Maxppm - 1);
    } else {
        tgr = 0;  //duty指示値が 100以上 の場合、319/320=99.7%のDuty
    }
    
    switch (pwm_port)
    {
    case M_L_PWM:
        MTU0.TGRC = tgr;
        break;
    case M_R_PWM:
        MTU0.TGRA = tgr;
        break;
    default:
        break;
    }
}
