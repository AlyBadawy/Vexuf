/*
 * vexuf_pwm.c
 *
 *  Created on: Aug 12, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_pwm.h"

extern PwmConfiguration pwmConfig;

extern TIM_HandleTypeDef htim3;     // LCD Back-light Timer
extern TIM_HandleTypeDef htim10;	// Servo 2 Timer
extern TIM_HandleTypeDef htim11;	// Serco 1 Timer

void PWM_init(void) {
	if (pwmConfig.pwm1Enabled == 1 && pwmConfig.pwm1Value <= 1000) {
		PWM_Start(PwmChannel1);
		PWM_setDutyCycle(PwmChannel1, pwmConfig.pwm1Value);
	}
	if (pwmConfig.pwm2Enabled == 1 && pwmConfig.pwm2Value <= 1000) {
		PWM_Start(PwmChannel2);
		PWM_setDutyCycle(PwmChannel2, pwmConfig.pwm2Value);
	}
}

void PWM_Start(PwmChannel channel) {
	switch (channel) {
	case PwmChannel1:
		if (pwmConfig.pwm1Enabled != 1) return;
	    HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
	    break;
	case PwmChannel2:
		if (pwmConfig.pwm2Enabled != 1) return;
	    HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
	    break;
	default:
		break;
	}
}

void PWM_Stop(PwmChannel channel) {
	switch (channel) {
	case PwmChannel1:
	    HAL_TIM_PWM_Stop(&htim11, TIM_CHANNEL_1);
	    break;
	case PwmChannel2:
		HAL_TIM_PWM_Stop(&htim10, TIM_CHANNEL_1);
	    break;
	default:
		break;
	}
}

void PWM_setDutyCycle(PwmChannel channel, uint16_t dutyCycle) {
	if (dutyCycle == 0xFFFF) return; // 0xFFFF means leave unchanged

	if (dutyCycle > 1000) dutyCycle = 1000;

	switch (channel) {
		case PwmChannel1:
			if (pwmConfig.pwm1Enabled != 1) return;
			__HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, dutyCycle);
		    break;
		case PwmChannel2:
			if (pwmConfig.pwm2Enabled != 1) return;
			__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, dutyCycle);
		    break;
		default:
			break;
		}
}
