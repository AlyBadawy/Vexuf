/*
 * vexuf_triggers.c
 *
 *  Created on: Aug 12, 2024
 *      Author: Aly Badawy
 */


#include "vexuf_triggers.h"
#include "vexuf_adc_avs.h"
#include "vexuf_pwm.h"

extern TriggerConfiguration triggers[TRIGS_COUNT];
extern uint32_t adcBuffer[5];
extern PwmConfiguration pwmConfig;


void TRIGGERS_runAll(void) {
	for (uint8_t i = 0; i < TRIGS_COUNT; i++) {
		TRIGGERS_runTrig(i);
	}
	ACTUATORS_updateShiftReg();
	PWM_setDutyCycle(PwmChannel1, pwmConfig.pwm1Value);
	PWM_setDutyCycle(PwmChannel2, pwmConfig.pwm2Value);
}

void TRIGGERS_runTrig(uint8_t index) {
	TrigComparison comparison;
	TrigCompareTest test;
	uint16_t fromValue,  toValue;

	comparison = triggers[index].status;
	test = triggers[index].test;
	fromValue = triggers[index].fromValue;
	toValue = triggers[index].toValue;

	bool triggered = TRIGGERS_isTriggered(comparison, test, fromValue, toValue);
	printf("Trigger #%i returned: %s\r\n", index, (triggered ? "True" : "False"));
	if (!triggered) return;



	// TODO: Act on trigger outputs!
	ACTUATORS_trigegr(triggers[index].actuators);

	if (triggers[index].pwm1 != 0xFFFF) pwmConfig.pwm1Value = triggers[index].pwm1;
	if (triggers[index].pwm2 != 0xFFFF) pwmConfig.pwm2Value = triggers[index].pwm2;


}

bool TRIGGERS_isTriggered(TrigComparison comparison, TrigCompareTest test, uint32_t fromValue, uint32_t toValue) {
	switch (comparison) {
	case Av1ToAv2:
		return TRIGGERS_compareValues(adcBuffer[2], test, adcBuffer[3], 0);
	case Av2ToAv3:
		return TRIGGERS_compareValues(adcBuffer[3], test, adcBuffer[4], 0);
	case Av1ToAv3:
		return TRIGGERS_compareValues(adcBuffer[2], test, adcBuffer[4], 0);
	case IntTempToExternal:
		return false; // TODO: IMPLEMENT EXTERNAL TEMP.
	case Av1ToTest:
		return TRIGGERS_compareValues(adcBuffer[2], test, fromValue, toValue);
	case Av2ToTest:
		return TRIGGERS_compareValues(adcBuffer[3], test, fromValue, toValue);
	case Av3ToTest:
		return TRIGGERS_compareValues(adcBuffer[4], test, fromValue, toValue);
	case IntTempToTest:
		return TRIGGERS_compareValues(adcBuffer[1], test, fromValue, toValue);
	case ExtTempToTest:
		return false; // TODO: IMPLEMENT EXTERNAL TEMP.
	default:
		return false;
	}
}

bool TRIGGERS_compareValues(uint16_t ref, TrigCompareTest test, uint32_t fromValue, uint32_t toValue) {
	switch (test) {
	case lessThan:
		return ref < fromValue;
	case lessThanOrEqual:
		return ref <= fromValue;
	case Equal:
		return ref == fromValue;
	case GreaterThanOrEqual:
		return ref >= fromValue;
	case GreaterThan:
		return ref > fromValue;
	case NotEqual:
		return ref != fromValue;
	case inRange:
		return ref >= fromValue && ref <= toValue;
	case outtaRange:
		return !(ref >= fromValue && ref <= toValue);
	default:
		return false;
	}
}
