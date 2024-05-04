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


#define FAN1_bm 0b00000001
#define FAN2_bm 0b00000010
#define FAN3_bm 0b00000100
#define FAN4_bm 0b00001000
#define FAN_COUNT 4

//predeclaring TCB0/TCB1 calculating rpm value
uint16_t pwm_to_rpm1();
uint16_t pwm_to_rpm2();

uint16_t pulseWidthReadings1[300];
uint16_t pulseWidthReadings2[300];
uint16_t duty_cycle_input;

int temperature;

uint8_t diagnoseIsRunning;


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
	
// 	printf("OVERVIEW \r\n");
// 	printf("\r\n");
// 	printf("Fan 1 rpm: %d\r\n", fan1.rpm);
// 	printf("Fan 1 state: %s\r\n",  state_names[fan1.State]);
// 	printf("Fan 2 rpm: %d\r\n", fan2.rpm);
// 	printf("Fan 2 state: %s\r\n", state_names[fan2.State]);
// 	printf("Fan 3 rpm: %d\r\n", fan3.rpm);
// 	printf("Fan 3 state: %s\r\n", state_names[fan3.State]);
// 	printf("Fan 4 rpm: %d\r\n", fan4.rpm);
// 	printf("Fan 4 state: %s\r\n", state_names[fan4.State]);
// 	
// 	temperature = AHT10_read();
// 	printf("Temperature: %d\r\n", temperature);
// 	printf("\r\n");
// 	printf("\r\n");
// 	printHomeScreen();
 for (uint8_t i = 0; i < FAN_COUNT; i++) {
	 printf("Fan %d rpm: %d\r\n", i + 1, fans[i].rpm);
	 printf("Fan %d state: %s\r\n", i + 1, state_names[fans[i].State]);
 }
  temperature = AHT10_read();
  printf("Temperature: %d\r\n\r\n", temperature);
  printHomeScreen();
}



void printSelectFan()
{
// 	printf("Controller\r\n");
// 	printf("Select what fan to change\r\n");
// 	printf("Alternatives: fan1, fan2, fan3 or fan4 \r\n");
	 printf("Controller\r\n");
	 printf("Select what fan to change\r\n");
	 printf("Alternatives: ");
	 
	 for (uint8_t i = 0; i < FAN_COUNT; i++) {
		 printf("fan%d", i + 1);
		 if (i != FAN_COUNT - 1) {
			 printf(", ");
		 }
	 }
	 
	 printf("\r\n");
}




	

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
		printf("Alternatives: low, medium, high or fan speed: 0-100 \r\n");
		chosenModeIsManual = 1;
		break ;
		
		case 6:
// 		printf("Received command: auto\r\n");
// 		chosenModeIsManual = 0;
// 		fanAuto();
		for (uint8_t i = 0; i < FAN_COUNT; i++) {
			if (chosenFan & (1 << i)) {
				fanAuto(i);
			}
			}
			
		break ;
		
		case 7:
		printf("Received command: overview\r\n");
		fans[0].rpm = pwm_to_rpm1();
		fans[1].rpm = pwm_to_rpm2();
		printOverview();
		break ;
		
		case 8:
		printf("Received command: controller\r\n");
		printSelectFan();
		break ;
		
		
		
		default :
		
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
				
				if (duty_cycle_input >= 0 && duty_cycle_input <= 100)
				{
					uint16_t new_duty_cycle = (duty_cycle_input / 100.0) * PWM_PERIOD;
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
		else printf("unknown command \r\n");
		
	}
	}
}

	


#endif /* MENYSYSTEM_H_ */