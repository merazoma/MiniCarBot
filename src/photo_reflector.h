/**
 * @file photo_reflector.h
 * @author Masaya Kobayashi
 * @brief 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __PHOTO_REFLECTOR_H__
#define __PHOTO_REFLECTOR_H__

/**
 * @brief フォトリフレクタ選択用の型。get_photo_reflecor_distanceの引数として使用。
 */
typedef enum enm_photo_port{
    //! 左前側フォトリフレクタ
    PHOTO_LEFT_FRONT = 0,
    //! 左側フォトリフレクタ
    PHOTO_LEFT_SIDE,
    //! 左前側フォトリフレクタ
    PHOTO_RIGHT_FRONT,
    //! 右側フォトリフレクタ
    PHOTO_RIGHT_SIDE,
} photo_port_t;

/**
 * @brief フォトリフレクタ初期化処理
 * @details フォトリフレクタのLED点滅駆動と、フォトトランジスタのAD取得割り込みが開始
 */
void init_photo_reflector();

/**
 * @brief フォトリフレクタで計測した距離を取得
 * @details LED ON時に取得したフォトトランジスタのAD値と、
 * @details LED OFF時に取得したAD値の差分から距離を算出
 * @param port フォトリフレクタ選択指示
 * @return int フォトリフレクタで計測した距離（LSB: 1 mm）
 */
int get_photo_reflector_distance(photo_port_t port);

/**
 * @brief  * @brief LED点灯時と消灯時の光強度（AD値）差を取得
 * 
 * @param port フォトリフレクタ選択指示
 * @return short LED点灯時と消灯時の光強度（AD値）差
 */
short get_photo_reflector_dif(photo_port_t port);

/**
 * @brief S12ADI0（ADスキャン終了割込み要求時）の割込みハンドラ
 * 
 * @attention Excep_S12AD0_S12ADI0でコールされることを想定
 */
void get_ad_photo_transitor();
#endif