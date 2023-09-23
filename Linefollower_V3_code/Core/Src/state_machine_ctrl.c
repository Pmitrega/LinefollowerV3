#include "state_machine_ctrl.h"
#include "main.h"
#include "battery.h"
#include "uart_handler.h"
#include <stdio.h>
#include "line_sensors.h"

#define WAIT_FOR_UART()         while(uart_ready == FALSE && timeout_value < WAIT_FOR_UART_TIMEOUT){\
                                timeout_value +=1;}
RobotMode robot_state = Robot_InitMode;


uint8_t robot_auto_follow = 0;
extern uint16_t adc_readings[];
extern UART_HandleTypeDef huart1;
char uart_buffer_SM[100];
extern uint8_t uart_ready;

void RobotManualModeManager(uint8_t command, int number){
    uint32_t timeout_value = 0;
    if(command == 'k'){
        if(number == 0){
            TURN_OFF_SENSORS();
        }
        else if(number == 1){
            TURN_ON_SENSORS();
        }
        else{
            /*
            Gets current state of line sensors
            */
            WAIT_FOR_UART();
            if(HAL_GPIO_ReadPin(SENSOR_ON_GPIO_Port, SENSOR_ON_Pin)){
                WAIT_FOR_UART();
                HAL_UART_Transmit_IT(&huart1, "K1\n", 20);
            }
            else{
                WAIT_FOR_UART();
                HAL_UART_Transmit_IT(&huart1, "K0\n", 20);
            }
        }
    }
    else if(command == 'm'){
        switch (number)
        {
        case Robot_AutoMode:
            WAIT_FOR_UART();
            HAL_UART_Transmit_IT(&huart1, "Entering Auto Mode\n", 20);
            robot_state = Robot_AutoMode;
            break;
        default:
            break;
        }
    }
    else if(command == 'd'){
        if(number == 2){
            HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        }   
        if(number == 3){
            HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        }   
        if(number == 4){
            HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
        }    
    }
    else if(command == 'b'){
        WAIT_FOR_UART();
        int l = get_battery_voltage_string(uart_buffer_SM);
        HAL_UART_Transmit_IT(&huart1, uart_buffer_SM, l);
    }

}

void RobotAutoModeManager(uint8_t command, int number){
    uint32_t timeout_value = 0;
    if(command == 's'){
        if(number == 1){
            /*
            Here we will enable linefollowing
            */
           robot_auto_follow = 1;
        }
        else if(number == 0){
            /*
            Here we will disable linefollower
            */
           robot_auto_follow = 0;
        }
    }
    else if(command == 'm'){
        switch (number)
        {
        case Robot_ManualMode:
            /*remember to disable lf auto follow*/
            WAIT_FOR_UART();
            HAL_UART_Transmit_IT(&huart1, "Entering Manual Mode\n", 22);
            robot_state = Robot_ManualMode;
            break;
        default:
            break;
        }
    }
    else if(command == 'b'){
        WAIT_FOR_UART();
        int l = get_battery_voltage_string(uart_buffer_SM);
        HAL_UART_Transmit_IT(&huart1, uart_buffer_SM, l);
    }
    else if(command == 'l'){
        if(number == 2){
            HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        }   
        if(number == 3){
            HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        }   
        if(number == 4){
            HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
        }    
    }
}

void ManageRobotStateMachine(uint8_t command, int number){
    switch (robot_state)
    {
    case Robot_InitMode:
        robot_state = Robot_ManualMode;
        break;
    
    case Robot_ManualMode:
        RobotManualModeManager(command, number);
        break;

    case Robot_AutoMode:
        RobotAutoModeManager(command, number);
        break;
        
    default:
        break;
    }
}