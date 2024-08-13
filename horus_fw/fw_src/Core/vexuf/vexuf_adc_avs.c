/*
 * adc_avs.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_adc_avs.h"
#include "vexuf_helpers.h"
#include "vexuf_indicators.h"


extern AvSensor avSensors[NUMBER_OF_AVS];

/*
 * avsBuffer:
 * 0: VrefValue
 * 1: CPU raw value
 * 2: AV1 Raw Value
 * 3: AV2 Raw Value
 * 4: AV3 Raw Value
 */
uint32_t adcBuffer[5];
float vref, cpuTempC, temp_sense;

void ADC_getVref(void) {
	if (adcBuffer[0] == 0) {
		vref = 0;
		return;
	}
	vref = (VREFINT * ADC_RESOLUTION) / adcBuffer[0];
}

void ADC_getCpuTempC(void) {
	temp_sense = (vref/ADC_RESOLUTION) * adcBuffer[1];
	cpuTempC =  (((V25 - temp_sense) / CPU_TEMP_AVG_SLOPE) + 25.0);
}

float ADC_rawToVoltage(uint32_t adcValue) {
	float voltage = ((adcValue * vref) / ADC_RESOLUTION) / adcRatio;
	return voltage;
}

// ADCs test
void ADC_Scan() {
	ADC_getVref();
	ADC_getCpuTempC();

	// TODO: handle high CPU temp (and low)

	Indicator ind;

	for (uint8_t i=0; i < NUMBER_OF_AVS; i++) {
		switch (i) {
		case 0:
			ind = IndAv1;
			break;
		case 1:
			ind = IndAv2;
			break;
		case 2:
			ind = IndAv3;
			break;
		default:
			break;
		}
		if (avSensors[i].enabled && avSensors[i].indicatorEnabled) {
			if (avSensors[i].statusSlow && (adcBuffer[2 + i] >= avSensors[i].minSlow && adcBuffer[2 + i] <= avSensors[i].maxSlow)) {
				Indicators_setStatus(ind, IndSLOW);
			} else if (avSensors[i].statusFast && (adcBuffer[2 + i] >= avSensors[i].minFast && adcBuffer[2 + i] <= avSensors[i].maxFast)) {
				Indicators_setStatus(ind, IndFAST);
			} else if (avSensors[i].statusOn && (adcBuffer[2 + i] >= avSensors[i].minOn && adcBuffer[2 + i] <= avSensors[i].maxOn)) {
				Indicators_setStatus(ind, IndON);
			} else {
				Indicators_setStatus(ind, IndOFF);
			}
		} else {
			Indicators_setStatus(ind, IndOFF);
		}
	}
}

void ADC_Output(void) {
	printf("\r\n");
	printf("Testing ADC functionality...\r\n");
	printf("  VREF: %lu - %0.3fV\r\n", adcBuffer[0], vref);
	printf("  Temperature CPU Raw: %lu\r\n", adcBuffer[1]);
	printf("  Temperature CPU C: %0.2f\r\n", cpuTempC);
	printf("  Temperature CPU F: %0.2f\r\n", cToF(cpuTempC));
	printf("  Av1 Raw: %lu\r\n", adcBuffer[2]);
	printf("  Av1 Volt: %0.3fV\r\n", ADC_rawToVoltage(adcBuffer[2]));
	printf("  Av2 Raw: %lu\r\n", adcBuffer[3]);
	printf("  Av2 Volt: %0.3fV\r\n", ADC_rawToVoltage(adcBuffer[3]));
	printf("  Av3 Raw: %lu\r\n", adcBuffer[4]);
	printf("  Av3 Volt: %0.3fV\r\n", ADC_rawToVoltage(adcBuffer[4]));
}
