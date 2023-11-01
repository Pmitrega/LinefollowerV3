#ifndef DATA_LOGING_H
#define DATA_LOGING_H
#include "main.h"
struct LogData
{
    int16_t velocity_l;
    int16_t velocity_r;
    int16_t l_PWM;
    int16_t r_PWM;
    int16_t sensors;
    uint16_t end;
};
typedef struct LogData LogData;

void updateLogData(LogData* l_data);



#endif