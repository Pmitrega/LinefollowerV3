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

float velocity_left = 0;
float velocity_right = 0;

int velocity_left_int = 0;
int velocity_right_int = 0;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

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