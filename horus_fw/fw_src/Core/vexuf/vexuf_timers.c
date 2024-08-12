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

extern bool timerTicked;

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

void TIMERS_10Hz(void) {
	Indicators_toggleIndWithStatus(IndFAST);
}
void TIMERS_1Hz(void) {
	Indicators_toggleIndWithStatus(IndSLOW);
}
void TIMERS_0d1Hz(void){
	timerTicked = true;
}



