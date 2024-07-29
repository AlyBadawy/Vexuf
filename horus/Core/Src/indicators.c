/*
 * indicators.c
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */


#include "indicators.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

static uint16_t indicatorsStatus = 0;

static IndicatorPin indicatorPins[] = {
    {ErrorInd_GPIO_Port, ErrorInd_Pin}, // ErrorInd
    {WarnInd_GPIO_Port, WarnInd_Pin}, // WarnInd
    {InfoInd_GPIO_Port, InfoInd_Pin}, // InfoInd
    {Buzzer_GPIO_Port, Buzzer_Pin}, // Buzzer
    {Av1Ind_GPIO_Port, Av1Ind_Pin}, // Av1Ind
    {Av2Ind_GPIO_Port, Av2Ind_Pin}, // Av2Ind
    {Av2Ind_GPIO_Port, Av2Ind_GPIO_Port}  // Av3Ind
};

void Indicators_init(void) {

    // Start TIM4, TIM9, TIM5 in interrupt mode
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim9);
    HAL_TIM_Base_Start_IT(&htim5);

}

void Indicators_setStatus(Indicator ind, uint8_t status) {
    if (ind < ErrorInd || ind > Av3Ind || status > 0b11) {
        return; // Invalid indicator or status
    }

    // Buzzer can only be on, off, or slow. Not fast
    if (ind == Buzzer && status == FAST) {
    	status = SLOW;
    }

    // Special handling for mutual exclusivity
    if ((status != OFF) && (ind == ErrorInd || ind == WarnInd || ind == InfoInd)) {
        indicatorsStatus &= ~(0b11 << (ErrorInd * 2)); // Clear ErrorInd
        indicatorsStatus &= ~(0b11 << (WarnInd * 2));  // Clear WarnInd
        indicatorsStatus &= ~(0b11 << (InfoInd * 2));  // Clear InfoInd
    }

    // Clear the current status bits for the indicator
    indicatorsStatus &= ~(0b11 << (ind * 2));

    // Set the new status
    indicatorsStatus |= (status << (ind * 2));
}

void Indicators_clearAllStatuses(void) {
    indicatorsStatus = 0;
}

void Indicators_toggleIndWithStatus(uint8_t status) {
	if (status > 0b11) {
		return; // Invalid status
	}

	for (Indicator ind = ErrorInd; ind <= Av3Ind; ind++) {
		if (Indicators_getStatus(ind) == status) {
			HAL_GPIO_TogglePin(indicatorPins[ind].port, indicatorPins[ind].pin);
		}
	}
}

uint8_t Indicators_getStatus(Indicator ind) {
    if (ind < ErrorInd || ind > Av3Ind) {
        return OFF; // Invalid indicator
    }

    return (indicatorsStatus >> (ind * 2)) & 0b11;
}





