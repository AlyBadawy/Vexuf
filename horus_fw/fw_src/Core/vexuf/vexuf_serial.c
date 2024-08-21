/*
 * vexuf_serial.c
 *
 *  Created on: Aug 21, 2024
 *      Author: Aly Badawy
 */

#include "vexuf_serial.h"
#include "vexuf_helpers.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

extern uint8_t ttlRxData[SERIAL_BUFFER_SIZE];
extern uint8_t tncRxData[SERIAL_BUFFER_SIZE];
extern uint16_t ttlRxIdx;
extern uint16_t tncRxIdx;

extern VexufStatus vexuf_status;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
	printf("received!\r\n"); // TODO: Remove before release

	if (huart == &huart1) {
	 	if (ttlRxData[size - 1] == '\r' || ttlRxData[size -1] == '\n' || size > 2) {
	 		vexuf_status.ttlBuffered = 1;
	 	}
	 	ttlRxIdx = size;
	 	HAL_UARTEx_ReceiveToIdle_IT(&huart1, ttlRxData, SERIAL_BUFFER_SIZE);
	} else if (huart == &huart6) {
		if (tncRxData[size - 1] == '\r' || tncRxData[size -1] == '\n' || size > 2) {
			vexuf_status.tncBuffered = 1;
		}
		tncRxIdx = size;
		HAL_UARTEx_ReceiveToIdle_IT(&huart6, tncRxData, SERIAL_BUFFER_SIZE);
	}
}
