/*
 * bexuf_timers.h
 *
 *  Created on: Aug 10, 2024
 *      Author: Aly Badawy
 */

#ifndef VEXUF_VEXUF_TIMERS_H_
#define VEXUF_VEXUF_TIMERS_H_

#include "../inc/main.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void TIMERS_Start(void);
void TIMERS_Stop(void);


#endif /* VEXUF_VEXUF_TIMERS_H_ */
