
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
int auto_rpm;



enum FanStatesEnum { // Define states for fans
	MANUAL,
	OFF,
	AUTO
	
};

 // Associate a state with an actual name
typedef struct{
	enum FanStatesEnum State;
	const char *name;
	
}FanState;

const FanState fanStates[] = {
	{MANUAL, "MANUAL"},
	{OFF, "OFF"},
	{AUTO, "AUTO"}
};
	
// Struct to store and read essential values from
typedef struct{
	enum FanStatesEnum State; 
	enum FanStatesEnum prevState;
	uint16_t rpm; // value to be calculated from tachometer 
	uint8_t *setSpeed; // Points to memory register of the respective compare channels
	const char *name;  //Points to the names of fans in the fan array
	uint16_t saved_rpm; //value to be stored for restart of fans after diagnose
	}Fan;
	
	
	// Declaring the fans
	Fan fan[NUM_FANS] = {
		{OFF, OFF, 0, NULL, "unused",0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_LCMP0, "FAN1", 0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_LCMP1, "FAN2", 0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_HCMP0, "FAN3", 0},
		{OFF, OFF, 0,(uint8_t*)&TCA0_SPLIT_HCMP1, "FAN4", 0}
		
	};




/**
 * @brief: Sets chosen fan to mode OFF and turns off the fan. ChosenFan is given as an UART command
 */
void setFanToOff() {
	//for (int i = 1; i < NUM_FANS; i++) {
		if (chosenFan >= 1 && chosenFan < NUM_FANS) {
			fan[chosenFan].State = OFF;
			fan[chosenFan].prevState = fan[chosenFan].State;
			*(fan[chosenFan].setSpeed) = 0; // Set compare register to 0
			fan[chosenFan].saved_rpm = manual_rpm_value;

			printf("%s is now in mode: off\n", fan[chosenFan].name);
			 // Exit loop after setting the fan state
		}
	
	printHomeScreen();
}


/**
 * @brief: Set chosen fan to mode AUTO. ChosenFan is given as an UART command
	Adjusting the rpm is handled in AutoRpmMode
 */
void setFanToAuto() {
	//for (int i = 1; i < NUM_FANS; i++) {
	if(chosenFan >= 1 && chosenFan < NUM_FANS) {
		//if (chosenFan == i) {
			fan[chosenFan].State = AUTO;
			fan[chosenFan].prevState = fan[chosenFan].State;
		}
	
	printHomeScreen();
}


/**
 * @brief: Set chosen fan to mode MANUAL. ChosenFan is given as an UART command
	Sets the rpm value of the chosen fan(0-100) based on manual_rpm_value given by UART
	Saves the rpm value to be used for restart of the fans after diagnose

 */
void setFanToManual() {
	if (chosenFan >= 1 && chosenFan < NUM_FANS){

			fan[chosenFan].State = MANUAL;
			fan[chosenFan].prevState = fan[chosenFan].State;
			
			*(fan[chosenFan].setSpeed) = manual_rpm_value; // Set compare register to manual RPM value
			fan[chosenFan].saved_rpm = manual_rpm_value;

		
			
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
	
}



/**
 * @brief: Changes the rpm value of the fans based on return value of autoRPMmode(temperature)

 */
void handleFansInAuto() {
	for (int i = 1; i < NUM_FANS; i++) {
		if (fan[i].State == AUTO) {
			
			auto_rpm = autoRPMmode(temperature);

			*(fan[i].setSpeed) = auto_rpm;
			fan[i].saved_rpm = auto_rpm;
		}
	}
}


/**
 * @brief  Resumes the fans to their original stats from before diagnosis began by
 *			looping through the fans and checking the previous stats.

 */
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


