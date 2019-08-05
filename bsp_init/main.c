#include "regs.h"
#include "led.h"
#include "usart.h"
#include "spi.h"
#include "utils.h"

void bsp_init() {
	led_init();
	usart_init();
	prints("USART2 initialized.\r\n");
	prints("Initializing SPI1...\r\n");
	spi_init();
}

int main (void) {
	
	bsp_init();

	// spi_send(SPI_DUMMY_MSG);
	uint16_t data = 0;
	while(1) {
		// spi_perror();
		// prints("\r\n");
		spi_send(SPI_DUMMY_MSG);
		data = spi_recv();
		prints("Recieved: ");
		printx(data);
		prints("\r\n");
		led_blink(8, 14);
	}
	char buf[128];
}
