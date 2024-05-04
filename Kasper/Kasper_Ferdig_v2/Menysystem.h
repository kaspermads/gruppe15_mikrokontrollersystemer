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
#include "pwm_to_rpm.h"



uint16_t duty_cycle_input;
int temperature;

// Diagnosis variables
uint8_t diagnoseIsRunning;
uint8_t changeAlarmDeviation;



/**
 * @brief Prints the home screen text
 * @return void
 */
void printHomeScreen()
{
	  
	printf("\r\n");  
	printf("HOMESCREEN:\r\n");
	printf("Type wanted action\r\n");
	printf("\r\n");  
	printf("Alternatives: overview, controller, alarm\r\n");
	
}


/**
 * @brief Prints the different command alternatives
 * @return void
 */
void PrintSelectFanMode()
{
	printf("Type wanted mode\r\n");
	printf("\r\n");
	printf("Alternatives: off, auto, manual\r\n"); 
}



/**
 * @brief: Prints the fans rpm and state. Also prints the current temperature
 
 * 
 * @return void
 */
void printOverview()
{
	
	printf("OVERVIEW \r\n");
	printf("\r\n");
	for (int i = 1; i < NUM_FANS; i++) {
		printf("Fan %d rpm: %d\r\n", i, (fan[i].State == OFF) ? 0 : fan[i].rpm);
		printf("Fan %d state: %s\r\n", i, fanStates[fan[i].State].name);
	}
	
	temperature = AHT10_read(); //reads the temperature
	printf("Temperature: %d\r\n", temperature);
	printf("\r\n");
	
	printHomeScreen();
}


void printSelectFan()
{
	printf("Controller\r\n");
	printf("Select what fan to change\r\n");
	printf("Alternatives: fan1, fan2, fan3 or fan4 \r\n");
}


/**
 * @brief: Main function for controlling the fans and alarm. Based on predefined commands from UART.
	Different modes and fans is selected by chosenMode(auto, off or manual) and chosenFan(fan1-fan4) 
	Manual mode expects the alternatives: low, medium, high or fan speed: 0-100 and executes based on given input 
	Auto mode sets the fan to mode Auto.
	Off mode turns the fan off
	Alarm gives the opportunity to change the deviation percentage of tear and wear alarm for each fan
 * 
 * @param command_number: Index given by predefined valid commands in static char commands [ NUMBER_OF_COMMANDS ][ MAX_COMMAND_LEN ] in Usart.h
 * @param command: Actual command given by UART
 * 
 * @return void
 */
void executeCommand(uint8_t command_number, char *command)
{
	if (diagnoseIsRunning) {
		printf("Diagnose is running!\r\n ");
	}
	else {
	
	switch ( command_number )// based on index given by predefined valid UART commands
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
		printf("Received command: off\r\n"); //turns the selected fan off
		chosenModeIsManual = 0;
		setFanToOff();
		break ;
		
		case 5:
		printf("Received command: manual\r\n"); //sets chosenModeIsManual to true. Breaks and expects another command
		printf("Type wanted rpm mode\r\n");
		printf("Alternatives: low, medium, high or fan speed: 0-100 \r\n");
		chosenModeIsManual = 1;
		break ;
		
		case 6:
		printf("Received command: auto\r\n");//Set the selected fan to mode Auto
		chosenModeIsManual = 0;
		setFanToAuto();
		break ;
		
		case 7:
		printf("Received command: overview\r\n"); // reads the rpm values of the fans and calls a function to print overview
		fan[1].rpm = pwm_to_rpm(pulseWidthReadings1);
		fan[2].rpm = pwm_to_rpm(pulseWidthReadings2);
		
		printOverview();
		break ;
		
		case 8:
		printf("Received command: controller\r\n");
		printSelectFan(); //calls a function to print the alternative fans
		break ;
		
		case 12: // if command = alarm. Display current deviation percentage
		changeAlarmDeviation = 1;
		printf("Type wanted deviation percentage for wear and tear alarm \n\r");
		printf("Current alarm deviation percentage is: %d\n\r", threshold_percentage);
		break;
		
		default :
		
		if (changeAlarmDeviation){// changes the deviation percentage based on UART command
				threshold_percentage = atoi(command); //change alarm deviaton for alarm type long
				changeAlarmDeviation = 0;
				printf("wear and tear alarm devation percentage is changed to: %d\n\r", threshold_percentage);
				break;
			}
			
		
		
		if(chosenModeIsManual)// Sets the selected fan to rpm value choosed. Low, medium, high or 0-100
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
				if (duty_cycle_input >= 0 && duty_cycle_input <= 100) //Return the value choosed (0-100%) 
				{
					uint16_t new_duty_cycle = (duty_cycle_input / 100.0) * FAN_RPM_HIGH;
					printf("%d\r\n", new_duty_cycle);
					manual_rpm_value = new_duty_cycle;
					setFanToManual(); //execute the command and set the fan rpm to choosen rpm value 
				}
				else
				{
					printf("Please insert a valid duty cycle (0-100)\r\n");
				}
				break;
				
				
			}
		
		
		}
		
		
		else printf("unknown command \r\n"); //if command is not equal to predefined valid UART commands
		
	}

   }
}

	


#endif /* MENYSYSTEM_H_ */