/*
 * fake_robot_actions.h
 *
 *  Created on: Jul 5, 2026
 *      Author: julius
 */

#ifndef FAKE_ROBOT_ACTIONS_H_
#define FAKE_ROBOT_ACTIONS_H_

#include <stdint.h>
#include <stdbool.h>

extern bool fake_forward_called;
extern uint8_t fake_forward_speed;

extern bool fake_backward_called;
extern uint8_t fake_backward_speed;

extern bool fake_stop_called;
extern uint8_t fake_stop_speed;

void fake_robot_actions_reset(void);

#endif /* FAKE_ROBOT_ACTIONS_H_ */
