/*
 * I2C_temperature.h
 *
 * Created: 25.04.2024 09:25:04
 *  Author: jarle
 */ 


#ifndef I2C_TEMPERATURE_H_
#define I2C_TEMPERATURE_H_




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


# include <avr/io.h>
# include <util/twi.h >
# include <util/delay.h>
# include <stdio.h>
#include "Usart.h"



static void TWI0_M_init(void) 
{
	PORTA.DIRSET = PIN2_bm | PIN3_bm;
	PORTA.PINCONFIG = PORT_PULLUPEN_bm;

	
	PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm;
	//TWI0.CTRLA = TWI_SDAHOLD_50NS_gc ; // Setting hold time to 50 NS.
	//TWI0.CTRLA = TWI_INPUTLVL_I2C_gc;
	//TWI0.CTRLA = TWI_SDASETUP_4CYC_gc;
	TWI0.MCTRLA = TWI_ENABLE_bm; // Enable TWI /I2C as controller .
	TWI0.MBAUD = (uint8_t)TWI0_BAUD (I2C_SCL_FREQ ,0); // Configure TWI baud rate .
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc; // Setting the I2C bus to idle .
	
	PORTE.DIRSET = PIN1_bm;
	PORTE.OUT = PIN1_bm;
	
}
/**
* @brief This function initiates a data transfer with the device on the specified address .
* @param addr The address of the device on the bus.
* @param dir Specifies direction of transfer . 1 is read , 0 is write .
*/
static void I2C0_M_start(uint8_t addr , uint8_t dir) 
{
	/* Specify read or write of client */
	TWI0.MADDR = ( addr << 1) | dir;
	TWI_WAIT();
		

}
/**
* @brief This function writes one byte to the device on the specified address .
* @param addr The address of the device on the bus.
* @param data The byte to be written .
*/
static void I2C_M_write(uint8_t addr, uint8_t data) 
{
	
	I2C0_M_start( addr , DIR_WRITE);
	TWI0.MDATA = data;
	TWI_WAIT();
	/* Check for NACK */
	if(TWI0.MSTATUS & TWI_RXACK_bm) 
	{
		// target is full
		printf("target NACK \n");
	}
	/* Issue a stop condition */
	TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
}
/**
* @brief This function reads len bytes to the device on the specified address .
* @param addr The address of the device on the bus.
* @param data Pointer to data array .
* @param len The number of bytes to be read .
*/
static void I2C_M_read( uint8_t addr , uint8_t *data , uint8_t len) 
{
	I2C0_M_start( addr , DIR_READ );
	uint8_t byte_count = 0;
	while ( byte_count < len ) {
		TWI_WAIT();
		data[byte_count] = TWI0.MDATA;
		byte_count++;
		if(byte_count != len) 
		{
			TWI0.MCTRLB = TWI_ACKACT_ACK_gc
			| TWI_MCMD_RECVTRANS_gc;
		}
	}
	/* Finish transaction with a NACK and stop condition */
	TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
}




static void AHT10_init() 
{
	// Send initialization command
	I2C_M_write(AHT10_I2C_ADDR, AHT10_INIT_COMMAND);
	//printf("i2c write");
	_delay_ms(75); // Delay for initialization, according to datasheet

	// Send soft reset command
	I2C_M_write(AHT10_I2C_ADDR, AHT10_SOFT_RESET_COMMAND);
	_delay_ms(20); // Delay for soft reset, according to datasheet
	
}

uint8_t AHT10_read() 
{
    // Send trigger measurement command
    I2C_M_write(AHT10_I2C_ADDR, AHT10_TRIGGER_MEASUREMENT_COMMAND);
    _delay_ms(75); // Delay for measurement, according to datasheet

    // Read the data
    uint8_t data[6];
    I2C_M_read(AHT10_I2C_ADDR, data, 6);
	
	// Process the data if needed
	// For example, extract temperature and humidity
	uint32_t tdata = data[3] & 0x0F;
	tdata <<= 8;
	tdata |= data[4];
	tdata <<= 8;
	tdata |= data[5];
	//uint16_t temperature = (tdata * 200 / 0x100000) - 50;
	uint16_t temperature = (uint16_t)((float)tdata * 200 / 0x100000) - 50;
	return temperature;

	
	//printf("Temperature: �C%d\n", temperature);
}

#endif /* I2C_TEMPERATURE_H_ */