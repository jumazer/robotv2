/*
 * systick_timer.c
 *
 *  Created on: May 24, 2026
 *      Author: julius
 */

#include <stdint.h>
#include <timers.h>

#include "stm32f411xe.h"
#include "utilities.h"

/*
 * stm32 nucleo's systick timer is 24 bits long, 2^24 = 16Mhz
 * By default, the HCLK is divided by 8, so 16MHz/8 = 2Mhz
 * 2 Mhz is much easier to work with
 * TICK_FREQ is the desired frequency 20hz = 1/20s = 50ms
 */

#define SYS_TICK_CLK 	2000000U
#define TICK_FREQ		20U
#define TIMER_LOAD		(SYS_TICK_CLK/TICK_FREQ) - 1
#define TIM5_CLOCK_HZ 	16000000U   // Replace with your actual TIM5 clock

static volatile ticktime_t global_timer = 0;

void init_systick(void) {
    // CLKSOURCE = 0 so SysTick clock is HCLK/8
    SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;

    // Set LOAD register
    MODIFY_FIELD(SysTick->LOAD, SysTick_LOAD_RELOAD, TIMER_LOAD);

    // Set interrupt priority
    NVIC_SetPriority(SysTick_IRQn, 3);

    // Reset current tick counter value to 0
    MODIFY_FIELD(SysTick->VAL, SysTick_VAL_CURRENT, 0);

    // Enable interrupt and counter
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void init_tim5(void) {

	// supply system clock to TIM5 peripheral
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	TIM5->CR1 = 0;

    // Divide the TIM5 clock down to 1 MHz.
    // 1 MHz means one counter increment every microsecond.
	TIM5->PSC = (TIM5_CLOCK_HZ / 1000000U) - 1U;

	// Allow the 32 bit ocunter to count through its full range
	TIM5->ARR = 0xFFFFFFFFU;

	// Load the new prescaler value
	TIM5->EGR = TIM_EGR_UG;

	// Clear the counter and status flags
	TIM5->CNT = 0;
	TIM5->SR = 0;

	// Start counting
	TIM5->CR1 |= TIM_CR1_CEN;
}

ticktime_t now(void) {
	return global_timer;
}

bool time_reached(ticktime_t now_t, ticktime_t deadline_t) {
	int32_t result = now_t - deadline_t;
	return result >= 0;
}

void delay_us(uint32_t delay) {
	uint32_t start = TIM5->CNT;
	while((uint32_t) (TIM5->CNT - start) < delay) {
		// wait
	}
}

void SysTick_Handler(void) {
	global_timer++;
}
