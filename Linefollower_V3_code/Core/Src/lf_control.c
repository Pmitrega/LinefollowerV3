#include "line_sensors.h"
#include "lf_control.h"

#define PIDLine_P_COEFF                     100.f
#define PIDLine_I_COEFF                     0.04f
#define PIDLide_D_COEFF                     0.0f

#define ANGLE_INTEGRAL_ANTIWINDUP_LIMIT     10000.f

int Linefollower_base_velocity = 1400;

extern int desired_left_velocity;
extern int desired_right_velocity;
extern float sensors_normalized[10];
extern float est_angle;

void PIDLineControl(){
    float angle = est_angle;
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


void NeuralNetworkControl(const float x1[3], float b_y1[2])
{
  static const float b_a[30] = {
      1.59431708F,  -8.5313549F,  8.78552151F,  11.5216932F,   14.9134178F,
      -30.594408F,  5.22555256F,  1.03992498F,  -0.699163139F, 1.42660379F,
      1.39891124F,  -4.66905975F, 0.572652F,    -4.15293074F,  7.13338232F,
      1.25220323F,  0.736937523F, 11.6238146F,  -1.31389463F,  1.56326556F,
      -1.51906729F, 3.90889883F,  0.866806209F, 0.295023412F,  -8.29672432F,
      1.51385224F,  0.478343785F, -5.08417749F, 0.615403056F,  -4.56265926F};
  static const float c_a[20] = {
      -1.6975497F,  0.229892239F,  0.168388382F, -0.256530344F, -3.89578795F,
      3.90195751F,  0.408338338F,  0.110047251F, -0.174106881F, 0.193362892F,
      0.155314356F, -0.344206333F, 3.84677219F,  -4.94162416F,  -0.135667786F,
      0.137450397F, -3.27864456F,  -1.51881599F, -0.20977217F,  0.336892426F};
  static const float fv[10] = {
      -2.69243884F, 2.88940525F,   -0.204791978F, 2.32627058F, -0.555990517F,
      0.110017508F, -0.195391268F, 4.72054911F,   2.26765752F, 3.93945312F};
  float a[10];
  float xp1_idx_0;
  float xp1_idx_1;
  float xp1_idx_2;
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
  xp1_idx_0 = (x1[0] - -251.0F) * 0.00398406386F - 1.0F;
  xp1_idx_1 = (x1[1] - -557.0F) * 0.000584966387F - 1.0F;
  xp1_idx_2 = (x1[2] - -1046.0F) * 0.000584966387F - 1.0F;
  /*  Layer 1 */
  /*  Sigmoid Symmetric Transfer Function */
  /*  Layer 2 */
  for (k = 0; k < 10; k++) {
    a[k] =
        2.0F / (expf(-2.0F * (((b_a[k] * xp1_idx_0 + b_a[k + 10] * xp1_idx_1) +
                               b_a[k + 20] * xp1_idx_2) +
                              fv[k])) +
                1.0F) -
        1.0F;
  }
  /*  Output 1 */
  /*  Map Minimum and Maximum Output Reverse-Processing Function */
  for (k = 0; k < 2; k++) {
    xp1_idx_0 = 0.0F;
    for (i = 0; i < 10; i++) {
      xp1_idx_0 += c_a[k + (i << 1)] * a[i];
    }
    b_y1[k] = ((xp1_idx_0 + (-0.250003219F * (float)k + 1.73587227F)) - -1.0F) /
                  (2.20934453E-7F * (float)k + 9.89805E-5F) -
              9999.0F;
  }
}