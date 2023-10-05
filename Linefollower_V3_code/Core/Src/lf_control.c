#include "line_sensors.h"
#include "lf_control.h"

#define PIDLine_P_COEFF         80.f
#define PIDLine_I_COEFF         0.02f
#define PIDLide_D_COEFF         0.0f

int Linefollower_base_velocity = 700;

extern int desired_left_velocity;
extern int desired_right_velocity;

void PIDLineControl(){
    float angle = EstimateAngle();
    desired_left_velocity = (int)(angle * PIDLine_P_COEFF) + Linefollower_base_velocity;
    desired_right_velocity = (int)(-angle * PIDLine_P_COEFF) + Linefollower_base_velocity;
}