/*
 * cmd.c
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stm32f411xe.h"
#include "command_processor.h"
#include "cbfifo.h"
#include "debug.h"
#include "utilities.h"

#define SIMPLE_CMD_LEN		1
#define JOYSTICK_CMD_LEN 	6
#define DEFAULT_SPEED		40


bool build_command(command_state* command_state, cbfifo* bluetooth_cbfifo) {
	if(!cb_empty(bluetooth_cbfifo)) {
		char c = cb_dequeue(bluetooth_cbfifo);

		if(c == '\n') {
			command_state->command[command_state->command_index] = '\0';
			return true;
		}

		if(command_state->command_index >= COMMAND_LENGTH - 1) {
		    return false;
		}

		command_state->command[command_state->command_index] = c;
		command_state->command_index++;
		return false;

	}
	return false;
}

static void parse_joystick_command(command_state* command_state, motor_command* motor_command) {
	char* command = command_state->command;
	char cmd_throttle_direction = command[0];
	char cmd_turn_direction = command[3];
	command_state->command_type = JOYSTICK;

	char fb_speed[3];
	fb_speed[0] = command[1];
	fb_speed[1] = command[2];
	fb_speed[2] = '\0';


	if(cmd_throttle_direction == 'f') {
		motor_command->throttle_speed = strtol(fb_speed, NULL, 10);
	} else if(cmd_throttle_direction == 'b') {
		motor_command->throttle_speed = -1 * strtol(fb_speed, NULL, 10);
	}

	char lr_speed[3];
	lr_speed[0] = command[4];
	lr_speed[1] = command[5];
	lr_speed[2] = '\0';

	if(cmd_turn_direction == 'r') {
		motor_command->turn_speed = strtol(lr_speed, NULL, 10);
	} else if(cmd_turn_direction == 'l') {
		motor_command->turn_speed = -1 * strtol(lr_speed, NULL, 10);
	}
}

static void parse_simple_command(command_state* command_state, motor_command* motor_command) {
	char* command = command_state->command;
	if(strcmp(command, "w") == 0) {
		command_state->command_type = SIMPLE;
		motor_command->throttle_speed = DEFAULT_SPEED;
		motor_command->turn_speed = 0;
	} else if(strcmp(command, "s") == 0) {
		command_state->command_type = SIMPLE;
		motor_command->throttle_speed = -DEFAULT_SPEED;
		motor_command->turn_speed = 0;
	} else if(strcmp(command, "a") == 0) {
		command_state->command_type = SIMPLE;
		motor_command->throttle_speed = 0;
		motor_command->turn_speed = -DEFAULT_SPEED;
	} else if(strcmp(command, "d") == 0) {
		command_state->command_type = SIMPLE;
		motor_command->throttle_speed = 0;
		motor_command->turn_speed = DEFAULT_SPEED;
	} else if(strcmp(command, "q") == 0) {
		command_state->command_type = SIMPLE;
		motor_command->throttle_speed = 0;
		motor_command->turn_speed = 0;
	} else {
		command_state->command_type = BAD;
	}
}


void parse_command(command_state* command_state, motor_command* motor_command) {
	char* command = command_state->command;
	if(strlen(command) == SIMPLE_CMD_LEN) { // parse simple command
		parse_simple_command(command_state, motor_command);
	} else if(strlen(command) == JOYSTICK_CMD_LEN &&
			(command[0] == 'f' || command[0] == 'b') &&
			(command[3] == 'r' || command[3] == 'l')) { // parse joystick command
		parse_joystick_command(command_state, motor_command);
	} else {
		command_state->command_type = BAD;
	}
}
