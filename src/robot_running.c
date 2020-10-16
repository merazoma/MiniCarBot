#include "robot_running.h"
#include "sonar.h"
#include "photo_reflector.h"
#include "control_motor.h"
#include "sound_buzzer.h"
#include "sci.h"

static int control_using_photo(int d_rf_rs_to_control);
static int abs(int j);
static void wait_until_front_clear();

static const short gain_p_photo = 20;
static const short gain_p_sonar_clock = 400;
static const short gain_p_sonar_anticlock = 400;
static const short gain_p_sonar_right_curving = 200;
static const short gain_p_sonar_right_too_near = 200;
static const short AngVelOfst = 90;
static const short DisRightSonarWhenCurving = 320;
static const short MinDisRightSonar = 160;
static const short MaxDisRightSonar = 240;

static const short ControlRangeFrontSonar = 120;
static const short RightDisWhenClockWise = 500;

static const short DefaultLinVel = 700;
static const short LinVelWhenCurving = 400;

static const short d_rf_rs_to_control = 0;
static const short ThshDisPhotoControl = -50;

static const short PhotoControlBuzzerFreq = 256;
static const short RightTooFarWarningFreq = 1024;
static const short RightTooNearWarningFreq = 512;

void robot_running(){
    int i;
	int lin_vel, ang_vel;
    int d_sonar[3], d_sonar_dif[3], d;
    static int d_sonar_old[3] = {0, 0, 0};
    int d_photo[4];
    int d_rf_rs_dif, d_sonar_rf_rs_dif;

	while(1) {
        lin_vel = DefaultLinVel;
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
        // sci_printf("Front Sonar distance = %d\r\n", d_sonar[SONAR_FRONT]);
        // sci_printf("Right Front Sonar distance = %d\r\n", d_sonar[SONAR_RIGHT_FRONT]);
        // sci_printf("Right Side Sonar distance = %d\r\n", d_sonar[SONAR_RIGHT_SIDE]);

        // 前方超音波センサで接近感知時の処理
        if (d_sonar[SONAR_FRONT] < ControlRangeFrontSonar) {
            lin_vel = (d_sonar[SONAR_FRONT] - ControlRangeFrontSonar/2) / 2;
            if (d_sonar[SONAR_RIGHT_SIDE] < RightDisWhenClockWise || d_sonar[SONAR_RIGHT_FRONT] < RightDisWhenClockWise) {
                ang_vel = 120;
            } else {
                ang_vel = -120;
            }
    		control_motor(lin_vel, ang_vel);
            continue;
        }
        // 右側超音波センサで壁を見失った場合の処理
        else if (d_sonar[SONAR_RIGHT_SIDE] > DisRightSonarWhenCurving || d_sonar[SONAR_RIGHT_FRONT] > DisRightSonarWhenCurving) {
            ang_vel = -gain_p_sonar_right_curving * (d_sonar[SONAR_RIGHT_SIDE] - MinDisRightSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            lin_vel = LinVelWhenCurving;
            sound_buzzer(RightTooFarWarningFreq);
        } 
        // 右側超音波センサで接近感知時の処理
        else if (d_sonar[SONAR_RIGHT_SIDE] < MinDisRightSonar) {
            ang_vel = -gain_p_sonar_right_too_near * (d_sonar[SONAR_RIGHT_SIDE] - MinDisRightSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            sound_buzzer(RightTooNearWarningFreq);
        }         
        // 右前側超音波センサで接近感知時の処理
        else if (d_sonar[SONAR_RIGHT_FRONT] < MinDisRightSonar) {
            ang_vel = -gain_p_sonar_right_too_near * (d_sonar[SONAR_RIGHT_FRONT] - MinDisRightSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            sound_buzzer(RightTooNearWarningFreq);
        }         
        // フォトリフレクタによる壁との平行制御
        else if ((d_photo[PHOTO_RIGHT_FRONT] < ThshDisPhotoControl) && (d_photo[PHOTO_RIGHT_SIDE] < ThshDisPhotoControl)) {
            d_rf_rs_dif = d_photo[PHOTO_RIGHT_FRONT] - d_photo[PHOTO_RIGHT_SIDE];
            ang_vel = control_using_photo(d_rf_rs_dif);
        }
        else {   
            d_sonar_rf_rs_dif = d_sonar[SONAR_RIGHT_FRONT] - d_sonar[SONAR_RIGHT_SIDE];
            if (d_sonar_rf_rs_dif < 0) {
                ang_vel = -gain_p_sonar_clock * d_sonar_rf_rs_dif / 128;
            } else {
                ang_vel = -gain_p_sonar_anticlock * d_sonar_rf_rs_dif / 128;
            }
			ang_vel += AngVelOfst;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            sci_printf("ang_vel: %d\r\n", (short)ang_vel);
            stop_buzzer();
        }

        control_motor(lin_vel, ang_vel);
        for (i = 0; i < 3; i++) {
            d_sonar_old[i] = d_sonar[i];
        }

	}
}

/**
 * @brief フォトリフレクタによる制御
 * @param d_rf_rs_dif int 右前光センサと右サイド光センサによる距離の差
 * @return ang_vel
 */
static int control_using_photo(int d_rf_rs_dif) {
    int err_rf_rs, ang_vel;
    sound_buzzer(PhotoControlBuzzerFreq);
    err_rf_rs = d_rf_rs_dif - d_rf_rs_to_control;
    ang_vel = - (gain_p_photo * err_rf_rs) / 128;
    ang_vel = upper_lower_limit(ang_vel, 90, -90);
    return ang_vel;
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