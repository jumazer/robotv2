/*
 * robot_actions.h
 *
 *  Created on: Jul 5, 2026
 *      Author: julius
 */

#ifndef ROBOT_ACTIONS_H_
#define ROBOT_ACTIONS_H_

#include <stdint.h>
#include "command_processor.h"

// simple commands
void set_motors_forward(command_state* command_state);

void set_motors_backward(command_state* command_state);

void set_motors_left(command_state* command_state);

void set_motors_right(command_state* command_state);

void set_motors_stop(command_state* command_state);


#endif /* ROBOT_ACTIONS_H_ */
