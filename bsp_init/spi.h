#include "regs.h"
#include "usart.h"

#define SPI_DUMMY_MSG 0xFFFF
void spi_init();

void spi_send(uint16_t data);
uint16_t spi_recv();

void spi_perror();
