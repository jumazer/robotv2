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
#include "robot_actions.h"
#include "utilities.h"

#define SIMPLE_CMD_LEN		1
#define JOYSTICK_CMD_LEN 	6
#define DEFAULT_SPEED		40
#define MIN_SPEED			0
#define MAX_SPEED			70

//static command_t command_table[] = {
//		{"w", set_motors_forward},
//		{"s", set_motors_backward},
//		{"q", set_motors_stop},
//		{"a", set_motors_left},
//		{"d", set_motors_right}
//};

//static uint16_t get_cmd_tbl_size(void) {
//	return sizeof(command_table) / sizeof(command_table[0]);
//}

static uint8_t clamp_speed(int speed) {
	if(speed > MAX_SPEED)
		return MAX_SPEED;

	if(speed < 0)
		return MIN_SPEED;

	return speed;
}


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

//int is_simple_command(char* command) {
//	uint16_t cmd_tbl_size = get_cmd_tbl_size();
//	for(uint16_t i = 0; i < cmd_tbl_size; i++) {
//		char* cmd_tmp = command_table[i].command;
//		uint8_t command_length = strlen(command);
//		uint8_t cmd_tmp_length = strlen(cmd_tmp);
//
//		if(command_length != cmd_tmp_length)
//			continue;
//
//
//		if(strncmp(cmd_tmp, command, command_length) == 0) {
//			return i;
//		}
//	}
//	return -1;
//}

void set_motors(command_state* command_state) {

	int throttle_speed = command_state->throttle_speed;

	if(throttle_speed > 0) { // set motors forward
		// default to have the motors 'move forward'
		// BIN1 = 1, BIN2 = 0
		GPIOB->BSRR = GPIO_BSRR_BS_6;
		GPIOA->BSRR = GPIO_BSRR_BR_7;

		// AIN1 = 1, AIN2 = 0
		GPIOA->BSRR = GPIO_BSRR_BS_9;
		GPIOA->BSRR = GPIO_BSRR_BR_8;
	} else if(throttle_speed < 0) { // set motors backward
		// BIN1 = 0, BIN2 = 1
		GPIOB->BSRR = GPIO_BSRR_BR_6;
		GPIOA->BSRR = GPIO_BSRR_BS_7;

		// AIN1 = 0, AIN2 = 1
		GPIOA->BSRR = GPIO_BSRR_BR_9;
		GPIOA->BSRR = GPIO_BSRR_BS_8;

		throttle_speed = ABS(throttle_speed);
	}

	// set throttle speed first
	TIM3->CCR1 = throttle_speed; // left motor
	TIM2->CCR3 = throttle_speed; // right motor

	// add or subtract in turning speed
	if(command_state->turn_speed > 0) { // robot is turning right
		TIM2->CCR3 = clamp_speed(TIM2->CCR3 - command_state->turn_speed); // subtract from right motor
		TIM3->CCR1 = clamp_speed(TIM3->CCR1 + command_state->turn_speed); // add to left motor
	} else if(command_state->turn_speed < 0) { // robot is turning left
		TIM2->CCR3 = clamp_speed(TIM2->CCR3 - command_state->turn_speed); // subtract from right motor
		TIM3->CCR1 = clamp_speed(TIM3->CCR1 + command_state->turn_speed); // add to left motor
	}

//	DBG_PRINTF("TIM3->CCR1: %lu \r\n", TIM3->CCR1);
//	DBG_PRINTF("TIM2->CCR3: %lu \r\n", TIM2->CCR3);
}

static void parse_joystick_command(command_state* command_state) {
	char* command = command_state->command;
	command_state->command_type = JOYSTICK;

	char fb_speed[3];
	fb_speed[0] = command[1];
	fb_speed[1] = command[2];
	fb_speed[2] = '\0';

	command_state->throttle_speed = strtol(fb_speed, NULL, 10);

	char lr_speed[3];
	lr_speed[0] = command[4];
	lr_speed[1] = command[5];
	lr_speed[2] = '\0';
	command_state->turn_speed = strtol(lr_speed, NULL, 10);
}

static void parse_simple_command(command_state* command_state) {
	char* command = command_state->command;
	if(strcmp(command, "w") == 0) {
		command_state->command_type = SIMPLE;
		command_state->throttle_speed = DEFAULT_SPEED;
		command_state->turn_speed = 0;
	} else if(strcmp(command, "s") == 0) {
		command_state->command_type = SIMPLE;
		command_state->throttle_speed = -DEFAULT_SPEED;
		command_state->turn_speed = 0;
	} else if(strcmp(command, "a") == 0) {
		command_state->command_type = SIMPLE;
		command_state->throttle_speed = 0;
		command_state->turn_speed = -DEFAULT_SPEED;
	} else if(strcmp(command, "d") == 0) {
		command_state->command_type = SIMPLE;
		command_state->throttle_speed = 0;
		command_state->turn_speed = DEFAULT_SPEED;
	} else if(strcmp(command, "q") == 0) {
		command_state->command_type = SIMPLE;
		command_state->throttle_speed = 0;
		command_state->turn_speed = 0;
	} else {
		command_state->command_type = BAD;
	}
}


void parse_command_details(command_state* command_state) {
	// int simple_cmd_index = is_simple_command(command);

	char* command = command_state->command;
	if(strlen(command) == SIMPLE_CMD_LEN) { // parse simple command
		parse_simple_command(command_state);
	} else if(strlen(command) == JOYSTICK_CMD_LEN &&
			(command[0] == 'f' || command[0] == 'b') &&
			(command[3] == 'r' || command[3] == 'l')) { // parse joystick command
		parse_joystick_command(command_state);
	} else {
		command_state->command_type = BAD;
	}
}
