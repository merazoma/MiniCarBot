#include "control_motor.h"
#include "general_io.h"
#include "pwm_drive.h"
#include "encoder.h"
#include "iodefine.h"
#include "math_lib.h"

static void update_Nrpm_to_control_now();

#define WheelSperation 75                                   //トレッド幅 unit: mm
#define WheelDiameter  48                                   //車輪直径 unit: mm
#define ToNrps (1/(WheelDiameter*314)*100)
#define GearTier 54
#define GearMotor 8
#define MaxMotorNrpm 9920                                   //SCR13-2005 定格回転数: 9920
#define NtoEncCnt (256*4)
#define MaxIterm (3000)
#define MinIterm (-MaxIterm)
#define MinMotorNrpmPerSec (100)

//! 左右輪回転数のアプリ指示値（単位: rpm）
static short motor_Nrpm_to_control[2] = {0, 0};
//! 左右輪回転数の現在の指示値（単位: rpm）
static short motor_Nrpm_to_control_now[2] = {0, 0};
//! 左右輪の指示加速度（単位: rpm/s）
static int motor_Nrpm_per_sec[2] = {0, 0};

const short gain_p[2] = {20, 20};
const short gain_i[2] = {3, 3};

void init_motor() {
    digital_write(M_STBY, HIGH);        //モータ駆動ICのスタンバイを解除
}

void control_motor(short lin_vel, short ang_vel, short lin_accel, short ang_accel) {
    short i;
    short wheel_vel[2];     //左右車輪での速度 unit: mm/s
    short motor_Nrpm[2];    //モータの回転速度 unit: rpm
    short wheel_accel[2];     //左右車輪での加速度 unit: mm/s^2
    short Nrpm_per_sec[2];    //モータの回転加速度 unit: rpm/s

    wheel_vel[LEFT]   = lin_vel - (ang_vel * WheelSperation * 314/100/180 / 2);
    wheel_vel[RIGHT]  = lin_vel + (ang_vel * WheelSperation * 314/100/180 / 2);
    wheel_accel[LEFT]   = lin_accel - (ang_accel * WheelSperation * 314/100/180 / 2);
    wheel_accel[RIGHT]  = lin_accel + (ang_accel * WheelSperation * 314/100/180 / 2);

    for (i = 0; i < 2; i++) {
        motor_Nrpm[i] = wheel_vel[i] * GearTier * 60 * 100 / 314 / WheelDiameter / GearMotor;
        Nrpm_per_sec[i] = wheel_accel[i] * GearTier * 60 * 100 / 314 / WheelDiameter / GearMotor;
        set_motor_Nrpm_to_control(i, motor_Nrpm[i], abs(Nrpm_per_sec[i]));
    }
}

void set_motor_Nrpm_to_control(motor_id_t motor_id, short Nrpm, int Nrpm_per_sec) {
    if (Nrpm > MaxMotorNrpm) {
        Nrpm = MaxMotorNrpm;
    }
    if (Nrpm_per_sec < MinMotorNrpmPerSec) {
        Nrpm_per_sec = MinMotorNrpmPerSec;
    }
    motor_Nrpm_to_control[motor_id] = Nrpm;
    motor_Nrpm_per_sec[motor_id] = Nrpm_per_sec;
}

void fb_control_motor_Nrpm() {
    static short i_term[2] = {0, 0};
    short tcnt_enc[2];
    short tcnt_to_control[2];
    short err_sig[2];
    short u_duty[2] = {0, 0};

    update_Nrpm_to_control_now();
    tcnt_to_control[LEFT] = motor_Nrpm_to_control_now[LEFT] * NtoEncCnt / (1000 * 60);
    tcnt_to_control[RIGHT] = motor_Nrpm_to_control_now[RIGHT] * NtoEncCnt / (1000 * 60);
    tcnt_enc[LEFT] = get_enc_count_dif(ENCODER_LEFT);
    tcnt_enc[RIGHT] = get_enc_count_dif(ENCODER_RIGHT);
    err_sig[LEFT] = tcnt_to_control[LEFT] - tcnt_enc[LEFT];
    err_sig[RIGHT] = tcnt_to_control[RIGHT] - tcnt_enc[RIGHT];
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

static void update_Nrpm_to_control_now() {
    short i;
    static short call_cnt = 0;
    
    call_cnt ++;
    // 10回コールされた場合に、左右輪回転数の現在の指示値を更新
    // 1ms割込みハンドラ中でコールされるため、10ms毎に左右輪回転数の現在の指示値を更新
    if (call_cnt > 9) {
        call_cnt = 0;
        for (i = 0; i < 2; i++) {
            if (motor_Nrpm_to_control_now[i] < motor_Nrpm_to_control[i]) { //加速
                motor_Nrpm_to_control_now[i] += motor_Nrpm_per_sec[i] / 100;            
                if (motor_Nrpm_to_control_now[i] > motor_Nrpm_to_control[i]) {
                    motor_Nrpm_to_control_now[i] = motor_Nrpm_to_control[i];
                }
            } else { //減速
                motor_Nrpm_to_control_now[i] -= motor_Nrpm_per_sec[i] / 100;            
                if (motor_Nrpm_to_control_now[i] < motor_Nrpm_to_control[i]) {
                    motor_Nrpm_to_control_now[i] = motor_Nrpm_to_control[i];
                }
            }
        }
    }

}

