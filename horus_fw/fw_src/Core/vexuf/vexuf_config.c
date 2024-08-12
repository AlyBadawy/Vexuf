/*
 * config.c
 *
 *  Created on: Jul 31, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_config.h"

#include "vexuf_helpers.h"
#include "vexuf_eeprom.h"
#include "vexuf_adc_avs.h"
#include "vexuf_triggers.h"
#include "vexuf_real_time.h"

extern char serialNumber[SERIAL_NUMBER_LENGTH];
extern uint32_t registrationNumber;
extern char callsign[CALLSIGN_LENGTH];
extern SerialConfiguration serialInterface;
extern SpiType spiType;
extern LcdConfiguration lcdConfig;
extern I2CConfiguration i2cConfig;
extern OutputConfiguration outputConfig;
extern ActuatorsValues actuatorsDefaults;
extern AlarmConfiguration alarmConfig[2];
extern PwmConfiguration pwmDefaultConfig;

extern TriggerConfiguration triggers[TRIGS_COUNT];
extern AvSensor avSensors[NUMBER_OF_AVS];

bool isConfigured;

bool CONFIG_IsConfigured(void) {
	isConfigured = EEPROM_Read(EEPROM_CONFIG_FLAG_ADDRESS) == CONFIG_FLAG;
	return isConfigured;
}
uint16_t CONFIG_GetConfigVersion(void) {
	if (!CONFIG_IsConfigured()) {
		return 0;
	}
	return  EEPROM_Read(EEPROM_CONFIG_VERSION_ADDRESS);
}
void CONFIG_SetIsConfigured(void) {
	EEPROM_Write(EEPROM_CONFIG_FLAG_ADDRESS, CONFIG_FLAG);
	EEPROM_Write(EEPROM_CONFIG_VERSION_ADDRESS, CONFIG_VERSION);
	CONFIG_WriteSerialNumber();
	//TODO: Increase config count
	isConfigured = true;
}
void CONFIG_HandleNoConfig(void) {
	Indicators_setStatus(IndWarn, IndOFF);
	while (!isConfigured) {
		Indicators_setStatus(IndError, IndOFF);
		Indicators_setStatus(IndWarn, IndON);
		HAL_Delay(300);
		Indicators_setStatus(IndError, IndON);
		Indicators_setStatus(IndWarn, IndOFF);
		HAL_Delay(300);
	}
}

void CONFIG_ReadSerialNumber(char serialNumberBuffer[25]){
	uint16_t buffer[13] = {0};
	EEPROM_ReadMultipleWords(EEPROM_SERIAL_NUMBER_ADDRESS, buffer, EEPROM_SERIAL_NUMBER_LENGTH);
	for (int i = 0; i < 12; i++) {
		serialNumberBuffer[2 * i] = buffer[i] & 0xFF;
		serialNumberBuffer[2 * i + 1] = (buffer[i] >> 8) & 0xFF;
	}
	serialNumberBuffer[24] = buffer[12] & 0xFF;
}
void CONFIG_WriteSerialNumber(void) {
	uint16_t buffer[EEPROM_SERIAL_NUMBER_LENGTH] = {0};
	for (int i = 0; i < 12; i++) {
		buffer[i] = (serialNumber[2 * i] & 0xFF) | ((serialNumber[2 * i + 1] & 0xFF) << 8);
	}
	buffer[12] = serialNumber[24] & 0xFF;
	EEPROM_WriteMultipleWords(EEPROM_SERIAL_NUMBER_ADDRESS, buffer, EEPROM_SERIAL_NUMBER_LENGTH);
}
void EEPROM_LoadRegNumber(void) {
    uint16_t buffer[2];
    EEPROM_ReadMultipleWords(EEPROM_REGISTERATION_NUMBER_ADDRESS, buffer, 2);
    registrationNumber = ((uint32_t)buffer[1] << 16) | buffer[0];
}
void EEPROM_SetRegNumber(uint32_t regNumber) {
	registrationNumber = regNumber;
    uint16_t buffer[2];
    buffer[0] = (uint16_t)(regNumber & 0xFFFF);         // Lower 16 bits
    buffer[1] = (uint16_t)((regNumber >> 16) & 0xFFFF); // Upper 16 bits
    EEPROM_WriteMultipleWords(EEPROM_REGISTERATION_NUMBER_ADDRESS, buffer, 2);
}
void CONFIG_LoadCallSign(void) {
	uint16_t buffer[5] = {0};
	EEPROM_ReadMultipleWords(EEPROM_CALLSIGN_ADDRESS, buffer, EEPROM_CALLSIGN_LENGTH);
	for (int i = 0; i < EEPROM_CALLSIGN_LENGTH; i++) {
		callsign[2 * i] = buffer[i] & 0xFF;
		callsign[2 * i + 1] = (buffer[i] >> 8) & 0xFF;
	}
}
void CONFIG_SetCallSign(char newCallSign[EEPROM_CALLSIGN_LENGTH]) {
	strcpy(callsign, newCallSign);
	uint16_t buffer[EEPROM_CALLSIGN_LENGTH] = {0};
	for (int i = 0; i < EEPROM_CALLSIGN_LENGTH; i++) {
		buffer[i] = (callsign[2 * i] & 0xFF) | ((callsign[2 * i + 1] & 0xFF) << 8);
	}
	EEPROM_WriteMultipleWords(EEPROM_CALLSIGN_ADDRESS, buffer, EEPROM_CALLSIGN_LENGTH);
}

void CONFIG_LoadSerialInterface(void) {
	uint16_t config = EEPROM_Read(EEPROM_SERIAL_INTERFACE_ADDRESS);
	memcpy(&serialInterface, &config, sizeof(serialInterface));

}
void CONFIG_SetSerialInterface(SerialConfiguration *newSerialInterface) {
	serialInterface.ttl_enabled = newSerialInterface->ttl_enabled;
	serialInterface.ttl_baud = newSerialInterface->ttl_baud;
	serialInterface.ttl_led_enabled = newSerialInterface->ttl_led_enabled;
	serialInterface.tnc_enabled = newSerialInterface->tnc_enabled;
	serialInterface.tnc__baud = newSerialInterface->tnc__baud;

	uint16_t data;
	memcpy(&data, &serialInterface, sizeof(data));
	EEPROM_Write(EEPROM_SERIAL_INTERFACE_ADDRESS, data);
}
void CONFIG_LoadLcdConfig(void) {
	lcdConfig.lcdAdd = EEPROM_Read(EEPROM_LCD_ADDRESS_ADDRESS);
	lcdConfig.lcdType = EEPROM_Read(EEPROM_LCD_TYPE_ADDRESS);
	lcdConfig.lcdPwm = EEPROM_Read(EEPROM_LCD_PWM_ADDRESS);
}
void config_setLcdConfig(LcdConfiguration *config) {
	lcdConfig.lcdAdd = config->lcdAdd;
	lcdConfig.lcdType = config->lcdType;
	lcdConfig.lcdPwm = config->lcdPwm;
	EEPROM_Write(EEPROM_LCD_ADDRESS_ADDRESS, config->lcdAdd);
	EEPROM_Write(EEPROM_LCD_TYPE_ADDRESS, config->lcdType);
	EEPROM_Write(EEPROM_LCD_PWM_ADDRESS, config->lcdPwm);
}
void CONFIG_LoadI2cConfig(void) {
	i2cConfig.i2cAdd = EEPROM_Read(EEPROM_I2C_ADDRESS_ADDRESS);
	i2cConfig.i2cType = EEPROM_Read(EEPROM_I2C_TYPE_ADDRESS);
}
void config_setI2cConfig(I2CConfiguration *config) {
	i2cConfig.i2cAdd = config->i2cAdd;
	i2cConfig.i2cType = config->i2cType;
	EEPROM_Write(EEPROM_I2C_ADDRESS_ADDRESS, config->i2cAdd);
	EEPROM_Write(EEPROM_I2C_TYPE_ADDRESS, config->i2cType);
}
void CONFIG_LoadSpiConfig(void) {

}
void CONFIG_SetSpiType(SpiType newSpiType){
	spiType = newSpiType;
	EEPROM_Write(EEPROM_SPI_TYPE_ADDRESS, spiType);
}

void CONFIG_LoadOutputs(void) {
	uint16_t config = EEPROM_Read(EEPROM_OUTPUT_ADDRESS);
	memcpy(&outputConfig, &config, sizeof(outputConfig));

}
void CONFIG_SetOutputs(OutputConfiguration *newOutputConf) {
	outputConfig.buzzer_enabled = newOutputConf->buzzer_enabled;
	outputConfig.buzzer_1s_enabled = newOutputConf->buzzer_1s_enabled;
	outputConfig.buzzer_hold_on_error = newOutputConf->buzzer_hold_on_error;
	outputConfig.global_indeicator_enabled = newOutputConf->buzzer_enabled;
	outputConfig.status_indicator_enabled = newOutputConf->status_indicator_enabled;
	outputConfig.sd_card_indicator_enabled = newOutputConf->sd_card_indicator_enabled;
	outputConfig.error_on_no_sd = newOutputConf->error_on_no_sd;
	outputConfig.log_info_to_sd = newOutputConf->log_info_to_sd;
	outputConfig.log_warn_to_sd = newOutputConf->log_warn_to_sd;
	outputConfig.log_error_to_sd = newOutputConf->log_error_to_sd;

	uint16_t data;
	memcpy(&data, &outputConfig, sizeof(data));
	EEPROM_Write(EEPROM_OUTPUT_ADDRESS, data);
}

void CONFIG_LoadActuatorsDefault(void) {
	uint16_t defaults = EEPROM_Read(EEPROM_ACTUATORS_DEFAULT_ADDRESS);
	memcpy(&actuatorsDefaults, &defaults, sizeof(actuatorsDefaults));
}
void CONFIG_SetActuatorsDefault(ActuatorsValues *newActuatorDefaults) {
	actuatorsDefaults.act1 = newActuatorDefaults->act1;
	actuatorsDefaults.act2 = newActuatorDefaults->act2;
	actuatorsDefaults.act3 = newActuatorDefaults->act3;
	actuatorsDefaults.act4 = newActuatorDefaults->act4;
	actuatorsDefaults.act5 = newActuatorDefaults->act5;
	actuatorsDefaults.act6 = newActuatorDefaults->act6;
	actuatorsDefaults.act7 = newActuatorDefaults->act7;
	actuatorsDefaults.act8 = newActuatorDefaults->act8;
	uint16_t data;
	memcpy(&data, &actuatorsDefaults, sizeof(data));
	EEPROM_Write(EEPROM_ACTUATORS_DEFAULT_ADDRESS, data);
}

void CONFIG_LoadAvSensor(uint8_t index) {
	avSensors[index].enabled = EEPROM_Read(EEPROM_AV_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index));
	avSensors[index].indicatorEnabled = EEPROM_Read(EEPROM_AV_LED_ADDRESS + (EEPROM_AV_SHIFT * index));


	uint16_t status_slow = EEPROM_Read(EEPROM_AV_SLOW_RULE_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index));
	avSensors[index].statusSlow = status_slow & 0x01;

	uint16_t status_fast = EEPROM_Read(EEPROM_AV_FAST_RULE_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index));
	avSensors[index].statusFast = status_fast & 0x01;

	uint16_t status_on = EEPROM_Read(EEPROM_AV_ON_RULE_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index));
	avSensors[index].statusOn = status_on & 0x01;

	avSensors[index].minSlow = EEPROM_Read(EEPROM_AV_SLOW_RULE_MIN_ADDRESS + (EEPROM_AV_SHIFT * index));
	avSensors[index].maxSlow = EEPROM_Read(EEPROM_AV_SLOW_RULE_MAX_ADDRESS + (EEPROM_AV_SHIFT * index));

	avSensors[index].minFast = EEPROM_Read(EEPROM_AV_FAST_RULE_MIN_ADDRESS + (EEPROM_AV_SHIFT * index));
	avSensors[index].maxFast = EEPROM_Read(EEPROM_AV_FAST_RULE_MAX_ADDRESS + (EEPROM_AV_SHIFT * index));

	avSensors[index].minOn = EEPROM_Read(EEPROM_AV_ON_RULE_MIN_ADDRESS + (EEPROM_AV_SHIFT * index));
	avSensors[index].maxOn = EEPROM_Read(EEPROM_AV_ON_RULE_MAX_ADDRESS + (EEPROM_AV_SHIFT * index));
}
void CONFIG_SetAvSensor(uint8_t index, AvSensor *sensor) {
    // Write the enabled and indicatorEnabled bits
    avSensors[index].enabled = sensor ->enabled;
    avSensors[index].enabled = sensor ->indicatorEnabled;
    EEPROM_Write(EEPROM_AV_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index), avSensors[index].enabled);
    EEPROM_Write(EEPROM_AV_LED_ADDRESS + (EEPROM_AV_SHIFT * index), avSensors[index].indicatorEnabled);

    // Write the status bits
    uint16_t status_slow = sensor->statusSlow ? 0x01 : 0x00;
    avSensors[index].statusSlow = sensor->statusSlow;
    EEPROM_Write(EEPROM_AV_SLOW_RULE_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index), status_slow);

    uint16_t status_fast = sensor->statusFast ? 0x01 : 0x00;
    avSensors[index].statusFast = sensor->statusFast;
    EEPROM_Write(EEPROM_AV_FAST_RULE_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index), status_fast);

    uint16_t status_on = sensor->statusOn ? 0x01 : 0x00;
    avSensors[index].statusOn = sensor->statusOn;
    EEPROM_Write(EEPROM_AV_ON_RULE_ENABLED_ADDRESS + (EEPROM_AV_SHIFT * index), status_on);

    EEPROM_Write(EEPROM_AV_SLOW_RULE_MIN_ADDRESS + (EEPROM_AV_SHIFT * index), sensor->minSlow);
    EEPROM_Write(EEPROM_AV_SLOW_RULE_MAX_ADDRESS + (EEPROM_AV_SHIFT * index), sensor->maxSlow);
    avSensors[index].minSlow = sensor->minSlow;
    avSensors[index].maxSlow = sensor->maxSlow;

    EEPROM_Write(EEPROM_AV_FAST_RULE_MIN_ADDRESS + (EEPROM_AV_SHIFT * index), sensor->minFast);
    EEPROM_Write(EEPROM_AV_FAST_RULE_MAX_ADDRESS + (EEPROM_AV_SHIFT * index), sensor->maxFast);
    avSensors[index].minFast = sensor->minFast;
    avSensors[index].maxFast = sensor->maxFast;

    EEPROM_Write(EEPROM_AV_ON_RULE_MIN_ADDRESS + (EEPROM_AV_SHIFT * index), sensor->minOn);
    EEPROM_Write(EEPROM_AV_ON_RULE_MAX_ADDRESS + (EEPROM_AV_SHIFT * index), sensor->maxOn);
    avSensors[index].minOn = sensor->minOn;
    avSensors[index].maxOn = sensor->maxOn;
}

void CONFIG_LoadAlarm(uint8_t index) {
	uint16_t alarmActsValues = EEPROM_Read(EEPROM_ALARM_ACTUATORS_ADDRESS + (EEPROM_ALARM_SHIFT * index));
	ActuatorsValues alarmActsConfig;
	memcpy(&alarmActsConfig, &alarmActsValues, sizeof(alarmActsConfig));

	alarmConfig[index].actuators.act1 = alarmActsConfig.act1;
	alarmConfig[index].actuators.act2 = alarmActsConfig.act2;
	alarmConfig[index].actuators.act3 = alarmActsConfig.act3;
	alarmConfig[index].actuators.act4 = alarmActsConfig.act4;
	alarmConfig[index].actuators.act5 = alarmActsConfig.act5;
	alarmConfig[index].actuators.act6 = alarmActsConfig.act6;
	alarmConfig[index].actuators.act7 = alarmActsConfig.act7;
	alarmConfig[index].actuators.act8 = alarmActsConfig.act8;


	uint16_t alarmOutputValues = EEPROM_Read(EEPROM_ALARM_OUTPUT_ADDRESS + (EEPROM_ALARM_SHIFT * index));
	AlarmTrigOutputConfiguration alarmOutputConfig;
	memcpy(&alarmOutputConfig, &alarmOutputValues, sizeof(alarmOutputConfig));
	alarmConfig[index].output.buzz = alarmOutputConfig.buzz;
	alarmConfig[index].output.info = alarmOutputConfig.info;
	alarmConfig[index].output.pwm1_change = alarmOutputConfig.pwm1_change;
	alarmConfig[index].output.pwm2_change = alarmOutputConfig.pwm2_change;

	alarmConfig[index].pwm1 = EEPROM_Read(EEPROM_ALARM_PWM1_ADDRESS + (EEPROM_ALARM_SHIFT * index));
	alarmConfig[index].pwm2 = EEPROM_Read(EEPROM_ALARM_PWM2_ADDRESS + (EEPROM_ALARM_SHIFT * index));
	alarmConfig[index].tncEnabled = EEPROM_Read(EEPROM_ALARM_TNC_ENABLED_ADDRESS + (EEPROM_ALARM_SHIFT * index));
	alarmConfig[index].tncPath = EEPROM_Read(EEPROM_ALARM_TNC_PATH_ADDRESS + (EEPROM_ALARM_SHIFT * index));
	alarmConfig[index].tncMessage = EEPROM_Read(EEPROM_ALARM_TNC_MSG_ADDRESS + (EEPROM_ALARM_SHIFT * index));

}
void CONFIG_SetAlarm(uint8_t index, AlarmConfiguration *newAlarmConfig) {
	alarmConfig[index].actuators.act1 = newAlarmConfig->actuators.act1;
	alarmConfig[index].actuators.act2 = newAlarmConfig->actuators.act2;
	alarmConfig[index].actuators.act3 = newAlarmConfig->actuators.act3;
	alarmConfig[index].actuators.act4 = newAlarmConfig->actuators.act4;
	alarmConfig[index].actuators.act5 = newAlarmConfig->actuators.act5;
	alarmConfig[index].actuators.act6 = newAlarmConfig->actuators.act6;
	alarmConfig[index].actuators.act7 = newAlarmConfig->actuators.act7;
	alarmConfig[index].actuators.act8 = newAlarmConfig->actuators.act8;
	uint16_t data;
	memcpy(&data, &alarmConfig[index].actuators, sizeof(data));

	EEPROM_Write(EEPROM_ALARM_ACTUATORS_ADDRESS + (EEPROM_ALARM_SHIFT * index), data);

	alarmConfig[index].output.buzz = newAlarmConfig->output.buzz;
	alarmConfig[index].output.info = newAlarmConfig->output.info;
	alarmConfig[index].output.pwm1_change = newAlarmConfig->output.pwm1_change;
	alarmConfig[index].output.pwm2_change = newAlarmConfig->output.pwm2_change;

	memcpy(&data, &alarmConfig[index].output, sizeof(data));
	EEPROM_Write(EEPROM_ALARM_OUTPUT_ADDRESS + (EEPROM_ALARM_SHIFT * index), data);

	alarmConfig[index].pwm1 = newAlarmConfig->pwm1;
	alarmConfig[index].pwm2 = newAlarmConfig->pwm2;
	alarmConfig[index].tncEnabled = newAlarmConfig->tncEnabled;
	alarmConfig[index].tncMessage = newAlarmConfig->tncMessage;
	alarmConfig[index].tncPath = newAlarmConfig->tncPath;
	EEPROM_Write(EEPROM_ALARM_PWM1_ADDRESS + (EEPROM_ALARM_SHIFT * index), alarmConfig[index].pwm1);
	EEPROM_Write(EEPROM_ALARM_PWM2_ADDRESS + (EEPROM_ALARM_SHIFT * index), alarmConfig[index].pwm2);
	EEPROM_Write(EEPROM_ALARM_TNC_ENABLED_ADDRESS + (EEPROM_ALARM_SHIFT * index), alarmConfig[index].tncEnabled);
	EEPROM_Write(EEPROM_ALARM_TNC_PATH_ADDRESS + (EEPROM_ALARM_SHIFT * index), alarmConfig[index].tncPath);
	EEPROM_Write(EEPROM_ALARM_TNC_MSG_ADDRESS + (EEPROM_ALARM_SHIFT * index), alarmConfig[index].tncMessage);
}

void CONFIG_LoadPwmDefaultConfigurations(void) {
	pwmDefaultConfig.pwm1Enabled = EEPROM_Read(EEPROM_PWM1_ENABLED_ADDRESS);
	pwmDefaultConfig.pwm1Value = EEPROM_Read(EEPROM_PWM1_DEFAULT_ADDRESS);
	pwmDefaultConfig.pwm2Enabled = EEPROM_Read(EEPROM_PWM2_ENABLED_ADDRESS);
	pwmDefaultConfig.pwm2Value = EEPROM_Read(EEPROM_PWM2_DEFAULT_ADDRESS);
}
void CONFIG_SetPwmDefaultConfigurations(PwmConfiguration *newPwmDefaultConfig) {
	pwmDefaultConfig.pwm1Enabled = newPwmDefaultConfig->pwm1Enabled;
	pwmDefaultConfig.pwm1Value = newPwmDefaultConfig->pwm1Value;
	pwmDefaultConfig.pwm2Enabled = newPwmDefaultConfig->pwm2Enabled;
	pwmDefaultConfig.pwm2Value = newPwmDefaultConfig->pwm2Value;
	EEPROM_Write(EEPROM_PWM1_ENABLED_ADDRESS, pwmDefaultConfig.pwm1Enabled);
	EEPROM_Write(EEPROM_PWM1_DEFAULT_ADDRESS, pwmDefaultConfig.pwm1Value);
	EEPROM_Write(EEPROM_PWM2_ENABLED_ADDRESS, pwmDefaultConfig.pwm2Enabled);
	EEPROM_Write(EEPROM_PWM2_DEFAULT_ADDRESS, pwmDefaultConfig.pwm2Value);
}

void CONFIG_LoadTrigConfiguration(uint8_t index) {
	triggers[index].status = EEPROM_Read(EEPROM_TRIG_STATUS_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	triggers[index].test = EEPROM_Read(EEPROM_TRIG_TEST_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	triggers[index].fromValue = EEPROM_Read(EEPROM_TRIG_FROM_VALUE_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	triggers[index].toValue = EEPROM_Read(EEPROM_TRIG_TO_VALUE_ADDRESS + (EEPROM_TRIG_SHIFT * index));

	uint16_t trigActsValues = EEPROM_Read(EEPROM_TRIG_ACTUATORS_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	ActuatorsValues trigActsConfig;
	memcpy(&trigActsConfig, &trigActsValues, sizeof(trigActsConfig));
	triggers[index].actuators.act1 = trigActsConfig.act1;
	triggers[index].actuators.act2 = trigActsConfig.act2;
	triggers[index].actuators.act3 = trigActsConfig.act3;
	triggers[index].actuators.act4 = trigActsConfig.act4;
	triggers[index].actuators.act5 = trigActsConfig.act5;
	triggers[index].actuators.act6 = trigActsConfig.act6;
	triggers[index].actuators.act7 = trigActsConfig.act7;
	triggers[index].actuators.act8 = trigActsConfig.act8;

	uint16_t trigOutputValues = EEPROM_Read(EEPROM_TRIG_OUTPUT_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	AlarmTrigOutputConfiguration trigOutputConfig;
	memcpy(&trigOutputConfig, &trigOutputValues, sizeof(trigOutputConfig));
	triggers[index].output.buzz = trigOutputConfig.buzz;
	triggers[index].output.info = trigOutputConfig.info;
	triggers[index].output.pwm1_change = trigOutputConfig.pwm1_change;
	triggers[index].output.pwm2_change = trigOutputConfig.pwm2_change;

	triggers[index].pwm1 = EEPROM_Read(EEPROM_TRIG_PWM_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	triggers[index].pwm2 = EEPROM_Read(EEPROM_TRIG_PWM_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 1);

	triggers[index].tnc1Enabled = EEPROM_Read(EEPROM_TRIG_TNC1_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	triggers[index].tnc1Path = EEPROM_Read(EEPROM_TRIG_TNC1_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 1);
	triggers[index].tnc1Message = EEPROM_Read(EEPROM_TRIG_TNC1_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 2);

	triggers[index].tnc2Enabled = EEPROM_Read(EEPROM_TRIG_TNC2_ADDRESS + (EEPROM_TRIG_SHIFT * index));
	triggers[index].tnc2Path = EEPROM_Read(EEPROM_TRIG_TNC2_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 1);
	triggers[index].tnc2Message = EEPROM_Read(EEPROM_TRIG_TNC2_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 2);
}
void CONFIG_SetTrigConfiguration(uint8_t index, TriggerConfiguration *newTrigConfig){
	triggers[index].status = newTrigConfig->status;
	EEPROM_Write(EEPROM_TRIG_STATUS_ADDRESS + (EEPROM_TRIG_SHIFT * index), triggers[index].status);
	triggers[index].test = newTrigConfig->test;
	EEPROM_Write(EEPROM_TRIG_TEST_ADDRESS + (EEPROM_TRIG_SHIFT * index), triggers[index].test);
	triggers[index].fromValue = newTrigConfig->fromValue;
	EEPROM_Write(EEPROM_TRIG_FROM_VALUE_ADDRESS + (EEPROM_TRIG_SHIFT * index), triggers[index].fromValue);
	triggers[index].toValue = newTrigConfig->toValue;
	EEPROM_Write(EEPROM_TRIG_TO_VALUE_ADDRESS + (EEPROM_TRIG_SHIFT * index), triggers[index].toValue);

	triggers[index].actuators.act1 = newTrigConfig->actuators.act1;
	triggers[index].actuators.act2 = newTrigConfig->actuators.act2;
	triggers[index].actuators.act3 = newTrigConfig->actuators.act3;
	triggers[index].actuators.act4 = newTrigConfig->actuators.act4;
	triggers[index].actuators.act5 = newTrigConfig->actuators.act5;
	triggers[index].actuators.act6 = newTrigConfig->actuators.act6;
	triggers[index].actuators.act7 = newTrigConfig->actuators.act7;
	triggers[index].actuators.act8 = newTrigConfig->actuators.act8;
	uint16_t data;
	memcpy(&data, &triggers[index].actuators, sizeof(data));
	EEPROM_Write(EEPROM_TRIG_ACTUATORS_ADDRESS + (EEPROM_ALARM_SHIFT * index), data);

	triggers[index].output.buzz = newTrigConfig->output.buzz;
	triggers[index].output.info = newTrigConfig->output.info;
	triggers[index].output.pwm1_change = newTrigConfig->output.pwm1_change;
	triggers[index].output.pwm2_change = newTrigConfig->output.pwm2_change;
	memcpy(&data, &triggers[index].output, sizeof(data));
	EEPROM_Write(EEPROM_TRIG_OUTPUT_ADDRESS + (EEPROM_ALARM_SHIFT * index), data);

	triggers[index].pwm1 = newTrigConfig->pwm1;
	triggers[index].pwm2 = newTrigConfig->pwm2;
	EEPROM_Write(EEPROM_TRIG_PWM_ADDRESS + (EEPROM_TRIG_SHIFT * index), triggers[index].pwm1);
	EEPROM_Write(EEPROM_TRIG_PWM_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 1, triggers[index].pwm2);

	triggers[index].tnc1Enabled = newTrigConfig->tnc1Enabled;
	triggers[index].tnc1Path = newTrigConfig->tnc1Path;
	triggers[index].tnc1Message = newTrigConfig->tnc1Message;
	triggers[index].tnc2Enabled = newTrigConfig->tnc2Enabled;
	triggers[index].tnc2Path = newTrigConfig->tnc2Path;
	triggers[index].tnc2Message = newTrigConfig->tnc2Message;

	EEPROM_Write(EEPROM_TRIG_TNC1_ADDRESS + (EEPROM_TRIG_SHIFT * index), triggers[index].tnc1Enabled);
	EEPROM_Write(EEPROM_TRIG_TNC1_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 1, triggers[index].tnc1Path);
	EEPROM_Write(EEPROM_TRIG_TNC1_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 2, triggers[index].tnc1Message);
	EEPROM_Write(EEPROM_TRIG_TNC2_ADDRESS + (EEPROM_TRIG_SHIFT * index), triggers[index].tnc2Enabled);
	EEPROM_Write(EEPROM_TRIG_TNC2_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 1, triggers[index].tnc2Path);
	EEPROM_Write(EEPROM_TRIG_TNC2_ADDRESS + (EEPROM_TRIG_SHIFT * index) + 2, triggers[index].tnc2Message);

}
//

void CONFIG_LoadSettingsFromEEPROM(void) {
	EEPROM_LoadRegNumber();
	CONFIG_LoadCallSign();
	CONFIG_LoadSerialInterface();
	CONFIG_LoadI2cConfig();
	CONFIG_LoadLcdConfig();
	CONFIG_LoadSpiConfig();
	CONFIG_LoadOutputs();
	CONFIG_LoadActuatorsDefault();
	CONFIG_LoadAlarm(0);
	CONFIG_LoadAlarm(1);
	CONFIG_LoadPwmDefaultConfigurations();
	for (int i=0; i < NUMBER_OF_AVS; i++) {
		CONFIG_LoadAvSensor(i);
	}
	for (int i=0; i < TRIGS_COUNT; i++) {
		CONFIG_LoadTrigConfiguration(i);
	}
}
