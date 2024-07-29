/*
 * indicators.h
 *
 *  Created on: Jul 29, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_INDICATORS_H_
#define INC_INDICATORS_H_

#include "main.h"

#define OFF 0b00
#define SLOW 0b01
#define FAST 0b10
#define ON 0b11



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
void Indicators_setStatus(Indicator ind, uint8_t status);
void Indicators_clearAllStatuses(void);
void Indicators_toggleIndWithStatus(uint8_t status);
uint8_t Indicators_getStatus(Indicator ind);


#endif /* INC_INDICATORS_H_ */
