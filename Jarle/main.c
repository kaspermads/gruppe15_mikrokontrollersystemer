
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
//#include "UsartBib.h"
#include "UsartTest.h"
#include "MenysystemBib.h"
#include "states.h"


static int USART3_printChar(char c, FILE *stream);
void USART3_sendChar(char c);
void USART3_sendString(char *str);
char USART3_readChar(void);
void USART3_init(void);
//char *executeCommand(char *command);
void read_commands();




ISR(USART3_RXC_vect)
{
	read_commands();
}


int main(void)
{
	
	USART3_init();
	sei(); // Enable Global Interrupts
	currentState = IDLE;
	char command[MAX_COMMAND_LEN];
	PORTB.DIRSET = PIN3_bm;
	printHomeScreen();
	
	
	while(1){
		
		int delayTime = rpm_value*10;
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


/*

int main(void)
{
	
	USART3_init();

	_delay_ms(10);
	//printf("test %d\r\n", 69);
	//printHomeScreen();
	sei(); // Enable Global Interrupts
	
	while (1)
	{
		;
		
		int selectedFan = 0;
		char selectedMode[MAX_COMMAND_LEN];
		
		switch (currentState) {
			case IDLE:
			// If idle, wait for command
			
			if (strcmp(command, "OVERVIEW") == 0) {
				printOverview();
				currentState = OVERVIEW;
				} else if (strcmp(command, "CONTROLLER") == 0) {
				currentState = CONTROLLER;
				
				} else {
				//printf("Unknown command \r\n");
			}
			break;
			case OVERVIEW:
			
			currentState = IDLE; // Reset state to idle after action
			break;
			
			case CONTROLLER:
			printf("Controller\r\n");
			printf("Select what fan to change\r\n");
			printf("Alternatives: 1, 2, 3 or 4 \r\n");
			currentState = SELECT_FAN;
			break;
			
			case SELECT_FAN:
			memset(command, 0, sizeof(command));
			
			if (strlen(command) > 0) {
				selectedFan = atoi(command);
				printf("Selected fan is %d\r\n", selectedFan);
				currentState = SELECT_MODE;
			}
			break;

			case SELECT_MODE:
			printf("Type wanted mode or rpm \r\n");
			printf("Alternatives: RPM, OFF, AUTO \r\n");

			strncpy(selectedMode, command, sizeof(selectedMode));
			//selectedMode = command;
			currentState = EXECUTE;
			break;
			
			case EXECUTE:
			
				switch (selectedFan) {
				
					case 1:
				
					if (strcmp(selectedMode, "RPM") == 0) {
						printf("Type wanted rpm \r\n");
						memset(command, 0, sizeof(command));
					
						if (strlen(command) > 0) {
							fan1.rpm = atoi(command);
						}	currentState = IDLE; // Reset state to idle after action
					}
					else if (strcmp(selectedMode, "OFF") == 0) {
						fan1.State = OFF;
						currentState = IDLE; // Reset state to idle after action
					}
					else if (strcmp(selectedMode, "AUTO") == 0) {
						fan1.State = AUTO;
						currentState = IDLE; // Reset state to idle after action
					}
				
					case 2:
					break;
				
					case 3:
					break;
				
					case 4:
					break;
				
				
					default:
					currentState = IDLE; // Reset state to idle after action

				}
			
		}
		
	}
}


*/

