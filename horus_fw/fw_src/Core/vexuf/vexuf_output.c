/*
 * vexuf_output.c
 *
 *  Created on: Aug 13, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_output.h"
#include "vexuf_indicators.h"

extern OutputConfiguration outputConfig;

void OUTPUT_buzzOnError(void) {
//	if (outputConfig.buzzer_hold_on_error) {
//		IND_setStatus(IndBuzzer, IndON);
//	}
}

void OUTPUT_BuzzOnStartUp(void) {
	if (outputConfig.buzzer_1s_enabled) {
		for(uint8_t i = 0; i <3; i++) {
			IND_setStatus(IndBuzzer, IndON);
			HAL_Delay(20);
			IND_setStatus(IndBuzzer, IndOFF);
			HAL_Delay(40);
		}
	}
}
