#ifndef BATTERY_H
#define BATTERY_H



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