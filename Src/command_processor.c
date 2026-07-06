/*
 * cmd.c
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#include <stdio.h>
#include <string.h>

#include "command_processor.h"
#include "cbfifo.h"
#include "debug.h"
#include "robot_actions.h"

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

command_status_t run_command(char* command, uint8_t speed) {

	if(speed > 100) {
		return CMD_STATUS_BAD_ARGUMENT;
	}

	if(strlen(command) == 0) {
		return CMD_STATUS_EMPTY;
	}

	bool command_found = false;
	uint16_t cmd_tbl_size = sizeof(command_table) / sizeof(command_table[0]);
	for(uint16_t i = 0; i < cmd_tbl_size; i++) {
		char* cmd_tmp = command_table[i].command;
		uint8_t command_length = strlen(command);
		uint8_t cmd_tmp_length = strlen(cmd_tmp);

		if(command_length != cmd_tmp_length)
			continue;


		if(strncmp(cmd_tmp, command, command_length) == 0) {
			command_found = true;
			DBG_PRINTF("In IF STMT \r\n");
			command_func_t run_command = command_table[i].function;
			run_command(speed);
			break;
		}
	}

	if(command_found) {
		return CMD_STATUS_OK;
	} else {
		return CMD_STATUS_UNKNOWN;
	}
}
