
/*
* GccApplication1.c
*
* Created: 31.01.2024 12:23:48
* Author : jarle
*/
#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) 
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "UsartBib.h"
#include "MenysystemBib.h"
#include "states.h"
#define MAX_COMMAND_LEN 100


//void USART3_init(void); //funksjon for å sende int
//void USART3_sendChar(char c); //funksjon for å sende char
//void USART3_sendString(char *str); //funksjon for å sende str


uint8_t USART3_read();
char USART3_readChar(void);


ISR(USART3_RXC_vect)
{
	returnUart(command);
}




int main(void)
{
	USART3_init();
	sei(); // Enable Global Interrupts
	_delay_ms(10);
	static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);
	stdout = &USART_stream;	//char command[MAX_COMMAND_LEN];
	//printHomeScreen();
	
	
	while (1)
	{

		menuStateMachine();
		
	}

}
}


/*
To do
Lage timeout for å ikke stå fast i en løkke





*/

