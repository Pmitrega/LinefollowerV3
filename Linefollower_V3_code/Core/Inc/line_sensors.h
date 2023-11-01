#ifndef SENSORS_H
#define SENSORS_H

#define TURN_OFF_SENSORS()               HAL_GPIO_WritePin(SENSOR_ON_GPIO_Port, SENSOR_ON_Pin, 0)
#define TURN_ON_SENSORS()                HAL_GPIO_WritePin(SENSOR_ON_GPIO_Port, SENSOR_ON_Pin, 1)

float EstimateAngle();
float sens_to_float();
int check_no_detections();
#endif