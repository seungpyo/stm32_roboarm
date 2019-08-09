#ifndef _SPI_H
#define _SPI_H

#include "spi.h"
#include "utils.h"

extern __REG spi_cs;

static void spi_pfifo() {
	prints("SPI1_TXFIFO level = ");
	printx(spi_get_txfifo_lvl());
	prints("\r\n");
	prints("SPI1_RXFIFO level = ");
	printx(spi_get_rxfifo_lvl());
	prints("\r\n");

}

/**
  * In this project, we use full-duplex communication.
  */
void spi_init() {
	
	spi_err_t err;

	// Enable SPI1, GPIOA clock
	*(RCC_APB2ENR) |= RCC_SPI1EN_MASK;
	*(RCC_AHB2ENR) |= RCC_GPIOAEN_MASK;
	
	// Set up GPIOA alternate funcitons
	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_NSS_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(4, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(4, GPIO_MODE_AF);
	*(GPIOA_PUPDR) |= GPIO_PUPD_MASK(4, GPIO_PUPD_UP);

	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_SCK_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(5, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(5, GPIO_MODE_AF);

	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_MISO_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(6, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(6, GPIO_MODE_AF);

	*(GPIOA_AFRL) |= GPIOA_AF_SPI1_MOSI_MASK;
	*(GPIOA_MODER) &= ~GPIO_MODE_MASK(7, GPIO_MODE_RESET);
	*(GPIOA_MODER) |= GPIO_MODE_MASK(7, GPIO_MODE_AF);
		
	// Configure Baud rate
	*(SPI1_CR1) &= ~SPI_CR1_BR_MASK;
	*(SPI1_CR1) |= (SPI_CR1_BR_DIV4_MASK);


	// configure clock polarity and phase.
	// I don't know if these two lines are necessary.
	// *(SPI1_CR1) |= SPI_CR1_CPOL_MASK;
	// *(SPI1_CR1) |= SPI_CR1_CPHA_MASK;

	// Reset LSBFIRST bit
	*(SPI1_CR1) &= ~SPI_CR1_LSBFIRST_MASK;
	
#ifdef SSM
	// Enable software slave management.
	*(SPI1_CR1) |= SPI_CR1_SSM_MASK;
#endif /* SSM */

	// Set Master bit
	*(SPI1_CR1) |= SPI_CR1_MSTR_MASK;

	// Enable SSOE
	*(SPI1_CR2) |= SPI_CR2_SSOE_MASK;

	// Configure FRXTH to 8-bit.
	*(SPI1_CR2) &= SPI_CR2_FRXTH_MASK;

	// We MUST set NSS high before enabling SPI.
	// IDK why, but it works, at least.
	spi_deselect();

	// Enable SPI1 after configuration.
	err = spi_enable();
	if (err != SPI_ERR_OK) {
		spi_perror(err, "enabling spi");
	}
	
	prints("After spi_init():\r\n");
	spi_pregs();
}

spi_err_t spi_enable() {
	*(SPI1_CR1) |= SPI_CR1_SPE_MASK;
	return spi_error();
}

/**
  * Disables SPI, igonoring standard steps.
  */
void spi_disable_force() {
	*(SPI1_CR1) &= ~SPI_CR1_SPE_MASK;
}

/**
  * Disables SPI according to standard process.
  * TODO: Infinite loops on some polling codes.
  */
void spi_disable() {
	while(*(SPI1_SR) & SPI_SR_FTLVL) {
		prints("txfifo not empty\r\n");	
	}
	while(*(SPI1_SR) & SPI_SR_BSY_MASK) {
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
  * I used a quickfix; just reset SSI until MODF doesn't happen.
  */
spi_err_t spi_select() {
		
#ifdef SSM
	prints("Before spi_select():\r\n");
	spi_pregs();

	spi_err_t err;
	do {
		*(SPI1_CR1) &= ~(SPI_CR1_SSI_MASK);
	}  while ((err = spi_error()) == SPI_ERR_MODEFAULT);
	
	prints("After spi_select():\r\n");
	spi_pregs();

	return err;
#else
	return SPI_ERR_OK;
#endif /* SSM */
}	

/**
  * To terminate SPI, we pull up NSS pin
  */
spi_err_t spi_deselect() {
#ifdef SSM
	*(SPI1_CR1) |= (SPI_CR1_SSI_MASK);
	return spi_error();
#else
	return SPI_ERR_OK;
#endif /* SSM */
}

spi_err_t spi_send(uint8_t data) {

	prints("starting spi_send:\r\n");
	spi_pfifo();

	while(!(*(SPI1_SR) & SPI_SR_TXE_MASK));
	*(SPI1_DR) = data;
	
	prints("exiting spi_send:\r\n");
	spi_pfifo();

	return spi_error();
}

spi_err_t spi_recv(uint8_t *data) {

	prints("starting spi_recv:\r\n");
	spi_pfifo();

	uint8_t temp;
	while (!(*(SPI1_SR) & SPI_SR_RXNE_MASK));

	temp = (uint8_t)(*(SPI1_DR));
	if (data != NULL) {
		*data = temp;
	}
		
	prints("exiting spi_recv:\r\n");
	spi_pfifo();

	return spi_error();
}

uint16_t spi_get_rxfifo_lvl() {
	return ((*(SPI1_SR) & SPI_SR_FRLVL) >> 9);
}

uint16_t spi_get_txfifo_lvl() {
	return ((*(SPI1_SR) & SPI_SR_FTLVL) >> 11);
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
	spi_pregs();
	prints("\r\nSystem suspended due to SPI error.\r\n");
	while(1);
}

void spi_pregs() {
	prints("********************************\r\n");
	prints("SPI1_CR1: "); printb(*SPI1_CR1); prints("\r\n");
	prints("SPI1_CR2: "); printb(*SPI1_CR2); prints("\r\n");
	prints("SPI1_SR:  "); printb(*SPI1_SR); prints("\r\n");
	prints("SPI1_DR:  "); printb(*SPI1_DR); prints("\r\n");
	prints("********************************\r\n");
}
#endif /* _SPI_H */
