#include "mpu9250.h"
#include "spi.h"
#include "utils.h"

void mpu9250_init() {
	/*
	spi_err_t err;
	err = mpu9250_write(MPU9250_PWR_MGMT1 | MPU9250_WRITE_MASK,
			MPU9250_PWR_MGMT1_H_RESET, NULL);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "mpu9250_init");
	}
	*/
}

spi_err_t mpu9250_write(uint8_t addr, uint8_t data, uint8_t * response) {
	spi_err_t err;
	spi_select();
	
	err = spi_send(addr | MPU9250_WRITE_MASK);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, sending address");
		return err;
	}
	
	err = spi_recv(NULL);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, receiving dummy");
		return err;
	}

	err = spi_send(data);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, sending data");
		return err;
	}

	err = spi_recv(response);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_write, receiving");
		return err;
	}

	spi_deselect();

	return SPI_ERR_OK;
} 

spi_err_t mpu9250_read(uint8_t addr, uint8_t *response) {
	spi_err_t err;
	spi_select();
		
	err = spi_send(addr | MPU9250_READ_MASK);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read, sending address");
		return err;
	}
	err = spi_recv(response);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read, receiving");
		return err;
	}

	spi_deselect();
	return SPI_ERR_OK;
}

uint8_t mpu9250_read_whoami() {
	uint8_t data, dummy;
	spi_err_t err;

	err = spi_select();
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, selecting");
	}

	err = spi_send(MPU9250_WHO_AM_I | MPU9250_READ_MASK);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, sending address");
	}
	err = spi_recv(&dummy);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, receiving dummy");
	}

	printx(dummy);
	prints(" received as dummy\r\n");

	err = spi_send(0 | MPU9250_READ_MASK);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, sending dummy");
	}
	err = (uint16_t)spi_recv(&data);
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, receiving Device ID");
	}

	err = spi_deselect();	
	if (err != SPI_ERR_OK) {
		spi_perror(err, "In mpu9250_read_whoami, deselecting");
	}
	return data;
}

