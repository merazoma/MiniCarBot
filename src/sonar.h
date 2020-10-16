/**
 * @file sonar.h
 * @brief 超音波センサ(HC-SR04)による距離計測
 * @author Masaya Kobayashi
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __SONAR_H__
#define __SONAR_H__

/**
 * @brief 超音波センサ選択用の型。get_sonar_distanceの引数として使用。
 */
typedef enum enm_sonar_port{
    //! 右前側超音波センサ
    SONAR_RIGHT_FRONT = 0,
    //! 前側超音波センサ
    SONAR_FRONT,
    //! 右側超音波センサ
    SONAR_RIGHT_SIDE
} sonar_port_t;

/**
 * @brief 超音波センサ初期化処理
 * @details 超音波センサのトリガ駆動の開始と、エコーの割込み入力が許可される
 */
void init_sonar();

/**
 * @brief 超音波センサを指定して、その超音波センサで測定した距離を取得
 * @details エコー割り込み入力の立ち下がりエッジで取得されたタイマカウントから、
 * @details 距離を算出し、返却
 * @param port sonar_port_t 超音波センサの選択
 * @return int 超音波センサで測定された距離（LSB: 1 um）
 */
int get_sonar_distance(sonar_port_t port);

/**
 * @brief 超音波センサエコーパルスの立ち上がりと立ち下がりエッジでの
 * @brief タイマカウントを取得する割込みハンドラ\n
 * @param port sonar_port_t 超音波センサの選択
 * @attention IRQ0の割り込み処理であるExcep_IRQ0, 1, 5の内部でコールすること
 */
void excep_sonar_echo(sonar_port_t port);


#endif