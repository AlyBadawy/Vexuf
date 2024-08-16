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
#include "vexuf_output.h"


typedef struct {
    ActuatorsValues actuators;
    AlarmOrTrigOutput output;
    uint16_t pwm1;								// 16 bits, 0xffff = no change
    uint16_t pwm2;								// 16 bits, 0xffff = no change
    uint16_t tncEnabled: 1;
    uint16_t tncMessage: 4;
    uint16_t tncPath: 3;
    uint16_t reserved: 8;

} AlarmConfiguration;

#endif /* VEXUF_VEXUF_REAL_TIME_H_ */
