/*
 * UsartTest.h
 *
 * Created: 23.04.2024 14:39:55
 *  Author: jarle
 */ 


#ifndef USARTTEST_H_
#define USARTTEST_H_

#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)
#define INIT_DELAY 10
#define MAX_COMMAND_LEN 100
#define NUMBER_OF_COMMANDS 12

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "states.h"
#include "MenysystemBib.h"



int rpm_value;
int manualFlag;

int FAN1Flag;
int FAN2Flag;
int FAN3Flag;
int FAN4Flag;


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
	
	
	};uint8_t compareCommands (char *src ){
	// Return index of command received or 255 if none
	for (uint8_t i = 0; i < NUMBER_OF_COMMANDS ; i ++) {
		if( strcmp (src , commands[i]) == 0) {
			return i;
		}
	}
	return 255;
}

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

static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);


void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}


char USART3_readChar(void)
{
	while (!(USART3.STATUS & USART_RXCIF_bm))
	{
		;
	}
	//return USART3.RXDATAL;
	uint8_t src = USART3.RXDATAL ;
	USART3_sendChar(src);
	return src;
}

void USART3_init(void)
{
	
	PORTB.DIRSET = PIN0_bm ; //TX -pin as output
	PORTB.DIRCLR = PIN1_bm ; //RX -pin as input
	USART3.CTRLA |= (1 << USART_RXCIE_bp);
	//PORTC.DIR &= ~PIN1_bm;
	//PORTC.DIR |= PIN0_bm;
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm ; // Enable both TX and RX
	stdout = &USART_stream;

	
}




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
		//printf("test command1: %s\r\n",command);

		executeCommand(compareCommands(command), command);
		
	}
}




#endif /* USARTTEST_H_ */