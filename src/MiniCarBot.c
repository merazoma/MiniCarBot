/**
 * @file MiniCarBot.c
 * @brief メイン処理
 * 
 * @copyright Copyright (c) 2020
 * 
 */
                  

#include "robot_running.h"
//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	init_rx220();
	robot_running();
	// test_get_photo_reflector();
	// test_sonar();

	// test_drive_motor();
	// test_enc();
	// test_analog_read();
	// test_get_photo_reflector_dif();
	// test_drive_pwm();
	// test_sound_buzzer_150();
	// test_analog_read();
	// test_digital_read();
	// test_sonar();
	// test_drive_pwm();
 	// test_general_io();
	// test_motor_sonar_buzzer();
	// test_fb_control_motor_Nrpm();
	// test_fb_control_motor_Nrpm_const();
	// test_control_motor_sonar_buzzer();
	// test_sound_buzzer_note();
	// test_photo_reflector();
	// test_general_io();
	// test_drive_pwm_0_100();
	// test_drive_pwm_update();
	// test_sound_buzzer();
	// test_sound_buzzer_update();
	// test_sound_buzzer_note();
	// test_sonar();
	// test_control_motor();
	// test_control_motor_sonar();
	// test_main_only_sonar();
	// test_parallel_photo();
	while(1) {
		
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
