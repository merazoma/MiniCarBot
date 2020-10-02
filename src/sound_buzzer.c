#include "sound_buzzer.h"
#include "iodefine.h"

#define TimerCLK 31250
const unsigned int MaxFreq = TimerCLK / 2;  //TimerCLK / 2 = 15.6 kHz
const unsigned int MinFreq = TimerCLK / (1 << 8);  //TimerCLK / 2**8 = 122 Hz

const unsigned short NOTE_C4 = 262;
const unsigned short NOTE_D4 = 294;
const unsigned short NOTE_E4 = 330;
const unsigned short NOTE_F4 = 349;
const unsigned short NOTE_G4 = 392;
const unsigned short NOTE_A4 = 440;
const unsigned short NOTE_B4 = 494;
const unsigned short NOTE_C5 = 523;
const unsigned short NOTE_C6 = 1047;

static init_8bit_timer();

void init_buzzer() {
    PORT3.PMR.BIT.B2    = 0x1;      //P32 を周辺機能に設定
    //MPUレジスタ設定
	MPC.PWPR.BIT.B0WI	= 0;
	MPC.PWPR.BIT.PFSWE	= 1;
	MPC.P32PFS.BYTE		= 0x5; 		//MPC設定 P32 TMO3
    init_8bit_timer();
}


static init_8bit_timer() {
    MSTP_TMR3           = 0x0;     //消費電力低減機能をOFF
    TMR3.TCCR.BIT.CSS   = 0x1;     //PCLKをクロックソース
    TMR3.TCCR.BIT.CKS   = 0x5;     //PCLK/1024 = 31.25 kHz
    TMR3.TCR.BIT.CCLR   = 0x1;     //TCORAとのコンペアマッチでTCNTクリア
    TMR3.TCSR.BIT.OSA   = 0x1;     //TCORAとのコンペアマッチでLow出力
    TMR3.TCSR.BIT.OSB   = 0x2;     //TCORBとのコンペアマッチでHigh出力
    stop_buzzer();
}

void sound_buzzer(unsigned short freq) {
    unsigned char tcora;
    if(freq > MaxFreq) {
        tcora = 2;
    } else if (freq < MinFreq) {
        tcora = 0xff;
    } else {
        tcora = TimerCLK / freq - 1;
    }
    TMR3.TCORA = tcora;
    TMR3.TCORB = tcora / 2;     //Duty比 50 %
}

void stop_buzzer() {
    TMR3.TCORA = 100;   //適当な値
    TMR3.TCORB = 101;   //TCORAより大きい適当な値
}