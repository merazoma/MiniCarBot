/**
 * @file sonar.h
 * @brief 超音波センサ(HC-SR04)による距離計測
 * @details 前側、右側と左側の超音波センサ兼用のトリガパルス（周期: 64ms, パルス幅: 10us）
 * @details をMTU3で生成し出力\n
 * @details 前側、右側と左側の超音波センサのエコーパルス入力は、それぞれ外部割り込み入力
 * @details IRQ0、IRQ1とIRQ5に割り当てられており、エコーパルスの立ち下がりエッジで、
 * @details MTU3（トリガパルス生成に使用）のタイマカウントを取得\n
 * @details トリガパルスの立ち上がりエッジ(MTU3.TCNT: 0)とエコーパルスの立ち上がりエッジの
 * @details 時間間隔が一定なので、立ち下がりエッジでのタイマカウント取得で、エコー信号の
 * @details パルス幅を実行的に取得可能\n
 * @details アプリからの超音波センサの距離取得要求時(get_sonar_distanceコール時)に、
 * @details タイマカウントを距離に換算して返却
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
    //! 左側超音波センサ
    SONAR_LEFT = 0,
    //! 前側超音波センサ
    SONAR_FRONT,
    //! 右側超音波センサ
    SONAR_RIGHT
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
 * @param port 超音波センサの選択
 * @return int 超音波センサで測定された距離（LSB: 1 mm）
 */
int get_sonar_distance(sonar_port_t port);

/**
 * @brief 左側超音波センサエコー割り込み入力端子の立ち下がりエッジで呼ばれる
 * @brief 割り込みハンドラで、エコーON時間を算出のためにタイマカウントを取得
 * @details 立ち下がりエッジでのMTU3（トリガ信号生成にも使用）のタイマカウントを取得\n
 * @details トリガ信号の立ち上がりエッジ(MTU3.TCNT: 0)とエコー信号の立ち上がりエッジの
 * @details 時間間隔が一定なので、立ち下がりエッジでのタイマカウント取得で、エコー信号の
 * @details パルス幅を実行的に取得できる
 * @attention IRQ5の割り込み処理であるExcep_IRQ5の内部でコールすること
 */
void excep_sonar_echo_left();

/**
 * @brief 前側超音波センサエコー割り込み入力端子の立ち下がりエッジで呼ばれる
 * @brief 割り込みハンドラで、エコーON時間を算出のためにタイマカウントを取得
 * @details 立ち下がりエッジでのMTU3（トリガ信号生成にも使用）のタイマカウントを取得\n
 * @details トリガ信号の立ち上がりエッジ(MTU3.TCNT: 0)とエコー信号の立ち上がりエッジの
 * @details 時間間隔が一定なので、立ち下がりエッジでのタイマカウント取得で、エコー信号の
 * @details パルス幅を実行的に取得できる
 * @attention IRQ0の割り込み処理であるExcep_IRQ0の内部でコールすること
 */
void excep_sonar_echo_front();

/**
 * @brief 右側超音波センサエコー割り込み入力端子の立ち下がりエッジで呼ばれる
 * @brief 割り込みハンドラで、エコーON時間を算出のためにタイマカウントを取得
 * @details 立ち下がりエッジでのMTU3（トリガ信号生成にも使用）のタイマカウントを取得\n
 * @details トリガ信号の立ち上がりエッジ(MTU3.TCNT: 0)とエコー信号の立ち上がりエッジの
 * @details 時間間隔が一定なので、立ち下がりエッジでのタイマカウント取得で、エコー信号の
 * @details パルス幅を実行的に取得できる
 * @attention IRQ1の割り込み処理であるExcep_IRQ1の内部でコールすること
 */
void excep_sonar_echo_right();

#endif