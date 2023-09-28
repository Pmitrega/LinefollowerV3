#include "main.h"
#include "MPU6050.h"
#include <stdio.h>
#include "uart_handler.h"
#include "madgwick.h"

#define CALIBRATION_MEASSUREMENTS 30

#define WAIT_FOR_UART()         while(uart_ready == FALSE && timeout_value < WAIT_FOR_UART_TIMEOUT){\
                                timeout_value +=1;}\
                                uart_ready = FALSE

int acc_raw_readings[3];
int gyro_raw_readings[3];
float acc_readings[3];
float gyro_readings[3];
int meassurment_ready = FALSE;

float acc_calibration_param[3];
float gyro_calibration_param[3];

float gyro_calib[3];
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart1;
char uart_buffer_IMU[100];
extern uint8_t uart_ready;
extern float SEq_1, SEq_2, SEq_3, SEq_4;


void UpdateIMU(){
    meassurment_ready = FALSE;
    MPU_get_acc_burst(acc_raw_readings);
    MPU_get_gyro_burst(gyro_raw_readings);
    MPU_raw_to_SI(acc_raw_readings, gyro_raw_readings, acc_readings, gyro_readings);
    meassurment_ready = TRUE;
    gyro_calib[0] = gyro_readings[0] - gyro_calibration_param[0];
    gyro_calib[1] = gyro_readings[1] - gyro_calibration_param[1];
    gyro_calib[2] = gyro_readings[2] - gyro_calibration_param[2];
    filterUpdate(gyro_calib[0], gyro_calib[1], gyro_calib[2], acc_readings[0], acc_readings[1], acc_readings[2]);
}

void send_IMU_raw_string(){
  uint32_t timeout_value = 0;
  while(!meassurment_ready){}
  WAIT_FOR_UART();
  int l = sprintf(uart_buffer_IMU, "%d %d %d %d %d %d\n", acc_raw_readings[0], acc_raw_readings[1],acc_raw_readings[2], gyro_raw_readings[0], gyro_raw_readings[1], gyro_raw_readings[2]);
  HAL_UART_Transmit_IT(&huart1, uart_buffer_IMU, l);
}

void send_quaternion(){
  uint32_t timeout_value = 0;
  WAIT_FOR_UART();
  int l = sprintf(uart_buffer_IMU, "%d %d %d %d\n",(int)(SEq_1*1000.f),(int)(SEq_2*1000.f),(int)(SEq_3*1000.f),(int)(SEq_4*1000.f));
  HAL_UART_Transmit_IT(&huart1, uart_buffer_IMU, l);
}

void calibrate_gyro(){
  gyro_calibration_param[0] = 0.f;
  gyro_calibration_param[1] = 0.f;
  gyro_calibration_param[2] = 0.f;
  for(int i = 0; i < CALIBRATION_MEASSUREMENTS;i++){
    MPU_get_gyro_burst(gyro_raw_readings);
    MPU_raw_to_SI(acc_raw_readings, gyro_raw_readings, acc_readings, gyro_readings);
    gyro_calibration_param[0] += gyro_readings[0];
    gyro_calibration_param[1] += gyro_readings[1];
    gyro_calibration_param[2] += gyro_readings[2];
    HAL_Delay(15);
  }
  gyro_calibration_param[0] = gyro_calibration_param[0]/(float)CALIBRATION_MEASSUREMENTS;
  gyro_calibration_param[1] = gyro_calibration_param[1]/(float)CALIBRATION_MEASSUREMENTS;
  gyro_calibration_param[2] = gyro_calibration_param[2]/(float)CALIBRATION_MEASSUREMENTS;
}

