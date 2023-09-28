#ifndef MOTORS_H
#define MOTORS_H

#include "main.h"
void InitEncoders();
void AdaptiveVelocityEstimation();
#define ENCODER_LEFT                    (htim1.Instance->CNT)
#define ENCODER_RIGHT                   (htim3.Instance->CNT)

#endif