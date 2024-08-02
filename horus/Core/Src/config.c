/*
 * config.c
 *
 *  Created on: Jul 31, 2024
 *      Author: Aly Badawy
 */

#include "config.h"
#include "93c86.h"


extern char serialNumber[SERIAL_NUMBER_LENGTH];
extern char callsign[CALLSIGN_LENGTH];
extern uint16_t registrationNumber;

extern Trigger triggers[16];
extern AvSensor avSensors[3];


bool CONFIG_IsConfigured(void) {
    uint16_t configFlag;
    EEPROM_ReadMultipleWords(EEPROM_CONFIG_FLAG_ADDR, &configFlag, 1);
    return configFlag == 0xEEEE;  // Assuming 0xEEEE indicates configured EEPROM
}






// TRIGGERS  ====================
uint8_t CONFIG_GetTriggerEnabled(uint8_t triggerIndex) {
    return triggers[triggerIndex].enabled;
}

void CONFIG_SetTriggerEnabled(uint8_t triggerIndex, uint8_t value) {
    triggers[triggerIndex].enabled = value;
}

uint8_t CONFIG_GetTriggerActuator(uint8_t triggerIndex, uint8_t actuatorIndex) {
    return (triggers[triggerIndex].actuators >> actuatorIndex) & 0x01;
}

void CONFIG_SetTriggerActuator(uint8_t triggerIndex, uint8_t actuatorIndex, uint8_t value) {
    if (value) {
        triggers[triggerIndex].actuators |= (1 << actuatorIndex);
    } else {
        triggers[triggerIndex].actuators &= ~(1 << actuatorIndex);
    }
}

uint8_t CONFIG_GetTriggerBuzzer(uint8_t triggerIndex) {
    return triggers[triggerIndex].buzzer;
}

void CONFIG_SetTriggerBuzzer(uint8_t triggerIndex, uint8_t value) {
    triggers[triggerIndex].buzzer = value;
}

uint8_t CONFIG_GetTriggerLogged(uint8_t triggerIndex) {
    return triggers[triggerIndex].logged;
}

void CONFIG_SetTriggerLogged(uint8_t triggerIndex, uint8_t value) {
    triggers[triggerIndex].logged = value;
}

TriggerSource CONFIG_GetTriggerSource(uint8_t triggerIndex) {
    return (TriggerSource)triggers[triggerIndex].source;
}

void CONFIG_SetTriggerSource(uint8_t triggerIndex, TriggerSource source) {
    triggers[triggerIndex].source = source;
}

uint16_t CONFIG_GetTriggerPwm1(uint8_t triggerIndex) {
    return triggers[triggerIndex].pwm1;
}

void CONFIG_SetTriggerPwm1(uint8_t triggerIndex, uint16_t value) {
    triggers[triggerIndex].pwm1 = value;
}

uint16_t CONFIG_GetTriggerPwm2(uint8_t triggerIndex) {
    return triggers[triggerIndex].pwm2;
}

void CONFIG_SetTriggerPwm2(uint8_t triggerIndex, uint16_t value) {
    triggers[triggerIndex].pwm2 = value;
}

uint16_t CONFIG_GetTriggerMin(uint8_t triggerIndex) {
    return triggers[triggerIndex].min;
}

void CONFIG_SetTriggerMin(uint8_t triggerIndex, uint16_t value) {
    triggers[triggerIndex].min = value;
}

uint16_t CONFIG_GetTriggerMax(uint8_t triggerIndex) {
    return triggers[triggerIndex].max;
}

void CONFIG_SetTriggerMax(uint8_t triggerIndex, uint16_t value) {
    triggers[triggerIndex].max = value;
}







// TRIGGERS  ====================
uint8_t CONFIG_GetAvSensorEnabled(uint8_t index) {
    return avSensors[index].enabled;
}

void CONFIG_SetAvSensorEnabled(uint8_t index, uint8_t value) {
    avSensors[index].enabled = value;
}

uint8_t CONFIG_GetAvSensorIndicatorEnabled(uint8_t index) {
    return avSensors[index].indicatorEnabled;
}

void CONFIG_SetAvSensorIndicatorEnabled(uint8_t index, uint8_t value) {
    avSensors[index].indicatorEnabled = value;
}

IndicatorStatus CONFIG_GetAvSensorStatus(uint8_t index) {
    return (IndicatorStatus)avSensors[index].statuses;
}

void CONFIG_SetAvSensorStatus(uint8_t index, IndicatorStatus status) {
    avSensors[index].statuses = status;
}

uint16_t CONFIG_GetAvSensorMinOff(uint8_t index) {
    return avSensors[index].minOff;
}

void CONFIG_SetAvSensorMinOff(uint8_t index, uint16_t value) {
    avSensors[index].minOff = value;
}

uint16_t CONFIG_GetAvSensorMaxOff(uint8_t index) {
    return avSensors[index].maxOff;
}

void CONFIG_SetAvSensorMaxOff(uint8_t index, uint16_t value) {
    avSensors[index].maxOff = value;
}

uint16_t CONFIG_GetAvSensorMinSlow(uint8_t index) {
    return avSensors[index].minSlow;
}

void CONFIG_SetAvSensorMinSlow(uint8_t index, uint16_t value) {
    avSensors[index].minSlow = value;
}

uint16_t CONFIG_GetAvSensorMaxSlow(uint8_t index) {
    return avSensors[index].maxSlow;
}

void CONFIG_SetAvSensorMaxSlow(uint8_t index, uint16_t value) {
    avSensors[index].maxSlow = value;
}

uint16_t CONFIG_GetAvSensorMinFast(uint8_t index) {
    return avSensors[index].minFast;
}

void CONFIG_SetAvSensorMinFast(uint8_t index, uint16_t value) {
    avSensors[index].minFast = value;
}

uint16_t CONFIG_GetAvSensorMaxFast(uint8_t index) {
    return avSensors[index].maxFast;
}

void CONFIG_SetAvSensorMaxFast(uint8_t index, uint16_t value) {
    avSensors[index].maxFast = value;
}

uint16_t CONFIG_GetAvSensorMinOn(uint8_t index) {
    return avSensors[index].minOn;
}

void CONFIG_SetAvSensorMinOn(uint8_t index, uint16_t value) {
    avSensors[index].minOn = value;
}

uint16_t CONFIG_GetAvSensorMaxOn(uint8_t index) {
    return avSensors[index].maxOn;
}

void CONFIG_SetAvSensorMaxOn(uint8_t index, uint16_t value) {
    avSensors[index].maxOn = value;
}



















extern bool v1Enabled, v2Enabled, v3Enabled, v1LedEnabled, v2LedEnabled, v3LedEnabled;
extern bool servo1Enabled, servo2Enabled, spiEnabled, i2cEnabled;
extern bool actuatorsEnabled[8];
extern bool ttlEnabled, tncEnabled, ttlIndicatorEnabled, buzzerEnabled;
extern uint16_t ttlBaudRate, tncBaudRate;


bool CONFIG_IsConfigured(void) {
    uint16_t configFlag;
    EEPROM_ReadMultipleWords(EEPROM_CONFIG_FLAG_ADDR, &configFlag, 1);
    return configFlag == 0xEEEE;  // Assuming 0xEEEE indicates configured EEPROM
}

void CONFIG_LoadSettingsFromEEPROM(void) {
    EEPROM_ReadMultipleWords(EEPROM_SERIAL_NUMBER_ADDR, (uint16_t*)serialNumber, 13);
    EEPROM_ReadMultipleWords(EEPROM_CALLSIGN_ADDR, (uint16_t*)callsign, 5);
    EEPROM_ReadMultipleWords(EEPROM_REGISTRATION_NUMBER_ADDR, &registrationNumber, 2);

    EEPROM_ReadMultipleWords(EEPROM_V1_ENABLED_ADDR, (uint16_t*)&v1Enabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_V2_ENABLED_ADDR, (uint16_t*)&v2Enabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_V3_ENABLED_ADDR, (uint16_t*)&v3Enabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_V1_LED_ENABLED_ADDR, (uint16_t*)&v1LedEnabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_V2_LED_ENABLED_ADDR, (uint16_t*)&v2LedEnabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_V3_LED_ENABLED_ADDR, (uint16_t*)&v3LedEnabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_SERVO1_ENABLED_ADDR, (uint16_t*)&servo1Enabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_SERVO2_ENABLED_ADDR, (uint16_t*)&servo2Enabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_SPI_ENABLED_ADDR, (uint16_t*)&spiEnabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_I2C_ENABLED_ADDR, (uint16_t*)&i2cEnabled, 1);

    for (int i = 0; i < 8; i++) {
        EEPROM_ReadMultipleWords(EEPROM_ACTUATORS_ENABLED_ADDR + i, (uint16_t*)&actuatorsEnabled[i], 1);
    }

    EEPROM_ReadMultipleWords(EEPROM_TTL_ENABLED_ADDR, (uint16_t*)&ttlEnabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_TNC_ENABLED_ADDR, (uint16_t*)&tncEnabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_TTL_INDICATOR_ENABLED_ADDR, (uint16_t*)&ttlIndicatorEnabled, 1);
    EEPROM_ReadMultipleWords(EEPROM_BUZZER_ENABLED_ADDR, (uint16_t*)&buzzerEnabled, 1);

    EEPROM_ReadMultipleWords(EEPROM_TTL_BAUD_RATE_ADDR, &ttlBaudRate, 1);
    EEPROM_ReadMultipleWords(EEPROM_TNC_BAUD_RATE_ADDR, &tncBaudRate, 1);
}

void CONFIG_SetDefaultSettings(void) {
    memset(serialNumber, 0, SERIAL_NUMBER_LENGTH);
    memset(callsign, 0, CALLSIGN_LENGTH);
    registrationNumber = 0;

    v1Enabled = false;
    v2Enabled = false;
    v3Enabled = false;
    v1LedEnabled = false;
    v2LedEnabled = false;
    v3LedEnabled = false;
    servo1Enabled = false;
    servo2Enabled = false;
    spiEnabled = false;
    i2cEnabled = false;

    for (int i = 0; i < 8; i++) {
        actuatorsEnabled[i] = false;
    }

    ttlEnabled = false;
    tncEnabled = false;
    ttlIndicatorEnabled = false;
    buzzerEnabled = false;

    ttlBaudRate = 8;  // Default baud rate
    tncBaudRate = 2;  // Default baud rate
}


void CONFIG_EnableV1(void) {
    v1Enabled = true;
    EEPROM_WriteBoolean(EEPROM_V1_ENABLED_ADDR, v1Enabled);
}

void CONFIG_DisableV1(void) {
    v1Enabled = false;
    EEPROM_WriteBoolean(EEPROM_V1_ENABLED_ADDR, v1Enabled);
}

void CONFIG_EnableV1LED(void) {
    v1LedEnabled = true;
    EEPROM_WriteBoolean(EEPROM_V1_LED_ENABLED_ADDR, v1LedEnabled);
}

void CONFIG_DisableV1LED(void) {
    v1LedEnabled = false;
    EEPROM_WriteBoolean(EEPROM_V1_LED_ENABLED_ADDR, v1LedEnabled);
}

void CONFIG_EnableV2(void) {
    v2Enabled = true;
    EEPROM_WriteBoolean(EEPROM_V2_ENABLED_ADDR, v2Enabled);
}

void CONFIG_DisableV2(void) {
    v2Enabled = false;
    EEPROM_WriteBoolean(EEPROM_V2_ENABLED_ADDR, v2Enabled);
}

void CONFIG_EnableV2LED(void) {
    v2LedEnabled = true;
    EEPROM_WriteBoolean(EEPROM_V2_LED_ENABLED_ADDR, v2LedEnabled);
}

void CONFIG_DisableV2LED(void) {
    v2LedEnabled = false;
    EEPROM_WriteBoolean(EEPROM_V2_LED_ENABLED_ADDR, v2LedEnabled);
}

void CONFIG_EnableV3(void) {
    v3Enabled = true;
    EEPROM_WriteBoolean(EEPROM_V3_ENABLED_ADDR, v3Enabled);
}

void CONFIG_DisableV3(void) {
    v3Enabled = false;
    EEPROM_WriteBoolean(EEPROM_V3_ENABLED_ADDR, v3Enabled);
}

void CONFIG_EnableV3LED(void) {
    v3LedEnabled = true;
    EEPROM_WriteBoolean(EEPROM_V3_LED_ENABLED_ADDR, v3LedEnabled);
}

void CONFIG_DisableV3LED(void) {
    v3LedEnabled = false;
    EEPROM_WriteBoolean(EEPROM_V3_LED_ENABLED_ADDR, v3LedEnabled);
}

void CONFIG_EnableServo1(void) {
    servo1Enabled = true;
    EEPROM_WriteBoolean(EEPROM_SERVO1_ENABLED_ADDR, servo1Enabled);
}

void CONFIG_DisableServo1(void) {
    servo1Enabled = false;
    EEPROM_WriteBoolean(EEPROM_SERVO1_ENABLED_ADDR, servo1Enabled);
}

void CONFIG_EnableServo2(void) {
    servo2Enabled = true;
    EEPROM_WriteBoolean(EEPROM_SERVO2_ENABLED_ADDR, servo2Enabled);
}

void CONFIG_DisableServo2(void) {
    servo2Enabled = false;
    EEPROM_WriteBoolean(EEPROM_SERVO2_ENABLED_ADDR, servo2Enabled);
}

void CONFIG_EnableSPI(void) {
    spiEnabled = true;
    EEPROM_WriteBoolean(EEPROM_SPI_ENABLED_ADDR, spiEnabled);
}

void CONFIG_DisableSPI(void) {
    spiEnabled = false;
    EEPROM_WriteBoolean(EEPROM_SPI_ENABLED_ADDR, spiEnabled);
}

void CONFIG_EnableI2C(void) {
    i2cEnabled = true;
    EEPROM_WriteBoolean(EEPROM_I2C_ENABLED_ADDR, i2cEnabled);
}

void CONFIG_DisableI2C(void) {
    i2cEnabled = false;
    EEPROM_WriteBoolean(EEPROM_I2C_ENABLED_ADDR, i2cEnabled);
}

void CONFIG_EnableActuator(uint8_t index) {
    if (index < 8) {
        actuatorsEnabled[index] = true;
        EEPROM_WriteBoolean(EEPROM_ACTUATORS_ENABLED_ADDR + index, actuatorsEnabled[index]);
    }
}

void CONFIG_DisableActuator(uint8_t index) {
    if (index < 8) {
        actuatorsEnabled[index] = false;
        EEPROM_WriteBoolean(EEPROM_ACTUATORS_ENABLED_ADDR + index, actuatorsEnabled[index]);
    }
}

void CONFIG_EnableTTL(void) {
    ttlEnabled = true;
    EEPROM_WriteBoolean(EEPROM_TTL_ENABLED_ADDR, ttlEnabled);
}

void CONFIG_DisableTTL(void) {
    ttlEnabled = false;
    EEPROM_WriteBoolean(EEPROM_TTL_ENABLED_ADDR, ttlEnabled);
}


void CONFIG_EnableTTLIndicator(void) {
    ttlIndicatorEnabled = true;
    EEPROM_WriteBoolean(EEPROM_TTL_INDICATOR_ENABLED_ADDR, ttlIndicatorEnabled);
}

void CONFIG_DisableTTLIndicator(void) {
    ttlIndicatorEnabled = false;
    EEPROM_WriteBoolean(EEPROM_TTL_INDICATOR_ENABLED_ADDR, ttlIndicatorEnabled);
}


void CONFIG_EnableTNC(void) {
    tncEnabled = true;
    EEPROM_WriteBoolean(EEPROM_TNC_ENABLED_ADDR, tncEnabled);
}

void CONFIG_DisableTNC(void) {
    tncEnabled = false;
    EEPROM_WriteBoolean(EEPROM_TNC_ENABLED_ADDR, tncEnabled);
}

void CONFIG_EnableBuzzer(void) {
    buzzerEnabled = true;
    EEPROM_WriteBoolean(EEPROM_BUZZER_ENABLED_ADDR, buzzerEnabled);
}

void CONFIG_DisableBuzzer(void) {
    buzzerEnabled = false;
    EEPROM_WriteBoolean(EEPROM_BUZZER_ENABLED_ADDR, buzzerEnabled);
}
