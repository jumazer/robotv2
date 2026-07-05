/*
 * usart.h
 *
 *  Created on: May 24, 2026
 *      Author: julius
 */

#ifndef USART_H_
#define USART_H_

#include "cbfifo.h"

void init_usart2(void);

cbfifo* get_debug_cbfifo(void);

void init_usart6(void);

cbfifo* get_bluetooth_cbfifo(void);

#endif /* USART_H_ */
