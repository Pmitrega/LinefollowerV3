#include "line_sensors.h"
#include "main.h"

extern uint16_t adc_readings[11];

const uint16_t *sensor_readings = adc_readings + 1;

#define BLACK_LINE_DETECTION_THRESHOLD 200

#define LINE_DETECTED(i, sens_read) sens_read > (sensor_black_value[i] + BLACK_LINE_DETECTION_THRESHOLD)

uint16_t sensor_white_value[10] = {1697, 1738, 1489, 1381, 1492,
                                   1891, 1711, 1991, 1840, 1628}; 
uint16_t sensor_black_value[10] = {1023, 836, 589, 534, 601,
                                   868 , 770, 931, 993, 911};

uint8_t black_detection_table[10] = {0};


float EstimateAngle(){
    for(int i =0; i < 10; i++){
        black_detection_table[i] = LINE_DETECTED(i, sensor_readings[i]);
    }
    return (float)sensor_readings[0];
}