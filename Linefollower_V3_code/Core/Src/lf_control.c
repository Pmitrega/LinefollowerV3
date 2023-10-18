#include "line_sensors.h"
#include "lf_control.h"

#define PIDLine_P_COEFF                     80.f
#define PIDLine_I_COEFF                     0.02f
#define PIDLide_D_COEFF                     0.0f

#define ANGLE_INTEGRAL_ANTIWINDUP_LIMIT     10000.f

int Linefollower_base_velocity = 700;

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