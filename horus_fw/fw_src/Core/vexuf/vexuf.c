/*
 * vexuf.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */


#include "vexuf.h"
#include "vexuf_helpers.h"
#include "vexuf_config.h"
#include "vexuf_sd_card.h"
#include "vexuf_indicators.h"
#include "vexuf_timers.h"
#include "vexuf_adc_avs.h"
#include "vexuf_triggers.h"
#include "vexuf_pwm.h"
#include "vexuf_output.h"
#include "vexuf_i2c_checker.h"
#include "vexuf_i2c_hd44780.h"
#include "vexuf_serial.h"
#include "vexuf_commands.h"



extern ADC_HandleTypeDef hadc1;
extern uint32_t adcBuffer[5];

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;



char serialNumber[SERIAL_NUMBER_LENGTH];
uint32_t registrationNumber;
char callsign[CALLSIGN_LENGTH];
SerialConfiguration serialConfig;
SpiType spiType;
LcdConfiguration lcdConfig;
I2CConfiguration i2cConfig;
OutputConfiguration outputConfig;
ActuatorsValues actuatorsDefaults;
AlarmConfiguration alarmConfig[2];
PwmConfiguration pwmConfig;

TriggerConfiguration triggers[TRIGS_COUNT];
AvSensor avSensors[NUMBER_OF_AVS];

VexufStatus vexuf_status;
IndStatus ind_status;

uint8_t ttlRxData[SERIAL_BUFFER_SIZE];
uint8_t tncRxData[SERIAL_BUFFER_SIZE];
uint8_t cdcRxData[SERIAL_BUFFER_SIZE];
uint16_t ttlRxIdx;
uint16_t tncRxIdx;
uint16_t cdcRxIdx;

float cpuTempC;


void VexUF_Init(void) {
//	I2C_ScanTest();

	// TODO: Move this to the LCD driver after creation;
	// Also, TODO: make the I2C address customizable.
	HD44780_Init(2);
	HD44780_Clear();
	HD44780_SetCursor(0,0);


	IND_setStatus(IndWarn, IndON);

	VexUF_GenerateSerialNumber();


	//	 Check if the EEPROM has configuration, otherwise halt!
	if (!CONFIG_IsConfigured()) {
		CONFIG_HandleNoConfig();
	}
	CONFIG_WriteSerialNumber();

	CONFIG_LoadSettingsFromEEPROM();

	PWM_init();

	HAL_Delay(500);

// 	ACTUATORS_Test(); // TODO: remove before release




 	HAL_UARTEx_ReceiveToIdle_IT(&huart1, ttlRxData, SERIAL_BUFFER_SIZE);
 	HAL_UARTEx_ReceiveToIdle_IT(&huart6, tncRxData, SERIAL_BUFFER_SIZE);


	HAL_Delay(20);
	TIMERS_Start();

	OUTPUT_BuzzOnStartUp();
	IND_setStatus(IndWarn, IndOFF);

//	HD44780_PrintStr("OK!");
}

void VEXUF_run(void) {
	ADC_Scan();
	SDCard_checkCard();

	if (vexuf_status.sd_card_error && outputConfig.error_on_no_sd) SDCard_HandleError();

	if (vexuf_status.timer_1hz_ticked == 1){
		HAL_ADC_Start_DMA(&hadc1, adcBuffer, 5);
		HAL_Delay(20);
		HAL_ADC_Stop_DMA(&hadc1);
		vexuf_status.timer_1hz_ticked = 0;
	}

	if (vexuf_status.timer_0d1hz_ticked == 1) {
		TRIGGERS_runAll();
		vexuf_status.timer_0d1hz_ticked = 0;
	}

	if (vexuf_status.ttlBuffered == 1) {
		COMMANDS_handleCommand(TtlUart);
		vexuf_status.ttlBuffered = 0;
	}
	if (vexuf_status.tncBuffered == 1) {
		COMMANDS_handleCommand(TncUart);
		vexuf_status.ttlBuffered = 0;
	}
}


