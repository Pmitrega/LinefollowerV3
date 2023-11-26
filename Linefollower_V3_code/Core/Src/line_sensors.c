#include "line_sensors.h"
#include "main.h"
#include <math.h>

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
    // GENRATED BY MATLAB 12.12.1996
  static const float c_a[100] = {
      -0.3301301F,   -1.66143596F,   -0.0806368664F, -0.498616517F,
      1.10668015F,   0.145152897F,   -2.90897846F,   2.33696771F,
      -1.40883803F,  1.55282795F,    -1.63277388F,   -1.22712827F,
      -0.884316325F, 0.0316828452F,  -1.36614358F,   -0.217217714F,
      0.110406846F,  1.34284484F,    -0.352770567F,  0.54896915F,
      -1.96243453F,  -0.213950083F,  -2.14414978F,   0.0321652517F,
      0.752376318F,  0.238695621F,   0.214566872F,   0.968249559F,
      0.790450037F,  0.554531693F,   0.4648121F,     0.00573264342F,
      -1.07078815F,  -0.170441866F,  -0.0672816709F, 0.134406149F,
      -1.42027462F,  -0.0331234746F, -0.859829783F,  0.11765679F,
      -0.907047331F, -0.237630174F,  -2.14929366F,   0.441426128F,
      0.707019448F,  0.516481638F,   3.74849844F,    0.106310368F,
      1.61928022F,   1.30910337F,    -0.787404F,     -1.59235275F,
      -1.581321F,    -0.650078F,     0.845954776F,   1.01760614F,
      1.95994139F,   -2.41873884F,   0.358477116F,   0.616539121F,
      0.342536569F,  1.0944308F,     0.481100649F,   0.233556256F,
      -0.641996503F, -0.274639487F,  -0.413659453F,  -0.673384488F,
      -0.422833145F, 1.02981925F,    -0.180702582F,  0.774131954F,
      1.02074742F,   0.569200873F,   -0.240594864F,  -1.00194752F,
      0.288344562F,  0.482417554F,   1.84880805F,    1.46176577F,
      -1.51290464F,  0.24527292F,    -1.1194936F,    2.07018948F,
      0.130978659F,  -0.882609427F,  2.05379629F,    -0.375094801F,
      0.192062393F,  -0.092039682F,  0.219530389F,   0.223229706F,
      1.91394424F,   -2.03730488F,   0.641127825F,   1.96290684F,
      -0.590932131F, -2.08990502F,   1.01092255F,    1.0096879F};
  static const float b_a[10] = {0.163318381F,  -0.282346815F, -0.0393550806F,
                                -0.655826092F, 0.233433992F,  -0.681411326F,
                                0.120230101F,  0.146932706F,  0.233500674F,
                                0.0499952249F};
  static const float fv[10] = {0.028F, 0.066F, 0.04F,  -0.002F, 0.017F,
                               0.011F, 0.017F, 0.013F, 0.026F,  -0.004F};
  static const float fv1[10] = {
      1.63532293F, 1.71232879F, 1.85185182F, 1.84672213F, 1.98019803F,
      1.96270859F, 1.84842885F, 1.82648396F, 1.79211473F, 1.7574693F};
  static const float fv2[10] = {
      -2.02871013F, 2.05686855F,  0.511588395F,  0.731246F,  -0.233325809F,
      -0.49698323F, -0.98434186F, -0.387874365F, 1.5258534F, 2.49007344F};
  float fcnOutput[10];
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
    fcnOutput[k] = (x1[k] - fv[k]) * fv1[k] - 1.0F;
  }
  /*  Layer 1 */
  /*  Sigmoid Symmetric Transfer Function */
  /*  Layer 2 */
  /*  Output 1 */
  /*  Map Minimum and Maximum Output Reverse-Processing Function */
  a = 0.0F;
  for (k = 0; k < 10; k++) {
    float f;
    f = 0.0F;
    for (i = 0; i < 10; i++) {
      f += c_a[k + 10 * i] * fcnOutput[i];
    }
    a += b_a[k] * (2.0F / (expf(-2.0F * (f + fv2[k])) + 1.0F) - 1.0F);
  }
  return ((a + 0.240477085F) - -1.0F) / 0.0217391308F - 43.0F;
}
