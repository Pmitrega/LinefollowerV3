#ifndef __MEMORY_H
#define __MEMORY_H
#define MEM_DEV_ADDR 0x50 << 1

#define MEM_STARTUP_ADDR 0x00
#define WHITE_VALUES_ADDR 0x02
#define BLACK_VALUES_ADDR 0x16
#define MAX_ADDRESS 0xFF
#include "main.h"
/**
 * @brief Send No of resets of MCU and increment it.
*/
int update_counter();

/**
 * @brief save uint16 on specified address
*/
int write_uint16(uint8_t address, uint16_t value);
uint16_t read_uint16(uint8_t address);
#endif