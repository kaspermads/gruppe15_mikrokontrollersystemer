/*
 * Temperature_I2C_sensor.h
 *
 * Created: 4/24/2024 1:10:39 PM
 *  Author: Kaspe
 */ 


#ifndef TEMPERATURE_I2C_SENSOR_H_
#define TEMPERATURE_I2C_SENSOR_H_

static void TWI0_M_init(void);
static void I2C0_M_start(uint8_t addr , uint8_t dir);
static void I2C_M_write(uint8_t addr, uint8_t data);
static void I2C_M_read( uint8_t addr , uint8_t *data , uint8_t len);
static void AHT10_init();
uint8_t AHT10_read();









#endif /* TEMPERATURE_I2C_SENSOR_H_ */