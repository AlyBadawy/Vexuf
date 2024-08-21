/*
 * vexuf_commands.c
 *
 *  Created on: Aug 21, 2024
 *      Author: Aly Badawy
 */


#include "vexuf_commands.h"
#include "vexuf_helpers.h"
#include "vexuf_serial.h"
#include "vexuf_config.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

extern uint8_t ttlRxData[SERIAL_BUFFER_SIZE];
extern uint8_t tncRxData[SERIAL_BUFFER_SIZE];
extern uint16_t ttlRxIdx;
extern uint16_t tncRxIdx;

extern char callsign[CALLSIGN_LENGTH];

static char prompt[] = "\r\n\r\nVexUF:Horus > \0";
static char ok[] = "\r\nOk!\0";

static char serialTxBuffer[SERIAL_BUFFER_SIZE];

void handle_get_callsign(const char *args);
void handle_set_callsign(const char *args);
void handle_get_temperature(const char *args);

Command commands[] = {
    {"get callsign", handle_get_callsign},
    {"set callsign ", handle_set_callsign},
    {"get temperature ", handle_get_temperature},
    // ... add more commands as needed ...
};


void COMMANDS_handleCommand(SerialInterface interface) {
	char command[SERIAL_BUFFER_SIZE];

	switch (interface) {
	case TtlUart:
		while (tncRxIdx > 0 && (ttlRxData[ttlRxIdx - 1] == '\r' || ttlRxData[ttlRxIdx - 1] == '\n')) {
			ttlRxData[--ttlRxIdx] = '\0';  // Replace the trailing character with a null terminator
		}
		memcpy(&command, &ttlRxData, ttlRxIdx);
		break;
	case TncUart:
		while (tncRxIdx > 0 && (tncRxData[tncRxIdx - 1] == '\r' || tncRxData[tncRxIdx - 1] == '\n')) {
			tncRxData[--tncRxIdx] = '\0';  // Replace the trailing character with a null terminator
		}
		memcpy(&command, &tncRxData, tncRxIdx);
		break;
	default:
		break;
	}


	for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		if (strncmp(command, commands[i].command_name, strlen(commands[i].command_name)) == 0) {
			const char *args = command + strlen(commands[i].command_name);
			commands[i].handler(args);
		}
	}
	// TODO: handle unknown commands

	switch (interface) {
	case TtlUart:
		if (strlen(serialTxBuffer) > 0) {
			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)serialTxBuffer, strlen(serialTxBuffer));
			HAL_Delay(100);
		}
		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)prompt, strlen(prompt));
		break;
	case TncUart:
		if (strlen(serialTxBuffer) > 0) {
			HAL_UART_Transmit_DMA(&huart6, (uint8_t *)serialTxBuffer, strlen(serialTxBuffer));
			HAL_Delay(100);
		}
		HAL_UART_Transmit_DMA(&huart6, (uint8_t *)prompt, strlen(prompt));
		break;
	default:
		break;
	}
	serialTxBuffer[0] = '\0';
}

void handle_get_callsign(const char *args) {
	CONFIG_LoadCallSign();
	sprintf(serialTxBuffer, "Callsign: %s", callsign);
}

void handle_set_callsign(const char *args) {
	CONFIG_SetCallSign(args);
	memcpy(serialTxBuffer, &ok, strlen(ok) + 1);
}

void handle_get_temperature(const char *args) {
    // Logic to get internal temperature
}
