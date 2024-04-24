/*
 * i2c_controller.h
 *
 * Created: 4/22/2024 3:59:39 PM
 *  Author: Kaspe
 */ 


#ifndef I2C_CONTROLLER_H_
#define I2C_CONTROLLER_H_

static void TWI0_M_init(void);
static void I2C0_M_start(uint8_t addr , uint8_t dir);
static void I2C_M_write(uint8_t addr, uint8_t data);
static void I2C_M_read( uint8_t addr , uint8_t * data , uint8_t len);




#endif /* I2C_CONTROLLER_H_ */