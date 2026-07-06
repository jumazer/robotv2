/*
 * cmd.h
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

#define DEFAULT_SPEED 30

#include <stdbool.h>

#include "cbfifo.h"

typedef void (*command_func_t)(uint8_t speed);

typedef struct {
	char* command;
	command_func_t function;
} command_t;

typedef enum {
	CMD_STATUS_OK,
	CMD_STATUS_UNKNOWN,
	CMD_STATUS_EMPTY,
	CMD_STATUS_BAD_ARGUMENT
} command_status_t;

bool build_command(char* command, uint8_t* command_index, cbfifo* bluetooth_cbfifo);

command_status_t run_command(char* command, uint8_t speed);

#endif /* COMMAND_PROCESSOR_H_ */
