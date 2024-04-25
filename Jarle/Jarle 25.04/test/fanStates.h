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


uint8_t chosenModeIsManual;
int manual_rpm_value;
int auto_rpm_value;
int temperature;

uint8_t chosenFanIs1;
uint8_t chosenFanIs2;
uint8_t chosenFanIs3;
uint8_t chosenFanIs4;

uint16_t pwm_to_rpm();


enum mode { //definerer states for vifter
	MANUAL,
	OFF,
	AUTO
};

const char *state_names[] = { "MANUAL", "OFF", "AUTO" }; //Defines as names to print to Uart
	
	
typedef struct rpm {
	enum mode State;
	uint16_t rpm; 
	}Fan;
	

	Fan fan1 = {OFF, 0 };
	Fan fan2 = {OFF, 0 };
	Fan fan3 = {OFF, 0 };
	Fan fan4 = {OFF, 0 };



void fanOff(){ //sets the chosen fan in mode OFF
	if (chosenFanIs1){
		fan1.State = OFF;
		TCA0_SPLIT_LCMP0 = 0;
		printf("fan1 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFanIs2) {
		fan2.State = OFF;
		TCA0_SPLIT_LCMP1 = 0;
		printf("fan2 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFanIs3) {
		fan3.State = OFF;
		TCA0_SPLIT_HCMP0 = 0;
		printf("fan3 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFanIs4) {
		fan4.State = OFF;
		TCA0_SPLIT_HCMP1 = 0;
		printf("fan4 is now in mode: off");
		printHomeScreen();
	}
}

void fanAuto(){ //sets the chosen fan in mode auto
	if (chosenFanIs1){
		fan1.State = AUTO;
		printf("fan1 is now in mode: Auto");
		printHomeScreen();
	}
	else if (chosenFanIs2) {
		fan2.State = AUTO;
		printf("fan2 is now in mode: Auto");
		printHomeScreen();
	}
	else if (chosenFanIs3) {
		fan3.State = AUTO;
		printf("fan3 is now in mode: Auto");
		printHomeScreen();
	}
	else if (chosenFanIs4) {
		fan4.State = AUTO;
		printf("fan4 is now in mode: Auto");
		printHomeScreen();
	}
}

void fanManual(){ //sets the chosen fan in manual mode based on chosen rpm value: low, medium or high
	if (chosenFanIs1) {
		fan1.State = MANUAL;
		TCA0_SPLIT_LCMP0 = manual_rpm_value;
		printf("fan1 is now in mode: Manual \r\n");
		printHomeScreen();
	}
	else if (chosenFanIs2) {
		fan2.State = MANUAL;
		TCA0_SPLIT_LCMP1 = manual_rpm_value;
		printf("fan2 is now in mode: Manual");
		printHomeScreen();
	}
	else if (chosenFanIs3) {
		fan3.State = MANUAL;
		TCA0_SPLIT_HCMP0 = manual_rpm_value;
		printf("fan3 is now in mode: Manual");
		printHomeScreen();
	}
	else if (chosenFanIs4) {
		fan4.State = MANUAL;
		TCA0_SPLIT_HCMP1 = manual_rpm_value;
		printf("fan4 is now in mode: Manual");
		printHomeScreen();
	}
}

int autoRPMmode(uint16_t temperature) {
	
	int a = 1.975;  // Slope of the line (change in RPM per degree Celsius)
	int b = -59.25; // Y-intercept of the line (RPM at 0 degrees Celsius)
	
	// Calculate RPM using the linear function: RPM = a * temperature + b
	float auto_rpm_value = (a * temperature + b);
	uint8_t auto_rpm_scaled_value = (uint8_t)((auto_rpm_value + 0.5));
	
	if (auto_rpm_scaled_value < 30)
	{
		auto_rpm_scaled_value = 30;
	}
	else if (auto_rpm_scaled_value > 79)
	{
		auto_rpm_scaled_value = 79;
	}
	
	return auto_rpm_scaled_value;
	
	
	
	return auto_rpm_value;
	
}


void handleFansInAuto(){ //change rpm based on temperature if fan.State is Auto
	if (fan1.State == AUTO){
		TCA0_SPLIT_LCMP0 = autoRPMmode(temperature);
	}
	if  (fan2.State == AUTO){
		TCA0_SPLIT_LCMP1 = autoRPMmode(temperature);
	}
	if  (fan3.State == AUTO){
		TCA0_SPLIT_HCMP0 = autoRPMmode(temperature);
	}
	if  (fan4.State == AUTO){
		TCA0_SPLIT_HCMP1 = autoRPMmode(temperature);
	}
}


#endif /* FANSTATES_H_ */


