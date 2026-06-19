/*
 * systick_timer.c
 *
 *  Created on: May 24, 2026
 *      Author: julius
 */

#include <stdint.h>

#include "stm32f411xe.h"
#include "systick_timer.h"
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

static volatile ticktime_t global_timer = 0;

void init_systick() {
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

ticktime_t now(void) {
	return global_timer;
}

bool time_reached(ticktime_t now_t, ticktime_t deadline_t) {
	int32_t result = now_t - deadline_t;
	return result >= 0;
}

void SysTick_Handler(void) {
	global_timer++;
}
