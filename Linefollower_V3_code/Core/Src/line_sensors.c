#include "line_sensors.h"
#include "main.h"

extern uint16_t adc_readings[11];

const uint16_t *sensor_readings = adc_readings + 1;

#define BLACK_LINE_DETECTION_THRESHOLD 300 

#define LINE_DETECTED(i, sens_read) sens_read < (sensor_black_value[i] + BLACK_LINE_DETECTION_THRESHOLD)
#define BLACK_VALUES_SENSORS_BOARD_1 {790, 654, 549, 842, 654, 793 , 868, 808, 703, 655}
#define BLACK_VALUES_SENSORS_BOARD_2 {1049, 966, 811, 896, 1201, 1007 , 776, 834, 1029, 829}

#define WHITE_VALUESE_SENSORS_BOARD_1 {1697, 1738, 1489, 1381, 1492, 1891, 1711, 1991, 1840, 1628}
#define WHITE_VALUESE_SENSORS_BOARD_2 {1915, 1813, 1809, 1854, 2446, 1869, 1572, 1590, 1809, 1447}

uint16_t sensor_white_value[10] = WHITE_VALUESE_SENSORS_BOARD_2; 
uint16_t sensor_black_value[10] = BLACK_VALUES_SENSORS_BOARD_2;

uint16_t sensors_difference[10];

uint8_t black_detection_table[10] = {0};

float sensors_normalized[10];

int16_t angle_int = 0;

float EstimateAngle(){
    int num_of_detections = 0;
    float est_angle = 0;
    static float last_est_angle = 0;
    for(int i =0; i < 5; i++){
        if(LINE_DETECTED(i, sensor_readings[i])){
            black_detection_table[i] = 1;
            num_of_detections += 1;
            est_angle += 5.f*(5.f-i);
        }
        else{
            black_detection_table[i] = 0;
        }
    }
    for(int i =5; i < 10; i++){
        if(LINE_DETECTED(i, sensor_readings[i])){
            black_detection_table[i] = 1;
            num_of_detections += 1;
            est_angle += 5.f*(4.f-i);
        }
        else{
            black_detection_table[i] = 0;
        }
        
    }
    if (num_of_detections == 0 || num_of_detections  > 2){
        angle_int = (int16_t)last_est_angle;
        return last_est_angle;
    }
    else{
        last_est_angle = est_angle/num_of_detections;
        est_angle = est_angle/num_of_detections;
        angle_int = (int16_t)est_angle;
        return est_angle;
    }
}

float sens_to_float(){
    float s_read = 0;
    for(int i = 0; i <10; i++){
        if(black_detection_table[i]){
            s_read += (float)(2<<i);
        }
    }
    return s_read;
}


int check_no_detections(){
    static int no_detections = 0;
    int detected = 0;
    for(int i = 0 ;i<10;i++){
        if(black_detection_table[i]){
            detected = 1;
            break;
        }
    }
    if(detected == 0){
        no_detections+=1;
    }
    else{
        no_detections = 0;
    }
}
void init_sensors(){
    for(int i = 0; i <10; i++){
        sensors_difference[i] = sensor_white_value[i] - sensor_black_value[i];
    }
}

void normalize_sensors(){
    for(int i = 0; i <10; i++){
        sensors_normalized[i] = (float)(sensor_readings[i] - sensor_black_value[i])/(float)(sensors_difference[i]);
    }
}


float NN_est_angle(const float x1[10])
{
  static const float c_a[50] = {
      0.438877016F,   -2.3732543F,    0.34014228F,   1.053532F,
      -1.7622112F,    1.20398223F,    -0.467269391F, 0.221135244F,
      0.146651268F,   -0.12054643F,   -0.412275881F, 1.89414513F,
      0.483207583F,   0.542699039F,   -1.0079596F,   -1.0936687F,
      3.05453587F,    -0.0623623505F, 0.395897985F,  1.69488537F,
      -0.810512185F,  0.530098379F,   0.755641699F,  0.198740825F,
      -0.0942604765F, -1.22629786F,   1.20293272F,   0.835314274F,
      -0.162943125F,  -0.334832549F,  1.71068954F,   0.504535556F,
      -1.60725486F,   -0.708099663F,  -1.52235019F,  -0.440735489F,
      1.88973057F,    0.171732292F,   -0.561619163F, 0.34472543F,
      -0.436059058F,  -2.3775866F,    1.43734777F,   1.18672264F,
      0.250350088F,   -0.980162144F,  -0.436250329F, -0.532632F,
      -1.41952765F,   1.33086324F};
  static const float fv[10] = {0.048F, 0.138F, 0.101F, 0.069F, 0.099F,
                               0.095F, 0.091F, 0.103F, 0.102F, -0.004F};
  static const float fv1[10] = {
      1.66666663F, 1.85185182F, 2.002002F,   2.01207232F, 2.15517235F,
      2.13903737F, 1.98807156F, 2.01612902F, 1.97433364F, 1.83150184F};
  static const float b_a[5] = {-0.933151782F, -0.201601103F, -0.877828956F,
                               0.874153F, -0.409597814F};
  static const float fv2[5] = {-0.623202503F, 0.738350809F, -0.51550436F,
                               -0.902741849F, 1.68561542F};
  float xp1[10];
  float a;
  int i;
  int k;
  /*  ===== NEURAL NETWORK CONSTANTS ===== */
  /*  Input 1 */
  /*  Layer 1 */
  /*  Layer 2 */
  /*  Output 1 */
  /*  ===== SIMULATION ======== */
  /*  Dimensions */
  /*  samples */
  /*  Input 1 */
  /*  ===== MODULE FUNCTIONS ======== */
  /*  Map Minimum and Maximum Input Processing Function */
  for (k = 0; k < 10; k++) {
    xp1[k] = (x1[k] - fv[k]) * fv1[k] - 1.0F;
  }
  /*  Layer 1 */
  /*  Sigmoid Symmetric Transfer Function */
  /*  Layer 2 */
  /*  Output 1 */
  /*  Map Minimum and Maximum Output Reverse-Processing Function */
  a = 0.0F;
  for (k = 0; k < 5; k++) {
    float f;
    f = 0.0F;
    for (i = 0; i < 10; i++) {
      f += c_a[k + 5 * i] * xp1[i];
    }
    a += b_a[k] * (2.0F / (expf(-2.0F * (f + fv2[k])) + 1.0F) - 1.0F);
  }
  return ((a + 0.139174148F) - -1.0F) / 0.0217391308F - 43.0F;
}
