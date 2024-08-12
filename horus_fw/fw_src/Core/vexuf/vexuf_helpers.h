/*
 * vexuf_helpers.h
 *
 *  Created on: Aug 10, 2024
 *      Author: Aly Badawy
 */

#ifndef VEXUF_VEXUF_HELPERS_H_
#define VEXUF_VEXUF_HELPERS_H_

#include "vexuf.h"
#include "vexuf_actuators.h"
#include "vexuf_real_time.h"

#define SERIAL_NUMBER_LENGTH		25
#define CALLSIGN_LENGTH				10




typedef enum {Baud300, Baud600, Baud1200, Baud4800, Baud9600, Baud19200, Baud57600, Baud115200} BaudRate;
typedef enum {NoLCD = 0, LCD2004 = 1, LCD1602 = 2} LcdType;
typedef enum {NoI2C = 0, AHT20 = 1, AHT21 = 2} I2CType;
typedef enum {NoSpi = 0, BLUETOOTH} SpiType;



typedef struct {
    uint16_t ttl_enabled : 1;                	// 1 bit
    BaudRate ttl_baud : 4;                		// 4 bit
    uint16_t ttl_led_enabled : 1;               // 1 bit
    uint16_t reserved1 : 2;                		// (padding to make 16 bits total)
    uint16_t tnc_enabled : 1;                	// 1 bit
    BaudRate tnc__baud : 4;                		// 4 bit
    uint16_t reserved2 : 3;                		// (padding to make 16 bits total)

} SerialConfiguration;

typedef struct {
    uint16_t lcdAdd;
    LcdType lcdType;  // Use the enum type for lcdType
    uint16_t lcdPwm;
} LcdConfiguration;

typedef struct {
    uint16_t i2cAdd;
    I2CType i2cType;  // Use the enum type for lcdType
} I2CConfiguration;

typedef struct {
	uint16_t buzzer_enabled: 1;					// 1 bit
	uint16_t buzzer_1s_enabled: 1;				// 1 bit
	uint16_t buzzer_hold_on_error: 1;			// 1 bit
	uint16_t global_indeicator_enabled: 1;		// 1 bit
	uint16_t status_indicator_enabled: 1;		// 1 bit
	uint16_t sd_card_indicator_enabled: 1;		// 1 bit
	uint16_t error_on_no_sd:1;					// 1 bit
	uint16_t log_info_to_sd:1;			 		// 1 bit
	uint16_t log_warn_to_sd:1;					// 1 bit
	uint16_t log_error_to_sd:1;					// 1 bit
	uint16_t reserved:6;						// 6 bit

} OutputConfiguration;

typedef struct {
	uint16_t pwm1Enabled;
	uint16_t pwm1Value;
	uint16_t pwm2Enabled;
	uint16_t pwm2Value;
} PwmConfiguration;




float cToF(float c);
float fToC(float f);
void base32_encode(const uint8_t *data, size_t length, char *output);
void VexUF_CurrentDateTimeString(char *string);
void VexUF_GenerateSerialNumber();

#endif /* VEXUF_VEXUF_HELPERS_H_ */
