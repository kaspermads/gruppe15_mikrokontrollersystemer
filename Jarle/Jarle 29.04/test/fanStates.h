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


uint8_t chosenModeIsManual;
uint8_t chosenFan;

int manual_rpm_value;
int auto_rpm_value;



#define FAN1_bm 0b00000001
#define FAN2_bm 0b00000010
#define FAN3_bm 0b00000100
#define FAN4_bm 0b00001000

#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 65
#define FAN_RPM_HIGH 79
#define PWM_PERIOD 79

     
enum mode { //definerer states for vifter
	MANUAL,
	OFF,
	AUTO
};

enum  mode SavedFanStates[4];


const char *state_names[] = { "MANUAL", "OFF", "AUTO" }; //Defines as names to print to Uart
	
	
typedef struct rpm {
	enum mode State;
	uint16_t rpm; 
	}Fan;
	
Fan fans[4] = {
	{OFF, 0},
	{OFF, 0},
	{OFF, 0},
	{OFF, 0}
};


void fanOff(){ //sets the chosen fan in mode OFF
	if (chosenFan & FAN1_bm){
		fans[0].State = OFF;
		TCA0_SPLIT_LCMP0 = 0;
		printf("fan1 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFan & FAN2_bm) {
		fans[1].State = OFF;
		TCA0_SPLIT_LCMP1 = 0;
		printf("fan2 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFan & FAN3_bm) {
		fans[2].State = OFF;
		TCA0_SPLIT_HCMP0 = 0;
		printf("fan3 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFan & FAN4_bm) {
		fans[3].State = OFF;
		TCA0_SPLIT_HCMP1 = 0;
		printf("fan4 is now in mode: off");
		printHomeScreen();
	}
}

void fanAuto(uint8_t fanIndex){ //sets the chosen fan in mode auto

	fans[fanIndex].State = AUTO;
	printf("fan%d is now in mode: Auto", fanIndex + 1);
	printHomeScreen();
}

void fanManual(){ //sets the chosen fan in manual mode based on chosen rpm value: low, medium or high
	if (chosenFan & FAN1_bm){
		fans[0].State = MANUAL;
		TCA0_SPLIT_LCMP0 = manual_rpm_value;
		printf("fan1 is now in mode: Manual \r\n");
		
		printHomeScreen();
	}
	else if (chosenFan & FAN2_bm) {
		fans[1].State = MANUAL;
		TCA0_SPLIT_LCMP1 = manual_rpm_value;
		printf("fan2 is now in mode: Manual");
		printHomeScreen();
	}
	else if (chosenFan & FAN3_bm) {
		fans[2].State = MANUAL;
		TCA0_SPLIT_HCMP0 = manual_rpm_value;
		printf("fan3 is now in mode: Manual");
		printHomeScreen();
	}
	else if (chosenFan & FAN4_bm) {
		fans[3].State = MANUAL;
		TCA0_SPLIT_HCMP1 = manual_rpm_value;
		printf("fan4 is now in mode: Manual");
		printHomeScreen();
	}
}



int autoRPMmode(uint16_t temperature) {
	
	int a = (47/40.0);  // Slope of the line (Change in RPM per degree Celsius)
	int b = -15; // Y-intercept of the line (RPM at 0 degrees Celsius)
	
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

	for (uint8_t i = 0; i < 4; i++) {
		if (fans[i].State == AUTO) {
			switch (i) {
				case 0:
				TCA0_SPLIT_LCMP0 = autoRPMmode(temperature);
				break;
				case 1:
				TCA0_SPLIT_LCMP1 = autoRPMmode(temperature);
				break;
				case 2:
				TCA0_SPLIT_HCMP0 = autoRPMmode(temperature);
				break;
				case 3:
				TCA0_SPLIT_HCMP1 = autoRPMmode(temperature);
				break;
				default:
				break;
			}
		}
	}
}



void startFansAfterDiagnose(){
	
// 	startup fan1
// 		if (SavedFan1State == OFF) {
// 			fan1.State = OFF;
// 			TCA0_SPLIT_LCMP0 = 0;
// 		} else if (SavedFan1State == AUTO){
// 			fan1.State = AUTO;
// 		} else if (SavedFan1State == MANUAL){
// 			fan1.State = MANUAL;
// 		}
// 		
// 		//startup fan2
// 		if (SavedFan2State == OFF) {
// 			fan2.State = OFF;
// 			TCA0_SPLIT_LCMP1 = 0;
// 			} else if (SavedFan2State == AUTO){
// 			fan2.State = AUTO;
// 			} else if (SavedFan2State == MANUAL){
// 			fan2.State = MANUAL;
// 		}
// 		
// 		//startup fan3
// 		if (SavedFan3State == OFF) {
// 			fan3.State = OFF;
// 			TCA0_SPLIT_HCMP0 = 0;
// 			} else if (SavedFan3State == AUTO){
// 			fan3.State = AUTO;
// 			} else if (SavedFan3State == MANUAL){
// 			fan3.State = MANUAL;
// 		}
// 		
// 		//startup fan4
// 		if (SavedFan4State == OFF) {
// 			fan4.State = OFF;
// 			TCA0_SPLIT_HCMP1 = 0;
// 			} else if (SavedFan4State == AUTO){
// 			fan4.State = AUTO;
// 			} else if (SavedFan4State == MANUAL){
// 			fan4.State = MANUAL;
// 		}
// 		
	for (uint8_t i = 0; i < 4; i++) {
		switch (SavedFanStates[i]) {
			case OFF:
			fans[i].State = OFF;
			break;
			case AUTO:
			fans[i].State = AUTO;
			break;
			case MANUAL:
			fans[i].State = MANUAL;
			break;
			default:
			break;
		}
	}
}


void saveFanModes(){
	
// 		SavedFan1State = fan1.State;
// 		SavedFan2State = fan2.State;
// 		SavedFan3State = fan3.State;
// 		SavedFan4State = fan4.State;
// 
	 for (uint8_t i = 0; i < 4; i++) {
		 SavedFanStates[i] = fans[i].State;
	 }
}




#endif /* FANSTATES_H_ */


