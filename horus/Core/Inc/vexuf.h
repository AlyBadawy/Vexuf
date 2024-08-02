/*
 * vexuf.h
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_H_
#define INC_VEXUF_H_

#include "main.h"
#include "93c86.h"
#include "adc_avs.h"
#include "actuators.h"
#include "i2c_Checker.h"
#include "i2c_aht20.h"
#include "indicators.h"
#include "sd_card.h"
#include "config.h"

// Define the struct for an AV sensor
typedef struct {
    uint16_t enabled : 1;                // 1 bit
    uint16_t indicatorEnabled : 1;       // 1 bit
    uint16_t statuses : 2;               // 2 bits (00: OFF, 01: SLOW, 10: FAST, 11: ON)
    uint16_t reserved : 12;              // 12 bits (padding to make 16 bits total)
    uint16_t minOff;                     // 16 bits
    uint16_t maxOff;                     // 16 bits
    uint16_t minSlow;                    // 16 bits
    uint16_t maxSlow;                    // 16 bits
    uint16_t minFast;                    // 16 bits
    uint16_t maxFast;                    // 16 bits
    uint16_t minOn;                      // 16 bits
    uint16_t maxOn;                      // 16 bits
} AvSensor;

// Define the possible trigger sources
typedef enum {
    Av1,
    Av2,
    Av3,
    InternalTemp,
    ExternalTemp
} TriggerSource;

// Define the struct for a trigger
typedef struct {
    uint16_t enabled : 1;           // 1 bit
    uint16_t actuators : 8;         // 8 bits (one for each actuator)
    uint16_t buzzer : 1;            // 1 bit
    uint16_t logged : 1;            // 1 bit
    uint16_t source : 3;            // 3 bits (enough to hold 5 possible values)
    uint16_t reserved : 2;          // 2 bits (padding to make 16 bits total)
    uint16_t pwm1;                  // 16 bits
    uint16_t pwm2;                  // 16 bits
    uint16_t min;                   // 16 bits
    uint16_t max;                   // 16 bits
} Trigger;




typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} IndicatorPin;


float cToF(float c);
float fToC(float f);
void base32_encode(const uint8_t *data, size_t length, char *output);

void SerialNumber_test(void);
void temperatureInternal_Test(void);

void VexUF_DateTimeString(char *string);
void VexUF_SerialNumber(char *serial_number);

void VexUF_USBWelcomeMessage(void);

#endif /* INC_VEXUF_H_ */
