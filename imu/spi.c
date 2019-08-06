#ifndef _SPI_H
#define _SPI_H

#include "spi.h"
#include "utils.h"

extern __REG spi_cs;

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
		
	// Set master configuration.
	// It seems like GPIOA should be pulled up
	// to maintain master setting.
	*(GPIOA_PUPDR) |= GPIO_PUPD_MASK(4, GPIO_PUPD_UP);

	prints("SPI1_CR1 (GPIO PA4 Pulled up): ");
	printb(*(SPI1_CR1));
	prints("\r\n");


	*(SPI1_CR1) |= SPI_CR1_MSTR_MASK;

	prints("SPI1_CR1 (MSTR set): ");
	printb(*(SPI1_CR1));
	prints("\r\n");

	// Enable software slave management.
	/*
	*(SPI1_CR1) |= SPI_CR1_SSM_MASK;
	prints("SPI1_CR1 (SSM set): ");
	printb(*(SPI1_CR1));
	prints("\r\n");
	*/

	// Reset LSBFIRST bit
	*(SPI1_CR1) &= ~SPI_CR1_LSBFIRST_MASK;
	
	// Enable SPI1 after configuration.
	*(SPI1_CR1) |= SPI_CR1_SPE_MASK;
	
	prints("SPI1_CR1 (SPE set): ");
	printb(*(SPI1_CR1));
	prints("\r\n");

}

void spi_enable() {
	*(SPI1_CR1) |= SPI_CR1_SPE_MASK;
}

/**
  * Disables SPI according to standard process.
  * TODO: Infinite loops on some polling codes.
  */
void spi_disable() {
	while(*(SPI1_SR) & SPI_SR_FTLVL) {
		prints("txfifo not empty\r\n");	
	}
	while(*(SPI1_SR) * SPI_SR_BSY_MASK) {
		prints("spi busy\r\n");
	}
	*(SPI1_CR1) &= ~SPI_CR1_SPE_MASK;
	uint16_t blackhole;
	while (*(SPI1_SR) & SPI_SR_FRLVL) {
		prints("rxfifo not empty\r\n");
		blackhole = *(SPI1_DR);
	}
}

/**
  * To initiate SPI, we pull down NSS pin
  */
void spi_select() {
	if (*(SPI1_CR1) & SPI_CR1_SSM_MASK) {
		*(SPI1_CR1) &= ~(SPI_CR1_SSI_MASK);
	}
}	

/**
  * To terminate SPI, we pull up NSS pin
  */
void spi_deselect() {
	if (*(SPI1_CR1) & SPI_CR1_SSM_MASK) {
		*(SPI1_CR1) |= (SPI_CR1_SSI_MASK);
	}
}

spi_err_t spi_send(uint8_t data) {
	int bomb = 0;
	while(!(*(SPI1_SR) & SPI_SR_TXE_MASK));
	*(SPI1_DR) = data;
	return spi_error();
}

spi_err_t spi_recv(uint8_t *data) {
	uint8_t temp;
	int bomb = 0;
	while (!(*(SPI1_SR) & SPI_SR_RXNE_MASK));
	temp = (uint8_t)(*(SPI1_DR));
	if (data != NULL) {
		*data = temp;
	}
	
	/*
	uint16_t rxfifo_lvl = 0;
	if (spi_error() == SPI_ERR_OVERRUN) {
		rxfifo_lvl = spi_get_rxfifo_lvl(); 
		prints("RXFIFO overrun detected; level is\r\n");
		printb(rxfifo_lvl);
		prints("\r\n");
		// temp = (uint8_t)(*(SPI1_SR));
	}
	*/
	return spi_error();
}

uint16_t spi_get_rxfifo_lvl() {
	return ((*(SPI1_SR) & SPI_SR_FRLVL) >> 9);
}

spi_err_t spi_error() {
	uint32_t raw = *(SPI1_SR) & SPI_SR_ERR_MASK;
	uint8_t error = (uint8_t)((raw & SPI_SR_ERR_MASK) >> 4);
	return error;
}

void spi_perror(spi_err_t error, const char *msg) {

	prints(msg);
	prints("\r\n");
	prints("SPI Error: ");

	switch (error) {
		case SPI_ERR_FRAME:
			prints(SPI_ERRMSG_FRAME);
			break;
		case SPI_ERR_OVERRUN:
			prints(SPI_ERRMSG_OVERRUN);
			break;
		case SPI_ERR_MODEFAULT:
			prints(SPI_ERRMSG_MODEFAULT);
			break;
		case SPI_ERR_CRC:
			prints(SPI_ERRMSG_CRC);
		default:
			return;
	}
	prints("\r\nSystem suspended due to SPI error.\r\n");
	while(1);
}

#endif /* _SPI_H */
