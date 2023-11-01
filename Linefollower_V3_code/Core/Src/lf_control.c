#include "line_sensors.h"
#include "lf_control.h"

#define PIDLine_P_COEFF                     60.f
#define PIDLine_I_COEFF                     0.02f
#define PIDLide_D_COEFF                     0.0f

#define ANGLE_INTEGRAL_ANTIWINDUP_LIMIT     10000.f

int Linefollower_base_velocity = 1000;

extern int desired_left_velocity;
extern int desired_right_velocity;

void PIDLineControl(){
    float angle = EstimateAngle();
    static float angle_last = 0;
    static float angle_integral = 0;

    /*
    * Antiwindup of angle integral
    */
    if(angle_integral > ANGLE_INTEGRAL_ANTIWINDUP_LIMIT){
        angle_integral = ANGLE_INTEGRAL_ANTIWINDUP_LIMIT;
    }
    else if (angle_integral < -ANGLE_INTEGRAL_ANTIWINDUP_LIMIT)
    {
        angle_integral = -ANGLE_INTEGRAL_ANTIWINDUP_LIMIT;
    }
    
    
    int wheel_PID_difference = (int)(angle * PIDLine_P_COEFF) + (int)(angle_integral * PIDLine_I_COEFF) + (int)((angle - angle_last) * PIDLide_D_COEFF);

    desired_left_velocity  = Linefollower_base_velocity + wheel_PID_difference;
    desired_right_velocity = Linefollower_base_velocity - wheel_PID_difference;

    angle_integral += angle;
    angle_last = angle;

}


void NeuralNetworkControl(const float x1, float b_y1[2]){
  static const float c_a[30] = {
      -0.611334383F, -0.093021743F, 0.154598266F,   -0.00592965493F,
      -0.558128834F, -0.365341812F, -0.0810924694F, -0.472409576F,
      0.327980369F,  0.143744498F,  -0.916849077F,  1.14971578F,
      -0.694526076F, 0.734135032F,  0.399537534F,   -0.398710221F,
      0.469225824F,  -0.469425648F, -0.433311671F,  -0.456869215F,
      -0.448796839F, 0.152801305F,  0.0454706252F,  -0.342760354F,
      0.13244696F,   -0.715191543F, 0.0103295436F,  0.889197171F,
      -1.13870299F,  1.11083817F};
  static const float b_a[15] = {
      -21.222477F,  21.0002346F,  20.9999962F, -21.0F,      -21.0000019F,
      20.9951057F,  -20.8740177F, 19.3349819F, 20.4308586F, 20.9998531F,
      -21.0000019F, -21.0F,       21.0000572F, 20.9632492F, 21.1692848F};
  static const float fv[15] = {
      20.777523F,   -17.9997654F, -15.0000038F,  12.0F,       8.99999F,
      -6.05176F,    2.59724474F,  -0.599515736F, 1.14170587F, 6.00196171F,
      -8.99998474F, -12.0F,       14.9999409F,   18.0382309F, 20.8814335F};
  float c[15];
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
  a = (x1 - -251.0F) * 0.00398406386F;
  /*  Layer 1 */
  /*  Sigmoid Symmetric Transfer Function */
  /*  Layer 2 */
  for (k = 0; k < 15; k++) {
    c[k] = 2.0F / (expf(-2.0F * (b_a[k] * (a - 1.0F) + fv[k])) + 1.0F) - 1.0F;
  }
  /*  Output 1 */
  /*  Map Minimum and Maximum Output Reverse-Processing Function */
  for (k = 0; k < 2; k++) {
    a = 0.0F;
    for (i = 0; i < 15; i++) {
      a += c_a[k + (i << 1)] * c[i];
    }
    b_y1[k] = ((a + (-0.840357304F * (float)k + 0.526803136F)) - -1.0F) /
                  1.49719274F +
              (-0.0804166645F * (float)k - 0.210833326F);
  }
}
