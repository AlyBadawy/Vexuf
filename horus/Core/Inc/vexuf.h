/*
 * vexuf.h
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */

#ifndef INC_VEXUF_H_
#define INC_VEXUF_H_

#include "main.h"


float cToF(float c);
float fToC(float f);
void generate_serial_number(char *serial_number);
void base32_encode(const uint8_t *data, size_t length, char *output);

void SerialNumber_test(void);
void c(void);

#endif /* INC_VEXUF_H_ */
