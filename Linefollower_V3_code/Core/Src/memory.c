#include "memory.h"
#include "main.h"
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
uint8_t message_buffer[30];
int write_uint16(uint8_t address, uint16_t value){
    static uint8_t uint16_buffer[2];
    uint16_buffer[0] = value >> 8;
    uint16_buffer[1] = (value << 8) >> 8;
    if(HAL_I2C_Mem_Write(&hi2c1, MEM_DEV_ADDR, address, 1, uint16_buffer, 2, 100)!= HAL_OK){
        HAL_UART_Transmit(&huart1, "WRITE ERR\n", 10, 10);
        return -1;
    }
    return 0;
}

uint16_t read_uint16(uint8_t address){
    static uint8_t uint16_buffer[2];
    if(HAL_I2C_Mem_Read(&hi2c1,MEM_DEV_ADDR, address, 1, uint16_buffer, 2, 100) != HAL_OK){
        HAL_UART_Transmit(&huart1, "READ ERROR\n", 11, 10);
    }
    return (uint16_t)(uint16_buffer[0] << 8) + (uint16_t)uint16_buffer[1];;
}

uint16_t startups;
int update_counter(){
    startups = read_uint16(MEM_STARTUP_ADDR);
    int l = sprintf(message_buffer, "No of resets: %d\n", (int)startups);
    HAL_UART_Transmit(&huart1, message_buffer, l, 1000);
    startups+=1;
    return write_uint16(MEM_STARTUP_ADDR, startups);
}