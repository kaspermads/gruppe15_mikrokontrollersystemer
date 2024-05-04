
#ifndef FANSTATES_H_
#define FANSTATES_H_
#include "Usart.h"
#include "pwm_to_rpm.h"
#define NUM_FANS 5 //0-4 fans. 0 is not used

#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 60
#define FAN_RPM_HIGH 79



int temperature;
//int savedFanStates[NUM_FANS]; //saved fanStates before diagnose

uint8_t chosenModeIsManual; 
uint8_t chosenFan;

int manual_rpm_value; //rpm value written to 
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
	
// Struct to store and read rpm values for the different fans
typedef struct{
	enum FanStatesEnum State; 
	enum FanStatesEnum prevState;
	uint16_t rpm; //value to be read from the tachometer 
	uint8_t *setSpeed;
	const char *name; 
	uint16_t saved_rpm; //value to be stored for restart of fans after diagnose
	}Fan;
	
	
	// Declaring the fans
	Fan fan[NUM_FANS] = {
		{OFF, OFF, 0, NULL, "unused",0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_LCMP0, "fan1", 0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_LCMP1, "fan2", 0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_HCMP0, "fan3", 0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_HCMP1, "fan4", 0}
		
	};




/**
 * @brief: Sets chosen fan to mode OFF and turns off the fan. ChosenFan is given as an UART command
 * 
 * 
 * @return void
 */
void setFanToOff() {
	//for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan >= 1 && chosenFan < NUM_FANS) {
			fan[chosenFan].State = OFF;
			*(fan[chosenFan].setSpeed) = 0; // Set compare register to 0
			printf("%s is now in mode: off\n", fan[chosenFan].name);
			 // Exit loop after setting the fan state
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
	//for (int i = 1; i < NUM_FANS; i++) {
	if(chosenFan >= 1 && chosenFan < NUM_FANS) {
		//if (chosenFan == i) {
			fan[chosenFan].State = AUTO;
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
	if (chosenFan >= 1 && chosenFan < NUM_FANS){

			fan[chosenFan].State = MANUAL;
			fan[chosenFan].prevState = fan[chosenFan].State;
			
			*(fan[chosenFan].setSpeed) = manual_rpm_value; // Set compare register to manual RPM value
			fan[chosenFan].saved_rpm = manual_rpm_value;

		
			
		}
	chosenFan = 0;
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
void handleFansInAuto() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (fan[i].State == AUTO) {
			*(fan[i].setSpeed) = autoRPMmode(temperature);
		}
	}
}


void startFansAfterDiagnose() {
	for (int i = 1; i < NUM_FANS; i++) {
		fan[i].State = fan[i].prevState;
		
		if (fan[i].prevState == OFF) {
			fan[i].State = OFF;
			*(fan[i].setSpeed) = 0;
			
			} else if (fan[i].prevState == MANUAL) {
			fan[i].State = MANUAL;
			*(fan[i].setSpeed) = fan[i].saved_rpm;
			

			} else if (fan[i].prevState == AUTO) {
			fan[i].State = AUTO;
			// No action needed for AUTO mode here
		}
	}
}


#endif /* FANSTATES_H_ */


