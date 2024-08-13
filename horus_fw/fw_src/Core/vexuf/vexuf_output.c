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
	if (outputConfig.buzzer_hold_on_error) {
		Indicators_setStatus(IndBuzzer, IndON);
	}
}

void OUTPUT_BuzzOnStartUp(void) {
	if (outputConfig.buzzer_1s_enabled) {
		Indicators_setStatus(IndBuzzer, IndON);
		HAL_Delay(1000);
		Indicators_setStatus(IndBuzzer, IndOFF);
	}
}
