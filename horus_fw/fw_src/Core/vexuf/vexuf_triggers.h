/*
 * vexuf_troggers.h
 *
 *  Created on: Aug 12, 2024
 *      Author: Aly Badawy
 */

#ifndef VEXUF_VEXUF_TRIGGERS_H_
#define VEXUF_VEXUF_TRIGGERS_H_

#include "vexuf.h"
#include "vexuf_real_time.h"
#include "vexuf_actuators.h"
#include "vexuf_output.h"

#define  TRIGS_COUNT		25


typedef enum {
	noTest 					= 0,
	lessThan 				= 1,
	lessThanOrEqual 		= 2,
	Equal 					= 3,
	GreaterThanOrEqual 		= 4,
	GreaterThan 			= 5,
	NotEqual 				= 6,
	inRange 				= 7,
	outtaRange				= 8
} TrigCompareTest;

typedef enum {
	trigDisabled 			= 0,
	Av1ToAv2				= 1,
	Av2ToAv3				= 2,
	Av1ToAv3				= 3,
	IntTempToExternal		= 4,
	Av1ToTest				= 5,
	Av2ToTest				= 6,
	Av3ToTest				= 7,
	IntTempToTest			= 8,
	ExtTempToTest			= 9
} TrigComparison;


typedef struct {
	TrigComparison status : 4;           		// 4 bits, enough to hold 10 choices
    TrigCompareTest test  : 4;					// 4 bits, enough to hold 9 choices
    uint16_t reserved 	  : 8;     			    // 8 bits (padding to make 16 bits total)
    uint16_t fromValue;                   		// 16 bits
    uint16_t toValue;                   		// 16 bits
    ActuatorsValues actuators;
    AlarmOrTrigOutput output;
    uint16_t pwm1;                  			// 16 bits, 0xffff = no change
    uint16_t pwm2;                  			// 16 bits, 0xffff = no change
    uint16_t tnc1Enabled  : 1;
	uint16_t tnc1Message  : 4;
	uint16_t tnc1Path     : 3;
	uint16_t tnc2Enabled  : 1;
	uint16_t tnc2Message  : 4;
	uint16_t tnc2Path     : 3;
} TriggerConfiguration;


void TRIGGERS_runAll(void);
void TRIGGERS_runTrig(uint8_t index);
bool TRIGGERS_isTriggered(TrigComparison comparison, TrigCompareTest test, uint32_t fromValue, uint32_t toValue);
bool TRIGGERS_compareValues(uint16_t ref, TrigCompareTest test, uint32_t fromValue, uint32_t toValue);


#endif /* VEXUF_VEXUF_TRIGGERS_H_ */
