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

bool build_command(char* command, uint8_t* command_index, cbfifo* bluetooth_cbfifo);

#endif /* COMMAND_PROCESSOR_H_ */
