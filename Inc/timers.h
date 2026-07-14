/*
 * systick_timer.h
 *
 *  Created on: May 24, 2026
 *      Author: julius
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <stdbool.h>

typedef uint32_t ticktime_t;

void init_systick(void);

void init_tim5(void);

ticktime_t now(void);

bool time_reached(ticktime_t now_t, ticktime_t deadline_t);

void delay_us(uint32_t us);

#endif /* TIMERS_H_ */
