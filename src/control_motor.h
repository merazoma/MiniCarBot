/**
 * @file control_motor.h
 * @brief モータ駆動用PWM出力。エンコーダカウント取得。\nこれらを使用した、モータ回転数制御。
 * @note pwm_drive.hに依存
 * @author Masaya Kobayashi
 * @version 0.1
 * @date 2020-09-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __CONTROLMOTOR_H__
#define __CONTROLMOTOR_H__

/**
 * @brief 左右モータ選択用の型。control_motor_Nrpmやdrive_motor_dutyの引数として使用。
 * 
 */
typedef enum enm_motor_id{
    //! 左モータ
    LEFT = 0,
    //! 右モータ
    RIGHT
	
} motor_id_t;

/**
 * @brief モータの正転・逆転選択用の型。drive_motor_dutyの引数で使用。
 * 
 */
typedef enum enm_motor_direction{
    //! モータ正転
	
    FORWARD = 0,
    //! モータ逆転
    BACKWARD
} motor_direction_t;

/**
 * @brief モータICの初期化処理。メイン側の初期化処理中で一回コールすること。
 * @note モータ駆動用のポート初期化処理である、init_pwm_driveもコールすること。
 * 
 */
void init_motor();

/**
 * @brief 与えられた、並進速度(lin_vel)と回転速度(ang_vel)で、車体を制御
 * @details 並進速度(lin_vel)と回転速度(ang_vel)を、左右輪の回転速度に変換し、
 * @details 左右輪の回転数を制御。
 * 
 * @param lin_vel 並進速度（単位: mm/s）
 * @param ang_vel 回転速度（単位: rad/s）
 * @note 内部でcontrol_motor_Nrpmを使用
 * @warning 未実装
 */
void control_motor(float lin_vel, float ang_vel);

/**
 * @brief 左右輪の回転数を指示
 * 
 * @param motor_id 左右輪の指示（LEFT, RIGHT）
 * @param Nrpm 指示回転数(単位: rpm)
 * @note 内部でdrive_motor_dutyを使用
 */
void set_motor_Nrpm_to_control(motor_id_t motor_id, int Nrpm);

/**
 * @brief 左右モータの回転数フィードバッグ制御
 * 
 * @note CMTの1 ms周期割込みに差し込むことを想定
 */
void fb_control_motor_Nrpm();

/**
 * @brief 指示されたDutyと回転の向きで、モータを駆動
 * 
 * @param motor_id 左右輪の指示（LEFT, RIGHT）
 * @param duty unsigned short Duty比（範囲: 0 ~ 1000000, LSB: 1 ppm） 
 * @param direction 回転向き（FORWARD, BACKWARD）
 */
void drive_motor_duty(motor_id_t motor_id, unsigned short duty, motor_direction_t direction);

#endif