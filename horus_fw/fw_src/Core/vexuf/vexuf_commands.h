/*
 * vexuf_commands.h
 *
 *  Created on: Aug 21, 2024
 *      Author: Aly Badawy
 */

#ifndef VEXUF_VEXUF_COMMANDS_H_
#define VEXUF_VEXUF_COMMANDS_H_

#include "vexuf.h"
#include "vexuf_serial.h"

typedef struct {
    const char *command_name;
    void (*handler)(const char *args);
} Command;


void COMMANDS_handleCommand(SerialInterface interface);


#endif /* VEXUF_VEXUF_COMMANDS_H_ */
