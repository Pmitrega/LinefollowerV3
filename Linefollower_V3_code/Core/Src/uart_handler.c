#include "uart_handler.h"
#include "line_sensors.h"
#include "state_machine_ctrl.h"

extern UART_HandleTypeDef huart1;
extern uint16_t adc_readings[];
uint8_t command;
uint8_t num_cmd[10];
uint8_t num_mess[10];
uint8_t recieve;
uint8_t uart_ready = TRUE;
uint8_t uart_buffer_V[64];
uint8_t sensors_enable = 0;
extern int auto_control_flag;
extern char uart_buffer[100];

/**
* @brief Function below is called from HAL_UART_RxCpltCallback
*        when proper command has been recieved
* @param command - letters from a-z
* @param number  - number from 0 to 9 999 999 999
* @related HAL_UART_RxCpltCallback
*/
void handle_command(uint8_t cmd,int number){
  ManageRobotStateMachine(cmd, number);
}

/**
* @brief Interpret command as letter and number, if there is no number as argument - can be used instead.
* @param huart 
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  static int num_ready = 0;
  static int num_pointer;

  if(recieve >= 'a' && recieve <= 'z'){
    num_ready = 1;
    command = recieve;
    num_pointer = 0;
  }
  else if(num_ready && ((recieve >= '0' && recieve <='9') || recieve == '-') && num_pointer < 10){
    num_cmd[num_pointer] = recieve;
    num_pointer++;
  }
  else if(num_ready == 1){
    num_cmd[num_pointer] = '\0';
    if(num_cmd[0] != '-'){
      handle_command(command, atoi(num_cmd));
    }
    else{
      handle_command(command, '-');
    }
    num_ready = 0;
  }
  else{
    num_ready = 0;
  }
  HAL_UART_Receive_IT(&huart1, &recieve, 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  uart_ready = TRUE;
}
