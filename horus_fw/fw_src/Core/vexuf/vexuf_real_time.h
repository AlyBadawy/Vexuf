/*
 * vexuf_real_time.h
 *
 *  Created on: Aug 12, 2024
 *      Author: Aly Badawy
 */

#ifndef VEXUF_VEXUF_REAL_TIME_H_
#define VEXUF_VEXUF_REAL_TIME_H_

#include "vexuf.h"
#include "vexuf_actuators.h"

typedef struct {
	uint16_t buzz: 1;							// 1 bit
	uint16_t info: 1;							// 1 bit
	uint16_t pwm1_change: 1;					// 1 bit
	uint16_t pwm2_change: 1;					// 1 bit
	uint16_t reserver: 12;						// 14 bit padding

} AlarmTrigOutputConfiguration;


typedef struct {
    ActuatorsValues actuators;
    AlarmTrigOutputConfiguration output;
    uint16_t pwm1;								// 16 bits, 0xffff = no change
    uint16_t pwm2;								// 16 bits, 0xffff = no change
    uint16_t tncEnabled;
    uint16_t tncMessage;
    uint16_t tncPath;
} AlarmConfiguration;

#endif /* VEXUF_VEXUF_REAL_TIME_H_ */
