/*
 * robot_actions.c
 *
 *  Created on: Jul 5, 2026
 *      Author: julius
 */

#include "robot_actions.h"
#include "stm32f411xe.h"

void forward(uint8_t speed) {
	// BIN1 = 1, BIN2 = 0
	GPIOB->BSRR = GPIO_BSRR_BS_6;
	GPIOA->BSRR = GPIO_BSRR_BR_7;

	// AIN1 = 1, AIN2 = 0
	GPIOA->BSRR = GPIO_BSRR_BS_9;
	GPIOA->BSRR = GPIO_BSRR_BR_8;

	TIM3->CCR1 = speed;
	TIM2->CCR3 = speed;
}

void backward(uint8_t speed) {
	// BIN1 = 0, BIN2 = 1
	GPIOB->BSRR = GPIO_BSRR_BR_6;
	GPIOA->BSRR = GPIO_BSRR_BS_7;

	// AIN1 = 0, AIN2 = 1
	GPIOA->BSRR = GPIO_BSRR_BR_9;
	GPIOA->BSRR = GPIO_BSRR_BS_8;

	TIM3->CCR1 = speed;
	TIM2->CCR3 = speed;
}

void left(uint8_t speed) {
	TIM3->CCR1 = (speed / 2); // make left motor half the speed
	TIM2->CCR3 = speed;
}

void right(uint8_t speed) {
	TIM3->CCR1 = speed;
	TIM2->CCR3 = (speed / 2); // make right motor half the speed
}

void stop(uint8_t speed) {
	TIM3->CCR1 = 0;
	TIM2->CCR3 = 0;

	// BIN1 = 0, BIN2 = 0
	GPIOB->BSRR = GPIO_BSRR_BR_6;
	GPIOA->BSRR = GPIO_BSRR_BR_7;

	// AIN1 = 0, AIN2 = 0
	GPIOA->BSRR = GPIO_BSRR_BR_9;
	GPIOA->BSRR = GPIO_BSRR_BR_8;
}
