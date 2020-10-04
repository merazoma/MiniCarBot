#include "analog_read.h"
#include "iodefine.h"

void init_adc(void) {
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
    MPC.P40PFS.BIT.ASEL = 1;  //P40をADCとして使用
    MPC.P41PFS.BIT.ASEL = 1;  //P41をADCとして使用
    MPC.P42PFS.BIT.ASEL = 1;  //P42をADCとして使用
    MPC.P43PFS.BIT.ASEL = 1;  //P43をADCとして使用
    MPC.P44PFS.BIT.ASEL = 1;  //P44をADCとして使用

    MSTP_S12AD = 0;                 //消費電力低減機能をOFF
    S12AD.ADCER.BIT.ADRFMT = 0x0;   //ADC Read Date を右詰め
    S12AD.ADCSR.BIT.DBLE   = 0x0;   //ダブルトリガモード非選択
    S12AD.ADCSR.BIT.EXTRG  = 0x0;   //同期トリガを選択
    S12AD.ADCSR.BIT.TRGE   = 0x1;   //トリガ開始許可

    S12AD.ADANSA.WORD = 0x000f;     //AN000～AN003までをグループA
    S12AD.ADCSR.BIT.ADCS = 0x0;     //シングルスキャンモード
    S12AD.ADCSR.BIT.ADIE = 0x1;     //スキャン終了後のS12ADIの割込み許可

    S12AD.ADSTRGR.BIT.TRSA = 0x8;   //グループAの起動要因をTRG4ABN
    IPR(S12AD, S12ADI0) = 0xf;      //割込み優先順位
    IEN(S12AD, S12ADI0) = 0x1;      //割込み許可
}


unsigned short analog_read(an_port_t an_port) {
    unsigned short temp_ad;
    switch (an_port)
    {
    case SENS_LF:
        temp_ad = S12AD.ADDR0;
        break;
    case SENS_LS:
        temp_ad = S12AD.ADDR1;
        break;
    case SENS_RF:
        temp_ad = S12AD.ADDR2;
        break;
    case SENS_RS:
        temp_ad = S12AD.ADDR3;
        break;    
    case SENS_BAT:
        temp_ad = S12AD.ADDR4;
        break;    
    default:
        break;
    }
    return temp_ad;
};

unsigned short analog_read_ondemand(an_port_t an_port) {
    unsigned short temp_ad;        
    S12AD.ADANSA.WORD = 1 << an_port;
    S12AD.ADCSR.BIT.ADST = 0x1;         //AD変換開始要求
    while (S12AD.ADCSR.BIT.ADST == 1);  //AD変換完了待ち
    temp_ad = analog_read(an_port);
    return temp_ad;
};
