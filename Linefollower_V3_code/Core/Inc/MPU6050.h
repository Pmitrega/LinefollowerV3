#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f4xx_hal.h"

/*
    TO BE CONFIGURATED
    MPU6050_ADDR -- I2C address of device
    MPU6050_I2C_PORT -- HAL I2C port 
*/
#define MPU6050_ADDR 0x69 << 1
#define MPU6050_I2C_PORT hi2c1


#define MPU6050_CONFIG 0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACC_CONFIG 0x1C

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C

#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E

#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40


#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44

#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46

#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48


#define MPU6050_TEMP_OUT_H 0x43
#define MPU6050_TEMP_OUT_L 0x44

extern I2C_HandleTypeDef MPU6050_I2C_PORT;

enum AXIS{
    X,
    Y,
    Z
};

/*
    configuration options of GYRO sensivity from 200 to 20000 degrees per second
*/
typedef enum {
    MPU6050_ACC_2G = 0x00U,
    MPU6050_ACC_4G = 0x01U,
    MPU6050_ACC_8G = 0x02U,
    MPU6050_ACC_16G = 0x03U
}MPU6050_ACC_RANGE;

/*
    configuration options of GYRO sensivity from +-200 to +-2000 degrees per second
*/
typedef enum {
    MPU6050_GYRO_200DPS = 0x00U,
    MPU6050_GYRO_500DPS = 0x01U,
    MPU6050_GYRO_1000DPS = 0x02U,
    MPU6050_GYRO_2000DPS = 0x03U
}MPU6050_GYRO_RANGE;

/*
    configuration options of Digital lowpass filter from 260Hz downto 5Hz (that are values for accelerometer
    they are slightly different for GYRO, need to be checked in doc file)
*/
typedef enum{
    MPU6050_DLPF_260 = 0x00U,
    MPU6050_DLPF_184 = 0x01U,
    MPU6050_DLPF_94 = 0x02U,
    MPU6050_DLPF_44 = 0x03U,
    MPU6050_DLPF_21  = 0x04U,
    MPU6050_DLPF_10 = 0x05U,
    MPU6050_DLPF_5 = 0x06U
}MPU6050_DLPF;

typedef struct
{
    MPU6050_ACC_RANGE ACC_RANGE;
    MPU6050_GYRO_RANGE GYRO_RANGE;
    MPU6050_DLPF DLPF;
}MPU6050_SETTINGS;

HAL_StatusTypeDef MPU6050_set_config(MPU6050_SETTINGS* sett);
HAL_StatusTypeDef MPU6050_wakeup();
void  MPU6050_default_setting(MPU6050_SETTINGS* sett);
int MPU_get_acc(enum AXIS axis);
int MPU_get_gyro(enum AXIS axis);
void MPU_get_acc_burst(int* acc_data);
void MPU_get_gyro_burst(int* gyro_data);
void MPU_raw_to_SI(const int* i_data_acc,const int* i_data_gyro, float* o_data_acc, float* o_data_gyro);
#endif