/*
 * MenysystemBib.h
 *
 * Created: 17.04.2024 12:56:51
 *  Author: jarle
 */ 


#ifndef MENYSYSTEM_H_
#define MENYSYSTEM_H_
#include "Usart.h"

int temperature;

void printHomeScreen()
{
	printf("\r\n");  
	printf("\r\n");  
	printf("\r\n");  
	printf("homescreen:\r\n");
	printf("Type wanted action\r\n");
	printf("\r\n");  
	printf("Alternatives: overview, controller\r\n");
	
}

void PrintSelectFanMode()
{
	printf("Type wanted mode\r\n");
	printf("\r\n");
	printf("Alternatives: off, auto, manual\r\n"); 
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
	
	printf("Temperature: %d\r\n", temperature);
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


	

void executeCommand(uint8_t command_number, char *command)
{
	
	
	switch ( command_number )
	{
		case 0:
		printf("Received command: fan1\r\n");
		PrintSelectFanMode();
		FAN1Flag = 1;
		FAN2Flag = 0;
		FAN3Flag = 0;
		FAN4Flag = 0;
		break ;
		
		case 1:
		printf("Received command: fan2\r\n");
		PrintSelectFanMode();
		FAN1Flag = 0;
		FAN2Flag = 1;
		FAN3Flag = 0;
		FAN4Flag = 0;
		break ;
		
		case 2:
		printf("Received command: fan3\r\n");
		PrintSelectFanMode();
		FAN1Flag = 0;
		FAN2Flag = 0;
		FAN3Flag = 1;
		FAN4Flag = 0;
		break ;
		
		case 3:
		printf("Received command: fan4\r\n");
		PrintSelectFanMode();
		FAN1Flag = 0;
		FAN2Flag = 0;
		FAN3Flag = 0;
		FAN4Flag = 1;
		break ;
		
		case 4:
		printf("Received command: off\r\n");
		manualFlag = 0;
		fanOff();
		break ;
		
		case 5:
		printf("Received command: manual\r\n");
		printf("Type wanted RPM value \r\n");
		printf("Alternatives: low, medium, high \r\n");
		manualFlag = 1;
		break ;
		
		case 6:
		printf("Received command: auto\r\n");
		manualFlag = 0;
		fanAuto();
		break ;
		
		case 7:
		printf("Received command: overview\r\n");
		printOverview();
		break ;
		
		case 8:
		printf("Received command: controller\r\n");
		printSelectFan();
		break ;
		
		case 9:
		manual_rpm_value = 30; //LOW = 30 i testing
		fanManual();
		break ;
		
		case 10:
		manual_rpm_value = 60; //MEDIUM = 60 i testing
		fanManual();
		break ;
		
		case 11:
		manual_rpm_value = 90; //HIGH = 90 i testing
		fanManual();
		
		break ;
		
		default :
		printf("unknown command \r\n");
	}

}

	


#endif /* MENYSYSTEM_H_ */