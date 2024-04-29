/*
 * MenysystemBib.h
 *
 * Created: 17.04.2024 12:56:51
 *  Author: jarle
 */ 


#ifndef MENYSYSTEM_H_
#define MENYSYSTEM_H_
#include "I2C_temperature.h"
#include "USART.h"
#include "fanStates.h"

// #define FAN1_bm 0b00000001
// #define FAN2_bm 0b00000010
// #define FAN3_bm 0b00000011
// #define FAN4_bm 0b00000100


//predeclaring TCB0/TCB1 calculating rpm value
uint16_t pwm_to_rpm1();
uint16_t pwm_to_rpm2();

uint16_t pulseWidthReadings1[100];
uint16_t pulseWidthReadings2[100];
uint16_t duty_cycle_input;


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


void printOverview() //printing overview of states and rpm values + temperature 
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
		chosenFan = FAN1_bm;
		break ;
		
		case 1:
		printf("Received command: fan2\r\n");
		PrintSelectFanMode();
		chosenFan = FAN2_bm;

		break ;
		
		case 2:
		printf("Received command: fan3\r\n");
		PrintSelectFanMode();
		chosenFan = FAN3_bm;

		break ;
		
		case 3:
		printf("Received command: fan4\r\n");
		PrintSelectFanMode();
		chosenFan = FAN4_bm;
		break ;
		
		case 4:
		printf("Received command: off\r\n");
		chosenModeIsManual = 0;
		fanOff();
		break ;
		
		case 5:
		printf("Received command: manual\r\n");
		printf("Type wanted rpm mode\r\n");
		printf("Alternatives: low, medium, high \r\n");
		chosenModeIsManual = 1;
		break ;
		
		case 6:
		printf("Received command: auto\r\n");
		chosenModeIsManual = 0;
		fanAuto();
		break ;
		
		case 7:
		printf("Received command: overview\r\n");
		fan1.rpm = pwm_to_rpm1();
		fan2.rpm = pwm_to_rpm2();
		printOverview();
		break ;
		
		case 8:
		printf("Received command: controller\r\n");
		printSelectFan();
		break ;
		
// 		case 9:
// 		manual_rpm_value = 35; //LOW
// 		fanManual();
// 		break ;
// 		
// 		case 10:
// 		manual_rpm_value = 60; //MEDIUM
// 		fanManual();
// 		break ;
// 		
// 		case 11:
// 		manual_rpm_value = 79; //HIGH
// 		fanManual();
// 		break ;
		
		default:
		
		if(chosenModeIsManual)
		{
			switch (command_number)
			{
				case 9:
				
				printf("Received command: low\r\n");
				manual_rpm_value = FAN_RPM_LOW; //LOW
				fanManual();
				break;
				
				case 10:
				
				printf("Received command: medium\r\n");
				manual_rpm_value = FAN_RPM_MEDIUM; //MEDIUM
				fanManual();
				break;
				
				case 11:
				
				printf("Received command: high\r\n");
				manual_rpm_value = FAN_RPM_HIGH; //HIGH
				fanManual();
				break;
				
				default:
				
				 duty_cycle_input = atoi(command);
				printf("%d\r\n", duty_cycle_input);
				if (duty_cycle_input >= 0 && duty_cycle_input <= 100)
				{
					uint16_t new_duty_cycle = (duty_cycle_input / 100.0) * FAN_RPM_HIGH;
					printf("%d\r\n", new_duty_cycle);
					// Set the duty cycle to the new value
					// Assuming TCA0_SPLIT_LCMP0 is where the duty cycle is set
					manual_rpm_value = new_duty_cycle;
					fanManual();
				}
				else
				{
					printf("Please insert a valid duty cycle (0-100)\r\n");
				}
				break;
				
				
			}
		}
		
	}

}

	


#endif /* MENYSYSTEM_H_ */