#include "robot_running.h"
#include "sonar.h"
#include "photo_reflector.h"
#include "control_motor.h"
#include "sound_buzzer.h"

static int abs(int j);
static void wait_until_front_clear();

static const short gain_p_photo = 20;
static const short gain_p_sonar_right = 50;
static const short gain_p_sonar_left = 300;
static const short MinDisRightSonar = 120;
static const short MinDisLeftSonar = 300;
static const short ControlRangeFrontSonar = 250;
static const short MinLinVel = 80;

static const short d_rf_rs_to_control = 0;

static const short FrontWarningBuzzerFreq = 128;
static const short LeftWarningBuzzerFreq = 512;
static const short RightWarningBuzzerFreq = 1024;

static const short DifAcuteCurve = 100;
static const short gain_p_turning_right = 500;


void robot_running(){
    int i;
	short lin_vel, ang_vel;
    int d_sonar[3], d_sonar_dif[3], d;
    static int d_sonar_old[3] = {0, 0, 0};
    int d_photo[4];
    int d_rf_rs_dif, err_rf_rs;

	while(1) {
        lin_vel = 500;
        for (i = 0; i < 3; i++) {
            d_sonar[i] = get_sonar_distance(i);
            if (d_sonar[i] < 0) {
                d_sonar[i] = d_sonar_old[i];
            }
            d_sonar_dif[i] = d_sonar[i] - d_sonar_old[i];
        }
        for (i = 0; i < 4; i++) {
    		d_photo[i] = get_photo_reflector_distance(i);
        }
        // 前方超音波センサで接近感知時の処理
        if (d_sonar[SONAR_FRONT] < ControlRangeFrontSonar) {
            lin_vel = MinLinVel;
            if (d_sonar[SONAR_RIGHT] < d_sonar[SONAR_LEFT]) {
                ang_vel = 180;
            } else {
                ang_vel = -180;
            }
    		control_motor(lin_vel, ang_vel);
            sound_buzzer(FrontWarningBuzzerFreq);
            wait_until_front_clear();
            stop_buzzer();
            for (i = 0; i < 3; i++) {
                d = get_sonar_distance(i);
                if (d > 0) {
                    d_sonar_old[i] = d;
                }
            }
            continue;
        }
        // 右側超音波センサで接近感知時の処理
        else if (d_sonar[SONAR_RIGHT] < MinDisRightSonar) {
            ang_vel = -gain_p_sonar_right * (d_sonar[SONAR_RIGHT] - MinDisRightSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            sound_buzzer(RightWarningBuzzerFreq);
        } 
        // 左側超音波センサで接近感知時の処理
        else if (d_sonar[SONAR_LEFT] < MinDisLeftSonar) {
            ang_vel = gain_p_sonar_left * (d_sonar[SONAR_LEFT] - MinDisLeftSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            sound_buzzer(LeftWarningBuzzerFreq);
        } 
        // 右側の鋭角カーブを曲がる制御
        // 右側超音波センサで前ステップで検知した距離との差が一定値以上のときギュインって曲がる処理
        else if (d_sonar_dif[i] > DifAcuteCurve){
            ang_vel = -gain_p_turning_right * (d_sonar[SONAR_RIGHT] - MinDisRightSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
        }
        // フォトリフレクタによる壁との平行制御
        else {
            stop_buzzer();
            d_rf_rs_dif = d_photo[PHOTO_RIGHT_FRONT] - d_photo[PHOTO_RIGHT_SIDE];
            err_rf_rs = d_rf_rs_dif - d_rf_rs_to_control;
            ang_vel = - (gain_p_photo * err_rf_rs) / 128;
            ang_vel = upper_lower_limit(ang_vel, 90, -90);
        }
        control_motor(lin_vel, ang_vel);
        // control_motor(lin_vel, 0);
        for (i = 0; i < 3; i++) {
            d_sonar_old[i] = d_sonar[i];
        }

	}
}

/**
 * @brief 前方の超音波センサの値が閾値を下回るまで待機
 * 
 */
static void wait_until_front_clear() {
    int d_sonar_front;
    do {
        d_sonar_front = get_sonar_distance(SONAR_FRONT);

    } while (d_sonar_front < ControlRangeFrontSonar);
}

/**
 * @brief 指定された値(val)を上限値・下限値で制限して返却
 * 
 * @param val int 制限したい値
 * @param u_limit int 上限値
 * @param l_limit int 下限値
 * @return int 制限した結果の値
 */
static int upper_lower_limit(int val, int u_limit, int l_limit) {
    if (val < l_limit) {
        val = l_limit;
    } else if (val > u_limit) {
        val = u_limit;
    }
    return val;
}

/**
 * @brief 絶対値を返却
 * 
 * @param j int 対象の数字
 * @return int 絶対値
 */
static int abs(int j) {
  return j < 0 ? -j : j;
}