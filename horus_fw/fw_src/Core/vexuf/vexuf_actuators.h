/*
 * actuators.h
 *
 *  Created on: Jul 22, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_ACTUATORS_H_
#define INC_VEXUF_ACTUATORS_H_


#include "vexuf.h"

#define  NUMBER_OF_Actuators			8

typedef enum {ActOff = 0, ActOn = 1, ActUnchanged = 2} ActStatus;

// Define an enum for the shift register pins
typedef enum {
    ACT_PIN_A1 = 0,
    ACT_PIN_A2,
    ACT_PIN_A3,
    ACT_PIN_A4,
    ACT_PIN_A5,
    ACT_PIN_A6,
    ACT_PIN_A7,
    ACT_PIN_A8
} ActuatorPin;

typedef struct {
	uint16_t act1:2;
	uint16_t act2:2;
	uint16_t act3:2;
	uint16_t act4:2;
	uint16_t act5:2;
	uint16_t act6:2;
	uint16_t act7:2;
	uint16_t act8:2;
} ActuatorsValues;

// Function prototypes
void Actuators_Write(uint8_t data);
void Actuators_SetPin(ActuatorPin pin);
void Actuators_ResetPin(ActuatorPin pin);
void Actuators_Update(void);
void Actuators_Lights(GPIO_PinState state);
void Actuators_Test(void);

#endif /* INC_VEXUF_ACTUATORS_H_ */




