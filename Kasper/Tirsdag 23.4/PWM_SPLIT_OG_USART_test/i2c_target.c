/*
 * i2c_target.c
 *
 * Created: 4/22/2024 3:18:37 PM
 *  Author: Kaspe
 */ 

# include "i2c_target.h"
# include <avr/io.h>
# include <avr/interrupt.h>
# include <stdbool.h>

# define TWI_READ true
# define TWI_WRITE false

static void (*writeHandler)(uint8_t);
static uint8_t (*readHandler)(void);
static void (*stopHandler)(void);
/**
* @brief This function initiates the TWI target with a specified address .
*
* @param address The desired address of the target .
*/
void TWI_initClient ( uint8_t address )
{
	// Init Function Pointers to Null
	writeHandler = 0;
	readHandler = 0;
	stopHandler = 0;
	// Set Client I2C Address
	TWI0.SADDR = address << 1;
	// Enable Data Interrupts , Address / Stop Interrupts , Enable STOP , and the TWI Peripheral
	TWI0.SCTRLA = TWI_DIEN_bm | TWI_APIEN_bm | TWI_PIEN_bm | TWI_ENABLE_bm ;}
/**
* @brief This function initiates the I/O pins used by the TWI .
*/
void TWI_initPins ( void )
{
	// PA2 /PA3
	// Output I/O
	PORTA . DIRSET = PIN2_bm | PIN3_bm ;
	# ifdef TWI_ENABLE_PULLUPS
	// Enable Pull -Ups
	PORTA . PINCONFIG = PORT_PULLUPEN_bm ;
	# endif
	// Select RA2/ RA3
	PORTA . PINCTRLUPD = PIN2_bm | PIN3_bm ;
}
/**
* @brief ISR for handling the various TWI interrupts .
*/
ISR ( TWI0_TWIS_vect )
{
	if ( TWI0 . SSTATUS & TWI_DIF_bm )
	{
		// Data Interrupt Flag
		uint8_t data = 0x00 ;
		if ((( TWI0 . SSTATUS & TWI_DIR_bm ) >> TWI_DIR_bp ) == TWI_WRITE )
		{
			// Data Write ( Host -> Client )
			data = TWI0 . SDATA ;
			if ( writeHandler )
			{
				writeHandler ( data );
			}
		}
		else
		{
			// Data Read ( Host <- Client )
			if ( readHandler )
			{
				data = readHandler () ;
			}
			TWI0 . SDATA = data ;
		}
		// ACK
		TWI0 . SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc ;
	}
	if ( TWI0 . SSTATUS & TWI_APIF_bm )
	{
		// Address Match or STOP
		if ( TWI0 . SSTATUS & TWI_AP_ADR_gc )
		{
			// Address Match
			TWI0 . SCTRLB = TWI_ACKACT_ACK_gc | TWI_SCMD_RESPONSE_gc ;
		}
		else
		{
			// STOP Condition .
			if ( stopHandler )
			{
				stopHandler () ;
			}
			TWI0 . SCTRLB = TWI_ACKACT_NACK_gc | TWI_SCMD_COMPTRANS_gc ;
		}
	}
}
/**
* @brief Assigns the function to call when a byte is written to this device
*
* @param onWrite a function with a uint8_t data parameter for handling received data .
*/
void TWI_assignByteWriteHandler ( void (* onWrite )( uint8_t ))
{
	writeHandler = onWrite ;
}
/**
* @brief Assigns the function to call when a byte is requested from this device
*
* @param onRead a function that returns a uint8_t value to be transmitted .
*/
void TWI_assignByteReadHandler ( uint8_t (* onRead )( void ) )
{
	readHandler = onRead ;
}
/**
* @brief Assigns the function to call when the I2C Bus stops .
*/
void TWI_assignStopHandler ( void (* onStop )( void ))
{
	stopHandler = onStop ;
}