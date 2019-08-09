#include <stdlib.h>
#include "regs.h"
#include "usart.h"

// #define SSM

#define SPI_DUMMY_MSG 0xFFFF

// BSY flag is excluded in this mask
#define SPI_SR_ERR_MASK 0x0170
#define SPI_ERR_FRAME 0x10
#define SPI_ERRMSG_FRAME "SPI frame error"
#define SPI_ERR_BUSY 0x08
#define SPI_ERRMSG_BUSY "SPI is busy; maybe Tx buffer is not empty"
#define SPI_ERR_OVERRUN 0x04
#define SPI_ERRMSG_OVERRUN "SPI overrun occurred"
#define SPI_ERR_MODEFAULT 0x02
#define SPI_ERRMSG_MODEFAULT "SPI modefault"
#define SPI_ERR_CRC 0x01
#define SPI_ERRMSG_CRC "SPI CRC error"
#define SPI_ERR_OK 0x00
#define SPI_ERRMSG_OK "SPI is OK"

typedef uint8_t spi_err_t;

volatile uint32_t * spi_cs;

void spi_init();

spi_err_t spi_enable();
void spi_disable();
void spi_disable_force();

spi_err_t spi_select();
spi_err_t spi_deselect();

spi_err_t spi_send(uint8_t data);
spi_err_t spi_recv(uint8_t *data);

uint16_t spi_get_rxfifo_lvl();
uint16_t spi_get_txfifo_lvl();
spi_err_t spi_error();
void spi_perror(spi_err_t error, const char * msg);
void spi_pregs();
