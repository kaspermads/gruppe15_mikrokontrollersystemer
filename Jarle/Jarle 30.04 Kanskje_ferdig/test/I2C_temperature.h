/*
 * I2C_temperature.h
 *
 * Created: 25.04.2024 09:25:04
 *  Author: jarle
 */ 


#ifndef I2C_TEMPERATURE_H_
#define I2C_TEMPERATURE_H_

#include <stdint.h>

// Formula for calculating baud rate . See chapter 29.3.2.2.1 , equation 2 in the datasheet .
# define TWI0_BAUD(F_SCL, T_RISE) ((((((float)F_CPU/(2*(float)F_SCL)))-5-((float)F_CPU*T_RISE)))/2)
// Defining 100 kHZ for the SCL frequency
# define I2C_SCL_FREQ 1000000
# define DIR_READ 1
# define DIR_WRITE 0
# define TWI_IS_CLOCKHELD() TWI0.MSTATUS & TWI_CLKHOLD_bm
# define TWI_IS_BUSERR() TWI0.MSTATUS & TWI_BUSERR_bm
# define TWI_IS_ARBLOST() TWI0.MSTATUS & TWI_ARBLOST_bm
# define CLIENT_NACK() TWI0.MSTATUS & TWI_RXACK_bm
# define CLIENT_ACK() !(TWI0.MSTATUS & TWI_RXACK_bm )
# define TWI_IS_BUSBUSY() ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc)==TWI_BUSSTATE_BUSY_gc)
// Macro for for ensuring proper timing between operations ( essentially waiting for the bus to be OK).
#define TWI_WAIT() while(!((TWI_IS_CLOCKHELD())||(TWI_IS_BUSERR())||(TWI_IS_ARBLOST())||(TWI_IS_BUSBUSY())))

#define AHT10_I2C_ADDR 0x38
#define AHT10_INIT_COMMAND 0xE1
#define AHT10_SOFT_RESET_COMMAND 0xBA
#define AHT10_TRIGGER_MEASUREMENT_COMMAND 0xAC


uint8_t AHT10_read();


#endif /* I2C_TEMPERATURE_H_ */