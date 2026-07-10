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

typedef void (*command_func_t)(uint8_t speed);

typedef struct {
	char* command;
	command_func_t function;
} command_t;

typedef enum {
	SIMPLE,
	JOYSTICK,
	BAD
} command_type;

typedef struct {
	command_type command_type;

	char forward_backward_cmd;
	char left_right_cmd;
	uint8_t forward_backward_speed;
	uint8_t left_right_speed;

	uint8_t simple_cmd_index;
} command_state;

bool build_command(char* command, uint8_t* command_index, cbfifo* bluetooth_cbfifo);

int is_simple_command(char* command);

void run_simple_command(command_state* command_state);

void run_joystick_command(command_state* command_state);

void parse_command(char* command, command_state* command_state);

#endif /* COMMAND_PROCESSOR_H_ */
