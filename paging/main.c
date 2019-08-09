#include "regs.h"
#include "led.h"
#include "usart.h"
#include "spi.h"
#include "utils.h"

void bsp_init() {
	led_init();
	usart_init();
	prints("USART2 initialized.\r\n");
}

int main (void) {
	
	bsp_init();

	

}
