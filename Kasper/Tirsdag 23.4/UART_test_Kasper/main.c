
#ifndef USART3_COMMANDS_H_
#define USART3_COMMANDS_H_

#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)

#define INIT_DELAY 10
#define MAX_COMMAND_LEN 100

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "USART_COMMANDS.h"

char c;
void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char *str);
char USART3_readChar(void);

void executeCommand(char *command);
void read_commands(void);



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
	return USART3.RXDATAL;
}

void USART3_init(void)
{
	
	PORTB.DIRSET = PIN0_bm ; //TX -pin as output
	PORTB.DIRCLR = PIN1_bm ; //RX -pin as input
	//PORTC.DIR &= ~PIN1_bm;
	//PORTC.DIR |= PIN0_bm;
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm ; // Enable both TX and RX
	
}


void executeCommand(char *command)
{
	if(strcmp(command, "AUTO") == 0)
	{
		//USART3_sendString("AUTOMATIC MODE ENABLED.\r\n");
		printf(("AUTOMATIC MODE ENABLED.\r\n"));

	}
	else if (strcmp(command, "MANUAL") == 0)
	{
		
		USART3_sendString("OK, LED OFF.\r\n");
	}
	else
	{
		USART3_sendString("Type AUTO/MANUAL to control the FANS.\r\n");
	}
}



	

int main(void)
{
	
	//uint8_t count = 0;
	char command[MAX_COMMAND_LEN];
	uint8_t index = 0;
	char c;
	
	USART3_init();
	
	static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);
	stdout = &USART_stream;
	
	_delay_ms(10);

	
	USART3_sendString("Type AUTO/MANUAL to control the FANS.\r\n");
	while (1)
	{
		//USART3_sendString("Hello World!\r\n");
		//_delay_ms(500);
		
		//printf("Counter value is: %d\r\n", count++);
		//_delay_ms(1000);
		
		c = USART3_readChar();
	
		if(c != '\n' && c != '\r')
		{
			command[index++] = c;
			if(index > MAX_COMMAND_LEN)
			{
				index = 0;
			}
		}
		
		if(c == '\r'|| c=='\n')
		{
			command[index] = '\0';
			index = 0;
			executeCommand(command);
		}

		 
	}
}#endif /* USART3_COMMANDS_H_ */