#include "photo_reflector.h"
#include "iodefine.h"
#include "analog_read.h"

//! フォトトランジスタのAD値から距離への変換用定数\n
//! 超音波センサで取得した距離を使って自動キャリブレーションできるようにしたい
static unsigned short CntToDistanceLeft = 100;
static unsigned short CntToDistanceLeftFront = 100;
static unsigned short CntToDistanceRight = 100;
static unsigned short CntToDistanceRightFront = 100;

static unsigned short ad_rf[2];
static unsigned short ad_rs[2];
static unsigned short ad_lf[2];
static unsigned short ad_ls[2];

//! 500Hzに相当するTGR(2 ms / (1/32MHz) = 64000)
#define TGR500Hz 64000
#define CntADVal 3900

static void init_mtu4();

void init_photo_reflector() {
    init_mtu4();
};

int get_photo_reflector_distance(photo_port_t port) {
    int d;
    d = CntADVal - get_photo_reflector_dif(port);
    if (d < 0) {
        d = 0;
    }
    return d;
}

short get_photo_reflector_dif(photo_port_t port) {
    short ad_dif;
    switch (port)
    {
    case PHOTO_LEFT_FRONT:
        ad_dif = ad_lf[1] - ad_lf[0];
        break;
    case PHOTO_LEFT_SIDE:
        ad_dif = ad_ls[1] - ad_ls[0];
        break;
    case PHOTO_RIGHT_FRONT:
        ad_dif = ad_rf[1] - ad_rf[0];
        break;
    case PHOTO_RIGHT_SIDE:
        ad_dif = ad_rs[1] - ad_rs[0];
        break;
    default:
        break;
    }
    if (ad_dif < 0) {
        ad_dif = -ad_dif;
    }    
    return ad_dif;
}

void get_ad_photo_transitor() {
    ad_lf[1] = ad_lf[0];
    ad_ls[1] = ad_ls[0];
    ad_rf[1] = ad_rf[0];
    ad_rs[1] = ad_rs[0];
    ad_lf[0] = analog_read(SENS_LF);
    ad_ls[0] = analog_read(SENS_LS);
    ad_rf[0] = analog_read(SENS_RF);
    ad_rs[0] = analog_read(SENS_RS);
}    

void init_mtu4() {
    PORTE.PMR.BIT.B1 = 0x1;     //PE1 を周辺機能に設定
    PORTE.PMR.BIT.B2 = 0x1;     //PE2 を周辺機能に設定

    //MPUレジスタ設定
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
    MPC.PE1PFS.BYTE = 0x1;      //MPC設定 PE1 MTIOC4C
    MPC.PE2PFS.BYTE = 0x1;      //MPC設定 PE2 MTIOC4A

    MSTP_MTU            = 0x0;
    MSTP_MTU4           = 0x0;          //消費電力低減機能をOFF
    MTU4.TCR.BIT.TPSC   = 0x0;          //PCLK/1
    MTU4.TCR.BIT.CCLR   = 0x2;          //TGRBのコンペアマッチでTCNTクリア
    MTU4.TMDR.BIT.MD    = 0x2;          //PWMモード1
    MTU4.TIORH.BYTE     = 0x25;         //MTIOC4A初期出力High, コンペアマッチでLow
    MTU4.TIORL.BYTE     = 0x12;         //MTIOC4C初期出力low, コンペアマッチでHigh
    MTU.TOER.BIT.OE4A   = 1;            //MTIOC4A出力許可
    MTU.TOER.BIT.OE4C   = 1;            //MTIOC4C出力許可
    MTU4.TGRB           = TGR500Hz - 1; //周期 500Hz
    MTU4.TGRD           = TGR500Hz - 1; //周期 500Hz
    //MTIOC4AとMTIOC4Cは逆位相
    //MTIOC4AのLED消灯と同時にMTIOC4CのLED点灯しているが、
    //AD取得はLED点灯~消灯の中間で実施しているので干渉はなし
    MTU4.TGRA           = TGR500Hz/2;   
    MTU4.TGRC           = TGR500Hz/2;   
    MTU4.TADCORA        = TGR500Hz/4;   //AD変換開始要求は、LED点灯~消灯の中間
    MTU4.TADCOBRA       = TGR500Hz/4;   //AD変換開始要求は、LED点灯~消灯の中間
    MTU4.TADCORB        = 3*TGR500Hz/4; //AD変換開始要求は、LED点灯~消灯の中間
    MTU4.TADCOBRB       = 3*TGR500Hz/4; //AD変換開始要求は、LED点灯~消灯の中間
    MTU4.TADCR.BIT.ITB3AE = 0;
    MTU4.TADCR.BIT.ITB4VE = 0;
    MTU4.TADCR.BIT.ITA3AE = 0;
    MTU4.TADCR.BIT.ITA4VE = 0;
    MTU4.TADCR.BIT.DT4BE = 0;
    MTU4.TADCR.BIT.DT4AE = 0;
    MTU4.TADCR.BIT.UT4BE = 1;
    MTU4.TADCR.BIT.UT4AE = 1;
    MTU4.TADCR.BIT.BF = 0x0;
    MTU.TSTR.BIT.CST4   = 0x1;          //MTU4タイマ起動
}