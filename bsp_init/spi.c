#ifndef _SPI_H
#define _SPI_H

#include "spi.h"
#include "utils.h"

/**
  * In this project, we use full-duplex communication.
  */
void spi_init() {

	// Enable SPI1, GPIOA clock
	*(RCC_APB2ENR) |= RCC_SPI1EN_MASK;
	*(RCC_AHB2ENR) |= RCC_GPIOAEN_MASK;
	
	// Set up GPIOA alternate funcitons
	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_NSS_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(4, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(4, GPIO_MODE_AF);

	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_SCK_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(5, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(5, GPIO_MODE_AF);

	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_MISO_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(6, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(6, GPIO_MODE_AF);

	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_MOSI_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(7, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(7, GPIO_MODE_AF);

	prints("GPIOA_AFRL: ");
	printx(*(GPIOA_AFRL));
	prints("\r\n");
	printb(*(GPIOA_AFRL));
	prints("\r\n");
	prints("GPIOA_MODER: ");
	printx(*(GPIOA_MODER));
	prints("\r\n");
	printb(*(GPIOA_MODER));
	prints("\r\n");

	// Disable SPI1 for configuration. 
	*(SPI1_CR1) &= ~SPI_CR1_SPE_MASK;

	// Reset LSBFIRST bit
	*(SPI1_CR1) &= ~SPI_CR1_LSBFIRST_MASK;
	
	// Set master configuration.
	// It seems like GPIOA should be pulled up
	// to maintain master setting.
	*(GPIOA_PUPDR) |= GPIO_PUPD_MASK(4, GPIO_PUPD_UP);
	*(SPI1_CR1) |= SPI_CR1_MSTR_MASK;

	// Enable SPI1 after configuration.
	*(SPI1_CR1) |= SPI_CR1_SPE_MASK;

	prints("SPI1_CR1: ");
	printx(*(SPI1_CR1));
	prints("\r\n");
	printb(*(SPI1_CR1));
	prints("\r\n");

}


void spi_send(uint16_t data) {
	int bomb = 0;
	while(!(*(SPI1_SR) & SPI_SR_TXE_MASK)) {
		// prints("Polling on SPI_SR_TXE...\r\n");
		if (bomb++>10) {
			prints("Infinite loop detected while polling SPI TXE\r\n");
			while(1);
		}
	}
	*(SPI1_DR) = data;
}

uint16_t spi_recv() {
	int bomb = 0;
	while (!(*(SPI1_SR) & SPI_SR_RXNE_MASK)) {
		// prints("Polling on SPI_SR_RXNE...\r\n");
		if (bomb++>10) {
			prints("Infinite loop detected while polling SPI RXNE\r\n");
			while(1);
		}
	}
	return (uint16_t)*(SPI1_DR);
}

void spi_perror() {
	uint32_t busy = 
		*(SPI1_SR) & SPI_SR_BSY_MASK;
	uint32_t overrun =
		 *(SPI1_SR) & SPI_SR_OVR_MASK;
	uint32_t modefault =
		*(SPI1_SR) & SPI_SR_MODF_MASK;
	uint32_t crcerror = 
		*(SPI1_SR) & SPI_SR_CRCERR_MASK;

	if (busy) {
		prints("Note: SPI busy!\r\n");
	} else if (overrun) {
		prints("Error: SPI overrun!\r\n");
		while(1);
	} else if (modefault) {
		prints("Error: SPI mode fault!\r\n");
		while(1);
	} else if (crcerror) {
		prints("Error: SPI CRC error!\r\n");
		while(1);
	} else {
		prints("No SPI error detected.\r\n");
	}
}

#endif /* _SPI_H */
