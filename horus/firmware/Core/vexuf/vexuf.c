/*
 * vexuf.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */


#include "../inc/main.h"
#include "vexuf.h"
#include "vexuf_helpers.h"
#include "vexuf_config.h"
#include "vexuf_sd_card.h"
#include "vexuf_indicators.h"
#include "vexuf_timers.h"


char serialNumber[SERIAL_NUMBER_LENGTH];

uint32_t registrationNumber;
char callsign[CALLSIGN_LENGTH];
SerialConfiguration serialInterface;
SpiType spiType;
LcdConfiguration lcdConfig;
I2CConfiguration i2cConfig;
OutputConfiguration outputConfig;
ActuatorsValues actuatorsDefaults;
AlarmConfiguration alarmConfig[2];
PwmConfiguration pwmDefaultConfig;

TriggerConfiguration triggers[CONFIG_TRIGS_COUNT];
AvSensor avSensors[CONFIG_NUMBER_OF_AVS];


void VexUF_Init(void) {
	Indicators_setStatus(IndWarn, IndON);

	VexUF_GenerateSerialNumber();

	CONFIG_SetIsConfigured();

	// Check if the EEPROM has configuration, otherwise halt!
	if (CONFIG_IsConfigured()) {
		CONFIG_LoadSettingsFromEEPROM();
	} else {
		CONFIG_HandleNoConfig();
	}

	// Mount the SD Card, and handle the error if fails
	if (SDCard_MountFS() != FR_OK) {
		SDCard_HandleError();
	}


	TIMERS_Start();


	Indicators_setStatus(IndWarn, IndOFF);
}

//
//void SerialNumber_test(void) {
//	char serial_number[25];
//	generate_serial_number(serial_number);
//	printf("===================================================\r\n\r\n");
//	printf("   VexUF:Horus\r\n\r\n");
//	printf("   Serial Number: %s\r\n\r\n", serial_number);
//	printf("===================================================\r\n");
//}
//
//
//void temperatureInternal_Test(void) {
//	if (AHT20_Init(&hi2c1) != HAL_OK) {
//		Error_Handler();
//	}
//
//	float temperature = 0.0f;
//	float humidity = 0.0f;
//	// Read temperature and humidity from AHT20
//	printf("Internal AHT20 sensor:\r\n");
//	if (AHT20_ReadTemperatureHumidity(&hi2c1, &temperature, &humidity) == HAL_OK) {
//		printf("  Temperature Internal C: %0.2f\r\n", temperature);
//		printf("  Temperature Internal F: %0.2f\r\n", cToF(temperature));
//		printf("  Humidity Internal %%: %0.2f\r\n", humidity);
//	} else {
//		printf("  Error reading from AHT20\n");
//	}
//}
//
//
//
//
//void VexUF_SerialNumber(char *serial_number) {
//	char serial[20];
//	uint32_t uid[3];
//    uint8_t uid_bytes[12];
//
//    uid[2] = HAL_GetUIDw0();
//    uid[1] = HAL_GetUIDw1();
//    uid[0] = HAL_GetUIDw2();
//
//    for (int i = 0; i < 3; i++) {
//    	uid_bytes[4*i] = (uid[i] >> 24) & 0xFF;
//    	uid_bytes[4*i + 1] = (uid[i] >> 16) & 0xFF;
//    	uid_bytes[4*i + 2] = (uid[i] >> 8) & 0xFF;
//    	uid_bytes[4*i + 3] = uid[i] & 0xFF;
//	}
//
//    base32_encode(uid_bytes, 12, serial);
//
//    int i, j;
//    int length = strlen(serial);
//    for (i = 0, j = 0; i < length; i++) {
//        if (i > 0 && i % 5 == 0) {
//        	serial_number[j++] = '-';
//        }
//        serial_number[j++] = serial[i];
//    }
//
//    serial_number[j] = '\0';
//}
//
//void VexUF_USBWelcomeMessage(void) {
//	char messageBuffer[85];
//	sprintf(messageBuffer, "Hello...\r\nThis is VexUF:Horus...\r\n\r\n");
//	CDC_Transmit_FS((uint8_t *)messageBuffer, strlen(messageBuffer));
//	sprintf(messageBuffer, "Serial Number: %c\r\n", serialNumber);
//	CDC_Transmit_FS((uint8_t *)messageBuffer, strlen(messageBuffer));
//}
