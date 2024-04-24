/*
 * states.h
 *
 * Created: 17.04.2024 14:50:26
 *  Author: jarle
 */ 


#ifndef FANSTATES_H_
#define FANSTATES_H_
#include "Usart.h"


void printHomeScreen();


uint8_t manualFlag;
int manual_rpm_value;
int auto_rpm_value;
int temperature;

uint8_t FAN1Flag;
uint8_t FAN2Flag;
uint8_t FAN3Flag;
uint8_t FAN4Flag;

uint16_t pwm_to_rpm();

enum mode { //definerer states for vifter
	RPM,
	OFF,
	AUTO
};

const char *state_names[] = { "RPM", "OFF", "AUTO" }; //Defines as names to print to Uart
	
	
typedef struct rpm {
	enum mode State;
	int rpm; 
	}Fan;
	

	Fan fan1 = {OFF, 0 };
	Fan fan2 = {OFF, 0 };
	Fan fan3 = {OFF, 0 };
	Fan fan4 = {OFF, 0 };



void fanOff(){ //sets the chosen fan in mode OFF
	if ((FAN1Flag == 1)){
		fan1.State = OFF;
		
		printf("fan1 is now in mode: off");
		printHomeScreen();
	}
	if ((FAN2Flag == 1)) {
		fan2.State = OFF;
		
		printf("fanw is now in mode: off");
		printHomeScreen();
	}
	if ((FAN3Flag == 1)) {
		fan3.State = OFF;
		
		printf("fan3 is now in mode: off");
		printHomeScreen();
	}
	if ((FAN4Flag == 1)) {
		fan4.State = OFF;
		
		printf("fan4 is now in mode: off");
		printHomeScreen();
	}
}

void fanAuto(){ //sets the chosen fan in mode auto
	if ((FAN1Flag == 1)){
		fan1.State = AUTO;
		printf("fan1 is now in mode: Auto");
		printHomeScreen();
	}
	if ((FAN2Flag == 1)) {
		fan2.State = AUTO;
		printf("fan2 is now in mode: Auto");
		printHomeScreen();
	}
	if ((FAN3Flag == 1)) {
		fan3.State = AUTO;
		printf("fan3 is now in mode: Auto");
		printHomeScreen();
	}
	if ((FAN4Flag == 1)) {
		fan4.State = AUTO;
		printf("fan4 is now in mode: Auto");
		printHomeScreen();
	}
}

void fanManual(){ //sets the chosen fan in manual mode based on chosen rpm value: low, medium or high
	if ((FAN1Flag = 1)) {
		fan1.State = RPM;
		TCA0_SPLIT_LCMP0 = manual_rpm_value;
		printf("fan1 is now in mode: Manual \r\n");
		printHomeScreen();
	}
	if ((FAN2Flag == 1)) {
		fan2.State = RPM;
		TCA0_SPLIT_LCMP0 = manual_rpm_value;
		printf("fan1 is now in mode: Manual");
		printHomeScreen();
	}
	if ((FAN3Flag == 1)) {
		fan3.State = RPM;
		TCA0_SPLIT_HCMP0 = manual_rpm_value;
		printf("fan1 is now in mode: Manual");
		printHomeScreen();
	}
	if ((FAN4Flag == 1)) {
		fan4.State = RPM;
		TCA0_SPLIT_HCMP1 = manual_rpm_value;
		printf("fan1 is now in mode: Manual");
		printHomeScreen();
	}
}

int returnRpmInModeAuto(int temperature) { //RPM 0-79
	
	int a = 2;  // Slope of the line (change in RPM per degree Celsius)
	int b = 50; // Y-intercept of the line (RPM at 0 degrees Celsius)
	
	// Calculate RPM using the linear function: RPM = a * temperature + b
	int auto_rpm_value = (a * temperature + b);
	
	
	return auto_rpm_value;
	
}

void handleFansInAuto(){ //change rpm based on temperature if fan.State is Auto
	if (fan1.State == AUTO){
		TCA0_SPLIT_LCMP0 = returnRpmInModeAuto(temperature);
	}
	if  (fan2.State == AUTO){
		TCA0_SPLIT_LCMP1 = returnRpmInModeAuto(temperature);
	}
	if  (fan3.State == AUTO){
		TCA0_SPLIT_HCMP0 = returnRpmInModeAuto(temperature);
	}
	if  (fan4.State == AUTO){
		TCA0_SPLIT_HCMP1 = returnRpmInModeAuto(temperature);
	}
}
#endif /* FANSTATES_H_ */
