/*
 * motor_control.c
 *
 *  Created on: Jul 2, 2026
 *      Author: julius
 */

#include "motor_control.h"
#include "stm32f411xe.h"

void init_motor_controls() {
	GPIOC->BSRR = GPIO_BSRR_BS_7; // STBY 1
	// set motor speed to 0
	TIM3->CCR1 = 0;
	TIM2->CCR3 = 0;
}
