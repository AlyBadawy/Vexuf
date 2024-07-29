/*
 * indicators.h
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_INDICATORS_H_
#define INC_INDICATORS_H_

#include "main.h"

typedef enum {
    OFF = 0b00,
    SLOW = 0b01,
    FAST = 0b10,
    ON = 0b11
} IndicatorStatus;



typedef enum {
    ErrorInd = 0,
    WarnInd = 1,
    InfoInd = 2,
    Buzzer = 3,
    Av1Ind = 4,
    Av2Ind = 5,
    Av3Ind = 6
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


#endif /* INC_INDICATORS_H_ */
