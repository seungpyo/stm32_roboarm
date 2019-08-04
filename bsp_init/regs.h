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
#define GPIO_MODE_MASK(pin, mode) \
	(((mode)&0x3) << (2*((pin)&0xF)))
// GPIO AF masks; both for CRL and CRH
#define GPIO_AF_MASK(pin, af) \
	(((af)&0xF) << (4*((pin)&0x7)))

// GPIO mode mask;
// this should be shifted to a given pin.
#define GPIO_MODE_IN (0x0)
#define GPIO_MODE_OUT (0x1)
#define GPIO_MODE_AF (0x2)
#define GPIO_MODE_ANALOG (0x3)
// reset state is analog.
#define GPIO_MODE_RESET (0x3)

// GPIO Port A registers
#define GPIOA_MODER ((__REG)(GPIOA))
#define GPIOA_AFRL ((__REG)(GPIOA+0x20))
#define GPIOA_AF_USART2_TX_MASK GPIO_AF_MASK(2, 7)
#define GPIOA_AF_USART2_RX_MASK GPIO_AF_MASK(3, 7)
#define GPIOA_AF_SPI1_NSS_MASK GPIO_AF_MASK(4, 5)
#define GPIOA_AF_SPI1_SCK_MASK GPIO_AF_MASK(5, 5)
#define GPIOA_AF_SPI1_MISO_MASK GPIO_AF_MASK(6, 5)
#define GPIOA_AF_SPI1_MOSI_MASK GPIO_AF_MASK(7, 5)

// GPIO Port C registers
#define GPIOC_MODER ((__REG)(GPIOC))
#define GPIOC_OTYPER ((__REG)(GPIOC+0x04))
#define GPIOC_BSRR ((__REG)(GPIOC+0x18))
#define GPIOC_BRR ((__REG)(GPIOC+0x28))

// GPIO Port D registers
#define GPIOD_AFRL ((__REG)(GPIOD+0x20))

// RCC registers
#define RCC_APB1ENR1 ((__REG)0x40021058)
#define RCC_APB2ENR ((__REG)0x40021060)
#define RCC_AHB2ENR ((__REG)0x4002104C)
#define RCC_CCIPR ((__REG)0x40021088)

// RCC masks
#define RCC_USART2EN_MASK (0x00020000)
#define RCC_GPIOAEN_MASK (0x00000001)
#define RCC_GPIOBEN_MASK (0x00000002)
#define RCC_GPIOCEN_MASK (0x00000004)
#define RCC_GPIODEN_MASK (0x00000008)
#define RCC_USART2SEL_MASK (0x0000000C)
#define RCC_SPI1EN_MASK (0x00001000)

// SPI registers
#define SPI1 (0x40013000)
#define SPI1_CR1 ((__REG)(SPI1))
#define SPI1_CR2 ((__REG)(SPI1 + 0x04))
#define SPI1_SR ((__REG)(SPI1 + 0x08))
#define SPI1_DR ((__REG)(SPI1 + 0x0C))

// SPI masks
#define SPI_CR1_RXONLY_MASK (0x00000400)
#define SPI_CR1_LSBFIRST_MASK (0x00000080)
#define SPI_CR1_SPE_MASK (0x00000040)
#define SPI_CR1_BR_MASK (0x00000038)
#define SPI_CR1_BR_DIV2_MASK (0x00000000)
#define SPI_CR1_BR_DIV4_MASK (0x00000008)
#define SPI_CR1_BR_DIV8_MASK (0x00000010)
#define SPI_CR1_BR_DIV16_MASK (0x00000018)
#define SPI_SR_TXE_MASK (0x00000002)
#define SPI_SR_RXNE_MASK (0x00000001)


// USART registers
#define USART2 (0x40004400)
#define USART2_CR1 ((__REG)USART2)
#define USART2_BRR ((__REG)(USART2 + 0x0C))
#define USART2_ISR ((__REG)(USART2 + 0x1C))
#define USART2_RDR ((__REG)(USART2 + 0x24))
#define USART2_TDR ((__REG)(USART2 + 0x28))
// USART masks
#define USART_CR1_UE_MASK 0x1
#define USART_CR1_RE_MASK 0x4
#define USART_CR1_TE_MASK 0x8
#define USART_CR1_TXEIE_MASK ((uint16_t) 0x2000)
#define USART_ISR_TXE_MASK	((uint16_t) 0x0080)
#define USART_ISR_RXNE_MASK ((uint16_t) 0x0020)

#endif /* _REGS_H */
