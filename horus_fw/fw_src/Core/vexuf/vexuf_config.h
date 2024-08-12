/*
 * config.h
 *
 *  Created on: Jul 31, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_CONFIG_H_
#define INC_VEXUF_CONFIG_H_

#include "vexuf.h"
#include "vexuf_indicators.h"
#include "vexuf_eeprom.h"
#include "vexuf_adc_avs.h"

#define  CONFIG_FLAG 					0x3E3E
#define  CONFIG_VERSION   				1




bool CONFIG_IsConfigured(void);
uint16_t CONFIG_GetConfigVersion(void);
void CONFIG_SetIsConfigured(void);
void CONFIG_HandleNoConfig(void);
void CONFIG_WriteSerialNumber(void);

void CONFIG_LoadSettingsFromEEPROM(void);


void CONFIG_SetAvSensor(uint8_t index, AvSensor *sensor);


#endif /* INC_VEXUF_CONFIG_H_ */
