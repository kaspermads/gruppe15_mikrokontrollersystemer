/*
 * states.h
 *
 * Created: 17.04.2024 14:50:26
 *  Author: jarle
 */ 


#ifndef STATES_H_
#define STATES_H_
#include "UsartTest.h"

int manualFlag;
int rpm_value;
int FAN1Flag;
int FAN2Flag;
int FAN3Flag;
int FAN4Flag;


enum mode { //definerer states for vifter
	RPM,
	OFF,
	AUTO
};

const char *state_names[] = { "RPM", "OFF", "AUTO" };
typedef struct rpm {
	enum mode State;
	uint8_t rpm; // RPM value (0-100)
	}Fan;
	

	Fan fan1 = {OFF, 0 };
	Fan fan2 = {OFF, 0 };
	Fan fan3 = {OFF, 0 };
	Fan fan4 = {OFF, 0 };



void fanOff(){
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

void fanAuto(){
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

void fanManual(command){
	if ((FAN1Flag = 1)) {
		fan1.State = RPM;
		fan1.rpm = rpm_value;
		printf("fan1 is now in mode: Manual \r\n");
		printHomeScreen();
	}
	if ((FAN2Flag == 1)) {
		fan2.State = RPM;
		fan2.rpm = rpm_value;
		printf("fan1 is now in mode: Manual");
		printHomeScreen();
	}
	if ((FAN3Flag == 1)) {
		fan3.State = RPM;
		fan3.rpm = rpm_value;
		printf("fan1 is now in mode: Manual");
		printHomeScreen();
	}
	if ((FAN4Flag == 1)) {
		fan4.State = RPM;
		fan4.rpm = rpm_value;
		printf("fan1 is now in mode: Manual");
		printHomeScreen();
	}
}


/*
void handleFan2(selectedMode){ //endre vifte 2
	
	if (strcmp(selectedMode, "RPM") == 0) {
		printf("Type wanted rpm \r\n");
		returnUart(command);
		fan.State = RPM;
		fan2.rpm = atoi(command);
		printf("fan2 RPM is set to: %d\r\n", atoi(command));
	}
	else if (strcmp(selectedMode, "OFF") == 0) {
		fan2.State = OFF;
		fan2.rpm = 0;
		printf("fan2 is now OFF\r\n");
	}
	else if (strcmp(selectedMode, "AUTO") == 0) {
		fan2.State = AUTO;
		printf("fan2 is now in mode AUTO\r\n");

	}
	
	else {
		;
	}
}
*/




#endif /* STATES_H_ */
