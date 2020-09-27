/**
 * @file pwm_drive.h
 * @author Masaya Kobayashi
 * @brief 左車輪モータと右車輪モータのPWM駆動
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef __PWMDRIVE_H__
#define __PWMDRIVE_H__

/**
 * @brief PWM駆動ポート選択用の型。drive_pwmの引数として使用。
 * 
 */
typedef enum enm_pwm_port{
    //! 左側車輪モータPWM駆動用ポート
    M_L_PWM = 0,
    //! 左側車輪モータPWM駆動用ポート
    M_R_PWM
} pwm_port_t;

/**
 * @brief PWM駆動用の初期化処理
 * @note 初期化処理では、Dutyは 0 %に設定されるので、drive_pwmをコールするまでPWM駆動はしない
 */
void init_pwm_drive();

/**
 * @brief 選択したPWM駆動用ポートで、Dutyを指定してPWM駆動を実施
 * @param pwm_port PWM駆動ポートの選択
 * @param float unsigned int Duty比（範囲: 0 ~ 100, LSB: 1 %） 
 */
void drive_pwm(pwm_port_t pwm_port, float duty);


#endif