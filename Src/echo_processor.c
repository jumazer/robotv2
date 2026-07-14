/*
 * echo_processor.c
 *
 *  Created on: Jul 14, 2026
 *      Author: julius
 */

#include "echo_processor.h"
#include "stm32f411xe.h"
#include "timers.h"

#define ECHO_TIMEOUT_US 38000U

void run_echo(echo_state* echo_state) {
	echo_state->echo_received = false;

	/*
	 * Send 10 microsecond trigger pulse
	 */
	GPIOB->BSRR = GPIO_BSRR_BS_9; // Set TRIG high
	delay_us(10); // delay 10us
	GPIOB->BSRR = GPIO_BSRR_BR_9; // Set TRIG low

	/*
	 * Wait for Echo to become High
	 */
	uint32_t wait_start = TIM5->CNT;
	while((GPIOB->IDR & GPIO_IDR_ID8) == 0U) {
		if((uint32_t)(TIM5->CNT - wait_start) >= ECHO_TIMEOUT_US) {
			return;
		}
	}

	/*
	 * Echo is now High record the beginning of the pulse
	 */
	uint32_t pulse_start = TIM5->CNT;

	/*
	 * Wait for Echo to return low
	 */
    while ((GPIOB->IDR & GPIO_IDR_ID8) != 0U) {
        if ((uint32_t)(TIM5->CNT - pulse_start) >= ECHO_TIMEOUT_US) {
        	return;
        }
    }

    // Echo is now low, pulse is complete
	uint32_t echo_duration_us = (uint32_t)(TIM5->CNT - pulse_start);

	// soeed of sound travels at 343 m/s or 0.0343 cm/us
	// distance = (echo_time * 0.0343)/2
	// disntace in cm = echo_duration/58
	echo_state->distance_cm = echo_duration_us / 58U;
	echo_state->echo_received = true;
}
