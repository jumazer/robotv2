/*
 * usart.c
 *
 *  Created on: Jun 14, 2026
 *      Author: julius
 */


#include "stm32f411xe.h"
#include "utilities.h"
#include "cbfifo.h"


#define F_USART_CLK			(16UL * 1000UL * 1000UL)
#define DEBUG_BAUD_RATE		115200UL


static cbfifo debug_rx; // cbfifo using for printingama

/*
 * This function is used to initialize usart2 which is used for printing
 */
void init_usart2(void) {
	 cb_init(&debug_rx);

	 // Enable USART2 to be used by the system clock
	 RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	 // Enable GPIOA to be used by the system clock
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	 // Put PA2 and PA3 in alternate mode
	 MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER2, ESF_GPIO_MODER_ALT_FUNC);
	 MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER3, ESF_GPIO_MODER_ALT_FUNC);
	 // Set PA2 and PA3 to be USART2 pins (AF7 on F411)
	 MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL2, 7); // PA2 = USART2_TX
	 MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL3, 7); // PA3 = USART2_RX

	 // Set baud rate
	 USART2->BRR = F_USART_CLK / DEBUG_BAUD_RATE;

	 // Turn off parity
	 MODIFY_FIELD(USART2->CR1, USART_CR1_PCE, 0);

	 // Set the word length: 8 data bits
	 MODIFY_FIELD(USART2->CR1, USART_CR1_M, 0);

	 // 1 stop bit
	 MODIFY_FIELD(USART2->CR2, USART_CR2_STOP, 0);

	 // Enable interrupt generation
	 MODIFY_FIELD(USART2->CR1, USART_CR1_RXNEIE, 1);

	 // Enable receiver and USART
	 MODIFY_FIELD(USART2->CR1, USART_CR1_TE, 1);
	 MODIFY_FIELD(USART2->CR1, USART_CR1_RE, 1);
	 MODIFY_FIELD(USART2->CR1, USART_CR1_UE, 1);

	 // Enable USART2 interrupts in NVIC
	 NVIC_SetPriority(USART2_IRQn, 2);
	 NVIC_ClearPendingIRQ(USART2_IRQn);
	 NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void) {
	unsigned char c;
	if(USART2->SR & USART_SR_RXNE) { // Receive buffer not empty
		// Reading from DR will clean RXNE
		c = USART2->DR;
		if(!cb_full(&debug_rx)) {
			cb_enqueue(&debug_rx, c);
		}
	}
}

static inline void UART_Transmit(char c) {
	while(!(USART2->SR & USART_SR_TXE)) { }
	USART2->DR = (uint8_t) c;
}

int __io_getchar(void) {
	while(!cb_empty(&debug_rx)) { }
	return (unsigned char) cb_dequeue(&debug_rx);
}

int __io_putchar(int c) {
	if(c == '\n')
		UART_Transmit('\r');

	UART_Transmit((char) c);
	return c;
}


