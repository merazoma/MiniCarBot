/**
 * @file analog_read.h
 * @brief フォトトランジスタとバッテリ電圧をAD取得
 * @details Flounderでは、AN000～AN004をADCに割り当て、
 * @details フォトトランジスタとバッテリ電圧監視に使用。\n
 * @details AN000～AN003は、フォトトランジスタに割り当てられており、MTUからの
 * @details 1ms周期で「右前+左」と「右+左前」を順に取得する。
 * @author Masaya Kobayashi
 * 
 * @version 0.1
 * @date 2020-09-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __ANALOGREAD_H__
#define __ANALOGREAD_H__

/**
 * @brief ADポート選択用の型。analog_readの引数として使用。
 */
typedef enum enm_an_port{
    //! 左前のフォトトランジスタ(AN000)
    SENS_LF = 0,    
    //! 左のフォトトランジスタ(AN001)
    SENS_LS,        
    //! 右前のフォトトランジスタ(AN002)
    SENS_RF,
    //! 右のフォトトランジスタ(AN003)
    SENS_RS,
    //! バッテリ電圧監視(AN004)
    SENS_BAT
} an_port_t;

/**
 * @brief ADの初期化処理。メイン側の初期化処理中で一回コールすること。
 * 
 */
void init_adc(void);

/**
 * @brief ADモジュールのデータレジスタ（ADDR*）にラッチされたデータを読み出し
 * 
 * @param an_port 読み取りたいADポート選択。（SENS_LF, SENS_LS, SENS_RF, SENS_RS, SENS_BATから選択すること)
 * @return unsigned short ADモジュールのデータレジスタ（ADDR*）から読み取った12bitの値（0～4095）
 * @attention データレジスタ（ADDR*）にラッチされたデータを読み出すだけで、トリガは
 * @attention 別の手段で行う必要あり。ソフトトリガならば、analog_read_ondemandを使用すること。
 */
unsigned short analog_read(an_port_t an_port);

/**
 * @brief ソフトトリガでAD取得を即時実施。その後、データレジスタ（ADDR*）から値を読み取り返却。
 * 
 * @param an_port 読み取りたいADポート選択。（SENS_LF, SENS_LS, SENS_RF, SENS_RS, SENS_BATから選択すること)
 * @return unsigned short ADモジュールのデータレジスタ（ADDR*）から読み取った12bitの値（0～4095）
 * @note 内部でanalog_readを使用
 */
unsigned short analog_read_ondemand(an_port_t an_port);

#endif