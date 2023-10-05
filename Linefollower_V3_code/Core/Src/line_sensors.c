#include "line_sensors.h"
#include "main.h"

extern uint16_t adc_readings[11];

const uint16_t *sensor_readings = adc_readings + 1;

#define BLACK_LINE_DETECTION_THRESHOLD 300 

#define LINE_DETECTED(i, sens_read) sens_read < (sensor_black_value[i] + BLACK_LINE_DETECTION_THRESHOLD)

uint16_t sensor_white_value[10] = {1697, 1738, 1489, 1381, 1492,
                                   1891, 1711, 1991, 1840, 1628}; 
uint16_t sensor_black_value[10] = {1023, 836, 589, 534, 601,
                                   868 , 770, 931, 993, 911};

uint8_t black_detection_table[10] = {0};


float EstimateAngle(){
    int num_of_detections = 0;
    float est_angle = 0;
    static float last_est_angle = 0;
    for(int i =0; i < 10; i++){
        if(LINE_DETECTED(i, sensor_readings[i])){
            black_detection_table[i] = 1;
            num_of_detections += 1;
            est_angle += (4.5f-(float)i)*5.f;
        }
        else{
            black_detection_table[i] = 0;
        }
        
    }
    if (num_of_detections == 0){
        return last_est_angle;
    }
    else{
        last_est_angle = est_angle;
        return est_angle;
    }
}

