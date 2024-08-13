/*
 * vexuf_output.h
 *
 *  Created on: Aug 13, 2024
 *      Author: Aly Badawy
 */

#ifndef VEXUF_VEXUF_OUTPUT_H_
#define VEXUF_VEXUF_OUTPUT_H_

#include "vexuf.h"

typedef struct {
	uint16_t buzzer_enabled: 1;					// 1 bit
	uint16_t buzzer_1s_enabled: 1;				// 1 bit
	uint16_t buzzer_hold_on_error: 1;			// 1 bit
	uint16_t global_indeicator_enabled: 1;		// 1 bit
	uint16_t status_indicator_enabled: 1;		// 1 bit
	uint16_t sd_card_indicator_enabled: 1;		// 1 bit
	uint16_t error_on_no_sd:1;					// 1 bit
	uint16_t log_info_to_sd:1;			 		// 1 bit
	uint16_t log_warn_to_sd:1;					// 1 bit
	uint16_t log_error_to_sd:1;					// 1 bit
	uint16_t reserved:6;						// 6 bit

} OutputConfiguration;


typedef struct {
	uint16_t buzz: 1;							// 1 bit
	uint16_t info: 1;							// 1 bit
	uint16_t log: 1;							// 1 bit
	uint16_t pwm1_change: 1;					// 1 bit
	uint16_t pwm2_change: 1;					// 1 bit
	uint16_t reserved: 11;						// 14 bit padding

} AlarmOrTrigOutput;

void OUTPUT_buzzOnError(void);
void OUTPUT_BuzzOnStartUp(void);


#endif /* VEXUF_VEXUF_OUTPUT_H_ */


