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

void TIMERS_10Hz(void);
void TIMERS_1Hz(void);
void TIMERS_0d1Hz(void);


#endif /* VEXUF_VEXUF_TIMERS_H_ */
