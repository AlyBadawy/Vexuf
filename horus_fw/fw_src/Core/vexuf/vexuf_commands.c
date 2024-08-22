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
#include "vexuf_i2c_aht20.h"

#include <ctype.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;


extern uint8_t ttlRxData[SERIAL_BUFFER_SIZE];
extern uint8_t tncRxData[SERIAL_BUFFER_SIZE];
extern uint16_t ttlRxIdx;
extern uint16_t tncRxIdx;

extern char callsign[CALLSIGN_LENGTH];
extern float cpuTempC;
extern OutputConfiguration outputConfig;

static char prompt[] = "\r\nVexUF:Horus > ";
static char ok[] = "\r\nOk!";
static char no[] = "\r\nNo!";
static char buzzerDisabled[] = "Buzzer is disabled. Can't configure buzzer.";
static char invalid[] = "Invalid Command!\r\nType 'help' or '?' for list of commands.";

static char serialTxBuffer[SERIAL_BUFFER_SIZE];

void handle_get_callsign(const char *args);
void handle_set_callsign(const char *args);
void handle_get_temperature(const char *args);
void handle_get_time(const char *args);
void handle_set_time(const char *args);
void handle_buzzer(const char *args);

Command commands[] = {
    {"get callsign", handle_get_callsign},
    {"set callsign ", handle_set_callsign},
    {"get temperature ", handle_get_temperature},
	{"get time", handle_get_time},
	{"set time ", handle_set_time},
	{"buzzer ", handle_buzzer}
    // ... add more commands as needed ...
};


void COMMANDS_handleCommand(SerialInterface interface) {
	char command[SERIAL_BUFFER_SIZE];

	memset(&serialTxBuffer[0], 0, sizeof(serialTxBuffer));

	switch (interface) {
	case TtlUart:
		while (ttlRxData[ttlRxIdx - 1] == '\r' || ttlRxData[ttlRxIdx - 1] == '\n') {
			ttlRxData[--ttlRxIdx] = '\0';  // Replace the trailing character with a null terminator
		}
		memcpy(&command, &ttlRxData, ttlRxIdx + 1);
		break;
	case TncUart:
		while (tncRxData[tncRxIdx - 1] == '\r' || tncRxData[tncRxIdx - 1] == '\n') {
			tncRxData[--tncRxIdx] = '\0';  // Replace the trailing character with a null terminator
		}
		memcpy(&command, &tncRxData, tncRxIdx + 1);
		break;
	default:
		break;
	}

	for(int i = 0; command[i]; i++){
	  command[i] = tolower(command[i]);
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



void handle_get_time(const char *args){
	char datetime[20];
	REALTIME_GetDateTime(datetime);
	sprintf(serialTxBuffer, "Date/Time: %s%s", datetime, ok);
}
void handle_set_time(const char *args) {
	REALTIME_SetDateTime(args);
	sprintf(serialTxBuffer, "Date and Time set...%s", ok);
}
void handle_get_temperature(const char *args) {
    if (strlen(args) == 0) {
    	// TODO: logic to show all temperatures
    }
    if ((strncmp(args, "internal", 8)) == 0) {
    	float temp;
    	float humid;
    	AHT20_ReadTemperatureHumidity(&temp, &humid);
    	sprintf(serialTxBuffer, "Temperature (Internal): %0.2fC (%0.2fF) - Humidity: %0.2f%%%s", temp, cToF(temp), humid, ok);
    }
    if ((strncmp(args, "external", 8)) == 0) {
        // TODO: implement external temperature
    }
    if ((strncmp(args, "cpu", 3)) == 0) {
    	ADC_getCpuTempC();
    	sprintf(serialTxBuffer, "Temperature (CPU): %0.2fC (%0.2fF)%s", cpuTempC, cToF(cpuTempC), ok);
	}
}
void handle_get_callsign(const char *args) {
	CONFIG_LoadCallSign();
	sprintf(serialTxBuffer, "Callsign: %s%s", callsign, ok);
}
void handle_set_callsign(const char *args) {
	CONFIG_SetCallSign(args);
	sprintf(serialTxBuffer, "%s", ok);
//	memcpy(serialTxBuffer, &ok, strlen(ok) + 1);
}

void handle_buzzer(const char *args) {
	// TODO: Refactor this function.
	OutputConfiguration newOutputConf;
	memcpy(&newOutputConf, &outputConfig, sizeof(outputConfig));
	if ((strncmp(args, "enable", 6)) == 0) {
		newOutputConf.buzzer_enabled = 1;
		CONFIG_SetOutputs(&newOutputConf);
		sprintf(serialTxBuffer, "Buzzer Enabled.%s", ok);
	}
	else if ((strncmp(args, "disable", 7)) == 0) {
		newOutputConf.buzzer_enabled = 0;
		CONFIG_SetOutputs(&newOutputConf);
		sprintf(serialTxBuffer, "Buzzer Disabled.%s", ok);
	}
	else if ((strncmp(args, "start beep on", 13)) == 0) {
		if(outputConfig.buzzer_enabled == 1) {
			newOutputConf.buzzer_1s_enabled = 1;
			CONFIG_SetOutputs(&newOutputConf);
			sprintf(serialTxBuffer, "Buzzer start beep Enabled.%s", ok);
		} else {
			sprintf(serialTxBuffer, "%s%s", buzzerDisabled, no);
		}
	}
	else if ((strncmp(args, "start beep off", 14)) == 0) {
		if(outputConfig.buzzer_enabled == 1) {
			newOutputConf.buzzer_1s_enabled = 0;
			CONFIG_SetOutputs(&newOutputConf);
			sprintf(serialTxBuffer, "Buzzer start beep Disabled.%s", ok);
		} else {
			sprintf(serialTxBuffer, "%s%s", buzzerDisabled, no);
		}
	}
	else if ((strncmp(args, "error on", 8)) == 0) {
		if(outputConfig.buzzer_enabled == 1) {
			newOutputConf.buzzer_hold_on_error = 1;
			CONFIG_SetOutputs(&newOutputConf);
			sprintf(serialTxBuffer, "Buzzer beep on error Enabled.%s", ok);
		} else {
			sprintf(serialTxBuffer, "%s%s", buzzerDisabled, no);
		}
	}
	else if ((strncmp(args, "error off", 9)) == 0) {
		if(outputConfig.buzzer_enabled == 1) {
			newOutputConf.buzzer_hold_on_error = 0;
			CONFIG_SetOutputs(&newOutputConf);
			sprintf(serialTxBuffer, "Buzzer beep on error Disabled.%s", ok);
		} else {
			sprintf(serialTxBuffer, "%s%s", buzzerDisabled, no);
		}
	}
	else if (strlen(args) == 0) {
		if(outputConfig.buzzer_enabled == 1) {
			sprintf(serialTxBuffer, "Buzzer is Enabled. Buzzer start beep is %s. Buzzer beep on error %s.%s", outputConfig.buzzer_1s_enabled == 1 ? "Enabled" : "Disabled", outputConfig.buzzer_hold_on_error == 1 ? "Enabled" : "Disabled", ok);
		} else {
			sprintf(serialTxBuffer, "Buzzer is disabled.%s", ok);
		}
	}
	else {
	}
}
