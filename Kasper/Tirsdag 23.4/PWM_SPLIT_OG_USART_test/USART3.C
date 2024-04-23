/*
 * USART3.C
 *
 * Created: 4/21/2024 1:06:17 PM
 *  Author: Kasper
 */ 


#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)

#define INIT_DELAY 10
#define MAX_COMMAND_LEN 100
#define FAN_LOW 35
#define FAN_MEDIUM 60
#define FAN_MAX 79
#define NUMBER_OF_COMMANDS 7

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "USART_COMMANDS.h"


static char commands [ NUMBER_OF_COMMANDS ][ MAX_COMMAND_LEN ] = {
	"off",
	"on",
	"manual",
	"auto"
	
	
};uint8_t compareCommands (char *src ){
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
	printf("Welcome!\r\n");
	printf("Type auto or manual to choose fan mode\r\n");

	
}

void manualMode(char *command)
{	
	//static uint8_t
	printf("In manual mode.\r\n");
	printf("Please select fan-mode [LOW MEDIUM MAX] or choose fan. \r\n");
	//printf("test command: %s\r\n",command);
// 	if (strcmp(command, "LOW") == 0) {
// 		// Set fan speed to low
// 		TCA0_SPLIT_LCMP0 = FAN_LOW;
// 	
// 		//isManualMode = false;
// 		
// 		} else if (strcmp(command, "MEDIUM") == 0) {
// 			
// 		// Set fan speed to medium
// 		TCA0_SPLIT_LCMP0 = FAN_MEDIUM;
// 		//isManualMode = false;
// 
// 		} else if (strcmp(command, "MAX") == 0) {
// 			
// 		// Set fan speed to max
// 		TCA0_SPLIT_LCMP0 = FAN_MAX;
// 		//isManualMode = false;
// 		
// 		} 
// 		//else if (strcmp(command, "manual") == 0) {
// 		else
// 		{	
// 			uint16_t duty_cycle_input = atoi(command);
// 			printf("%d\r\n", duty_cycle_input);
// 			if (duty_cycle_input >= 0 && duty_cycle_input <=100)
// 			{
// 				uint16_t new_duty_cycle = (duty_cycle_input/100.0)*FAN_MAX;
// 				printf("%d\r\n", new_duty_cycle);
// 				TCA0_SPLIT_LCMP0 = new_duty_cycle;
// 			}
// 			else
// 			{
// 				printf("Please insert a valid duty cycle");
// 			}
// 		
// 		
// 		
// 		} 

		chooseFan(command);
}

//void automaticMode()

void executeCommand(uint8_t command_number, char *command)
{
	static uint8_t isManualMode = 0;
	
	switch ( command_number )
	{
	case 0:
		
		printf("Received command: off\r\n");
		TCA0_SPLIT_LCMP0 = 0;
		isManualMode = 0;
		
		break ;
		
	case 1:
	
		printf("Received command: on\r\n");
		TCA0_SPLIT_LCMP0 = FAN_MEDIUM;
		break ;
		
	case 2:
	
		printf("Received command: manual\r\n");
		
		isManualMode = 1;
		
		break ;
		
	case 3:
	
		printf("auto\r\n");
		isManualMode = 0;
		break ;
		
	default :
		if (isManualMode)
		{
			manualMode(command);
		}
		else
		{
			printf("\r\n");
		}
	}
	
// 	if(strcmp(command, "AUTO") == 0)
// 	{
// 		printf(("AUTOMATIC MODE ENABLED.\r\n"));
// 
// 	}
// 	else if (strcmp(command, "MANUAL") == 0)
// 	{
// 		
// 		manualMode(command);
// 
// 	}
// 	else
// 	{	
// 		printf("Type AUTO/MANUAL to control the FANS.\r\n");
// 	}
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



 int chooseFan(char *command)
{
	static uint8_t fanChoosingState = 1;
	
	if (fanChoosingState)
	{
	
		if (strcmp(command, "FAN1"))
		{
			fanChoosingState = 0;
			printf("Please select speed\r\n");
			TCA0.SPLIT.LCMP0 = setFanSpeed(command);
			
			
			
		}
		else if (strcmp(command, "FAN2"))
		
		{
			fanChoosingState = 0;
			return 2;
		}
	}
	else
	{
		printf("Please select a fan first");
	}
}

int setFanSpeed(char *command)
{
	uint16_t duty_cycle_input = atoi(command);

		if (duty_cycle_input >= 0 && duty_cycle_input <=100)
		{
			uint16_t new_duty_cycle = (duty_cycle_input/100.0)*FAN_MAX;
			//printf("%d\r\n", new_duty_cycle);
			//TCA0_SPLIT_LCMP0 = new_duty_cycle;
			return new_duty_cycle;
		}
		else
		{
		
			printf("Please insert a valid duty cycle");
			return 0;
		}
	
}

