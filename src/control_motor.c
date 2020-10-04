#include "control_motor.h"
#include "general_io.h"
#include "pwm_drive.h"
#include "encoder.h"
#include "iodefine.h"

#define WheelSperation 75                                   //トレッド幅 unit: mm
#define WheelDiameter  48                                   //車輪直径 unit: mm
#define ToNrps (1/(WheelDiameter*314)*100)
#define GearTier 54
#define GearMotor 8
#define MaxMotorNrpm 9920                                   //SCR13-2005 定格回転数: 9920
#define NtoEncCnt (256*4)
static short motor_Nrpm_to_control[2] = {0, 0};
static short u_duty[2] = {0, 0};
short gain_p[2] = {20, 20};
short gain_i[2] = {3, 3};
#define MaxIterm (3000)
#define MinIterm (-MaxIterm)

void init_motor() {
    digital_write(M_STBY, HIGH);        //モータ駆動ICのスタンバイを解除
}

void control_motor(short lin_vel, short ang_vel) {
    short wheel_vel[2];     //左右車輪での速度 unit: mm/s
    short motor_Nrpm[2];    //モータの回転速度 unit: rpm

    wheel_vel[LEFT]   = lin_vel - (ang_vel * WheelSperation * 314/100/180 / 2);
    wheel_vel[RIGHT]  = lin_vel + (ang_vel * WheelSperation * 314/100/180 / 2);

    motor_Nrpm[LEFT]   = wheel_vel[LEFT] * GearTier * 60 * 100 / 314 / WheelDiameter / GearMotor;
    motor_Nrpm[RIGHT]   = wheel_vel[RIGHT] * GearTier * 60 * 100 / 314 / WheelDiameter / GearMotor;

    set_motor_Nrpm_to_control(LEFT, motor_Nrpm[LEFT]);
    set_motor_Nrpm_to_control(RIGHT, motor_Nrpm[RIGHT]);
}

void set_motor_Nrpm_to_control(motor_id_t motor_id, short Nrpm) {
    if (Nrpm > MaxMotorNrpm) {
        Nrpm = MaxMotorNrpm;
    }
    motor_Nrpm_to_control[motor_id] = Nrpm;
}

void fb_control_motor_Nrpm() {
    static short i_term[2] = {0, 0};
    short tcnt_enc[2];
    short tcnt_to_control[2];
    short err_sig[2];
    tcnt_to_control[LEFT] = motor_Nrpm_to_control[LEFT] * NtoEncCnt / (1000 * 60);
    tcnt_to_control[RIGHT] = motor_Nrpm_to_control[RIGHT] * NtoEncCnt / (1000 * 60);
    tcnt_enc[LEFT] = get_enc_count_dif(ENCODER_LEFT);
    tcnt_enc[RIGHT] = get_enc_count_dif(ENCODER_RIGHT);
    err_sig[LEFT] = tcnt_to_control[LEFT] - tcnt_enc[LEFT];
    err_sig[RIGHT] = tcnt_to_control[RIGHT] - tcnt_enc[RIGHT];
    if (err_sig[LEFT] < 0) {
        digital_write(M_LED0, HIGH);
        digital_write(M_LED1, LOW);
    } else {
        digital_write(M_LED0, LOW);
        digital_write(M_LED1, HIGH);

    }
    if (err_sig[RIGHT] < 0) {
        digital_write(DBG_LED0, HIGH);
    } else {
        digital_write(DBG_LED0, LOW);
    }

    i_term[LEFT] += err_sig[LEFT];
    i_term[RIGHT] += err_sig[RIGHT];
    if (i_term[LEFT] > MaxIterm) {
        i_term[LEFT] = MaxIterm;
    } else if (MinIterm > i_term[LEFT]) {
        i_term[LEFT] = MinIterm;
    }
    if (i_term[RIGHT] > MaxIterm) {
        i_term[RIGHT] = MaxIterm;
    } else if (MinIterm > i_term[RIGHT]) {
        i_term[RIGHT] = MinIterm;
    } 
    u_duty[LEFT] = gain_p[LEFT] * err_sig[LEFT] + gain_i[LEFT] * i_term[LEFT];
    u_duty[RIGHT] = gain_p[RIGHT] * err_sig[RIGHT] + gain_i[RIGHT] * i_term[RIGHT];
    if (u_duty[LEFT] < 0) {
        drive_motor_duty(LEFT, -u_duty[LEFT], BACKWARD);
        // drive_motor_duty(LEFT, 0, FORWARD);
    } else {
        drive_motor_duty(LEFT, u_duty[LEFT], FORWARD);
    }
    if (u_duty[RIGHT] < 0) {
        drive_motor_duty(RIGHT, -u_duty[LEFT], BACKWARD);
        // drive_motor_duty(LEFT, 0, FORWARD);
    } else {
        drive_motor_duty(RIGHT, u_duty[RIGHT], FORWARD);
    }
}

void drive_motor_duty(motor_id_t motor_id, unsigned short duty, motor_direction_t direction) {
    switch (motor_id)
    {
    case LEFT:
        switch (direction)
        {
        case FORWARD:
            digital_write(M_L_IN1, LOW);
            digital_write(M_L_IN2, HIGH);
            break;
        case BACKWARD:        
            digital_write(M_L_IN1, HIGH);
            digital_write(M_L_IN2, LOW);
        default:
            break;
        }
        drive_pwm(M_L_PWM, duty);
        break;
    case RIGHT:
        switch (direction)
        {
        case FORWARD:
            digital_write(M_R_IN1, LOW);
            digital_write(M_R_IN2, HIGH);
            break;
        case BACKWARD:        
            digital_write(M_R_IN1, HIGH);
            digital_write(M_R_IN2, LOW);
        default:
            break;
        }
        drive_pwm(M_R_PWM, duty);
        break;
    default:
        break;
    }
}

