/*
 * indicators.c
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */


#include <vexuf_indicators.h>

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

static uint16_t indicatorsStatus = 0;

static IndicatorPin indicatorPins[] = {
    {ErrorInd_GPIO_Port, ErrorInd_Pin}, // ErrorInd
    {WarnInd_GPIO_Port, WarnInd_Pin}, // WarnInd
    {InfoInd_GPIO_Port, InfoInd_Pin}, // InfoInd
    {Buzzer_GPIO_Port, Buzzer_Pin}, // Buzzer
    {Av1Indicator_GPIO_Port, Av1Indicator_Pin}, // Av1Ind
    {Av2Indicator_GPIO_Port, Av2Indicator_Pin}, // Av2Ind
    {Av3Indicator_GPIO_Port, Av3Indicator_Pin}  // Av3Ind
};

void Indicators_init(void) {

    // Start TIM4, TIM9, TIM5 in interrupt mode
    HAL_TIM_Base_Start_IT(&htim4);
    HAL_TIM_Base_Start_IT(&htim9);
    HAL_TIM_Base_Start_IT(&htim5);

}

void Indicators_setStatus(Indicator ind, IndicatorStatus status) {
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

    if (status == ON) {
    	HAL_GPIO_WritePin(indicatorPins[ind].port, indicatorPins[ind].pin, GPIO_PIN_SET);
    } else if (status == OFF) {
    	HAL_GPIO_WritePin(indicatorPins[ind].port, indicatorPins[ind].pin, GPIO_PIN_RESET);
    }
}

void Indicators_clearAllStatuses(void) {
    indicatorsStatus = 0;
}

void Indicators_toggleIndWithStatus(IndicatorStatus status) {
	if (status > 0b11) {
		return; // Invalid status
	}

	for (Indicator ind = ErrorInd; ind <= Av3Ind; ind++) {
		IndicatorStatus s = Indicators_getStatus(ind);
		if (s == status) {
			HAL_GPIO_TogglePin(indicatorPins[ind].port, indicatorPins[ind].pin);
		}
	}
}

IndicatorStatus Indicators_getStatus(Indicator ind) {
    if (ind < ErrorInd || ind > Av3Ind) {
        return OFF; // Invalid indicator
    }

    return (indicatorsStatus >> (ind * 2)) & 0b11;
}





