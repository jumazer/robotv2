/*
 * fake_robot_actions.c
 *
 *  Created on: Jul 5, 2026
 *      Author: julius
 */

#include "fake_robot_actions.h"

bool fake_forward_called = false;
uint8_t fake_forward_speed = 0;

bool fake_backward_called = false;
uint8_t fake_backward_speed = 0;

bool fake_stop_called = false;
uint8_t fake_stop_speed = 0;

void forward(uint8_t speed) {
    fake_forward_called = true;
    fake_forward_speed = speed;
}

void backward(uint8_t speed) {
    fake_backward_called = true;
    fake_backward_speed = speed;
}

void stop(uint8_t speed) {
	fake_stop_called = true;
	fake_stop_speed = 0;
}

void fake_robot_actions_reset(void) {
    fake_forward_called = false;
    fake_forward_speed = 0;

    fake_backward_called = false;
    fake_backward_speed = 0;

	fake_stop_called = false;
	fake_stop_speed = 0;
}
