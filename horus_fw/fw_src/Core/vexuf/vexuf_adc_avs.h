/*
 * adc_avs.h
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_ADC_AVS_H_
#define INC_VEXUF_ADC_AVS_H_

#include "vexuf.h"

#define VREFINT      		1.22
#define ADC_RESOLUTION    	4096	// 12-bit ADC resolution


#define V25					0.76
#define CPU_TEMP_AVG_SLOPE  0.0025

#define adcR1               33000.0
#define adcR2               3300.0
#define adcRatio			(adcR2 / (adcR1 + adcR2))

#define  NUMBER_OF_AVS		3

typedef struct {
     uint16_t enabled : 1;                		// 1 bit
     uint16_t indicatorEnabled : 1;       		// 1 bit
     uint16_t statusSlow : 1;             		// 1 bit
     uint16_t statusFast : 1;             		// 1 bit
     uint16_t statusOn : 1;               		// 1 bit
     uint16_t reserved : 11;              		// 10 bits (padding to make 16 bits total)
     uint16_t minSlow;                    		// 16 bits
     uint16_t maxSlow;                    		// 16 bits
     uint16_t minFast;                    		// 16 bits
     uint16_t maxFast;                    		// 16 bits
     uint16_t minOn;                      		// 16 bits
     uint16_t maxOn;                      		// 16 bits
} AvSensor;

void ADC_getVref(void);
void ADC_getCpuTempC(void);
float ADC_rawToVoltage(uint32_t adcValue);

void ADC_Scan();

#endif /* INC_VEXUF_ADC_AVS_H_ */
