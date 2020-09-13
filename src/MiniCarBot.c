/**
 * @file MiniCarBot.c
 * @brief メイン処理
 * @version 0.1
 * @date 2020-09-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */
                  


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
	
	// test_sci_printf();
	//test_analog_read();
	test_drive_pwm();
	test_sonar();
	// test_general_io();
	// test_drive_pwm_0_100();
	// test_drive_pwm_update();
	// test_sound_buzzer();
	// test_sound_buzzer_update();
	// test_sound_buzzer_note();
	// test_sonar();
	while(1);
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
