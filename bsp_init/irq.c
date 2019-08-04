#include "usart.h"

void irq_hard_fault_handler() {
	usart_init();
	usart_puts("Hard fault caught, system suspending...\r\n");
	while(1);
}
