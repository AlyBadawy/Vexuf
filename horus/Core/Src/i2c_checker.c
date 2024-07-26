/*
 * i2c_checker.c
 *
 *  Created on: Jul 26, 2024
 *      Author: alybadawy
 */

#include "i2c_checker.h"

// Function to scan I2C bus for devices
void I2C_Scan(I2C_HandleTypeDef *hi2c) {
    uint8_t i2c_address;
    HAL_StatusTypeDef result;
    printf("Scanning I2C bus...\n");
    for (i2c_address = 1; i2c_address < 128; i2c_address++) {
        result = HAL_I2C_IsDeviceReady(hi2c, (i2c_address << 1), 1, 10);
        if (result == HAL_OK) {
            printf("  I2C found 0x%02X\n", i2c_address);
        }
    }
    printf("I2C scan completed.\n");
}

