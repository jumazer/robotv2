/*
 * cmd.h
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_


#include <stdbool.h>

#include "cbfifo.h"
#include "motor_control.h"


#define COMMAND_LENGTH		16

typedef enum {
	SIMPLE,
	JOYSTICK,
	BAD
} command_type;

typedef struct {
	command_type command_type;
	char command[COMMAND_LENGTH];
	uint8_t command_index;
} command_state;

typedef void (*command_func_t)(command_state* command_state);

typedef struct {
	char* command;
	command_func_t speed_function;
	command_func_t motor_function;
} command_t;


bool build_command(command_state* command_state, cbfifo* bluetooth_cbfifo);

void parse_command(command_state* command_state, motor_command* motor_command);

#endif /* COMMAND_PROCESSOR_H_ */
