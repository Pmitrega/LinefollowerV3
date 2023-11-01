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


void NeuralNetworkControl(const float x1[3], float b_y1[6]){
  static const float b_a[60] = {
      -3.43357277F, -14.5573101F, -4.44120836F, 0.0105262026F, -3.05893F,
      -3.46567464F, 3.7678628F,   2.52059698F,  0.039341066F,  13.0602F,
      -32.3383064F, 7.24928188F,  1.05722702F,  -15.8697767F,  3.18784642F,
      -9.49895191F, -3.19554806F, 11.06812F,    0.0265824068F, -8.13119221F,
      11.9761801F,  8.98114204F,  1.56614602F,  -1.92875886F,  -6.081429F,
      1.41074979F,  1.29252F,     -3.025635F,   -4.5990653F,   -7.08337069F,
      10.7294579F,  6.11043692F,  0.366618931F, -7.63457727F,  1.66416812F,
      -9.4138422F,  1.86586726F,  -28.3304119F, 1.21766484F,   -11.2584972F,
      -1.18843532F, 5.94090748F,  10.1653786F,  -5.28733826F,  5.79345322F,
      6.8570838F,   -0.95951283F, -1.07854247F, -0.179478854F, -5.4883666F,
      7.84105778F,  1.46944976F,  10.4305286F,  23.2236843F,   0.64492476F,
      -13.9479303F, 4.54600954F,  0.341382951F, -7.14387512F,  14.0487576F};
  static const float fv[60] = {
      12.1511402F,  2.21281528F,  4.24625921F,  4.24111128F,  4.34212446F,
      2.36996126F,  -2.35803366F, -2.03364348F, 0.75756979F,  12.7686396F,
      -10.5247736F, 1.3026036F,   -4.22124863F, -7.12121153F, 0.761074603F,
      -8.86613274F, -3.6849122F,  -13.6452093F, -9.58823872F, -10.4120617F,
      12.1511402F,  2.21281528F,  4.24625921F,  4.24111128F,  4.34212446F,
      2.36996126F,  -2.35803366F, -2.03364348F, 0.75756979F,  12.7686396F,
      -10.5247736F, 1.3026036F,   -4.22124863F, -7.12121153F, 0.761074603F,
      -8.86613274F, -3.6849122F,  -13.6452093F, -9.58823872F, -10.4120617F,
      12.1511402F,  2.21281528F,  4.24625921F,  4.24111128F,  4.34212446F,
      2.36996126F,  -2.35803366F, -2.03364348F, 0.75756979F,  12.7686396F,
      -10.5247736F, 1.3026036F,   -4.22124863F, -7.12121153F, 0.761074603F,
      -8.86613274F, -3.6849122F,  -13.6452093F, -9.58823872F, -10.4120617F};
  static const float c_a[40] = {
      6.7025156F,   -0.0589261465F, 4.46148968F,   7.53365278F,
      -3.24764657F, -0.284515381F,  -0.410080403F, -2.22999525F,
      -3.79047704F, -1.01714528F,   5.59277725F,   1.08408034F,
      -1.18382F,    -0.104606129F,  4.08666468F,   2.37334418F,
      13.2193174F,  -4.03923F,      0.595130205F,  5.70177889F,
      -2.2462008F,  -11.1828833F,   -1.6706754F,   2.90820217F,
      -1.87947762F, -0.639497042F,  1.59017599F,   -0.13292563F,
      8.31979179F,  -8.47649574F,   -0.955450892F, 0.00634149183F,
      1.51454198F,  0.753843069F,   -8.56440353F,  -0.671775043F,
      1.44845533F,  -3.94342971F,   -2.5567081F,   -0.288243711F};
  static const float fv1[6] = {-0.133401647F, -2.75683689F,  -0.133401647F,
                               -2.75683689F,  -0.133401647F, -2.75683689F};
  float a[60];
  float xp1[9];
  float a2[6];
  float f;
  int i;
  int i1;
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
  for (k = 0; k < 3; k++) {
    f = x1[k];
    xp1[3 * k] = f * 0.0732667819F - 1.0F;
    xp1[3 * k + 1] = f * 0.0732600763F - 1.0F;
    xp1[3 * k + 2] = f * 0.00195503421F - 1.0F;
  }
  /*  Layer 1 */
  /*  Sigmoid Symmetric Transfer Function */
  for (i = 0; i < 20; i++) {
    float f1;
    float f2;
    f = b_a[i];
    f1 = b_a[i + 20];
    f2 = b_a[i + 40];
    for (i1 = 0; i1 < 3; i1++) {
      k = i + 20 * i1;
      a[k] = ((f * xp1[3 * i1] + f1 * xp1[3 * i1 + 1]) + f2 * xp1[3 * i1 + 2]) +
             fv[k];
    }
  }
  /*  Layer 2 */
  for (k = 0; k < 60; k++) {
    a[k] = 2.0F / (expf(-2.0F * a[k]) + 1.0F) - 1.0F;
  }
  for (i = 0; i < 2; i++) {
    for (i1 = 0; i1 < 3; i1++) {
      f = 0.0F;
      for (k = 0; k < 20; k++) {
        f += c_a[i + (k << 1)] * a[k + 20 * i1];
      }
      k = i + (i1 << 1);
      a2[k] = f + fv1[k];
    }
  }
  /*  Output 1 */
  /*  Map Minimum and Maximum Output Reverse-Processing Function */
  for (k = 0; k < 3; k++) {
    i = k << 1;
    f = (a2[i] - -1.0F) / 0.0732556F;
    a2[i] = f;
    b_y1[i] = f;
    f = (a2[i + 1] - -1.0F) / 0.0732477754F;
    a2[i + 1] = f;
    b_y1[i + 1] = f + 0.00125F;
  }
}
