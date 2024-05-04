/*
 * states.h
 *
 * Created: 17.04.2024 14:50:26
 *  Author: jarle
 */ 


#ifndef FANSTATES_H_
#define FANSTATES_H_
#include "Usart.h"
#include "pwm_to_rpm.h"
#define MAX_VALUE
#define NUM_FANS 5 //0-4 fans. 0 is not used

#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 60
#define FAN_RPM_HIGH 79

void printHomeScreen();

int temperature;
//saved states
int savedFanStates[NUM_FANS];

uint8_t chosenModeIsManual;
uint8_t chosenFan;

int manual_rpm_value;
int auto_rpm_value;



	
enum FanStatesEnum { //definerer states for vifter
	MANUAL,
	OFF,
	AUTO
	
};


typedef struct{
	enum FanStatesEnum State;
	const char *name;
	
	}FanState;
	
const FanState fanStates[] = {
	{MANUAL, "MANUAL"},
	{OFF, "OFF"},
	{AUTO, "AUTO"}
};
//const char *state_names[] = { "MANUAL", "OFF", "AUTO" }; //Defines as names to print to Uart
	

// struct to save read rpm value
typedef struct{
	enum FanStatesEnum State; 
	uint16_t rpm;
	uint16_t *compare_register;
	const char *name;
	uint16_t saved_rpm;
	}Fan;
	
	
	
	Fan fan[NUM_FANS] = {
		{OFF, 0, 0, "unused"},
		{OFF, 0, &TCA0_SPLIT_LCMP0, "fan1"},
		{OFF, 0, &TCA0_SPLIT_LCMP1, "fan2"}, 
		{OFF, 0, &TCA0_SPLIT_HCMP0, "fan3"}, 
		{OFF, 0, &TCA0_SPLIT_HCMP1, "fan4"},
			
	};


void setFanToOff() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan == i) {
			fan[i].State = OFF;
			*(fan[i].compare_register) = 0; // Set compare register to 0
			printf("%s is now in mode: off\n", fan[i].name);
			break; // Exit loop after setting the fan state
		}
	}
	printHomeScreen();
}

void setFanToAuto() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan == i) {
			fan[i].State = AUTO;
			break;
		}
	}
	printHomeScreen();
}


void setFanToManual() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan == i) {
			fan[i].State = MANUAL;
			*(fan[i].compare_register) = manual_rpm_value; // Set compare register to manual RPM value
			fan[i].saved_rpm = manual_rpm_value;
			printf("%s is now in mode: Manual\n", fan[i].name);
			break; // Exit loop after setting the fan state
		}
	}
	printHomeScreen();
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


void handleFansInAuto() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (fan[i].State == AUTO) {
			*(fan[i].compare_register) = autoRPMmode(temperature);
		}
	}
}



void startFansAfterDiagnose() {
	for (int i = 1; i < NUM_FANS; i++) { // Start from 1 as fan[0] is unused
		Fan *currentFan = &fan[i];
		int savedState = savedFanStates[i];

		currentFan->State = savedState;

		if (savedState == MANUAL) {
			*(currentFan->compare_register) = currentFan->saved_rpm;
			} else {
			*(currentFan->compare_register) = 0;
		}
	}
}

void saveFanModes(){
		for (int i = 1; i < NUM_FANS; i++)
		{
			savedFanStates[i] = fan[i].State;
		}
}




#endif /* FANSTATES_H_ */


