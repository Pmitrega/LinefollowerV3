#ifndef BATTERY_H
#define BATTERY_H

#define TURN_OFF_SENSORS()               HAL_GPIO_WritePin(SENSOR_ON_GPIO_Port, SENSOR_ON_Pin, 0)
#define TURN_ON_SENSORS()                HAL_GPIO_WritePin(SENSOR_ON_GPIO_Port, SENSOR_ON_Pin, 1)

enum BatteryState{
    BatteryLow,
    BatteryMiddle,
    BatteryHigh,
    BatteryNotInit
    };
typedef enum BatteryState BatteryState;


void battery_monitor();
int get_battery_voltage_string(char* buffer);

#endif