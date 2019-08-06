#include "mpu9250.h"
#include "spi.h"

void mpu9250_init() {
	spi_err_t err;

	mpu9250_write(MPU9250_PWR_MGMT1 | MPU9250_WRITE_MASK,
			MPU9250_PWR_MGMT1_H_RESET);

}

uint8_t mpu9250_write(uint8_t addr, uint8_t data) {
	spi_err_t err;
	uint8_t response = 0x0;
	spi_select();
	
	err = spi_send(addr | MPU9250_WRITE_MASK);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, sending address");
	}
	
	err = spi_recv(NULL);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, receiving dummy");
	}

	err = spi_send(data);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, sending data");
	}

	err = spi_recv(&response);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, receiving");
	}

	spi_deselect();
	return response;
} 

uint8_t mpu9250_read(uint8_t addr) {
	spi_err_t err;
	uint8_t response = 0x0;
	spi_select();
		
	err = spi_send(addr | MPU9250_READ_MASK);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read, sending address");
	}
	err = spi_recv(&response);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read, receiving");
	}

	spi_deselect();
	return response;
}

uint8_t mpu9250_read_whoami() {
	uint8_t data;
	spi_err_t err;
	err = spi_send(MPU9250_WHO_AM_I | MPU9250_READ_MASK);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, sending address");
	}
	err = spi_recv(NULL);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, receiving dummy");
	}
	err = spi_send(0);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, sending dummy");
	}
	err = (uint16_t)spi_recv(&data);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, receiving Device ID");
	}

	return data;
}

