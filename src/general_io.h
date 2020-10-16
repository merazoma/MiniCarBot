/**
 * @file general_io.h
 * @author Masaya Kobayashi
 * @brief 汎用ポートのON/OFF出力 
 * @version 0.1
 * @date 2020-09-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __GENERALIO_H__
#define __GENERALIO_H__

/**
 * @brief 汎用ポート選択用の型。digital_writeの引数として使用。
 * 
 */
typedef enum enm_port{
    //! デバッグ用LED0(P54)
    DBG_LED0,
    //! デバッグ用LED1(P55)
    DBG_LED1,
    //! デバッグ用LED2(PH0)
    DBG_LED2,
    //! モード表示用LED0(P15)
    M_LED0,
    //! モード表示用LED1(P16)
    M_LED1,
    //! 右車輪用モータの正転指示方向(PB5)
    M_R_IN1,
    //! 左車輪用モータの正転指示方向(PB6)
    M_L_IN1,
    //! 右車輪用モータの逆転指示方向(PB7)
    M_R_IN2,
    //! 左車輪用モータの逆転指示方向(PC2)
    M_L_IN2,
    //! モータ駆動開始指示(PC3)
    M_STBY,
    //! 前方超音波センサのエコー入力(PH1)
    ECHO_FRONT,
    //! 右側超音波センサのエコー入力(PH2)
    ECHO_RIGHT_SIDE,
    //! 左側超音波センサのエコー入力(PE5)
    ECHO_RIGHT_FRONT,
    //! モード切り替えスイッチ(PH3)
    MODE_SW
} port_t;

/**
 * @brief 出力のHigh/Low指示。digital_writeの引数の型。
 * 
 */
typedef enum enm_level{
    //! 出力Low
    LOW = 0,
    //! 出力High
    HIGH = 1
} outlevel_t;

/**
 * @brief 汎用出力初期化処理。メイン側の初期化処理中で一回コールすること。
 * 
 */
void init_general_io(void);

/**
 * @brief 汎用出力ポートの出力切り替え指示
 * 
 * @param port 出力切り替えしたい出力ポートの指示
 * @param level 出力のHigh/Low指示
 */
void digital_write(port_t port, outlevel_t level);

/**
 * @brief 汎用入力ポートの取得
 * 
 * @param port 取得したい入力ポートの指示
 * @return unsigned short 入力ポートのHigh/Low
 */
unsigned short digital_read(port_t port);
#endif