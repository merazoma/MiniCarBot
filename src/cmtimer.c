#include "cmtimer.h"
#include "iodefine.h"

/**
 * @brief CMT0（モータ回転数制御用のタイマ）の初期化処理
 * 
 */
static void init_cmt0();

void init_cmtimer() {
    init_cmt0();
}

void init_cmt0() {
    MSTP_CMT0 = 0x0;            //消費電力低減機能をOFF
    CMT0.CMCR.BIT.CKS = 0;      //PCLK/8
    CMT0.CMCR.BIT.CMIE = 1;     //コンペアマッチ割込み(CMIn)を許可      
    CMT0.CMCOR = 4000;          //周期1ms
    IPR(CMT0, CMI0) = 0xff;       //割込み優先順位
    IEN(CMT0, CMI0) = 0x1;        //割込み許可
    IR(CMT0, CMI0) = 0x0;         //割込みフラグクリア
    CMT.CMSTR0.BIT.STR0 = 1;    //CMT0.CMCNTカウンタスタート
}
