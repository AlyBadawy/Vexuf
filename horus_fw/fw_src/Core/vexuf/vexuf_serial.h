/*
 * vexuf_serial.h
 *
 *  Created on: Aug 21, 2024
 *      Author: Aly Badawy
 */

#ifndef VEXUF_VEXUF_SERIAL_H_
#define VEXUF_VEXUF_SERIAL_H_

#include "vexuf.h"

#define SERIAL_BUFFER_SIZE		100

typedef enum {Baud300, Baud600, Baud1200, Baud4800, Baud9600, Baud19200, Baud57600, Baud115200} BaudRate;
typedef enum {TtlUart, TncUart, CDC} SerialInterface;

typedef struct {
    uint16_t ttl_enabled : 1;                	// 1 bit
    BaudRate ttl_baud : 4;                		// 4 bit
    uint16_t ttl_led_enabled : 1;               // 1 bit
    uint16_t tnc_enabled : 1;                	// 1 bit
    BaudRate tnc__baud : 4;                		// 4 bit
	uint16_t reserved: 5;

} SerialConfiguration;


#endif /* VEXUF_VEXUF_SERIAL_H_ */
