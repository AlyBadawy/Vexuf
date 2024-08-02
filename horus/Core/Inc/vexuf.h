/*
 * vexuf.h
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_H_
#define INC_VEXUF_H_

#include "main.h"
#include "vexuf_actuators.h"
#include "vexuf_adc_avs.h"
#include "vexuf_eeprom.h"
#include "vexuf_i2c_aht20.h"
#include "vexuf_i2c_checker.h"
#include "vexuf_indicators.h"
#include "vexuf_sd_card.h"

float cToF(float c);
float fToC(float f);
void base32_encode(const uint8_t *data, size_t length, char *output);

void SerialNumber_test(void);
void temperatureInternal_Test(void);

void VexUF_DateTimeString(char *string);
void VexUF_SerialNumber(char *serial_number);

void VexUF_USBWelcomeMessage(void);

#endif /* INC_VEXUF_H_ */
