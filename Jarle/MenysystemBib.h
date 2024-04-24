/*
 * MenysystemBib.h
 *
 * Created: 17.04.2024 12:56:51
 *  Author: jarle
 */ 


#ifndef MENYSYSTEMBIB_H_
#define MENYSYSTEMBIB_H_
#include "states.h"



void menuStateMachine();
				 
void printHomeScreen()
{
	USART3_sendString("\r\n");  
	USART3_sendString("\r\n");  
	USART3_sendString("\r\n");  
	USART3_sendString("homescreen:\r\n");
	USART3_sendString("Type wanted menu\r\n");
	USART3_sendString("\r\n");  
	USART3_sendString("overview\r\n"); //hastighet for hver vifte - temperatur 
	USART3_sendString("controller\r\n"); //start stopp for hver vifte - modus for hver vifte
}

void PrintSelectFanMode()
{
	USART3_sendString("Type wanted mode\r\n");
	USART3_sendString("\r\n");
	USART3_sendString("Alternatives: off, auto, manual\r\n"); 
}


void printOverview()
{
	
	printf("OVERVIEW \r\n");
	printf("\r\n");
	printf("Fan 1 rpm: %d\r\n", fan1.rpm);
	printf("Fan 1 state: %s\r\n",  state_names[fan1.State]);
	printf("Fan 2 rpm: %d\r\n", fan2.rpm);
	printf("Fan 2 state: %s\r\n", state_names[fan2.State]);
	printf("Fan 3 rpm: %d\r\n", fan3.rpm);
	printf("Fan 3 state: %s\r\n", state_names[fan3.State]);
	printf("Fan 4 rpm: %d\r\n", fan4.rpm);
	printf("Fan 4 state: %s\r\n", state_names[fan4.State]);
	
	printf("Temperature: %d\r\n", 69);
	printf("\r\n");
	printf("\r\n");
	printHomeScreen();
}


void printSelectFan()
{
	printf("Controller\r\n");
	printf("Select what fan to change\r\n");
	printf("Alternatives: fan1, fan2, fan3 or fan4 \r\n");
}


void Menu(char *command) {
	
	if (strcmp(command, "OVERVIEW") == 0) {
		printOverview();
		} else if (strcmp(command, "CONTROLLER") == 0) {
	    printSelectFan();
		} else {
		//printf("Unknown command \r\n");
	}
}


typedef enum {
	IDLE,
	OVERVIEW,
	CONTROLLER,
	SELECT_FAN,
	SELECT_MODE,
	EXECUTE
	
} menuState;

// Declare state variable
menuState currentState = IDLE;



	
	


#endif /* MENYSYSTEMBIB_H_ */