
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
#include "Usart.h"
#include "Menysystem.h"
#include "FanStates.h"


//USART
static int USART3_printChar(char c, FILE *stream);
void USART3_sendChar(char c);
void USART3_sendString(char *str);
char USART3_readChar(void);
void USART3_init(void);
void executeCommand(uint8_t command_number, char *command);
void read_commands();

//MenySystemBib
void printHomeScreen();
void PrintSelectFanMode();
void printOverview();
void printSelectFan();

//states
void fanOff();
void fanAuto();
void fanManual();
void handleFansInAuto();
int returnRpmInModeAuto(int temperature);
	
char command[MAX_COMMAND_LEN];
int temperature = 25; //test temperature

ISR(USART3_RXC_vect)
{
	read_commands();
}


int main(void)
{
	
	USART3_init();
	sei(); // Enable Global Interrupts
	PORTB.DIRSET = PIN3_bm; //LED builtin = output
	printHomeScreen(); 
	
	
	
	while(1){
		
		handleFansInAuto(); //changes the rpm of the fans in mode auto based on temperature
		_delay_ms(10); //whyyy? funker ikkje uten delay pga handleFansInAuto();
		int delayTime = fan1.rpm*10;
		 PORTB.OUTSET = PIN3_bm; // Turn on the LED
		 for (int i = 0; i < delayTime; i++) {
			 _delay_ms(1); // Delay for 1 millisecond
		 }
		 PORTB.OUTCLR = PIN3_bm; // Turn off the LED
		 for (int i = 0; i < delayTime; i++) {
			 _delay_ms(1); // Delay for 1 millisecond
		 }
	}
		
}

