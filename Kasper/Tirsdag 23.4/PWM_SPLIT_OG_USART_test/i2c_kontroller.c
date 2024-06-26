/*
 * i2c_slave.c
 *
 * Created: 4/22/2024 3:12:29 PM
 *  Author: Kaspe
 */ 
/*
* ielet2111_twi_demo .c
*
* Created : 28.02.2023 14:32:24
* Author : crfosse
* Sourced from :
** https :// github .com / microchip -pic -avr - examples / avr128db48 -bare -metal -twi - mplab / blob / master /twi - host .X
/ peripherals / TWI / TWI_host .c
** https :// github .com / MicrochipTech / avr128da48 -i2c -send - receive - example
*/
# define F_CPU 4000000UL
// Formula for calculating baud rate . See chapter 29.3.2.2.1 , equation 2 in the datasheet .
# define TWI0_BAUD (F_SCL,T_RISE) ((((((float)F_CPU/(2*(float)F_SCL)))-5-((float)F_CPU*T_RISE)))/2)
// Defining 100 kHZ for the SCL frequency
# define I2C_SCL_FREQ 100000
# define DIR_READ 1
# define DIR_WRITE 0
# define TWI_IS_CLOCKHELD() TWI0.MSTATUS & TWI_CLKHOLD_bm
# define TWI_IS_BUSERR() TWI0.MSTATUS & TWI_BUSERR_bm
# define TWI_IS_ARBLOST() TWI0.MSTATUS & TWI_ARBLOST_bm
# define CLIENT_NACK() TWI0.MSTATUS & TWI_RXACK_bm
# define CLIENT_ACK() !(TWI0.MSTATUS & TWI_RXACK_bm )
# define TWI_IS_BUSBUSY() ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc)==TWI_BUSSTATE_BUSY_gc)
// Macro for for ensuring proper timing between operations ( essentially waiting for the bus to be OK).
# define TWI_WAIT() while(!((TWI_IS_CLOCKHELD()) || (TWI_IS_BUSERR()) || ( TWI_IS_ARBLOST() ) || (TWI_IS_BUSBUSY())))

# include <avr/io.h>
# include <util/twi.h >
# include <util/delay.h>
# include <stdio.h>
# include "USART_COMMANDS.h"


static void TWI0_M_init(void) {
	PORTA.DIRSET = PIN2_bm | PIN3_bm;
	PORTA.PINCONFIG = PORT_PULLUPEN_bm;
	PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm;
	// TWI0 . CTRLA = TWI_SDAHOLD_50NS_gc ; // Setting hold time to 50 NS.
	TWI0.MCTRLA = TWI_ENABLE_bm; // Enable TWI /I2C as controller .
	TWI0.MBAUD = (uint8_t)TWI0_BAUD(I2C_SCL_FREQ ,0); // Configure TWI baud rate .
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc; // Setting the I2C bus to idle .
	PORTD.DIRSET = PIN4_bm;
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
static void I2C_M_read( uint8_t addr , uint8_t * data , uint8_t len) 
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
// int main(void)
// {
// 	USART3_init();
// 	TWI0_M_init();
// 	printf("Sample start\n");
// 	uint8_t data[7];
// 	/* Replace with your application code */
// 	while (1)
// 	{
// 		I2C_M_write(0x38 ,6);
// 		_delay_ms(1000);
// 		I2C_M_read(0x38, data ,6);
// 		data[6]='\0';
// 		_delay_ms(1000);
// 		printf("%s\n", data);
// 	}
// 	return 0;
//