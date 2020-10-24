#include "robot_running.h"
#include "sonar.h"
#include "photo_reflector.h"
#include "control_motor.h"
#include "sound_buzzer.h"
#include "math_lib.h"
#include "sci.h"

static int control_using_photo(int d_rf_rs_to_control);

// 壁との並走制御
//! 時計周りに曲がるときのゲイン
static const short gain_p_sonar_clock = 200;
//! 反時計周りに曲がるときのゲイン
static const short gain_p_sonar_anticlock = 100;

//右壁に近づいた際に離れる制御
//! 右壁に近づきすぎた際に時計回りに回転するゲイン
static const short gain_p_sonar_right_too_near = 60;
//! 右壁との近づき過ぎを判定する右側超音波センサによる距離（単位: mm）
static const short MinDisRightSonar = 180;
//! 右壁との近づき過ぎを判定する右前側超音波センサによる距離（単位: mm）
static const short MinDisRightFrontSonar = 220;

//右壁から離れすぎた際に近寄る制御
//! 右壁から離れすぎた際に反時計回りに回転するゲイン
static const short gain_p_sonar_right_too_far = 15;
//! 右壁との離れ過ぎを判定する右側超音波センサによる距離（未使用）（単位: mm）
static const short MaxDisRightSonar = 240;
//! 右壁との離れ過ぎを判定する右前側超音波センサによる距離（単位: mm）
static const short MaxDisRightFrontSonar = 300;

// 急カーブ旋回の制御
//! 急カーブを判定するための右壁との超音波センサによる距離（単位: mm）
static const short DisRightSonarWhenCurving = 360;
//! 急カーブ旋回時の直進速度
static short LinVelWhenCurving = 600;
//! 急カーブ旋回時の回転半径
static short RadiusOfRotation = 200;
//! 急カーブ旋回時の回転速度
#define AngVelWhenCurving (-180 * 100 / 314 * LinVelWhenCurving / RadiusOfRotation)


// 光センサにる前方接近時の緊急停止制御
static const short EmergencyDisSonar = 120;
//! 前側光センサで前方壁との近づくすぎを判定する距離（単位: -光量）
static const short EmergencyDisPhoto = -2000;
//! 前方接近時の直進速度（単位: mm/s）
static const short EmergencyLinVel = 0;

//! デフォルトの直進速度
static const short DefaultLinVel = 600;

static const short d_rf_rs_to_control = 0;

//! アラーム用の周波数
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
            d_sonar_old[i] = d_sonar[i];
        }
        for (i = 0; i < 4; i++) {
    		d_photo[i] = get_photo_reflector_distance(i);
        }
        // sci_printf("Front Sonar distance = %d\r\n", d_sonar[SONAR_FRONT]);
        // sci_printf("Right Front Sonar distance = %d\r\n", d_sonar[SONAR_RIGHT_FRONT]);
        // sci_printf("Right Side Sonar distance = %d\r\n", d_sonar[SONAR_RIGHT_SIDE]);
        
        if (d_photo[PHOTO_LEFT_FRONT] < EmergencyDisPhoto || d_photo[PHOTO_RIGHT_FRONT] < EmergencyDisPhoto) {
            lin_vel = EmergencyLinVel;
            ang_vel = 180;
    		control_motor(lin_vel, ang_vel, 2000, 1080);
            continue;
        }
        // 右前側超音波センサで接近感知時の処理
        // else if (d_sonar[SONAR_RIGHT_FRONT] < MinDisRightFrontSonar) {
        //     // lin_vel = (d_sonar[SONAR_RIGHT_FRONT] - ControlRangeFrontSonar/2) * 3;
        //     // ang_vel = -gain_p_sonar_right_too_near * (d_sonar[SONAR_RIGHT_FRONT] + d_sonar[SONAR_RIGHT_SIDE] - 2*MinDisRightSonar) / 128;
        //     // ang_vel = upper_lower_limit(ang_vel, 180, -180);
        //     ang_vel = -AngVelWhenCurving;
        //     lin_vel = LinVelWhenCurving;
    	// 	control_motor(lin_vel, ang_vel, 500, 720);
        //     continue;
        //     // sound_buzzer(RightTooNearWarningFreq);
        // }
        // 右側超音波センサで壁切れを検出し、急カーブを曲がる処理
        else if (d_sonar[SONAR_RIGHT_FRONT] > DisRightSonarWhenCurving) {
            // ang_vel = -gain_p_sonar_right_curving * (d_sonar[SONAR_RIGHT_FRONT] - DisRightSonarWhenCurving) / 128;
            // ang_vel = upper_lower_limit(ang_vel, 180, -180);
            ang_vel = AngVelWhenCurving;
            lin_vel = LinVelWhenCurving;
            control_motor(lin_vel, ang_vel, 300, 2040);
            continue;
            // sound_buzzer(RightTooFarWarningFreq);
        } 
        // 右側超音波センサで壁を見失った場合の処理
        else if (d_sonar[SONAR_RIGHT_FRONT] > MaxDisRightFrontSonar) {
            ang_vel = -gain_p_sonar_right_too_far * (d_sonar[SONAR_RIGHT_FRONT] - MaxDisRightFrontSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            control_motor(lin_vel, ang_vel, 300, 2040);
            continue;
            // sound_buzzer(RightTooFarWarningFreq);
        } 
        // 右側超音波センサで接近感知時の処理
        else if (d_sonar[SONAR_RIGHT_SIDE] < MinDisRightSonar) {
            ang_vel = -gain_p_sonar_right_too_near * (d_sonar[SONAR_RIGHT_SIDE] + d_sonar[SONAR_RIGHT_FRONT] - 2*MinDisRightSonar) / 128;
            ang_vel = upper_lower_limit(ang_vel, 180, -180);
            control_motor(lin_vel, ang_vel, 300, 720);
            sound_buzzer(RightTooNearWarningFreq);
            continue;
        }         
        else {   
            d_sonar_rf_rs_dif = d_sonar[SONAR_RIGHT_FRONT] - d_sonar[SONAR_RIGHT_SIDE] * 12 / 10;
            if (d_sonar_rf_rs_dif < 0) {
                ang_vel = -gain_p_sonar_clock * d_sonar_rf_rs_dif / 128;
                ang_vel = upper_lower_limit(ang_vel, 480, -480);
                control_motor(lin_vel, ang_vel, 300, 1080);
            } else {
                ang_vel = -gain_p_sonar_anticlock * d_sonar_rf_rs_dif / 128;
                ang_vel = upper_lower_limit(ang_vel, 480, -480);
                control_motor(lin_vel, ang_vel, 300, 1080);
            }
            stop_buzzer();
            continue;
        }
	}
}