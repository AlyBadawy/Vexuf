/*
 * vexuf_timers.c
 *
 *  Created on: Aug 10, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_timers.h"
#include "vexuf_indicators.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM4) { // every 100ms (10Hz);
    	Indicators_toggleIndWithStatus(IndFAST);
    } else if (htim->Instance == TIM9) { // every 1s (1Hz)
    	Indicators_toggleIndWithStatus(IndSLOW);
    } else if (htim->Instance == TIM5) { // every 10 seconds (0.1Hz)

    }
}

void TIMERS_Start(void) {
	// Start TIM4, TIM9, TIM5 in interrupt mode
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_Base_Start_IT(&htim9);
}


void TIMERS_Stop(void) {
	HAL_TIM_Base_Stop_IT(&htim4);
	HAL_TIM_Base_Stop_IT(&htim5);
	HAL_TIM_Base_Stop_IT(&htim9);
}



