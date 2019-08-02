#ifndef _REGS_H
#define _REGS_H

#include <stdint.h>

// NOTE. Define base addresses as numerical numbers, not pointers!

#define __REG volatile uint32_t *

// GPIO base registers
#define GPIOA (0x48000000)
#define GPIOB (0x48000400)
#define GPIOC (0x48000800)
#define GPIOD (0x48000C00)
#define GPIOE (0x48001000)
#define GPIOH (0x48001C00)

// GPIO Pin masks
#define GPIO_PIN_MASK(n) (0x3 << (2*n))

// GPIO registers
#define GPIOC_MODER ((__REG)(GPIOC))
#define GPIOC_OTYPER ((__REG)(GPIOC+0x04))
#define GPIOC_BSRR ((__REG)(GPIOC+0x18))
#define GPIOC_BRR ((__REG)(GPIOC+0x28))

// GPIO alternate functions
#define GPIOD_AFRL ((__REG)(GPIOD+0x20))
#define GPIOD_AF_USART2_TX_MASK (0x00700000)

// RCC registers
#define RCC_APB1ENR1 ((__REG)0x40021058)
#define RCC_AHB2ENR ((__REG)0x4002104C)
// RCC masks
#define RCC_USART2EN_MASK (0x00020000)
#define RCC_GPIOAEN_MASK (0x00000001)
#define RCC_GPIOBEN_MASK (0x00000002)
#define RCC_GPIOCEN_MASK (0x00000004)
#define RCC_GPIODEN_MASK (0x00000008)

// USART registers
#define USART2 (0x40004400)
#define USART2_CR1 ((__REG)USART2)
#define USART2_ISR ((__REG)(USART2 + 0x1C))
#define USART2_TDR ((__REG)(USART2 + 0x28))
// USART masks
#define USART_CR1_UE_MASK 0x1
#define USART_CR1_RE_MASK 0x4
#define USART_CR1_TE_MASK 0x8
#define USART_CR1_TXEIE_MASK ((uint16_t) 0x2000)
#define USART_ISR_TXE_MASK	((uint16_t) 0x0080)

#endif /* _REGS_H */
