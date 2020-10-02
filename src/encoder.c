#include "encoder.h"
#include "iodefine.h"

//!エンコーダカウントの初期値
#define InitialEncCnt (10000)

void init_encoder() {
    PORTA.PMR.BIT.B1    = 0x1;      //PA1 を周辺機能に設定
    PORTA.PMR.BIT.B3    = 0x1;      //PA3 を周辺機能に設定
    PORTA.PMR.BIT.B4    = 0x1;      //PA4 を周辺機能に設定
    PORTA.PMR.BIT.B6    = 0x1;      //PA6 を周辺機能に設定
    //MPUレジスタ設定
	MPC.PWPR.BIT.B0WI	= 0;
	MPC.PWPR.BIT.PFSWE	= 1;
	MPC.PA1PFS.BYTE		= 0x2; 		//MPC設定 PA1 MTCLKC
	MPC.PA3PFS.BYTE		= 0x2; 		//MPC設定 PA3 MTCLKD
	MPC.PA4PFS.BYTE		= 0x2; 		//MPC設定 PA4 MTCLKA
	MPC.PA6PFS.BYTE		= 0x2; 		//MPC設定 PA6 MTCLKB
    MSTP_MTU1           = 0x0;      //消費電力低減機能をOFF
    MSTP_MTU2           = 0x0;      //消費電力低減機能をOFF
    MTU1.TMDR.BIT.MD    = 0x4;      //位相計数モード1
    MTU2.TMDR.BIT.MD    = 0x4;      //位相計数モード1
    MTU.TSTR.BIT.CST1   = 0x1;      //MTU1タイマ起動
    MTU.TSTR.BIT.CST2   = 0x1;      //MTU2タイマ起動
}

unsigned short get_enc_count(encoder_id_t encoder_id) {
    unsigned short tcnt_enc;
    switch (encoder_id)
    {
    case ENCODER_LEFT:
        tcnt_enc = MTU2.TCNT;
        break;
    case ENCODER_RIGHT:
        tcnt_enc = MTU1.TCNT;
        break;
    default:
        break;
    }
    return tcnt_enc;
}

short get_enc_count_dif(encoder_id_t encoder_id) {
    unsigned short tcnt_enc;
    short tcnt_enc_dif;
    switch (encoder_id)
    {
    case ENCODER_LEFT:
        tcnt_enc = get_enc_count(ENCODER_LEFT);
        set_enc_count(ENCODER_LEFT, InitialEncCnt);
        tcnt_enc_dif = tcnt_enc - InitialEncCnt;  
        break;
    case ENCODER_RIGHT:
        tcnt_enc = get_enc_count(ENCODER_RIGHT);
        set_enc_count(ENCODER_RIGHT, InitialEncCnt);
        tcnt_enc_dif = -(tcnt_enc - InitialEncCnt);  
        break;
    default:
        break;
    }
    return tcnt_enc_dif;
};


void set_enc_count(encoder_id_t encoder_id, unsigned short enc_count) {
    switch (encoder_id)
    {
    case ENCODER_LEFT:
        MTU2.TCNT = enc_count;
        break;
    case ENCODER_RIGHT:
        MTU1.TCNT = enc_count;
        break;
    default:
        break;
    }
}
