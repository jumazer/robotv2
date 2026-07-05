/*
 * cmd.h
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

#define MOTOR_CONST 30

#include <stdbool.h>

#include "cbfifo.h"

typedef void (*command_func_t)(void);

typedef struct {
	char* command;
	command_func_t function;
} command_t;

bool build_command(char* command, uint8_t* command_index, cbfifo* bluetooth_cbfifo);

void forward(void);

void backward(void);

void stop(void);

void run_command(char* command);

#endif /* COMMAND_PROCESSOR_H_ */
