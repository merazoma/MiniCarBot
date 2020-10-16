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

void test_general_io(void)
{
    digital_write(DBG_LED0, HIGH);
    digital_write(DBG_LED1, LOW);
    digital_write(DBG_LED2, HIGH);
    digital_write(M_LED0, HIGH);
    digital_write(M_LED1, LOW);
}

void test_sci_printf()
{
    volatile char cnt;
    while (1)
    {
        cnt++;
        sci_printf("count = %d\r\n", cnt);
    }
}

void test_analog_read()
{
    unsigned short ad_data;
    int i;
    int j;
    while (1)
    {
        for (i = 0; i < 5; i++)
        {
            ad_data = analog_read_ondemand(i);
            sci_printf("AD(AN00%d) = %d\r\n", i, ad_data);
            for (j = 0; j < 2000000; j++)
                ;
        }
    }
}

void test_drive_pwm()
{
    drive_pwm(M_L_PWM, 10000);
    drive_pwm(M_R_PWM, 2000);
}

void test_drive_pwm_0_100()
{
    drive_pwm(M_L_PWM, 0);
    drive_pwm(M_R_PWM, 100);
}

void test_drive_pwm_update()
{
    int i;
    int j;
    unsigned int duty;
    while (1)
    {
        for (i = 0; i <= 100; i += 10)
        {
            duty = i;
            drive_pwm(M_L_PWM, duty);
            drive_pwm(M_R_PWM, 100 - duty);
            for (j = 0; j < 20; j++)
                ;
        }
    }
}

void test_sound_buzzer_150()
{
    sound_buzzer(150);
}

void test_sound_buzzer_update()
{
    unsigned short freq;
    int i;
    int j;
    while (1)
    {
        for (i = 0; i <= 75; i++)
        {
            freq = 200 * (i + 1);
            sound_buzzer(freq);
            for (j = 0; j < 4000000; j++)
                ;
        }
    }
}

/**
 * @brief shooting star song!!!
 *
 */
void test_sound_buzzer_note()
{
    int j;
    while (1)
    {
        sound_buzzer(NOTE_C4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_C4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_G4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_G4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_A4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_A4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_G4);
        for (j = 0; j < 8000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_F4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_F4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_E4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_E4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_D4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_D4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
        sound_buzzer(NOTE_C4);
        for (j = 0; j < 4000000; j++)
            ;
        stop_buzzer();
        for (j = 0; j < 4000000; j++)
            ;
    }
}

void test_sonar()
{
    int d;
    int i;
    while (1)
    {
        d = get_sonar_distance(SONAR_RIGHT_FRONT);
        sci_printf("Right Front Sonar distance = %d\r\n", d);
        d = get_sonar_distance(SONAR_FRONT);
        sci_printf("Front Sonar distance = %d\r\n", d);
        d = get_sonar_distance(SONAR_RIGHT_SIDE);
        sci_printf("Right Side Sonar distance = %d\r\n", d);
        for (i = 0; i < 2000000; i++)
            ;
    }
}

void test_photo_reflector()
{
    int photo_ad;
    int i, j;
    int d_photo[4];
    while (1)
    {
        for (j = 0; j < 4; j++)
        {
            d_photo[j] = get_photo_reflector_distance(j);
        }
        sci_printf("LF = %d, LS = %d, RF = %d, RS = %d\r\n", d_photo[0], d_photo[1], d_photo[2], d_photo[3]);
        for (i = 0; i < 2000000; i++)
            ;
    }
}

void test_drive_motor()
{
    drive_motor_duty(LEFT, 8000, FORWARD);
    drive_motor_duty(RIGHT, 0, FORWARD);
}

void test_enc()
{
    unsigned short enc_l, enc_r;
    int j;
    while (1)
    {
        enc_l = get_enc_count(ENCODER_LEFT);
        enc_r = get_enc_count(ENCODER_RIGHT);
        sci_printf("Left Enc Count = %u\r\n", enc_l);
        sci_printf("Right Enc Count = %u\r\n", enc_r);
        if (enc_l > 10000)
        {
            set_enc_count(ENCODER_LEFT, 2000);
        }
        if (enc_r > 10000)
        {
            set_enc_count(ENCODER_RIGHT, 2000);
        }
        for (j = 0; j < 2000000; j++)
            ;
    }
}

void test_cmt()
{
    static int cnt = 0;
    cnt++;
    if (cnt % 2 == 0)
    {
        digital_write(DBG_LED0, HIGH);
    }
    else
    {
        digital_write(DBG_LED0, LOW);
    }
}

void test_fb_control_motor_Nrpm()
{
    int i;
    int j;
    while (1)
    {
        for (i = 0; i < 20; i++)
        {
            set_motor_Nrpm_to_control(LEFT, i * 1000 - 10000);
            set_motor_Nrpm_to_control(RIGHT, i * 1000 - 10000);
            for (j = 0; j < 4000000; j++)
                ;
        }
    }
}

void test_fb_control_motor_Nrpm_const()
{
    set_motor_Nrpm_to_control(LEFT, 5000);
    set_motor_Nrpm_to_control(RIGHT, 2000);
}

void test_enc_dif()
{
    short enc_l;
    enc_l = get_enc_count_dif(ENCODER_LEFT);
    drive_motor_duty(RIGHT, enc_l * 10, FORWARD);
}

void test_control_motor()
{
    int i;
    int j;
    short ang_vel;
    while (1)
    {
        for (i = 0; i < 20; i++)
        {
            ang_vel = (i - 10) * 200;
            control_motor(1000, ang_vel);
            for (j = 0; j < 4000000; j++)
                ;
        }
    }
}

void test_GBADI()
{
    while (1)
    {
        S12AD.ADCSR.BIT.ADST = 0x1; //AD変換開始要求
        while (S12AD.ADCSR.BIT.ADST == 1)
            ; //AD変換完了待ち
    }
}

void test_get_photo_reflector_dif()
{
    short d_rs, d_rf, d_ls, d_lf;
    int j;
    while (1)
    {
        d_rs = get_photo_reflector_dif(PHOTO_RIGHT_SIDE);
        d_rf = get_photo_reflector_dif(PHOTO_RIGHT_FRONT);
        d_ls = get_photo_reflector_dif(PHOTO_LEFT_SIDE);
        d_lf = get_photo_reflector_dif(PHOTO_LEFT_FRONT);
        sci_printf("Right Side = %d\r\n", d_rs);
        sci_printf("Right Front = %d\r\n", d_rf);
        sci_printf("Left Side = %d\r\n", d_ls);
        sci_printf("Left Front = %d\r\n", d_lf);
        for (j = 0; j < 2000000; j++)
            ;
    }
}

void test_get_photo_reflector()
{
    short d_rs, d_rf, d_ls, d_lf;
    int j;
    while (1)
    {
        d_rs = get_photo_reflector_distance(PHOTO_RIGHT_SIDE);
        d_rf = get_photo_reflector_distance(PHOTO_RIGHT_FRONT);
        d_ls = get_photo_reflector_distance(PHOTO_LEFT_SIDE);
        d_lf = get_photo_reflector_distance(PHOTO_LEFT_FRONT);
        sci_printf("Right Side = %d\r\n", d_rs);
        sci_printf("Right Front = %d\r\n", d_rf);
        sci_printf("Left Side = %d\r\n", d_ls);
        sci_printf("Left Front = %d\r\n", d_lf);
        for (j = 0; j < 2000000; j++)
            ;
    }
}


void test_digital_read()
{
    unsigned short level;
    while (1)
    {
        level = digital_read(MODE_SW);
        sci_printf("MODE SW: %u\r\n", level);
        level = digital_read(ECHO_RIGHT_FRONT);
        sci_printf("SONAR_RIGHT_FRONT: %u\r\n", level);
        level = digital_read(ECHO_RIGHT_SIDE);
        sci_printf("SONAR_RIGHT_SIDE: %u\r\n", level);
        level = digital_read(ECHO_FRONT);
        sci_printf("SONAR_FRONT: %u\r\n", level);
    }
}
