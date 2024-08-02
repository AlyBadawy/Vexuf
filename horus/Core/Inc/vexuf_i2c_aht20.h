/*
 * i2c_aht20.h
 *
 *  Created on: Jul 26, 2024
 *      Author: alybadawy
 */

#ifndef INC_VEXUF_I2C_AHT20_H_
#define INC_VEXUF_I2C_AHT20_H_

#include "main.h"

HAL_StatusTypeDef AHT20_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef AHT20_ReadTemperatureHumidity(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity);


#endif /* INC_VEXUF_I2C_AHT20_H_ */
