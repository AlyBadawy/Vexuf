/*
 * indicators.c
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */


#include "vexuf_indicators.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

static uint16_t indicatorsStatus = 0;

static IndicatorPin indicatorPins[] = {
    {ErrorInd_GPIO_Port, ErrorInd_Pin}, // ErrorInd
    {WarnInd_GPIO_Port, WarnInd_Pin}, // WarnInd
    {InfoInd_GPIO_Port, InfoInd_Pin}, // InfoInd
	{SdioInd_GPIO_Port, SdioInd_Pin}, // SDIOInd
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
    if (ind < IndError || ind > IndAv3 || status > 0b11) {
        return; // Invalid indicator or status
    }
    // Return if current status equals new status
    if (((indicatorsStatus >> (ind * 2)) & 0b11) == (status << (ind * 2))) {
    	return;
    }
    // Buzzer can only be on, off, or slow. Not fast
    if (ind == IndBuzzer && status == IndFAST) {
    	status = IndSLOW;
    }
    // Special handling for mutual exclusivity
    if ((status != IndOFF) && (ind == IndError || ind == IndWarn || ind == IndInfo)) {
        indicatorsStatus &= ~(0b11 << (IndError * 2)); // Clear ErrorInd
        indicatorsStatus &= ~(0b11 << (IndWarn * 2));  // Clear WarnInd
        indicatorsStatus &= ~(0b11 << (IndInfo * 2));  // Clear InfoInd
    }
    // Clear the current status bits for the indicator
    indicatorsStatus &= ~(0b11 << (ind * 2));
    // Set the new status
    indicatorsStatus |= (status << (ind * 2));

    if (status == IndON) {
    	HAL_GPIO_WritePin(indicatorPins[ind].port, indicatorPins[ind].pin, GPIO_PIN_SET);
    } else if (status == IndOFF) {
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

	for (Indicator ind = IndError; ind <= IndAv3; ind++) {
		IndicatorStatus s = Indicators_getStatus(ind);
		if (s == status) {
			HAL_GPIO_TogglePin(indicatorPins[ind].port, indicatorPins[ind].pin);
		}
	}
}

IndicatorStatus Indicators_getStatus(Indicator ind) {
    if (ind < IndError || ind > IndAv3) {
        return IndOFF; // Invalid indicator
    }

    return (indicatorsStatus >> (ind * 2)) & 0b11;
}





