#include "general_io.h"
#include "sci.h"
#include "analog_read.h"
#include "pwm_drive.h"
#include "sound_buzzer.h"
#include "sonar.h"
#include "iodefine.h"
#include "control_motor.h"
#include "photo_reflector.h"


void test_general_io(void) {
	digital_write(M_R_IN1, HIGH);
	digital_write(M_R_IN2, HIGH);
}

void test_sci_printf() {
	volatile char cnt;
	while (1)
	{
		cnt ++;
		sci_printf("count = %d\r\n", cnt);
	}	
}

void test_analog_read() {
	unsigned short ad_data;
	int i;
	int j;
	while (1)
	{
		for (i = 0; i < 5; i++)
		{
			ad_data = analog_read_ondemand(i);
			sci_printf("AD(AN00%d) = %d\r\n", i, ad_data);
			for (j=0; j<2000000;j++);
		}		
	}	
}

void test_drive_pwm() {
	drive_pwm(M_L_PWM, 50);
	drive_pwm(M_R_PWM, 10);
}

void test_drive_pwm_0_100() {
	drive_pwm(M_L_PWM, 0);
	drive_pwm(M_R_PWM, 100);
}

void test_drive_pwm_update() {
	int i;
	int j;
	unsigned int duty;
	while (1)
	{
		for (i = 0; i <= 100; i+=10)
		{
			duty = i;
			drive_pwm(M_L_PWM, duty);
			drive_pwm(M_R_PWM, 100-duty);
			for (j=0;j<20;j++);
		}
	}
}

void test_sound_buzzer_150() {
	sound_buzzer(150);
}

void test_sound_buzzer_update() {
	unsigned short freq;
	int i;
	int j;
	while (1)
	{
		for (i = 0; i <= 75; i++)
		{
			freq = 200 * (i+1);
			sound_buzzer(freq);
			for (j=0; j<4000000;j++);
		}
	}

}

/**
 * @brief shooting star song!!!
 * 
 */
void test_sound_buzzer_note(){
	int j;
	while (1)
	{
		sound_buzzer(NOTE_C4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_C4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_G4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_G4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_A4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_A4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_G4);
		for (j=0; j<8000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_F4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_F4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_E4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_E4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_D4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_D4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
		sound_buzzer(NOTE_C4);
		for (j=0; j<4000000;j++);
		stop_buzzer();
		for (j=0; j<4000000;j++);
	}
}

void test_sonar() {
	int d;
	int i;
	while (1)
	{
		d = get_sonar_distance(SONAR_LEFT);
		sci_printf("Left Sonar distance = %u\r\n", d);
		// d = get_sonar_distance(SONAR_FRONT);
		// sci_printf("Front Sonar distance = %u\r\n", d);
		// d = get_sonar_distance(SONAR_RIGHT);
		// sci_printf("Right Sonar distance = %u\r\n", d);
		for (i=0; i<2000000;i++);
	}
}

void test_photo_reflector() {
	int photo_ad;
	int i;
	while (1)
	{
		photo_ad = get_photo_reflecor_distance(PHOTO_SENS_LF);
		sci_printf("Left Front Photo AD value = %d\r\n", photo_ad);
		for (i=0; i<2000000;i++);
	}

}

void test_drive_motor() {
	// drive_motor_duty(LEFT, 40, FORWARD);
	// drive_motor_duty(LEFT, 40, FORWARD);
	// drive_motor_duty(RIGHT, 40, FORWARD);
	drive_motor_duty(LEFT, 40, BACKWARD);
	drive_motor_duty(RIGHT, 90, BACKWARD);

}