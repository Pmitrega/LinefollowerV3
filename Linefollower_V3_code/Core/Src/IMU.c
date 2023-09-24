#include "main.h"
#include "MPU6050.h"
#include <stdio.h>
#include "uart_handler.h"
#include "madgwick.h"

#define WAIT_FOR_UART()         while(uart_ready == FALSE && timeout_value < WAIT_FOR_UART_TIMEOUT){\
                                timeout_value +=1;}\
                                uart_ready = FALSE

int acc_raw_readings[3];
int gyro_raw_readings[3];
float acc_readings[3];
float gyro_readings[3];
int meassurment_ready = FALSE;

extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
char uart_buffer_IMU[100];
extern uint8_t uart_ready;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim == &htim7){
    meassurment_ready = FALSE;
    MPU_get_acc_burst(acc_raw_readings);
    MPU_get_gyro_burst(gyro_raw_readings);
    MPU_raw_to_SI(acc_raw_readings, gyro_raw_readings, acc_readings, gyro_readings);
    meassurment_ready = TRUE;
    filterUpdate(gyro_readings[0], gyro_readings[1], gyro_readings[2], acc_readings[0], acc_readings[1], acc_readings[2]);
  }
}

void send_IMU_raw_string(){
    uint32_t timeout_value = 0;
    while(!meassurment_ready){}
    WAIT_FOR_UART();
    int l = sprintf(uart_buffer_IMU, "%d %d %d %d %d %d\n", acc_raw_readings[0], acc_raw_readings[1],acc_raw_readings[2], gyro_raw_readings[0], gyro_raw_readings[1], gyro_raw_readings[2]);
    HAL_UART_Transmit_IT(&huart1, uart_buffer_IMU, l);
}