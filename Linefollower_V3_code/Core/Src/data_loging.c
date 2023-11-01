#include "data_loging.h"

extern int velocity_left_int;
extern int velocity_right_int;

extern int desired_left_velocity;
extern int desired_right_velocity;
extern int est_angle;
extern int right_PWM_val;
extern int left_PWM_val;
extern uint8_t black_detection_table[10];
void updateLogData(LogData* l_data){
    l_data->velocity_l = (uint16_t)velocity_left_int;
    l_data->velocity_r = (uint16_t)velocity_right_int;
    l_data->l_PWM = (uint16_t)right_PWM_val;
    l_data->r_PWM = (uint16_t)left_PWM_val;
    uint16_t sensors_data = 0;
    // for(int i=0; i<10;i++){
    //     sensors_data += (uint16_t)(black_detection_table[i])<<i;
    // }
    l_data->sensors = (int16_t)est_angle;
}