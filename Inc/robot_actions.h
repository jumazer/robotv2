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
void forward(uint8_t speed);

void backward(uint8_t speed);

void left(uint8_t speed);

void right(uint8_t right);

void stop(uint8_t speed);

// joystick commands
void forward_joystick(command_state* command_state);

#endif /* ROBOT_ACTIONS_H_ */
