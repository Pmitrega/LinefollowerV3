#include "MPU6050.h"

static uint8_t MSB;
static uint8_t LSB;
static uint8_t MPU6050_i2c_buffer;
int MPU_get_acc(enum AXIS axis){
    uint32_t readings;
    if(axis == X){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_XOUT_L, 1, &LSB, 1, 1000);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, &MSB, 1, 1000);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else if(axis == Y){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_YOUT_L, 1, &LSB, 1, 1000);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_YOUT_H, 1, &MSB, 1, 1000);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else{
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_ZOUT_L, 1, &LSB, 1, 1000);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_ZOUT_H, 1, &MSB, 1, 1000);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    
}

int MPU_get_gyro(enum AXIS axis){
    uint32_t readings;
    if(axis == X){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_XOUT_L, 1, &LSB, 1, 1000);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_XOUT_H, 1, &MSB, 1, 1000);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else if(axis == Y){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_YOUT_L, 1, &LSB, 1, 1000);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_YOUT_H, 1, &MSB, 1, 1000);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else{
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_ZOUT_L, 1, &LSB, 1, 1000);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_ZOUT_H, 1, &MSB, 1, 1000);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    
}

HAL_StatusTypeDef MPU6050_wakeup(){
    MPU6050_i2c_buffer = 0x00;
    return HAL_I2C_Mem_Write(&MPU6050_I2C_PORT, MPU6050_ADDR,0x6B,1,&MPU6050_i2c_buffer,1,100);
}

HAL_StatusTypeDef MPU6050_set_config(MPU6050_SETTINGS* sett){
    HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACC_CONFIG, 1, &MPU6050_i2c_buffer, 1, 1000);
    MPU6050_i2c_buffer = (MPU6050_i2c_buffer & 0xE7) + (sett->ACC_RANGE << 3);
    HAL_I2C_Mem_Write(&MPU6050_I2C_PORT, MPU6050_ADDR,MPU6050_ACC_CONFIG,1,&MPU6050_i2c_buffer,1,100);

    HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_CONFIG, 1, &MPU6050_i2c_buffer, 1, 1000);
    MPU6050_i2c_buffer = (MPU6050_i2c_buffer & 0xF8) + sett->DLPF;
    HAL_I2C_Mem_Write(&MPU6050_I2C_PORT, MPU6050_ADDR,MPU6050_CONFIG,1,&MPU6050_i2c_buffer,1,100);

    HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_CONFIG, 1, &MPU6050_i2c_buffer, 1, 1000);
    MPU6050_i2c_buffer = (MPU6050_i2c_buffer & 0xE7) + (sett->GYRO_RANGE << 3);
    HAL_I2C_Mem_Write(&MPU6050_I2C_PORT, MPU6050_ADDR,MPU6050_GYRO_CONFIG,1,&MPU6050_i2c_buffer,1,100);
}

void  MPU6050_default_setting(MPU6050_SETTINGS* sett){
    sett->ACC_RANGE = MPU6050_ACC_2G;
    sett->DLPF = MPU6050_DLPF_260;
    sett->GYRO_RANGE = MPU6050_GYRO_200DPS;
}