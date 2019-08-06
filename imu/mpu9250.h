#ifndef _MPU9250_H
#define _MPU9250_H

#include "regs.h"

#define MPU9250_WRITE_MASK 0x00
#define MPU9250_READ_MASK 0x80

#define MPU9250_ACCEL_XOUT_H 0x3B
#define MPU9250_ACCEL_XOUT_L 0x3C

#define MPU9250_WHO_AM_I 0x75

#define MPU9250_PWR_MGMT1 0x6B
#define MPU9250_PWR_MGMT1_H_RESET 0x80

void mpu9250_init();
uint8_t mpu9250_write(uint8_t addr, uint8_t data);
uint8_t mpu9250_read(uint8_t addr);
uint8_t mpu9250_read_whoami();

#endif /* _MPU_9250_H */
