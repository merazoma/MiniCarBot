/**
 * @file sound_buzzer.h
 * @author Masaya Kobayashi
 * @brief ブザーを駆動するための周波数可変の矩形波出力
 * @version 0.1
 * @date 2020-09-09
 * @warning P26(TMO0)からP32(TMO3)に変更後、テスト未実施
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __SOUNDBUZZER_H__
#define __SOUNDBUZZER_H__

//! 音階C4の周波数
extern const unsigned short NOTE_C4;
//! 音階D4の周波数
extern const unsigned short NOTE_D4;
//! 音階E4の周波数
extern const unsigned short NOTE_E4;
//! 音階F4の周波数
extern const unsigned short NOTE_F4;
//! 音階G4の周波数
extern const unsigned short NOTE_G4;
//! 音階A4の周波数
extern const unsigned short NOTE_A4;
//! 音階B4の周波数
extern const unsigned short NOTE_B4;
//! 音階C5の周波数
extern const unsigned short NOTE_C5;
//! 音階C6の周波数
extern const unsigned short NOTE_C6;

/**
 * @brief ブザー駆動出力の初期化処理
 * 
 */
void init_buzzer();

/**
 * @brief ブザー駆動用の周波数可変の矩形波出力指示
 * 
 * @param freq 周波数
 */
void sound_buzzer(unsigned short freq);

/**
 * @brief ブザー出力停止指示
 * 
 */
void stop_buzzer();

#endif