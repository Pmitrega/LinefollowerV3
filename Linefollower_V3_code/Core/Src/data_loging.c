#include "data_loging.h"

extern int velocity_left_int;
extern int velocity_right_int;

extern int desired_left_velocity;
extern int desired_right_velocity;
extern uint8_t black_detection_table[10];
void updateLogData(LogData* l_data){
    l_data->velocity_l = (uint16_t)velocity_left_int;
    l_data->velocity_r = (uint16_t)velocity_right_int;
    l_data->velocity_l_ctrl = (uint16_t)desired_left_velocity;
    l_data->velocity_r_ctrl = (uint16_t)desired_right_velocity;
    uint16_t sensors_data = 0;
    for(int i=0; i<10;i++){
        sensors_data += (uint16_t)(black_detection_table[i])<<i;
    }
    l_data->sensors = sensors_data;
}