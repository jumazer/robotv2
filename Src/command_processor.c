/*
 * cmd.c
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "command_processor.h"
#include "cbfifo.h"
#include "debug.h"
#include "robot_actions.h"

#define JOYSTICK_CMD_LEN 	6
#define DEFAULT_SPEED		40

static command_t command_table[] = {
		{"w", forward},
		{"s", backward},
		{"q", stop},
		{"a", left},
		{"d", right}
};

static uint16_t get_cmd_tbl_size(void) {
	return sizeof(command_table) / sizeof(command_table[0]);
}

bool build_command(char* command, uint8_t* command_index, cbfifo* bluetooth_cbfifo) {
	if(!cb_empty(bluetooth_cbfifo)) {
		char c = cb_dequeue(bluetooth_cbfifo);
		if(c == '\n' || c == '\r') {
			return true;
		} else {
			command[*command_index] = c;
			(*command_index)++;
			return false;
		}
	}
	return false;
}

int is_simple_command(char* command) {
	uint16_t cmd_tbl_size = get_cmd_tbl_size();
	for(uint16_t i = 0; i < cmd_tbl_size; i++) {
		char* cmd_tmp = command_table[i].command;
		uint8_t command_length = strlen(command);
		uint8_t cmd_tmp_length = strlen(cmd_tmp);

		if(command_length != cmd_tmp_length)
			continue;


		if(strncmp(cmd_tmp, command, command_length) == 0) {
			return i;
		}
	}
	return -1;
}

void run_simple_command(command_state* command_state) {
	command_func_t run_command = command_table[command_state->simple_cmd_index].function;
	run_command(DEFAULT_SPEED);
}

void run_joystick_command(command_state* command_state) {
	if(command_state->forward_backward_cmd == 'f') {
		forward(command_state->forward_backward_speed);
	} else if(command_state->forward_backward_cmd == 'b') {
		backward(command_state->forward_backward_speed);
	}
}

void parse_command(char* command, command_state* command_state) {
	int simple_cmd_index = is_simple_command(command);

	if(strlen(command) == JOYSTICK_CMD_LEN &&
			(command[0] == 'f' || command[0] == 'b') &&
			(command[3] == 'r' || command[3] == 'l')) {
		command_state->command_type = JOYSTICK;
		command_state->forward_backward_cmd = command[0];
		command_state->left_right_cmd = command[3];

		char fb_speed[3];
		fb_speed[0] = command[1];
		fb_speed[1] = command[2];
		fb_speed[2] = '\0';
		command_state->forward_backward_speed = strtol(fb_speed, NULL, 10);

		char lr_speed[3];
		lr_speed[0] = command[4];
		lr_speed[1] = command[5];
		lr_speed[2] = '\0';
		command_state->left_right_speed = strtol(lr_speed, NULL, 10);
	} else if(simple_cmd_index > -1) {
		command_state->command_type = SIMPLE;
		command_state->simple_cmd_index = (uint8_t) simple_cmd_index;
	} else {
		// bad command
		command_state->command_type = BAD;
	}
}
