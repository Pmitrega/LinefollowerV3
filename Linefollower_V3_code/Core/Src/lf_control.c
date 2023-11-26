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
  static const float b_a[60] = {
      -2.22478366F,  -0.339556515F,  7.6515336F,   1.12703288F,
      -3.51257443F,  1.83612049F,    0.693418622F, -1.76672971F,
      8.10961533F,   1.19000232F,    11.7193279F,  0.25310269F,
      1.59978187F,   -3.51222944F,   2.20193553F,  -5.45474243F,
      -1.54600668F,  0.00903928652F, -3.07775927F, -2.36416316F,
      3.55161858F,   2.59043431F,    0.161278576F, 3.35606456F,
      0.0228407364F, 2.07946682F,    4.76360655F,  -2.39311481F,
      -1.22320342F,  -1.21627951F,   0.278550178F, -3.87099F,
      5.61472273F,   -1.31145573F,   3.15537429F,  -0.230272979F,
      -1.82104969F,  1.84416485F,    -1.47074699F, 2.23635173F,
      0.586611569F,  1.72230327F,    0.451570183F, 2.58783793F,
      -2.59926748F,  -2.6520803F,    1.73902273F,  -2.16236615F,
      -0.985343754F, -3.5485096F,    0.64264518F,  3.37790346F,
      -7.30901384F,  -0.0800632313F, -4.44884539F, -2.20023394F,
      2.14991355F,   4.56586504F,    1.82668221F,  -2.86143255F};
  static const float c_a[40] = {
      1.84349906F,   -1.21657145F, -0.562434F,    1.39022911F,   1.72522426F,
      -3.76837039F,  0.741367936F, -1.0588814F,   -1.16266155F,  0.808187783F,
      2.00413251F,   -1.20733488F, -0.67788142F,  0.519482136F,  -0.244204581F,
      -1.71534944F,  0.83104986F,  2.16366768F,   -0.450980753F, 0.233402923F,
      -3.36831427F,  1.786376F,    1.00513518F,   -0.61427784F,  0.333198577F,
      -0.259794265F, 2.17899728F,  0.393962F,     0.369519472F,  -0.2493359F,
      -2.71731377F,  0.967753172F, -0.993757546F, 0.267461836F,  -0.150321573F,
      0.313299447F,  0.940242F,    0.529206812F,  1.57389545F,   -1.02243495F};
  static const float fv[20] = {
      3.34774208F,   1.59125292F,  -0.79900229F, -1.8733722F,  1.42239738F,
      -1.61795223F,  -2.75403833F, 1.1053673F,   0.158904836F, -0.264820784F,
      -0.365849406F, 2.39371228F,  2.0726521F,   -2.1991539F,  3.36594796F,
      -1.86442041F,  -3.14797688F, -1.66445529F, -3.74790525F, -4.55258656F};
  static const float fv1[2] = {1.34261477F, -0.456573427F};
  float a[20];
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
  for (k = 0; k < 20; k++) {
    a[k] =
        2.0F / (expf(-2.0F * (((b_a[k] * xp1_idx_0 + b_a[k + 20] * xp1_idx_1) +
                               b_a[k + 40] * xp1_idx_2) +
                              fv[k])) +
                1.0F) -
        1.0F;
  }
  /*  Output 1 */
  /*  Map Minimum and Maximum Output Reverse-Processing Function */
  for (k = 0; k < 2; k++) {
    xp1_idx_0 = 0.0F;
    for (i = 0; i < 20; i++) {
      xp1_idx_0 += c_a[k + (i << 1)] * a[i];
    }
    b_y1[k] = ((xp1_idx_0 + fv1[k]) - -1.0F) /
                  (2.20934453E-7F * (float)k + 9.89805E-5F) -
              9999.0F;
  }
}