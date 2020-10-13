#include "robot_running.h"
#include "sonar.h"
#include "photo_reflector.h"
#include "control_motor.h"
#include "sound_buzzer.h"

static const short gain_p_photo = 8;
static const short gain_p_sonar = 64;
static const short ControlRangeSonar = 300;
static const short DisSonarToControl = 200;
static const short ControlRangeFrontSonar = 300;
static const short MinLinVel = 80;

static const short d_rf_rs_to_control = 0;

static const short FrontWarningBuzzerFreq = 128;
static const short LeftWarningBuzzerFreq = 512;

void robot_running(){
    int i;
	short lin_vel, ang_vel;
    int d_sonar[3], d_sonar_dif[3];
    static int d_sonar_old[3] = {0, 0, 0};
    int d_photo[4];
    int d_rf_rs_dif, err_rf_rs, err_r_sonar;

	while(1) {
        ang_vel = 0;
        lin_vel = 500;
        for (i = 0; i < 3; i++) {
            d_sonar[i] = get_sonar_distance(i);
            d_sonar_dif[i] = d_sonar[i] - d_sonar_old[i];
        }
        for (i = 0; i < 4; i++) {
    		d_photo[i] = get_photo_reflector_distance(i);
        }
        if (d_sonar[SONAR_FRONT] < ControlRangeFrontSonar) {
            lin_vel = MinLinVel;
            if (d_sonar[SONAR_RIGHT] < d_sonar[SONAR_LEFT]) {
                ang_vel = 180;
            } else {
                ang_vel = -180;
            }
    		control_motor(lin_vel, ang_vel);
            sound_buzzer(FrontWarningBuzzerFreq);
            continue;
        }
        if (d_sonar[SONAR_LEFT] < ControlRangeSonar) {
            ang_vel += gain_p_sonar * (d_sonar[SONAR_LEFT] - DisSonarToControl) / 128;
    		control_motor(lin_vel, ang_vel);
            sound_buzzer(LeftWarningBuzzerFreq);
            continue;
        } 

        stop_buzzer();
        ang_vel += -gain_p_sonar * (d_sonar[SONAR_RIGHT] - DisSonarToControl) / 128;
		d_rf_rs_dif = d_photo[PHOTO_RIGHT_FRONT] - d_photo[PHOTO_RIGHT_SIDE];
 		err_rf_rs = d_rf_rs_dif - d_rf_rs_to_control;
        err_r_sonar = d_sonar[SONAR_RIGHT] - DisSonarToControl; 
		ang_vel += - (gain_p_photo * err_rf_rs) / 128;

        if (ang_vel < -90) {
            ang_vel = -90;
        } else if (ang_vel > 90) {
            ang_vel = 90;
        }
		control_motor(lin_vel, ang_vel);

        for (i = 0; i < 3; i++) {
            d_sonar_old[i] = d_sonar[i];
        }

	}
}