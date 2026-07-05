/*
 * cmd.c
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#include <stdio.h>
#include <string.h>

#include "stm32f411xe.h"
#include "command_processor.h"
#include "cbfifo.h"
#include "debug.h"

static command_t command_table[] = {
		{"w", forward},
		{"b", backward},
		{"q", stop}
};

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

void print_bytes(const char *s)
{
    for (size_t i = 0; s[i] != '\0'; i++) {
        DBG_PRINTF("[%u] 0x%02X\r\n", i, (unsigned char)s[i]);
    }

    DBG_PRINTF("[end] 0x00\r\n");
}

void forward(void) {
	// BIN1 = 1, BIN2 = 0
	GPIOB->BSRR = GPIO_BSRR_BS_6;
	GPIOA->BSRR = GPIO_BSRR_BR_7;

	// AIN1 = 1, AIN2 = 0
	GPIOA->BSRR = GPIO_BSRR_BS_9;
	GPIOA->BSRR = GPIO_BSRR_BR_8;

	TIM3->CCR1 = MOTOR_CONST;
	TIM2->CCR3 = MOTOR_CONST;
}

void backward(void) {
	// BIN1 = 0, BIN2 = 1
	GPIOB->BSRR = GPIO_BSRR_BR_6;
	GPIOA->BSRR = GPIO_BSRR_BS_7;

	// AIN1 = 0, AIN2 = 1
	GPIOA->BSRR = GPIO_BSRR_BR_9;
	GPIOA->BSRR = GPIO_BSRR_BS_8;

	TIM3->CCR1 = MOTOR_CONST;
	TIM2->CCR3 = MOTOR_CONST;
}

void stop(void) {
	TIM3->CCR1 = 0;
	TIM2->CCR3 = 0;

	// BIN1 = 0, BIN2 = 0
	GPIOB->BSRR = GPIO_BSRR_BR_6;
	GPIOA->BSRR = GPIO_BSRR_BR_7;

	// AIN1 = 0, AIN2 = 0
	GPIOA->BSRR = GPIO_BSRR_BR_9;
	GPIOA->BSRR = GPIO_BSRR_BR_8;
}

void run_command(char* command) {
	uint16_t cmd_tbl_size = sizeof(command_table) / sizeof(command_table[0]);
	for(uint16_t i = 0; i < cmd_tbl_size; i++) {
		char* cmd_tmp = command_table[i].command;
		uint8_t cmd_len = strlen(cmd_tmp);

		if(strncmp(cmd_tmp, command, cmd_len) == 0) {
			DBG_PRINTF("In IF STMT \r\n");
			command_func_t run_command = command_table[i].function;
			run_command();
			break;
		}
	}
}
