/*
 * indicators.c
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */


#include "vexuf_indicators.h"
#include "vexuf_helpers.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

extern IndStatus ind_status;


static IndicatorPin indicatorPins[] = {
    {ErrorInd_GPIO_Port, ErrorInd_Pin},
    {WarnInd_GPIO_Port, WarnInd_Pin},
    {InfoInd_GPIO_Port, InfoInd_Pin},
    {Av1Indicator_GPIO_Port, Av1Indicator_Pin},
    {Av2Indicator_GPIO_Port, Av2Indicator_Pin},
    {Av3Indicator_GPIO_Port, Av3Indicator_Pin},
	{Buzzer_GPIO_Port, Buzzer_Pin},
	{SdioInd_GPIO_Port, SdioInd_Pin}

};

void IND_applyStatusesToGPIO(void);

IndStatusOption IND_getCurrentStatus(Indicator ind) {
	switch (ind) {
	case(IndError):
		return ind_status.indErrorStatus;
	case(IndWarn):
		return ind_status.indWarnStatus;
	case(IndInfo):
		return ind_status.indInfoStatus;
	case(IndAv1):
		return ind_status.indAv1Status;
	case(IndAv2):
		return ind_status.indAv2Status;
	case(IndAv3):
		return ind_status.indAv3Status;
	case(IndBuzzer):
		return ind_status.indBuzzerStatus;
	case(IndSdio):
		return ind_status.indSdioStatus;
	default:
		return IndOFF;
	}
}

void IND_setStatus(Indicator ind, IndStatusOption option) {
    // Return if current status equals new status
    if (IND_getCurrentStatus(ind) == option) {
    	return;
    }

    // Special handling for mutual exclusivity
    if ((option != IndOFF) && (ind == IndError || ind == IndWarn || ind == IndInfo)) {
    	ind_status.indErrorStatus = IndOFF;
    	ind_status.indWarnStatus = IndOFF;
    	ind_status.indInfoStatus = IndOFF;
    }

    switch (ind) {
    case(IndError):
		ind_status.indErrorStatus = option;
   		break;
	case(IndWarn):
		ind_status.indWarnStatus = option;
		break;
	case(IndInfo):
		ind_status.indInfoStatus = option;
		break;
	case(IndAv1):
		ind_status.indErrorStatus = option;
		break;
	case(IndAv2):
		ind_status.indErrorStatus = option;
		break;
	case(IndAv3):
		ind_status.indErrorStatus = option;
		break;
	case(IndBuzzer):
		if(option == IndFAST) {
			ind_status.indBuzzerStatus = IndSLOW;
		} else {
			ind_status.indBuzzerStatus = option;
		}
		break;
	case(IndSdio):
		if(option == IndFAST || option == IndSLOW) {
			ind_status.indSdioStatus = IndON;
		} else {
			ind_status.indSdioStatus = option;
		}
		break;
	default:
		break;
	}

    IND_applyStatusesToGPIO();
}



void IND_toggleIndWithStatusOption(IndStatusOption option) {
	if (option != IndFAST || option != IndSLOW) {
		return; // Invalid status
	}

	for (Indicator ind = IndError; ind <= IndAv3; ind++) {
		IndStatusOption s = IND_getCurrentStatus(ind);
		if (s == option) {
			HAL_GPIO_TogglePin(indicatorPins[ind].port, indicatorPins[ind].pin);
		}
	}
}

void IND_applyStatusesToGPIO(void) {
	for (Indicator ind = IndError; ind <= IndSdio; ind++) {
		IndStatusOption status = IND_getCurrentStatus(ind);
		if (status == IndON) {
			HAL_GPIO_WritePin(indicatorPins[ind].port, indicatorPins[ind].pin, GPIO_PIN_SET);
		} else if (status == IndOFF) {
			HAL_GPIO_WritePin(indicatorPins[ind].port, indicatorPins[ind].pin, GPIO_PIN_RESET);
		}
	}
}





