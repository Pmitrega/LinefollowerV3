#include "MPU6050.h"

static uint8_t MSB;
static uint8_t LSB;
static uint8_t MPU6050_i2c_buffer;
MPU6050_SETTINGS MPU_global_settings;


#define I2C_TIMEOUT 30
#define DEG_TO_RAD (3.14159/180.f)


int MPU_get_acc(enum AXIS axis){
    if(axis == X){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_XOUT_L, 1, &LSB, 1, I2C_TIMEOUT);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, &MSB, 1, I2C_TIMEOUT);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else if(axis == Y){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_YOUT_L, 1, &LSB, 1, I2C_TIMEOUT);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_YOUT_H, 1, &MSB, 1, I2C_TIMEOUT);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else{
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_ZOUT_L, 1, &LSB, 1, I2C_TIMEOUT);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_ZOUT_H, 1, &MSB, 1, I2C_TIMEOUT);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    
}

void MPU_get_acc_burst(int* acc_data){
    uint8_t recieve_data[6];
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, 1, recieve_data, 6, I2C_TIMEOUT);
    if(status == HAL_OK){
        acc_data[0] = (int)(int16_t)(((uint16_t)recieve_data[0]<<8) + (uint16_t)recieve_data[1]);
        acc_data[1] = (int)(int16_t)(((uint16_t)recieve_data[2]<<8) + (uint16_t)recieve_data[3]);
        acc_data[2] = (int)(int16_t)(((uint16_t)recieve_data[4]<<8) + (uint16_t)recieve_data[5]);
    }
}

void MPU_get_gyro_burst(int* gyro_data){
    uint8_t recieve_data[6];
    HAL_StatusTypeDef status;
    status = HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_XOUT_H, 1, recieve_data, 6, I2C_TIMEOUT);
    if(status == HAL_OK){
        gyro_data[0] = (int)(int16_t)(((uint16_t)recieve_data[0]<<8) + (uint16_t)recieve_data[1]);
        gyro_data[1] = (int)(int16_t)(((uint16_t)recieve_data[2]<<8) + (uint16_t)recieve_data[3]);
        gyro_data[2] = (int)(int16_t)(((uint16_t)recieve_data[4]<<8) + (uint16_t)recieve_data[5]);
    }
}

int MPU_get_gyro(enum AXIS axis){
    uint32_t readings;
    if(axis == X){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_XOUT_L, 1, &LSB, 1, I2C_TIMEOUT);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_XOUT_H, 1, &MSB, 1, I2C_TIMEOUT);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else if(axis == Y){
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_YOUT_L, 1, &LSB, 1, I2C_TIMEOUT);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_YOUT_H, 1, &MSB, 1, I2C_TIMEOUT);
        return  (int)(int16_t)(((uint16_t)MSB<<8) + (uint16_t)LSB);
    }
    else{
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_ZOUT_L, 1, &LSB, 1, I2C_TIMEOUT);
        HAL_I2C_Mem_Read(&MPU6050_I2C_PORT, MPU6050_ADDR, MPU6050_GYRO_ZOUT_H, 1, &MSB, 1, I2C_TIMEOUT);
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
    MPU_global_settings.ACC_RANGE = sett->ACC_RANGE;
    MPU_global_settings.DLPF = sett->DLPF;
    MPU_global_settings.GYRO_RANGE = sett->GYRO_RANGE;
}

void MPU_raw_to_SI(const int* i_data_acc,const int* i_data_gyro, float* o_data_acc, float* o_data_gyro){
    o_data_acc[0] = ((float)i_data_acc[0]/16384.f)/(float)(1<<MPU_global_settings.ACC_RANGE);
    o_data_acc[1] = ((float)i_data_acc[1]/16384.f)/(float)(1<<MPU_global_settings.ACC_RANGE);
    o_data_acc[2] = ((float)i_data_acc[2]/16384.f)/(float)(1<<MPU_global_settings.ACC_RANGE);
    o_data_gyro[0] = ((float)i_data_gyro[0]/131.f * DEG_TO_RAD)/(float)(1<<MPU_global_settings.GYRO_RANGE);
    o_data_gyro[1] = ((float)i_data_gyro[1]/131.f * DEG_TO_RAD)/(float)(1<<MPU_global_settings.GYRO_RANGE);
    o_data_gyro[2] = ((float)i_data_gyro[2]/131.f * DEG_TO_RAD)/(float)(1<<MPU_global_settings.GYRO_RANGE);

}

void  MPU6050_default_setting(MPU6050_SETTINGS* sett){
    sett->ACC_RANGE = MPU6050_ACC_2G;
    sett->DLPF = MPU6050_DLPF_260;
    sett->GYRO_RANGE = MPU6050_GYRO_200DPS;
}