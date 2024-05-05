/*****************************************************************************
*    All USART setup is fetched from "Getting Started With USART"
*    Author: Microchip
*    Date: 2020
*    Availability: https://github.com/microchip-pic-avr-examples/atmega4809-getting-started-with-usart-studio/blob/master/Send_Formatted_String_Using_Printf/main.c
				   https://github.com/microchip-pic-avr-examples/atmega4809-getting-started-with-usart-studio/blob/master/Receive_Control_Commands/main.c
****************************************************************************/

#ifndef USART_H_
#define USART_H_


#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)
#define INIT_DELAY 10
#define MAX_COMMAND_LEN 100
#define NUMBER_OF_COMMANDS 13

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "FanStates.h"
#include "Menysystem.h"



// Predefined valid commands received from UART
static char commands [ NUMBER_OF_COMMANDS ][ MAX_COMMAND_LEN ] = {  
	"fan1",
	"fan2",
	"fan3",
	"fan4",
	"off",
	"manual",
	"auto",
	"overview",
	"controller",
	"low",
	"medium",
	"high",
	"alarm"

	
	// Checks if command is equal to one of the predefined commands in the array commands[]. 
	// Return the index of the command received in the array or 255 if none
	};uint8_t compareCommands (char *src ){
		
	for (uint8_t i = 0; i < NUMBER_OF_COMMANDS ; i ++) {
		if(strcmp(src, commands[i]) == 0) {
			return i; // return the index of the command when valid 
		}
	}
	return 255; // if the command is not valid
}


/**
 * \brief Sends the char c to the TX pin, i.e through serial.
 * 
 * \param c A character
 */
void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}


static int USART3_printChar(char c,  FILE *stream)
{
	USART3_sendChar(c);
	return 0;
}

static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE); // Initializes a stream of characters to be sent to Uart


void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}


/**
 * @brief: Reading characters from UART 
 * @return: char 
 */
char USART3_readChar(void) 
{
	while (!(USART3.STATUS & USART_RXCIF_bm))
	{
		;
	}
	
	uint8_t src = USART3.RXDATAL;
	USART3_sendChar(src);
	return src;
}



/**
 * \brief Initiates USART, declaring the pins
 */
void USART3_init(void)
{
	
	PORTB.DIRSET = PIN0_bm ; //TX -pin as output
	PORTB.DIRCLR = PIN1_bm ; //RX -pin as input
	USART3.CTRLA |= (1 << USART_RXCIE_bp);
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600); // Sets the baud rate 
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm ; // Enable both TX and RX
	stdout = &USART_stream;


}



/**
 * @brief: Combines the cha cr to an array. Calls a function to execute different commands based on the array
d
 */
void read_commands() 
{
	char command[MAX_COMMAND_LEN];
	static uint8_t index = 0;
	char c = USART3_readChar();
	
	if(c != '\n' && c != '\r' && index < MAX_COMMAND_LEN - 1)
	{
		command[index++] = c;
		
	}
	
	if(c == '\r'|| c=='\n')
	{
		command[index] = '\0';
		index = 0;

		executeCommand(compareCommands(command), command); // executes actions based on command as an argument. Two arguments: One as the array and one used as index number for a switch case 
		
		
	}
}




#endif /* USART_H_ */