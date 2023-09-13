#include "uart_handler.h"
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
void print_int(int number){
  int l;
  l = sprintf(num_mess, "\n%d\n", number);
  HAL_UART_Transmit_IT(&huart1, num_mess, l);
}
void handle_command(uint8_t cmd,int number){
  static uint32_t timeout_value;
  if(cmd == 'l'){
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
  else if(cmd == 'b'){
    timeout_value = 0;
    while(uart_ready == FALSE && timeout_value < WAIT_FOR_UART_TIMEOUT){
      timeout_value +=1;
    }
    uint32_t adc_reading_uart = (uint32_t)((float)adc_readings[0] / 4096.0 * 3.3 / 2.34 * 7.5 *1000);
    int l = sprintf(uart_buffer_V, "B:%d.%d\r\n", adc_reading_uart/1000, adc_reading_uart%1000);
    HAL_UART_Transmit_IT(&huart1, uart_buffer_V, l);
  }  // all_handler(cmd, number);

  else if(cmd == 'k'){
    if(number == 1){
      sensors_enable = 1;
    }
    else if (number == 0)
    {
      sensors_enable = 0;
    }
    else{
      timeout_value = 0;
      while(uart_ready == FALSE && timeout_value < WAIT_FOR_UART_TIMEOUT){
        timeout_value +=1;
      }
      if(HAL_GPIO_ReadPin(SENSOR_ON_GPIO_Port, SENSOR_ON_Pin)){
        HAL_UART_Transmit_IT(&huart1, "K:1\r\n", 5);
      }
      else{
        HAL_UART_Transmit_IT(&huart1, "K:0\r\n", 5);
      }
    }
  }  // all_handler(cmd, number);
  else if(cmd == 's'){
    timeout_value = 0;
    while(uart_ready == FALSE && timeout_value < WAIT_FOR_UART_TIMEOUT){
      timeout_value +=1;
    }
    if(number >=0 && number < 11){
      int l = sprintf(uart_buffer_V, "s%d: %d\n\r",number, adc_readings[number]);
      HAL_UART_Transmit_IT(&huart1, uart_buffer_V, l);
    }
  }  // all_handler(cmd, number);
}
int num_ready = 0;
int num_pointer;

/**
* @brief Interpret command as letter and number, if there is no number as argument - can be used instead.
* @param huart 
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if(recieve >= 'a' && recieve <= 'z'){
    num_ready = 1;
    command = recieve;
    num_pointer = 0;
  }
  else if(num_ready && ((recieve >= '0' && recieve <='9') || recieve == '-') && num_pointer < 10){
    num_cmd[num_pointer] = recieve;
    num_pointer++;
  }
  else if(num_pointer > 0 && num_ready == 1){
    num_cmd[num_pointer] = '\0';
    handle_command(command, atoi(num_cmd));
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
