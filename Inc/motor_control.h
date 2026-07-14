/*
 * motor_control.h
 *
 *  Created on: Jul 2, 2026
 *      Author: julius
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

typedef struct {
	int throttle_speed;
	int turn_speed;
} motor_command;


void init_motor_controls(void);

void set_motors(motor_command* motor_command);

void stop_motors(void);

#endif /* MOTOR_CONTROL_H_ */
