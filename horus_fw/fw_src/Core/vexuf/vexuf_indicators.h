/*
 * indicators.h
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_INDICATORS_H_
#define INC_VEXUF_INDICATORS_H_

#include "vexuf.h"

typedef enum {
    IndOFF = 0b00,
    IndSLOW = 0b01,
    IndFAST = 0b10,
    IndON = 0b11
} IndStatusOption;

typedef enum {
    IndError = 0,
    IndWarn = 1,
    IndInfo = 2,
    IndAv1 = 3,
	IndAv2 = 4,
	IndAv3 = 5,
	IndBuzzer = 6,
	IndSdio = 7
} Indicator;

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} IndicatorPin;

typedef struct {
	IndStatusOption indErrorStatus: 2;
	IndStatusOption indWarnStatus: 2;
	IndStatusOption indInfoStatus: 2;
	IndStatusOption indAv1Status: 2;
	IndStatusOption indAv2Status: 2;
	IndStatusOption indAv3Status: 2;
	IndStatusOption indBuzzerStatus: 2;
	IndStatusOption indSdioStatus: 2;
} IndStatus;

IndStatusOption IND_getCurrentStatus(Indicator ind);
void IND_setStatus(Indicator ind, IndStatusOption option);
void IND_toggleIndWithStatusOption(IndStatusOption option);


#endif /* INC_VEXUF_INDICATORS_H_ */
