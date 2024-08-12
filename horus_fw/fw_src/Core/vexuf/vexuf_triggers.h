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

#define  TRIGS_COUNT				25


typedef enum {Av1, Av2, Av3, InternalTemp, ExternalTemp} TriggerSource;
typedef enum {lessThan, lessThanOrEqual, Equal, GreaterThanOrEqual, GreaterThan, NotEqual} CompareTest;


typedef struct {
    uint16_t enabled : 1;           			// 1 bit
    CompareTest test:3;							// 3 bits, enough to hold 6 choices
    uint16_t reserved : 12;     			    // 12 bits (padding to make 16 bits total)
    uint16_t min;                   			// 16 bits
    uint16_t max;                   			// 16 bits
    ActuatorsValues actuators;
    AlarmTrigOutputConfiguration output;
    uint16_t pwm1;                  			// 16 bits, 0xffff = no change
    uint16_t pwm2;                  			// 16 bits, 0xffff = no change
    uint16_t tnc1Enabled;
	uint16_t tnc1Message;
	uint16_t tnc1Path;
	uint16_t tnc2Enabled;
	uint16_t tnc2Message;
	uint16_t tnc2Path;
} TriggerConfiguration;


void TRIGGERS_run(void);

#endif /* VEXUF_VEXUF_TRIGGERS_H_ */
