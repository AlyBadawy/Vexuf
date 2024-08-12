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
#include "vexuf_adc_avs.h"



extern ADC_HandleTypeDef hadc1;
extern uint32_t adcBuffer[5];


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
AvSensor avSensors[NUMBER_OF_AVS];

bool timerTicked = false;


void VexUF_Init(void) {
	Indicators_setStatus(IndWarn, IndON);

	VexUF_GenerateSerialNumber();

	CONFIG_SetIsConfigured(); // TODO: Remove this before release!

	// Check if the EEPROM has configuration, otherwise halt!
	if (!CONFIG_IsConfigured()) CONFIG_HandleNoConfig();
	CONFIG_WriteSerialNumber();
	// Mount the SD Card, and handle the error if fails
	if (SDCard_MountFS() != FR_OK) SDCard_HandleError();

	CONFIG_LoadSettingsFromEEPROM();

 	HAL_ADC_Start_DMA(&hadc1, adcBuffer, 5);
	HAL_Delay(50);

	HAL_Delay(500);

	AvSensor sens;
	sens.enabled = true;
	sens.indicatorEnabled = true;
	sens.minFast = 300;
	sens.maxFast = 400;
	sens.statusFast = true;
	CONFIG_SetAvSensor(2, &sens);

	TIMERS_Start();


	Indicators_setStatus(IndWarn, IndOFF);
}

void VEXUF_run(void) {

	ADC_Scan();
	if (timerTicked) {
		timerTicked = false;
	}
}


