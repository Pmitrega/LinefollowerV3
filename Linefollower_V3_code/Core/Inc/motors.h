#ifndef MOTORS_H
#define MOTORS_H

#include "main.h"
void InitEncoders();
int AdaptiveVelocityEstimation();
int VelocityEstimation();
void SetLeftMotorPWM(int PWMValue);
void SetRightMotorPWM(int PWMValue);
void LeftMotorPID();
void RightMotorPID();
#define ENCODER_LEFT                    (htim1.Instance->CNT)
#define ENCODER_RIGHT                   (htim3.Instance->CNT)



enum MotorDirection{
    BACKWARD = 0,
    FORWARD = 1
};

#define SELECT_MOTOR_DIRECTION(PWMValue)       (PWMValue > 0)

#define SET_LEFT_DIRECTION(DIRECTION)          HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin, !(DIRECTION));\
                                               HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, (DIRECTION))

#define SET_RIGHT_DIRECTION(DIRECTION)         HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, (DIRECTION));\
                                               HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, !(DIRECTION))



#endif