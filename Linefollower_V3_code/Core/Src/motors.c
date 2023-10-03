#include "motors.h"

#define MOTOR_PULSE_PER_REVOLUTION (180.f)

#define ENCODER_SAMPLING_TIME           (0.001f)
#define WHEEL_DIAMATER                  (32.f)       /*WHEEL diamater in mm*/
#define PI_CONST                        (3.141592f)
#define DISTANCE_PER_ROTATION           (WHEEL_DIAMATER*PI_CONST)
#define ENCODER_LEFT                    (htim1.Instance->CNT)
#define ENCODER_RIGHT                   (htim3.Instance->CNT)

#define IMPULSE_TO_MMPS_COEFF            (DISTANCE_PER_ROTATION/MOTOR_PULSE_PER_REVOLUTION/ENCODER_SAMPLING_TIME)

#define ENCODER_INITIAL_VALUE           (200U)

#define ENCODER_OVERFLOW_THRESHOLD      (10000)



float left_motor_P = 16.f;
float left_motor_I = 0.3f;
float left_motor_D = 0.f;

float right_motor_P = 16.f;
float right_motor_I = 0.3f;
float right_motor_D = 0.f;


float velocity_left = 0;
float velocity_right = 0;

int velocity_left_int = 0;
int velocity_right_int = 0;

int desired_left_velocity;
int desired_right_velocity;


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim8;

#define SET_RIGHT_PWM(PWM_VALUE)        htim8.Instance->CCR3 = (PWM_VALUE)
#define SET_LEFT_PWM(PWM_VALUE)         htim8.Instance->CCR4 = (PWM_VALUE)

void InitEncoders(){
    htim1.Instance->CNT = ENCODER_INITIAL_VALUE;
    htim3.Instance->CNT = ENCODER_INITIAL_VALUE;
}

int RightEncoderDifference(){
    static uint16_t last_right_encoder = ENCODER_INITIAL_VALUE;
    int difference = (int)ENCODER_RIGHT - (int)last_right_encoder;
    last_right_encoder = ENCODER_RIGHT;
    if( difference < -ENCODER_OVERFLOW_THRESHOLD ){
        return difference + 65535;
    }
    else if(difference > ENCODER_OVERFLOW_THRESHOLD){
        return -65535 + difference;
    }
    else{
        return difference;
    }
    
}

int LeftEncoderDifference(){
    static uint16_t last_left_encoder = ENCODER_INITIAL_VALUE;
    int difference = (int)ENCODER_LEFT - (int)last_left_encoder;
    last_left_encoder = ENCODER_LEFT;
    if( difference < -ENCODER_OVERFLOW_THRESHOLD ){
        return difference + 65535;
    }
    else if(difference > ENCODER_OVERFLOW_THRESHOLD){
        return -65535 + difference;
    }
    else{
        return difference;
    }
    

}

/*
Adaptive velocity estimation with changing sampling time from 10 to 200ms
*/
void AdaptiveVelocityEstimation(){
    static uint16_t left_count = 0;
    static uint16_t right_count = 0;
    static int velocity_right_temp = 0;
    static int velocity_left_temp = 0;

    if( ((velocity_left_temp < 25) && (velocity_left_temp > -25) && left_count < 200) || left_count < 10 ){
        left_count +=1;
        velocity_left_temp += LeftEncoderDifference();
    }
    else{
        velocity_left = (float)velocity_left_temp/(float)left_count*IMPULSE_TO_MMPS_COEFF;
        velocity_left_int = (int)velocity_left;
        // velocity_left_int = (int)left_count;
        velocity_left_temp = 0;
        left_count = 0;
    }
    if( ((velocity_right_temp < 25) && (velocity_right_temp > -25) && right_count < 200) || right_count < 10){
        right_count +=1;
        velocity_right_temp += RightEncoderDifference();
    }
    else{
        velocity_right = (float)velocity_right_temp/(float)right_count*IMPULSE_TO_MMPS_COEFF;
        velocity_right_int = (int)velocity_right;
        // velocity_right_int = (int)right_count;
        velocity_right_temp = 0;
        right_count = 0;
    }
}


/** @brief Sets Left motor PWM with direction.
 *
 *  @param PMWValue integer from -10000 to 10000
 */
void SetLeftMotorPWM(int PWMValue){
    if(PWMValue >= 0){
        if(PWMValue > 10000){
            PWMValue = 10000;
        }
        SET_LEFT_DIRECTION(FORWARD);
        SET_LEFT_PWM(PWMValue);
    }
    else{
        PWMValue = -PWMValue;
        if(PWMValue > 10000){
            PWMValue = 10000;
        }
        SET_LEFT_DIRECTION(BACKWARD);
        SET_LEFT_PWM(PWMValue);

    }
    
}

/** @brief Sets Right motor PWM with direction.
 *
 *  @param PMWValue integer from -10000 to 10000
 */
void SetRightMotorPWM(int PWMValue){
    if(PWMValue >= 0){
        if(PWMValue > 10000){
            PWMValue = 10000;
        }
        SET_RIGHT_DIRECTION(FORWARD);
        SET_RIGHT_PWM(PWMValue);
    }
    else{
        PWMValue = -PWMValue;
        if(PWMValue > 10000){
            PWMValue = 10000;
        }
        SET_RIGHT_DIRECTION(BACKWARD);
        SET_RIGHT_PWM(PWMValue);

    }
    
}

/*
Should be called every 1 ms
*/
void LeftMotorPID(){
    int control_value = 0;
    static float error_integral = 0;
    if(error_integral > 20000){
        error_integral = 20000;
    }
    else if(error_integral < -20000){
        error_integral = -20000;
    }
    float error =(float) (desired_left_velocity - velocity_left_int);
    control_value = (int)(left_motor_P * error + error_integral * left_motor_I);
    error_integral += error;
    SetLeftMotorPWM(control_value);
}

void RightMotorPID(){

}

/*
Should be called every 1 ms
*/