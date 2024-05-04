
#ifndef FANSTATES_H_
#define FANSTATES_H_
#include "Usart.h"
#include "pwm_to_rpm.h"
#define MAX_VALUE
#define NUM_FANS 5




int temperature;
int savedFanStates[NUM_FANS]; //saved fanStates before diagnose

uint8_t chosenModeIsManual; 
uint8_t chosenFan;

int manual_rpm_value; //rpm value written to 
int auto_rpm_value;


#define NUM_FANS 5 //0-4 fans. 0 is not used

#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 60
#define FAN_RPM_HIGH 79


// Defines fanStates 
enum mode { 
	MANUAL,
	OFF,
	AUTO
};

const char *state_names[] = { "MANUAL", "OFF", "AUTO" }; //Defines as names for better understanding 
	
	
// Struct to store and read rpm values for the different fans
typedef struct rpm {
	enum mode State; 
	uint16_t rpm; //value to be read from the tachometer 
	uint16_t saved_rpm; //value to be stored for restart of fans after diagnose
	}Fan;
	
	
	// Declaring the fans
	Fan fan[NUM_FANS] = {
		{OFF, 0}, //unused
		{OFF, 0}, // fan[1]
		{OFF, 0}, // fan[2]
		{OFF, 0}, // fan[3]
		{OFF, 0}  // fan[4]
			
	};




/**
 * @brief: Sets chosen fan to mode OFF and turns off the fan. ChosenFan is given as an UART command
 * 
 * 
 * @return void
 */
void setFanToOff() {
	for (int i = 1; i < NUM_FANS; i++) { //Sets the choosed fan to OFF
		if (chosenFan == i) {
			fan[i].State = OFF;
			switch (i) { 
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
				break;
			}
		}
	}
	printHomeScreen();
}


/**
 * @brief: Set chosen fan to mode AUTO. ChosenFan is given as an UART command
	Adjusting the rpm is handled in main.c
 * 
 * 
 * @return void
 */
void setFanToAuto() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan == i) {
			fan[i].State = AUTO;
			break;
		}
	}
	printHomeScreen();
}


/**
 * @brief: Set chosen fan to mode MANUAL. ChosenFan is given as an UART command
	Sets the rpm value of the chosen fan(0-100) based on manual_rpm_value given by UART
	Saves the rpm value to be used for restart of the fans after diagnose
 * 
 * 
 * @return void
 */
void setFanToManual() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan == i) {
			fan[i].State = MANUAL;
			switch (i) { // argument is chosenFan 
				case 1:
				TCA0_SPLIT_LCMP0 = manual_rpm_value; //sets the rpm value of the fan
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


/**
 * @brief: 
 * 
 * @param temperature
 * 
 * @return: Returns calculated rpm value adjustes by temperature 
 */
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



/**
 * @brief: Changes the rpm value of the fans based on return value of autoRPMmode(temperature)
 * 
 * 
 * @return void
 */
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
	if (savedFanStates[1] == OFF) {
		fan[1].State = OFF;
		TCA0_SPLIT_LCMP0 = 0;
	} else if (savedFanStates[1] == AUTO){
		fan[1].State = AUTO;
	} else if (savedFanStates[1] == MANUAL){
		fan[1].State = MANUAL;
		TCA0_SPLIT_LCMP0 = fan[1].saved_rpm;
	}
	
	//startup fan2
	if (savedFanStates[2] == OFF) {
		fan[2].State = OFF;
		TCA0_SPLIT_LCMP1 = 0;
		} else if (savedFanStates[2] == AUTO){
		fan[2].State = AUTO;
		} else if (savedFanStates[2] == MANUAL){
		fan[2].State = MANUAL;
		TCA0_SPLIT_LCMP1 = fan[2].saved_rpm;
	}
	
	//startup fan3
	if (savedFanStates[3] == OFF) {
		fan[3].State = OFF;
		TCA0_SPLIT_HCMP0 = 0;
		} else if (savedFanStates[3] == AUTO){
		fan[3].State = AUTO;
		} else if (savedFanStates[3] == MANUAL){
		fan[3].State = MANUAL;
		TCA0_SPLIT_HCMP0 = fan[3].saved_rpm;
	}
	
	//startup fan4
	if (savedFanStates[4] == OFF) {
		fan[4].State = OFF;
		TCA0_SPLIT_HCMP1 = 0;
		} else if (savedFanStates[4] == AUTO){
		fan[4].State = AUTO;
		} else if (savedFanStates[4] == MANUAL){
		fan[4].State = MANUAL;
		TCA0_SPLIT_HCMP1 = fan[4].saved_rpm;
	}
	
}


/**
 * @brief: Iterates through the different fans and saves the fanStates in an array based on the current fanStates 
 * 
 * 
 * @return void
 */
void saveFanModes(){
		for (int i = 1; i < NUM_FANS; i++) {
			savedFanStates[i] = fan[i].State;
		}
}




#endif /* FANSTATES_H_ */


