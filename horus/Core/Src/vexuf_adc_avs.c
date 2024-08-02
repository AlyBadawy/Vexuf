/*
 * adc_avs.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_adc_avs.h"

extern ADC_HandleTypeDef hadc1;

float getVref(uint32_t adcValue) {
	float value = adcValue;
	return (VREFINT * ADC_RESOLUTION) / value;
}

float getCpuTempC(uint32_t vrefValue, uint32_t adcValue) {
	float vref = getVref(vrefValue);
	float temp_sense = (vref/ADC_RESOLUTION) * adcValue;
	return (((V25 - temp_sense) / CPU_TEMP_AVG_SLOPE) + 25.0);
}

float adcToAv(uint32_t vrefValue, uint32_t adcValue) {
	float vref = getVref(vrefValue);
	float voltage = ((adcValue * vref) / ADC_RESOLUTION) / adcRatio;
	if (voltage < 0.05) { voltage = 0.0; }
	return voltage;
}

// ADCs test
void testAvs(uint32_t avsBuffer[5]) {


	HAL_ADC_Start_DMA(&hadc1, avsBuffer, 5);
	HAL_Delay(50);
	HAL_ADC_Stop_DMA(&hadc1);

	uint32_t vrefValue = avsBuffer[0];
	float vrefValueFloat = avsBuffer[0];
	printf("\r\n");
	printf("Testing ADC functionality...\r\n");
	printf("  VREF: %f - %0.3fV\r\n", vrefValueFloat, getVref(vrefValue));
	float tempratureValue = avsBuffer[1];
	float cpuTempC = getCpuTempC(vrefValue, tempratureValue);
	printf("  Temperature CPU Raw: %0.2f\r\n", tempratureValue);
	printf("  Temperature CPU C: %0.2f\r\n", cpuTempC);
	printf("  Temperature CPU F: %0.2f\r\n", cToF(cpuTempC));
	float av1 = avsBuffer[2];
	float av2 = avsBuffer[3];
	float av3 = avsBuffer[4];
	printf("  Av1 Raw: %f\r\n", av1);
	printf("  Av1 Volt: %0.3fV\r\n", adcToAv(vrefValue, av1));
	printf("  Av2 Raw: %f\r\n", av2);
	printf("  Av2 Volt: %0.3fV\r\n", adcToAv(vrefValue, av2));
	printf("  Av3 Raw: %f\r\n", av3);
	printf("  Av3 Volt: %0.3fV\r\n", adcToAv(vrefValue, av3));


}
