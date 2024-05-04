/*
 * MenysystemBib.h
 *
 * Created: 17.04.2024 12:56:51
 *  Author: jarle
 */ 


#ifndef MENYSYSTEM_H_
#define MENYSYSTEM_H_
#include "I2C_temperature.h"
#include "Usart.h"
#include "fanStates.h"
#include "Error_Prediction.h"


//predeclaring TCB0/TCB1 calculating rpm value
uint16_t pwm_to_rpm1();
uint16_t pwm_to_rpm2();


uint16_t pulseWidthReadings1[100];
uint16_t pulseWidthReadings2[100];
uint16_t duty_cycle_input;

int temperature;

uint8_t diagnoseIsRunning;
uint8_t changeAlarmDeviation;

// printing alternatives in the menu
void printHomeScreen()
{
	printf("\r\n");  
	printf("\r\n");  
	printf("\r\n");  
	printf("homescreen:\r\n");
	printf("Type wanted action\r\n");
	printf("\r\n");  
	printf("Alternatives: overview, controller, alarm\r\n");
	
}

// printing alternatives in controller
void PrintSelectFanMode()
{
	printf("Type wanted mode\r\n");
	printf("\r\n");
	printf("Alternatives: off, auto, manual\r\n"); 
}

 //printing overview of fanstates, fan rpm values and temperature
void printOverview()
{
	
	printf("OVERVIEW \r\n");
	printf("\r\n");
	printf("Fan 1 rpm: %d\r\n", fan[1].rpm);
	printf("Fan 1 state: %s\r\n",  state_names[fan[1].State]);
	printf("Fan 2 rpm: %d\r\n", fan[2].rpm);
	printf("Fan 2 state: %s\r\n", state_names[fan[2].State]);
	printf("Fan 3 rpm: %d\r\n", fan[3].rpm);
	printf("Fan 3 state: %s\r\n", state_names[fan[3].State]);
	printf("Fan 4 rpm: %d\r\n", fan[4].rpm);
	printf("Fan 4 state: %s\r\n", state_names[fan[4].State]);
	
	temperature = AHT10_read();
	printf("Temperature: %d\r\n", temperature);
	printf("\r\n");
	printf("\r\n");
	
	printHomeScreen();
}

// printing alternatives for fans
void printSelectFan()
{
	printf("Controller\r\n");
	printf("Select what fan to change\r\n");
	printf("Alternatives: fan1, fan2, fan3 or fan4 \r\n");
}


	
// main function for controlling the fans and alarm. Based on predefined commands from UART.

void executeCommand(uint8_t command_number, char *command)
{
	if (diagnoseIsRunning) {
		printf("Diagnose is running!\r\n ");
	}
	else {
	
	switch ( command_number )
	{
		case 0:
		printf("Received command: fan1\r\n");
		PrintSelectFanMode();
		chosenFan = 1;
		break ;
		
		case 1:
		printf("Received command: fan2\r\n");
		PrintSelectFanMode();
		chosenFan = 2;
		break ;
		
		case 2:
		printf("Received command: fan3\r\n");
		PrintSelectFanMode();
		chosenFan = 3;
		break ;
		
		case 3:
		printf("Received command: fan4\r\n");
		PrintSelectFanMode();
		chosenFan = 4;
		break ;
		
		case 4:
		printf("Received command: off\r\n");
		chosenModeIsManual = 0;
		setFanToOff();
		break ;
		
		case 5:
		printf("Received command: manual\r\n");
		printf("Type wanted rpm mode\r\n");
		printf("Alternatives: low, medium, high or fan speed: 0-100 \r\n");
		chosenModeIsManual = 1;
		break ;
		
		case 6:
		printf("Received command: auto\r\n");
		chosenModeIsManual = 0;
		setFanToAuto();
		break ;
		
		case 7:
		printf("Received command: overview\r\n");
		fan[1].rpm = pwm_to_rpm1();
		fan[2].rpm = pwm_to_rpm2();
		
		
		printOverview();
		break ;
		
		case 8:
		printf("Received command: controller\r\n");
		printSelectFan();
		break ;
		
		case 12: // if command = alarm
		changeAlarmDeviation = 1;
		printf("Type wanted deviation percentage for wear and tear alarm \n\r");
		printf("Current alarm deviation percentage is: %d\n\r", threshold_percentage);
		break;
		
		default :
		
		if (changeAlarmDeviation){
				threshold_percentage = atoi(command); //change alarm deviaton for alarm type long
				changeAlarmDeviation = 0;
				printf("wear and tear alarm devation percentage is changed to: %d\n\r", threshold_percentage);
				break;
			}
			
		
		
		
		if(chosenModeIsManual)
		{
			switch (command_number)
			{
				case 9:
				
				printf("Received command: low\r\n");
				manual_rpm_value = FAN_RPM_LOW; //LOW
				setFanToManual();
				break;
				
				case 10:
				
				printf("Received command: medium\r\n");
				manual_rpm_value = FAN_RPM_MEDIUM; //MEDIUM
				setFanToManual();
				break;
				
				case 11:
				
				printf("Received command: high\r\n");
				manual_rpm_value = FAN_RPM_HIGH; //HIGH
				setFanToManual();
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
					setFanToManual();
				}
				else
				{
					printf("Please insert a valid duty cycle (0-100)\r\n");
				}
				break;
				
				
			}
		
		
		}
		
		
		else printf("unknown command \r\n");
		
	}

   }
}

	


#endif /* MENYSYSTEM_H_ */