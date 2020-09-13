#include "analog_read.h"
#include "iodefine.h"

void init_adc(void) {
    MSTP_S12AD = 0;                 //消費電力低減機能をOFF
    S12AD.ADCER.BIT.ADRFMT = 0x0;   //ADC Read Date を右詰め
    S12AD.ADCSR.BIT.DBLE   = 0x0;   //ダブルトリガモード非選択
    S12AD.ADCSR.BIT.EXTRG  = 0x0;   //同期トリガを選択
    S12AD.ADCSR.BIT.TRGE   = 0x1;   //トリガ開始許可
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
