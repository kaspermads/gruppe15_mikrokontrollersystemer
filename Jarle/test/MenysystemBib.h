/*
 * MenysystemBib.h
 *
 * Created: 17.04.2024 12:56:51
 *  Author: jarle
 */ 


#ifndef MENYSYSTEMBIB_H_
#define MENYSYSTEMBIB_H_
#include "states.h"

char command[MAX_COMMAND_LEN];


				 
void printHomeScreen()
{
	USART3_sendString("\r\n");  
	USART3_sendString("\r\n");  
	USART3_sendString("\r\n");  
	USART3_sendString("homescreen:\r\n");
	USART3_sendString("Type wanted menu\r\n");
	USART3_sendString("\r\n");  
	USART3_sendString("OVERVIEW\r\n"); //hastighet for hver vifte - temperatur 
	USART3_sendString("CONTROLLER\r\n"); //start stopp for hver vifte - modus for hver vifte
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


void printController()
{
	printf("Controller\r\n");
	printf("Select what fan to change\r\n");
	printf("Alternatives: 1, 2, 3 or 4 \r\n");
	returnUart(command);
	selectFan(command);
}


void mainMenu() {
	returnUart(command);
	if (strcmp(command, "OVERVIEW") == 0) {
		printOverview();
		} else if (strcmp(command, "CONTROLLER") == 0) {
	    printController();
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

void menuStateMachine() {
	int selectedFan = 0;
	char selectedMode[MAX_COMMAND_LEN];
	
	switch (currentState) {
		case IDLE:
		// If idle, wait for command
		returnUart(command);
		if (strcmp(command, "OVERVIEW") == 0) {
			currentState = OVERVIEW;
			} else if (strcmp(command, "CONTROLLER") == 0) {
			currentState = CONTROLLER;
			
			} else {
			//printf("Unknown command \r\n");
		}
		break;
		case OVERVIEW:
		printOverview();
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
		returnUart(command);
		if (strlen(command) > 0) {
			selectedFan = atoi(command);
			printf("Selected fan is %d\r\n", selectedFan);
			currentState = SELECT_MODE;
		}
		break;

		case SELECT_MODE:
		printf("Type wanted mode or rpm \r\n");
		printf("Alternatives: RPM, OFF, AUTO \r\n");
		returnUart(command);
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
				returnUart(command);
				if (strlen(command) > 0) {
					fan1.rpm = atoi(command);
				}	currentState = IDLE; // Reset state to idle after action
			}
			else if (strcmp(selectedMode, "OFF") == 0) {
				fan1.State = OFF;
			currentState = IDLE; } // Reset state to idle after action
			else if (strcmp(selectedMode, "AUTO") == 0) {
				fan1.State = AUTO;
			currentState = IDLE; } // Reset state to idle after action
			
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
	


#endif /* MENYSYSTEMBIB_H_ */