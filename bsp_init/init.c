#include "regs.h"
#include "led.h"
#include "usart.h"
#include "utils.h"

void bsp_init() {
	led_init();
	usart_init();
}

int main (void) {
	
	char buf[128];
	usart_println("Echo test");
	while (1) {
		usart_puts("> ");
		usart_gets(buf);
		usart_println(buf);
		memset(buf, 0, 128);
	} 	
}
