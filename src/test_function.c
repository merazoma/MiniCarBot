#include "general_io.h"
#include "sci.h"
#include "analog_read.h"
#include "pwm_drive.h"
#include "sound_buzzer.h"
#include "sonar.h"
#include "iodefine.h"
#include "encoder.h"
#include "control_motor.h"
#include "photo_reflector.h"


void test_general_io(void) {
	digital_write(DBG_LED0, HIGH);
	digital_write(DBG_LED1, LOW);
	digital_write(DBG_LED2, HIGH);
	digital_write(M_LED0, HIGH);
	digital_write(M_LED1, LOW);
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
	drive_pwm(M_L_PWM, 10000);
	drive_pwm(M_R_PWM, 2000);
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
		sci_printf("Left Sonar distance = %d\r\n", d);
		d = get_sonar_distance(SONAR_FRONT);
		sci_printf("Front Sonar distance = %d\r\n", d);
		d = get_sonar_distance(SONAR_RIGHT);
		sci_printf("Right Sonar distance = %d\r\n", d);
		for (i=0; i<2000000;i++);
	}
}

// void test_photo_reflector() {
// 	int photo_ad;
// 	int i;
// 	while (1)
// 	{
// 		photo_ad = get_photo_reflecor_distance(PHOTO_SENS_LF);
// 		sci_printf("Left Front Photo AD value = %d\r\n", photo_ad);
// 		for (i=0; i<2000000;i++);
// 	}
// }

void test_drive_motor() {
	drive_motor_duty(LEFT, 8000, FORWARD);
	drive_motor_duty(RIGHT, 0, FORWARD);
}

void test_motor_sonar_buzzer(){
	int d_f, d_r, d_l;
	while(1) {
		d_f = get_sonar_distance(SONAR_FRONT);
		d_r = get_sonar_distance(SONAR_RIGHT);
		d_l = get_sonar_distance(SONAR_LEFT);
		if (d_f > 1000) {
			sound_buzzer(1000);
		} else {
			sound_buzzer(d_f);		
		}
		if (d_r > 1000) {
			drive_motor_duty(RIGHT, 1000000, FORWARD);
		} else {
			drive_motor_duty(RIGHT, d_r*1000, FORWARD);
		}
		if (d_l > 1000) {
			drive_motor_duty(LEFT, 100, FORWARD);
		} else {
			drive_motor_duty(LEFT, d_l*1000, FORWARD);
		}
	}
}

void test_control_motor_sonar_buzzer(){
	int d_f, d_r, d_l;
	while(1) {
		d_f = get_sonar_distance(SONAR_FRONT);
		d_r = get_sonar_distance(SONAR_RIGHT);
		d_l = get_sonar_distance(SONAR_LEFT);
		if (d_f > 1000) {
			sound_buzzer(1000);
		} else {
			sound_buzzer(d_f);		
		}
		if (d_r > 1000) {
			set_motor_Nrpm_to_control(RIGHT, 10000);
		} else {
			set_motor_Nrpm_to_control(RIGHT, d_r*10);
		}
		if (d_l > 1000) {
			set_motor_Nrpm_to_control(LEFT, 10000);
		} else {
			set_motor_Nrpm_to_control(LEFT, d_l*10);
		}
	}
}

void test_control_motor_sonar(){
	int d_f, d_r, d_l;
	while(1) {
		d_f = get_sonar_distance(SONAR_FRONT);
		d_r = get_sonar_distance(SONAR_RIGHT);
		d_l = get_sonar_distance(SONAR_LEFT);
		if (d_f > 2000) {
			control_motor(2000, 0);
		} else {
			control_motor(d_f, 0);
		}
		// if (d_r > 1000) {
		// 	set_motor_Nrpm_to_control(RIGHT, 10000);
		// } else {
		// 	set_motor_Nrpm_to_control(RIGHT, d_r*10);
		// }
		// if (d_l > 1000) {
		// 	set_motor_Nrpm_to_control(LEFT, 10000);
		// } else {
		// 	set_motor_Nrpm_to_control(LEFT, d_l*10);
		// }
	}
}

void test_enc() {
	unsigned short enc_l, enc_r;
	int j;
	while (1)
	{
		enc_l = get_enc_count(ENCODER_LEFT);
		enc_r = get_enc_count(ENCODER_RIGHT);
		sci_printf("Left Enc Count = %u\r\n", enc_l);
		sci_printf("Right Enc Count = %u\r\n", enc_r);
		if (enc_l > 10000) {
			set_enc_count(ENCODER_LEFT, 2000);
		}
		if (enc_r > 10000) {
			set_enc_count(ENCODER_RIGHT, 2000);
		}
		for (j=0; j<2000000;j++);
	}	
}

void test_cmt() {
	static int cnt = 0;
    cnt ++;
    if (cnt % 2 == 0) {
        digital_write(DBG_LED0, HIGH);
    } else {
        digital_write(DBG_LED0, LOW);
    }
}

void test_fb_control_motor_Nrpm() {
	int i;
	int j;
	while (1)
	{
		for (i = 0; i < 20; i++)
		{
			set_motor_Nrpm_to_control(LEFT, i*1000-10000);
			set_motor_Nrpm_to_control(RIGHT, i*1000-10000);
			for (j=0; j<4000000;j++);
		}		
	}	
}

void test_fb_control_motor_Nrpm_const() {
	set_motor_Nrpm_to_control(LEFT, 5000);
	set_motor_Nrpm_to_control(RIGHT, 2000);
}

void test_enc_dif() {
	short enc_l;
	enc_l = get_enc_count_dif(ENCODER_LEFT);
	drive_motor_duty(RIGHT, enc_l*10, FORWARD);
}

// void test_main_only_sonar() {
// 	static short gain_p = 1;
// 	static short gain_curve = 5;
// 	static int d_r_l_to_control = 0;
// 	static int err_d_r_l = 0;
// 	static short VelWhenCurve = 500;
// 	int d_f, d_r, d_l;
// 	int d_r_l_dif;

// 	short lin_vel, lin_vel_from_ang, ang_vel;
// 	while(1) {
// 		d_f = get_sonar_distance(SONAR_FRONT);
// 		d_r = get_sonar_distance(SONAR_RIGHT);
// 		d_l = get_sonar_distance(SONAR_LEFT);
// 		d_r_l_dif = d_r - d_l;
// 		err_d_r_l = d_r_l_dif - d_r_l_to_control;
// 		ang_vel = - err_d_r_l * gain_p / 8;
// 		if (d_f < 400) {
// 			control_motor(0, -180);
// 			continue;
// 		} else {
// 			lin_vel = 500;
// 		}
// 		// lin_vel_from_ang = lin_vel + ang_vel * gain_curve;
// 		// if (lin_vel_from_ang < 0 ) {
// 		// 	lin_vel = VelWhenCurve;
// 		// } else if (lin_vel > lin_vel_from_ang){
// 		// 	lin_vel = lin_vel_from_ang;
// 		// } 
// 		control_motor(lin_vel, ang_vel);
// 	}
// }

// void test_main_only_sonar() {
// 	static short gain_p = 1;
// 	static short gain_d = 1;
// 	static short gain_curve = 5;
// 	static int d_r_to_control = 1000;
// 	static int d_r_old = 0;
// 	static int err_d_r = 0;
// 	static short MaxLinVel = 2000;
// 	static short MinLinVel = 500;
// 	static short MinAngVel = -360;
// 	int d_f, d_r;
// 	int d_r_dif;

// 	short lin_vel;
// 	short lin_vel_from_ang, ang_vel;
// 	while(1) {
// 		d_f = get_sonar_distance(SONAR_FRONT);
// 		d_r = get_sonar_distance(SONAR_RIGHT);
// 		if (d_f < 400) {
// 			lin_vel = 0;
// 			control_motor(lin_vel, -180);
// 			continue;
// 		} else {
// 			lin_vel  = 2000;
// 		}
// 		d_r_dif = d_r - d_r_old;
// 		d_r_old = d_r;
// 		ang_vel = d_r_dif * gain_d;		
// 		err_d_r = d_r - d_r_to_control;
// 		ang_vel = -(gain_p * err_d_r - gain_d * d_r_dif);
// 		if (ang_vel < -180) {
// 			lin_vel -= 1000;
// 		}
// 		control_motor(lin_vel, ang_vel);
// 	}
// }

void test_main_only_sonar() {
	static short gain_p = 64;
	static short gain_curve = 5;
	static int d_r_l_to_control = 0;
	static int err_d_r_l = 0;
	static short MaxLinVel = 2000;
	static short MinLinVel = 500;
	static short MinAngVel = -360;
	int d_f, d_r, d_l, d_r_l_dif;

	short lin_vel;
	short lin_vel_from_ang, ang_vel;
	while(1) {
		d_f = get_sonar_distance(SONAR_FRONT);
		d_r = get_sonar_distance(SONAR_RIGHT);
		d_l = get_sonar_distance(SONAR_LEFT);
		if (d_f < 500) {
			lin_vel = 0;
			control_motor(lin_vel, -180);
			continue;
		} else {
			lin_vel  = 1500;
		}
		d_r_l_dif = d_r - d_l;
		err_d_r_l = d_r_l_dif - d_r_l_to_control;
		ang_vel = - gain_p * d_r_l_dif / 128;
		if (ang_vel < -90) {
			lin_vel = 800;
		}
		control_motor(lin_vel, ang_vel);
	}
}


void test_control_motor(){
	int i;
	int j;
	short ang_vel;
	while (1)
	{
		for (i = 0; i < 20; i++)
		{
			ang_vel = (i - 10)*200;
			control_motor(1000, ang_vel);
			for (j=0; j<4000000;j++);
		}		
	}	
}

void test_GBADI(){
	while(1) {
		S12AD.ADCSR.BIT.ADST = 0x1;         //AD変換開始要求
		while (S12AD.ADCSR.BIT.ADST == 1);  //AD変換完了待ち
	}
}