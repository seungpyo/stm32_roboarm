#include "debug.h"

char * wtoa(uint32_t word) {
	printk("wtoa NYI\r\n");
	return NULL;
}


void println(const char *str) {
	usart_puts(str);
	usart_putc('\r');
	usart_putc('\n');
}

