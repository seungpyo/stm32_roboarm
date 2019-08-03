#include "regs.h"
#include "led.h"
#define USART_FREQ (4*1000000)
#define USART_BAUD (9600)

void * memset(void *b, int c, uint32_t len) {
	for(int i = 0; i < len; i++) {
		((char *)b)[i] = c;
	}
	return b;
}

/**
  * puts a character c through USART2.
  * Polls TXE bit in USART_ISR
  */
void usart_putc(const char c) {
	while (!(*(USART2_ISR) & USART_ISR_TXE_MASK));
	*(USART2_TDR) = (c & 0xFF);
}

char usart_getc(void) {
	char c = 0;
	while(!(*(USART2_ISR) & USART_ISR_RXNE_MASK));
	c = *(USART2_RDR) & 0xFF;
	return c;
}

/**
  * transmits a string through USART2.
  * returns a number of bytes printed.
  */
int usart_puts(const char *str) {
	int ret = 0;
	while (*str) {
		usart_putc(*str);
		str++;
		ret++;
	}
	return ret;
}

int usart_gets(char *buf) {
	int ret = 0;
	char c;
	while (c = usart_getc()) {
		if (c == '\n' || c == '\r' || c == ' ' || c == '\t') {
			break;
		}
		buf[ret++] = c;
	}
	return ret;
}

void usart_println(const char *str) {
	usart_puts(str);
	usart_putc('\r');
	usart_putc('\n');
}

void usart_init() {

	// Enable USART2, GPIOA clock
	*(RCC_APB1ENR1) |= RCC_USART2EN_MASK;
	*(RCC_AHB2ENR) |= RCC_GPIOAEN_MASK;

	// Set up GPIOA alternate functions (Tx, Rx)
	*(GPIOA_AFRL) = GPIOA_AF_USART2_TX_MASK;
	*(GPIOA_MODER) &= ~GPIO_PIN_MASK(2, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_PIN_MASK(2, GPIO_MODE_AF);

	*(GPIOA_AFRL) |= GPIOA_AF_USART2_RX_MASK;
	*(GPIOA_MODER) &= ~GPIO_PIN_MASK(3, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_PIN_MASK(3, GPIO_MODE_AF);

	// Turn off USART to set baud rate
	*(USART2_CR1) &= ~USART_CR1_UE_MASK;
	
	// The setting below yields error of 0.16%, 
	// which isn't that problematic.
	uint16_t usartdiv = (float)USART_FREQ / USART_BAUD;
	
	// Set USARTDIV as (freq = 4MHz / baud = 9600)
	*(USART2_BRR) = usartdiv;

	// Enable Rx, Tx in USART
	*(USART2_CR1) |= USART_CR1_RE_MASK;
	*(USART2_CR1) |= USART_CR1_TE_MASK;
	
	// For now, we don't use TXEIE
	// *(USART2_CR1) |= USART_CR1_TXEIE_MASK;

	// Now, turn on USART
	*(USART2_CR1) |= USART_CR1_UE_MASK;

}

int main (void) {
	
	led_init();
	usart_init();
	
	char buf[128];
	usart_println("Echo test");
	while (1) {
		usart_puts("> ");
		usart_gets(buf);
		usart_println(buf);
		memset(buf, 0, 128);
	} 	
}
