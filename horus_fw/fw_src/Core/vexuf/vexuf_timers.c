/*
 * vexuf_timers.c
 *
 *  Created on: Aug 10, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_timers.h"
#include "vexuf_indicators.h"
#include "vexuf_helpers.h"

extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

extern VexufStatus vexuf_status;

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
	vexuf_status.timer_0d1hz_ticked = 0;
	vexuf_status.timer_1hz_ticked = 0;
	vexuf_status.timer_10hz_ticked = 0;

}

void TIMERS_10Hz(void) {
	IND_toggleIndWithStatusOption(IndFAST);
}
void TIMERS_1Hz(void) {
	vexuf_status.timer_1hz_ticked = 1;
	IND_toggleIndWithStatusOption(IndSLOW);
}
void TIMERS_0d1Hz(void){
	vexuf_status.timer_0d1hz_ticked = 1;
}



