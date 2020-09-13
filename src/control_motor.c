#include "control_motor.h"
#include "general_io.h"
#include "pwm_drive.h"
#include "iodefine.h"

#define WheelSperation 90                                   //トレッド幅 unit: mm
#define WheelDiameter  48                                   //車輪直径 unit: mm
const float VelocityConst = 60 / (WheelDiameter * 3.1415);
const float GearRatio       = 54/8;                         //ピニオンギア: 8 歯, 平ギア: 54 歯
#define MaxMotorNrpm 9920                                   //SCR13-2005 定格回転数: 9920

void init_motor() {
    digital_write(M_STBY, HIGH);        //モータ駆動ICのスタンバイを解除
};

void control_motor(float lin_vel, float ang_vel) {
    float wheel_vel[2];     //左右車輪での速度 unit: mm/s
    int motor_Nrpm[2];      //モータの回転速度 unit: rpm

    wheel_vel[LEFT]   = lin_vel - (ang_vel * WheelSperation / 2);
    wheel_vel[RIGHT]  = lin_vel + (ang_vel * WheelSperation / 2);

    motor_Nrpm[LEFT]   = (int) (wheel_vel[LEFT] * VelocityConst * GearRatio);
    motor_Nrpm[RIGHT]  = (int) (wheel_vel[RIGHT] * VelocityConst * GearRatio);

    control_motor_Nrpm(LEFT, motor_Nrpm[LEFT]);
    control_motor_Nrpm(LEFT, motor_Nrpm[RIGHT]);
};

void control_motor_Nrpm(motor_id_t motor_id, int Nrpm) {
};

void drive_motor_duty(motor_id_t motor_id, float duty, motor_direction_t direction) {
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
};

unsigned short get_enc_count(motor_id_t motor_id) { 

};
