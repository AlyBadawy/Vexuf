/*
 * actuators.h
 *
 *  Created on: Jul 22, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_ACTUATORS_H_
#define INC_ACTUATORS_H_

#include "main.h"

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


// Function prototypes
void Actuators_Write(uint8_t data);
void Actuators_SetPin(ActuatorPin pin);
void Actuators_ResetPin(ActuatorPin pin);
void Actuators_Update(void);
void Actuators_Lights(GPIO_PinState state);
void Actuators_Test(void);

#endif /* INC_ACTUATORS_H_ */




