#include "regs.h"
#include "led.h"

int puts(const char *str)
{

	// Uses PCLK as a default clock.
	led_4states(1);
	while (*str) {

		int bark = 0;
		while (!(*(USART2_ISR) & USART_ISR_TXE_MASK)) {
			// Oscillating LEDs for an infiite loop.
			led_blink(bark++&1?8:6, 12);
		}
		led_4states(2);		
		*(USART2_TDR) = *str++ & 0xFF;

	}
	led_4states(3);
	return 0;
}


int main (void) {
	
	led_init();
	// Enable Clocks
	*(RCC_APB1ENR1) |= RCC_USART2EN_MASK;
	// I hate writing at GPIOA.
	*(RCC_AHB2ENR) |= RCC_GPIOAEN_MASK;
	// *(RCC_AHB2ENR) |= RCC_GPIODEN_MASK;

	// Set up GPIOD alternate function
	*(GPIOA_AFRL) = GPIOA_AF_USART2_TX_MASK;
	*(GPIOA_AFRL) |= GPIOA_AF_USART2_RX_MASK;
	
	// Character transmission protocol,
	// based on RM0351 page 1340.
	// Turn on USART 

	// *(USART2_CR1) |= USART_CR1_UE_MASK;
	*(USART2_CR1) |= USART_CR1_RE_MASK;
	*(USART2_CR1) |= USART_CR1_TE_MASK;
	// *(USART2_CR1) = 0xC;
	*(USART2_CR1) |= USART_CR1_TXEIE_MASK;
	
	puts("Hello World!");
}
