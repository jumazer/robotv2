/*
 * pwm.c
 *
 *  Created on: Jul 2, 2026
 *      Author: julius
 */

#include "pwm.h"
#include "stm32f411xe.h"
#include "utilities.h"

// prescaler = (F_clk / (f_pwm) * (ARR + 1))
// f_pwm = 10000
// F_clk = 16Mhz

#define PWM_ARR				99
#define PWM_PRESCALER		16


void init_tim3_pwm() {
	// Enable the clock to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /*
     * PA6 -> TIM3_CH1 -> PWMB
     */
    MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER6, ESF_GPIO_MODER_ALT_FUNC);
    MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL6, 2); // AF2 = TIM3_CH1

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    TIM3->PSC = PWM_PRESCALER - 1;
    TIM3->ARR = PWM_ARR;
    TIM3->CR1 = 0;

    TIM3->CCR1 = 0;
    MODIFY_FIELD(TIM3->CCMR1, TIM_CCMR1_OC1M, 6); // PWM mode 1
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM3->CCER  |= TIM_CCER_CC1E;

    TIM3->EGR |= TIM_EGR_UG;
    TIM3->CR1 |= TIM_CR1_CEN;

    /*
     * PB10 -> TIM2_CH3 -> PWMA
     */
    MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER10, ESF_GPIO_MODER_ALT_FUNC);
    MODIFY_FIELD(GPIOB->AFR[1], GPIO_AFRH_AFSEL10, 1); // AF1 = TIM2_CH3

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC = PWM_PRESCALER - 1;
    TIM2->ARR = PWM_ARR;
    TIM2->CR1 = 0;

    TIM2->CCR3 = 0;

    MODIFY_FIELD(TIM2->CCMR2, TIM_CCMR2_OC3M, 6); // PWM mode 1
    TIM2->CCMR2 |= TIM_CCMR2_OC3PE;

    TIM2->CCER |= TIM_CCER_CC3E;

    TIM2->EGR |= TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_CEN;
}
