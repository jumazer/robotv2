/*
 * echo_processor.h
 *
 *  Created on: Jul 14, 2026
 *      Author: julius
 */

#ifndef ECHO_PROCESSOR_H_
#define ECHO_PROCESSOR_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	bool echo_received;
	uint32_t distance_cm;

} echo_state;

void run_echo(echo_state* echo_state);

#endif /* ECHO_PROCESSOR_H_ */
