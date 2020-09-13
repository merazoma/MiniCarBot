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
    PHOTO_LEFT,
    //! 左前側フォトリフレクタ
    PHOTO_RIGHT_FRONT,
    //! 右側フォトリフレクタ
    PHOTO_RIGHT,
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
int get_photo_reflecor_distance(photo_port_t port);

#endif