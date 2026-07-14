/*
 * motor_control.c
 *
 *  Created on: Jul 2, 2026
 *      Author: julius
 */

#include "motor_control.h"
#include "stm32f411xe.h"
#include "utilities.h"

#define MIN_SPEED			0
#define MAX_SPEED			60

void init_motor_controls(void) {
	GPIOC->BSRR = GPIO_BSRR_BS_7; // STBY 1
	// set motor speed to 0
	TIM3->CCR1 = 0;
	TIM2->CCR3 = 0;
}

static uint8_t clamp_speed(int speed) {
	if(speed > MAX_SPEED)
		return MAX_SPEED;

	if(speed < 0)
		return MIN_SPEED;

	return speed;
}

void set_motors(motor_command* motor_command) {
	// default to have the motors 'move forward'
	// BIN1 = 0, BIN2 = 1
	GPIOB->BSRR = GPIO_BSRR_BR_6;
	GPIOA->BSRR = GPIO_BSRR_BS_7;

	// AIN1 = 0, AIN2 = 1
	GPIOA->BSRR = GPIO_BSRR_BR_9;
	GPIOA->BSRR = GPIO_BSRR_BS_8;


	int throttle_speed = motor_command->throttle_speed;

	if(throttle_speed > 0) { // set motors forward
		// BIN1 = 0, BIN2 = 1
		GPIOB->BSRR = GPIO_BSRR_BR_6;
		GPIOA->BSRR = GPIO_BSRR_BS_7;

		// AIN1 = 0, AIN2 = 1
		GPIOA->BSRR = GPIO_BSRR_BR_9;
		GPIOA->BSRR = GPIO_BSRR_BS_8;
	} else if(throttle_speed < 0) { // set motors backward
		// BIN1 = 1, BIN2 = 0
		GPIOB->BSRR = GPIO_BSRR_BS_6;
		GPIOA->BSRR = GPIO_BSRR_BR_7;

		// AIN1 = 1, AIN2 = 0
		GPIOA->BSRR = GPIO_BSRR_BS_9;
		GPIOA->BSRR = GPIO_BSRR_BR_8;

		throttle_speed = ABS(throttle_speed);
	}

	// set throttle speed first
	TIM3->CCR1 = throttle_speed; // left motor
	TIM2->CCR3 = throttle_speed; // right motor

	// add or subtract in turning speed
	int turn_speed = motor_command->turn_speed;
	if(turn_speed > 0) { // robot is turning right
		TIM2->CCR3 = clamp_speed(TIM2->CCR3 - turn_speed); // subtract from right motor
		TIM3->CCR1 = clamp_speed(TIM3->CCR1 + turn_speed); // add to left motor
	} else if(turn_speed < 0) { // robot is turning left
		TIM2->CCR3 = clamp_speed(TIM2->CCR3 - turn_speed); // subtract from right motor
		TIM3->CCR1 = clamp_speed(TIM3->CCR1 + turn_speed); // add to left motor
	}
}

void stop_motors(void) {
	TIM3->CCR1 = 0; // left motor
	TIM2->CCR3 = 0; // right motor

	// BIN1 = 0, BIN2 = 0
	GPIOB->BSRR = GPIO_BSRR_BR_6;
	GPIOA->BSRR = GPIO_BSRR_BR_7;

	// AIN1 = 0, AIN2 = 0
	GPIOA->BSRR = GPIO_BSRR_BR_9;
	GPIOA->BSRR = GPIO_BSRR_BR_8;
}
