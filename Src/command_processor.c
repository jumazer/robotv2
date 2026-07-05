/*
 * cmd.c
 *
 *  Created on: Jul 4, 2026
 *      Author: julius
 */

#include <command_processor.h>
#include "cbfifo.h"
#include "debug.h"

bool build_command(char* command, uint8_t* command_index, cbfifo* bluetooth_cbfifo) {
	if(!cb_empty(bluetooth_cbfifo)) {
		char c = cb_dequeue(bluetooth_cbfifo);
		command[*command_index] = c;
		(*command_index)++;

		if(c == '\n')
			return true;
		else
			return false;
	}
	return false;
}
