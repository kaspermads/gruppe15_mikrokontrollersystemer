/*
 * states.h
 *
 * Created: 17.04.2024 14:50:26
 *  Author: jarle
 */ 



#include "USART.h"
#include "fanStates.h"
#include "TCn.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>

     


void fanOff(){ //sets the chosen fan in mode OFF
	if (chosenFan & FAN1_bm){
		fan1.State = OFF;
		TCA0_SPLIT_LCMP0 = 0;
		printf("fan1 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFan & FAN2_bm) {
		fan2.State = OFF;
		TCA0_SPLIT_LCMP1 = 0;
		printf("fan2 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFan & FAN3_bm) {
		fan3.State = OFF;
		TCA0_SPLIT_HCMP0 = 0;
		printf("fan3 is now in mode: off");
		printHomeScreen();
	}
	else if (chosenFan & FAN4_bm) {
		fan4.State = OFF;
		TCA0_SPLIT_HCMP1 = 0;
		printf("fan4 is now in mode: off");
		printHomeScreen();
	}
}

void fanAuto(){ //sets the chosen fan in mode auto
	if (chosenFan & FAN1_bm){
		fan1.State = AUTO;
		printf("fan1 is now in mode: Auto");
		printHomeScreen();
	}
	else if (chosenFan & FAN2_bm) {
		fan2.State = AUTO;
		printf("fan2 is now in mode: Auto");
		printHomeScreen();
	}
	else if (chosenFan & FAN3_bm) {
		fan3.State = AUTO;
		printf("fan3 is now in mode: Auto");
		printHomeScreen();
	}
	else if (chosenFan & FAN4_bm) {
		fan4.State = AUTO;
		printf("fan4 is now in mode: Auto");
		printHomeScreen();
	}
}

void fanManual(){ //sets the chosen fan in manual mode based on chosen rpm value: low, medium or high
	if (chosenFan & FAN1_bm){
		fan1.State = MANUAL;
		TCA0_SPLIT_LCMP0 = manual_rpm_value;
		printf("fan1 is now in mode: Manual \r\n");
		
		printHomeScreen();
	}
	else if (chosenFan & FAN2_bm) {
		fan2.State = MANUAL;
		TCA0_SPLIT_LCMP1 = manual_rpm_value;
		printf("fan2 is now in mode: Manual");
		printHomeScreen();
	}
	else if (chosenFan & FAN3_bm) {
		fan3.State = MANUAL;
		TCA0_SPLIT_HCMP0 = manual_rpm_value;
		printf("fan3 is now in mode: Manual");
		printHomeScreen();
	}
	else if (chosenFan & FAN4_bm) {
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



void startFansAfterDiagnose(){
	
	//startup fan1
	if (SavedFan1State == OFF) {
		fan1.State = OFF;
		TCA0_SPLIT_LCMP0 = 0;
	} else if (SavedFan1State == AUTO){
		fan1.State = AUTO;
	} else if (SavedFan1State == MANUAL){
		fan1.State = MANUAL;
	}
	
	//startup fan2
	if (SavedFan2State == OFF) {
		fan2.State = OFF;
		TCA0_SPLIT_LCMP1 = 0;
		} else if (SavedFan2State == AUTO){
		fan2.State = AUTO;
		} else if (SavedFan2State == MANUAL){
		fan2.State = MANUAL;
	}
	
	//startup fan3
	if (SavedFan3State == OFF) {
		fan3.State = OFF;
		TCA0_SPLIT_HCMP0 = 0;
		} else if (SavedFan3State == AUTO){
		fan3.State = AUTO;
		} else if (SavedFan3State == MANUAL){
		fan3.State = MANUAL;
	}
	
	//startup fan4
	if (SavedFan4State == OFF) {
		fan4.State = OFF;
		TCA0_SPLIT_HCMP1 = 0;
		} else if (SavedFan4State == AUTO){
		fan4.State = AUTO;
		} else if (SavedFan4State == MANUAL){
		fan4.State = MANUAL;
	}
	
}


void saveFanModes(){
		SavedFan1State = fan1.State;
		SavedFan2State = fan2.State;
		SavedFan3State = fan3.State;
		SavedFan4State = fan4.State;
}






