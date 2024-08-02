/*
 * adc_avs.h
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_ADC_AVS_H_
#define INC_VEXUF_ADC_AVS_H_

#include "main.h"

#define VREFINT      		1.22
#define ADC_RESOLUTION    	4096	// 12-bit ADC resolution


#define V25					0.76
#define CPU_TEMP_AVG_SLOPE  0.0025

#define adcR1               33000.0
#define adcR2               3300.0
#define adcRatio			(adcR2 / (adcR1 + adcR2))

float getVref(uint32_t adcValue);
float getCpuTempC(uint32_t vrefValue, uint32_t adcValue);
float adcToAv(uint32_t vrefValue, uint32_t adcValue);

void testAvs(uint32_t avsBuffer[5]);

#endif /* INC_VEXUF_ADC_AVS_H_ */
