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

	int throttle_speed;
	int turn_speed;
} command_state;

typedef void (*command_func_t)(command_state* command_state);

typedef struct {
	char* command;
	command_func_t speed_function;
	command_func_t motor_function;
} command_t;


bool build_command(command_state* command_state, cbfifo* bluetooth_cbfifo);

int is_simple_command(char* command);

void set_motors(command_state* command_state);

// void run_joystick_command(command_state* command_state);

void parse_command_details(command_state* command_state);

#endif /* COMMAND_PROCESSOR_H_ */
