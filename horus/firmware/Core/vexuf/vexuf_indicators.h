/*
 * indicators.h
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_INDICATORS_H_
#define INC_VEXUF_INDICATORS_H_

#include "main.h"

typedef enum {
    IndOFF = 0b00,
    IndSLOW = 0b01,
    IndFAST = 0b10,
    IndON = 0b11
} IndicatorStatus;

typedef enum {
    IndError = 0,
	IndWarn = 1,
	IndInfo = 2,
	IndBuzzer = 3,
	IndSdio = 4,
	IndAv1 = 5,
	IndAv2 = 6,
	IndAv3 = 7
} Indicator;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} IndicatorPin;

void Indicators_init(void);
void Indicators_setStatus(Indicator ind, IndicatorStatus status);
void Indicators_clearAllStatuses(void);
void Indicators_toggleIndWithStatus(IndicatorStatus status);
IndicatorStatus Indicators_getStatus(Indicator ind);


#endif /* INC_VEXUF_INDICATORS_H_ */
