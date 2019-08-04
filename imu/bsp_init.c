#include "regs.h"
#include "usart.h"
#include "utils.h"
#include "led.h"
#include "debug.h"

/**
  * An entry point for the whole kernel.
  */

void bsp_init(void) {
	
	led_init();
	led_blink(0, 14);
	led_blink(1, 14);

	usart_init();
	
	printk = usart_puts;

	printk("bsp_init complete.\r\n");
	printk("Booting kernel...\r\n");

}
