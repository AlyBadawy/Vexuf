/*
 * config.h
 *
 *  Created on: Jul 31, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_CONFIG_H_
#define INC_VEXUF_CONFIG_H_

#include "main.h"
#include "vexuf_indicators.h"
#include "vexuf_eeprom.h"

#define  CONFIG_FLAG 					0x3E3E
#define  CONFIG_VERSION   				1
#define  TRIGS_COUNT					30
#define  NUMBER_OF_AVS					3
#define  NUMBER_OF_Actuators			8




// Memory MAp addresses
#define EEPROM_CONFIG_FLAG_ADDRESS                          0x0000
#define EEPROM_CONFIG_VERSION_ADDRESS                       0x0001
#define EEPROM_CONFIG_COUNT_ADDRESS							0x0002
#define EEPROM_SERIAL_NUMBER_ADDRESS                        0x0003
#define EEPROM_SERIAL_NUMBER_LENGTH							13          // 13 WORDS - 25 BYTES
#define EEPROM_REGISTERATION_NUMBER_ADDRESS                 0x0010
#define EEPROM_CALLSIGN_ADDRESS		                        0x0012
#define EEPROM_CALLSIGN_LENGTH							 	5           // 5 WORDS - 10 BYTES

#define EEPROM_SERIAL_INTERFACE_ADDRESS						0x0017
#define EEPROM_I2C_ADDRESS_ADDRESS                          0x0018
#define EEPROM_I2C_TYPE_ADDRESS                             0x0019
#define EEPROM_LCD_ADDRESS_ADDRESS                          0x001A
#define EEPROM_LCD_TYPE_ADDRESS                             0x001B
#define EEPROM_LCD_PWM_ADDRESS                              0x001C
#define EEPROM_SPI_TYPE_ADDRESS                             0x001D
#define EEPROM_OUTPUT_ADDRESS								0x001E
#define EEPROM_ACTUATORS_DEFAULT_ADDRESS         			0x001F

#define EEPROM_ALARM_ACTUATORS_ADDRESS						0x0020
#define EEPROM_ALARM_OUTPUT_ADDRESS							0x0021
#define EEPROM_ALARM_PWM1_ADDRESS							0x0022
#define EEPROM_ALARM_PWM2_ADDRESS							0x0023
#define EEPROM_ALARM_TNC_ENABLED_ADDRESS					0x0024
#define EEPROM_ALARM_TNC_PATH_ADDRESS						0x0025
#define EEPROM_ALARM_TNC_MSG_ADDRESS						0x0026
#define EEPROM_ALARM_SHIFT									8

#define	EEPROM_PWM1_ENABLED_ADDRESS							0x0031
#define	EEPROM_PWM1_DEFAULT_ADDRESS							0x0032
#define	EEPROM_PWM2_ENABLED_ADDRESS							0x0033
#define	EEPROM_PWM2_DEFAULT_ADDRESS							0x0034

#define EEPROM_AV_EN_AND_LED_ADDRESS                        0x0040
#define EEPROM_AV_OFF_RULE_ENABLED_ADDRESS                  0x0041
#define EEPROM_AV_OFF_RULE_MIN_ADDRESS                      0x0042
#define EEPROM_AV_OFF_RULE_MAX_ADDRESS                      0x0043
#define EEPROM_AV_SLOW_RULE_ENABLED_ADDRESS                 0x0045
#define EEPROM_AV_SLOW_RULE_MIN_ADDRESS                     0x0046
#define EEPROM_AV_SLOW_RULE_MAX_ADDRESS                     0x0047
#define EEPROM_AV_FAST_RULE_ENABLED_ADDRESS                 0x0049
#define EEPROM_AV_FAST_RULE_MIN_ADDRESS                     0x004A
#define EEPROM_AV_FAST_RULE_MAX_ADDRESS                     0x004B
#define EEPROM_AV_ON_RULE_ENABLED_ADDRESS                   0x004D
#define EEPROM_AV_ON_RULE_MIN_ADDRESS                       0x004E
#define EEPROM_AV_ON_RULE_MAX_ADDRESS                       0x004F
#define EEPROM_AV_SHIFT                                     16


#define EEPROM_TRIG_ENABLE_ADDRESS                          0x0070
#define EEPROM_TRIG_MIN_ADDRESS                             0x0071
#define EEPROM_TRIG_MAX_ADDRESS                             0x0072
#define EEPROM_TRIG_TEST_ADDRESS                            0x0073
#define EEPROM_TRIG_ACTUATORS_ADDRESS                       0x0074
#define EEPROM_TRIG_OUTPUT_ADDRESS                          0x0075
#define EEPROM_TRIG_PWM_ADDRESS                           	0x0076			// 2 words
#define EEPROM_TRIG_TNC1_ADDRESS                           	0x0078			// 4 words
#define EEPROM_TRIG_TNC2_ADDRESS                           	0x007C
#define EEPROM_TRIG_SHIFT                                   16


#define EEPROM_TNC_MESSAGE_ADDRESS							0x0200
#define EEPROM_TNC_MESSAGE_LENGTH_SHIFT						32
#define EEPROM_TNC_MESSAGE_COUNT							10

#define EEPROM_TNC_PATH_ADDRESS								0x0350
#define EEPROM_TNC_PATH_LENGTH_SHIFT						16
#define EEPROM_TNC_PATH_COUNT								5



bool CONFIG_IsConfigured(void);
uint16_t CONFIG_GetConfigVersion(void);
void CONFIG_SetIsConfigured(void);

void CONFIG_LoadSettingsFromEEPROM(void);
void CONFIG_HandleNoConfig(void);



#endif /* INC_VEXUF_CONFIG_H_ */
