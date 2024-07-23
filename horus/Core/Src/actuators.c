/*
 * actuators.c
 *
 *  Created on: Jul 22, 2024
 *      Author: Aly Badawy
 */

#include "actuators.h"

static uint8_t actuatorsData = 0;

void Actuators_Write(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        // Write the data bit by bit
        HAL_GPIO_WritePin(ActDa_GPIO_Port, ActDa_Pin, (data & (1 << (7 - i))) ? GPIO_PIN_SET : GPIO_PIN_RESET);

        // Toggle the clock pin
        HAL_GPIO_WritePin(ActCk_GPIO_Port, ActCk_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ActCk_GPIO_Port, ActCk_Pin, GPIO_PIN_RESET);
    }

    // Toggle the latch pin to output the data
    HAL_GPIO_WritePin(ActLa_GPIO_Port, ActLa_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ActLa_GPIO_Port, ActLa_Pin, GPIO_PIN_RESET);
}

void Actuators_SetPin(ActuatorPin pin) {
    if (pin >= ACT_PIN_A1 && pin <= ACT_PIN_A8) {
    	actuatorsData |= (1 << (pin));
        Actuators_Update();
    }
}

void Actuators_ResetPin(ActuatorPin pin) {
    if (pin >= ACT_PIN_A1 && pin <= ACT_PIN_A8) {
    	actuatorsData &= ~(1 << (pin));
        Actuators_Update();
    }
}

void Actuators_Update(void) {
	Actuators_Write(actuatorsData);
}

void Actuators_Lights(GPIO_PinState state) {
	HAL_GPIO_WritePin(ActLd_GPIO_Port, ActLd_Pin, state);
}

