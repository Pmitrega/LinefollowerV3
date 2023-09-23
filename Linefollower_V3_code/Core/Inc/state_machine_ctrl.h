#ifndef STATE_MACHINE_CTRL_H
#define STATE_MACHINE_CTRL_H
#include "main.h"
enum RobotMode{
    Robot_InitMode = 0,
    Robot_ManualMode,
    Robot_AutoMode
};
typedef enum RobotMode RobotMode;
void ManageRobotStateMachine(uint8_t command, int number);

#endif