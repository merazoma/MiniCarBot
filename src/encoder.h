/**
 * @file encoder.h
 * @author Masaya Kobayashi
 * @brief 左モータと右モータのエンコーダカウントを取得
 * @copyright Copyright (c) 2020
 */
#ifndef __ENCODER_H__
#define __ENCODER_H__

/**
 * @brief エンコーダカウント用タイマの初期化処理
 * 
 */
void init_encoder();

/**
 * @brief 左右モータのエンコーダ選択用の型。
 * 
 */
typedef enum enm_encoder_id{
    //! 左モータのエンコーダ
    ENCODER_LEFT = 0,
    //! 右モータのエンコーダ
    ENCODER_RIGHT
} encoder_id_t;

/**
 * @brief エンコーダカウントの取得
 * 
 * @param encoder_id 左右モータのエンコーダ選択
 * @return unsigned short エンコーダカウント
 */
unsigned short get_enc_count(encoder_id_t encoder_id);

/**
 * @brief 前回コール時からのエンコーダカウントの差分を取得
 * 
 * @param encoder_id 左右モータのエンコーダ選択
 * @return short 前回コール時からのエンコーダカウントの差分
 * @note 1 ms周期の割込みハンドラでの使用を想定
 */
short get_enc_count_dif(encoder_id_t encoder_id);

/**
 * @brief エンコーダカウントの初期値を設定
 * 
 * @param encoder_id 左右モータのエンコーダ選択
 * @param enc_count 設定したいエンコーダカウント
 */
void set_enc_count(encoder_id_t encoder_id, unsigned short enc_count);

#endif 