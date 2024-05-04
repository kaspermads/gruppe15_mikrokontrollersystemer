/*
 * states.h
 *
 * Created: 17.04.2024 14:50:26
 *  Author: jarle
 */ 


#ifndef FANSTATES_H_
#define FANSTATES_H_
#include "Usart.h"
#define MAX_VALUE
void printHomeScreen();

int temperature;

uint16_t pwm_to_rpm1();
uint16_t pwm_to_rpm2();

//saved states
enum mode SavedFan1State;
enum mode SavedFan2State;
enum mode SavedFan3State;
enum mode SavedFan4State;

uint8_t chosenModeIsManual;
uint8_t chosenFan;

int manual_rpm_value;
int auto_rpm_value;


#define NUM_FANS 5 //0-4 fans. 0 is not used

#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 60
#define FAN_RPM_HIGH 79

	
enum mode { //definerer states for vifter
	MANUAL,
	OFF,
	AUTO
};

const char *state_names[] = { "MANUAL", "OFF", "AUTO" }; //Defines as names to print to Uart
	

// struct to save read rpm value
typedef struct rpm {
	enum mode State; 
	uint16_t rpm;
	uint16_t saved_rpm;
	}Fan;
	
	
	
	Fan fan[NUM_FANS] = {
		{OFF, 0}, //unused
		{OFF, 0}, // fan[1]
		{OFF, 0}, // fan[2]
		{OFF, 0}, // fan[3]
		{OFF, 0}  // fan[4]
			
	};




void setFanToOff() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan == i) {
			fan[i].State = OFF;
			switch (i + 1) { // Adjust the fan number for printing
				case 1:
				TCA0_SPLIT_LCMP0 = 0;
				printf("fan1 is now in mode: off\n");
				break;
				case 2:
				TCA0_SPLIT_LCMP1 = 0;
				printf("fan2 is now in mode: off\n");
				break;
				case 3:
				TCA0_SPLIT_HCMP0 = 0;
				printf("fan3 is now in mode: off\n");
				break;
				case 4:
				TCA0_SPLIT_HCMP1 = 0;
				printf("fan4 is now in mode: off\n");
				break;
				default:
				// Handle invalid fan index
				break;
			}
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
			switch (i) { // Adjust the fan number for printing
				case 1:
				TCA0_SPLIT_LCMP0 = manual_rpm_value;
				fan[1].saved_rpm = manual_rpm_value;
				printf("fan1 is now in mode: Manual \n");
				break;
				case 2:
				TCA0_SPLIT_LCMP1 = manual_rpm_value;
				fan[2].saved_rpm = manual_rpm_value;
				printf("fan2 is now in mode: Manual\n");
				break;
				case 3:
				TCA0_SPLIT_HCMP0 = manual_rpm_value;
				fan[3].saved_rpm = manual_rpm_value;
				printf("fan3 is now in mode: Manual\n");
				break;
				case 4:
				TCA0_SPLIT_HCMP1 = manual_rpm_value;
				fan[4].saved_rpm = manual_rpm_value;
				printf("fan4 is now in mode: Manual\n");
				break;
				default:
				// Handle invalid fan index
				break;
			}
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


void handleFansInAuto(){ //change rpm based on temperature if fan.State is Auto
	if (fan[1].State == AUTO){
		TCA0_SPLIT_LCMP0 = autoRPMmode(temperature);
	}
	
	if  (fan[2].State == AUTO){
		TCA0_SPLIT_LCMP1 = autoRPMmode(temperature);
	}
	if  (fan[3].State == AUTO){
		TCA0_SPLIT_HCMP0 = autoRPMmode(temperature);
	}
	if  (fan[4].State == AUTO){
		TCA0_SPLIT_HCMP1 = autoRPMmode(temperature);
	} 
	
}



void startFansAfterDiagnose(){
	
	//startup fan1
	if (SavedFan1State == OFF) {
		fan[1].State = OFF;
		TCA0_SPLIT_LCMP0 = 0;
	} else if (SavedFan1State == AUTO){
		fan[1].State = AUTO;
	} else if (SavedFan1State == MANUAL){
		fan[1].State = MANUAL;
		TCA0_SPLIT_LCMP0 = fan[1].saved_rpm;
	}
	
	//startup fan2
	if (SavedFan2State == OFF) {
		fan[2].State = OFF;
		TCA0_SPLIT_LCMP1 = 0;
		} else if (SavedFan2State == AUTO){
		fan[2].State = AUTO;
		} else if (SavedFan2State == MANUAL){
		fan[2].State = MANUAL;
		TCA0_SPLIT_LCMP1 = fan[2].saved_rpm;
	}
	
	//startup fan3
	if (SavedFan3State == OFF) {
		fan[3].State = OFF;
		TCA0_SPLIT_HCMP0 = 0;
		} else if (SavedFan3State == AUTO){
		fan[3].State = AUTO;
		} else if (SavedFan3State == MANUAL){
		fan[3].State = MANUAL;
		TCA0_SPLIT_HCMP0 = fan[3].saved_rpm;
	}
	
	//startup fan4
	if (SavedFan4State == OFF) {
		fan[4].State = OFF;
		TCA0_SPLIT_HCMP1 = 0;
		} else if (SavedFan4State == AUTO){
		fan[4].State = AUTO;
		} else if (SavedFan4State == MANUAL){
		fan[4].State = MANUAL;
		TCA0_SPLIT_HCMP1 = fan[4].saved_rpm;
	}
	
}


void saveFanModes(){
		SavedFan1State = fan[1].State;
		SavedFan2State = fan[2].State;
		SavedFan3State = fan[3].State;
		SavedFan4State = fan[4].State;
}




#endif /* FANSTATES_H_ */


