#include "regs.h"
#include "led.h"
#include "usart.h"
#include "spi.h"
#include "utils.h"

#include "mpu9250.h"

void bsp_init() {
	led_init();
	usart_init();
	prints("USART2 initialized.\r\n");
	prints("Initializing SPI1...\r\n");
	spi_init();
	mpu9250_init();
}


int main (void) {
	bsp_init();
    
	// spi_send(SPI_DUMMY_MSG);
	uint16_t data = 0;
	while(1) {
		data = mpu9250_read_whoami();
		prints("ID: ");
		printx(data);
		prints("\r\n");
		exp_delay(14);
	}
	char buf[128];
}
