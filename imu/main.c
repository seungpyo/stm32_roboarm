#include "regs.h"
#include "led.h"
#include "usart.h"
#include "utils.h"
#include "debug.h"

void spi_init() {

	// Enable clocks.
	*(RCC_APB2ENR) |= RCC_SPI1EN_MASK;

	// Set AFs for GPIO pins.
	// Pins below serve the same function
	// written on the nucleo board.
	// PA4 --> SPI1_NSS
	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_NSS_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(4, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_AF_MASK(4, GPIO_MODE_AF);
	// PA5 --> SPI1_SCK
	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_SCK_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(5, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_AF_MASK(5, GPIO_MODE_AF);
	// PA6 --> SPI1_MISO
	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_MISO_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(6, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_AF_MASK(6, GPIO_MODE_AF);
	// PA7 --> SPI1_MOSI
	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_MOSI_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(7, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_AF_MASK(7, GPIO_MODE_AF);

	// Turn off the SPI for initial configuration
	*(SPI1_CR1) &= ~SPI_CR1_SPE_MASK;

	// Configure baud rate for SPI
	// Our IMU uses 1MHz as a default frequency.
	// Since PCLK is 4MHz, we divide it by 4.
	*(SPI1_CR1) &= ~SPI_CR1_BR_MASK;
	*(SPI1_CR1) |= SPI_CR1_BR_DIV4_MASK;

	// Set RXONLY bit, so that we don't need to
	// fill TXFIFO to start communication.
	*(SPI1_CR1) |= SPI_CR1_RXONLY_MASK;	

	// Our IMU uses MSB-first as default,
	// so set LSBFIRST as false.
	*(SPI1_CR1) &= ~SPI_CR1_LSBFIRST_MASK;

	// Turn on the SPI
	*(SPI1_CR1) |= SPI_CR1_SPE_MASK;

	printk("Configuration of SPI1 complete\r\n");
}	

uint16_t spi_recv(void) {
	// SPI data regsiter uses 16-bit field.
	uint16_t data;
	int bomb = 0;
	// Poll RXNE bit in SPIx_SR register
	while (!(*(SPI1_SR) & SPI_SR_RXNE_MASK)) {
		usart_println("Polling SPI_SR_RXNE...");
		if(bomb++==10) {
			
			usart_println("Infite loop detected; suspending");
			while(1);
		}
	}
	// and read data.
	data = *(SPI1_DR);
	// READ operation automatically accesses to RxFIFO.
	return data;
}

int main (void) {
	
	led_init();
	usart_init();
	printk = usart_puts;
	spi_init();

	uint16_t spi_data;
	usart_println("Start reading IMU");

	while (1) {
		spi_data = spi_recv();
		spi_data &= 0xFF;
		spi_data += '0';
		usart_puts("IMU sent: ");
		// For debugging only; number should oscillate.
		usart_puts((const char *)&spi_data);
		//usart_puts(itoa(spi_data));
		usart_println("");
	} 
}
