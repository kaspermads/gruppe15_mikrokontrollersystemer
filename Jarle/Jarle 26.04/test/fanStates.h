/*
 * fanStates.h
 *
 * Created: 4/28/2024 10:20:35 AM
 *  Author: Kaspe
 */ 


#ifndef FANSTATES_H_
#define FANSTATES_H_

#define FAN1_bm 0b00000001
#define FAN2_bm 0b00000010
#define FAN3_bm 0b00000100
#define FAN4_bm 0b00001000
#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 60
#define FAN_RPM_HIGH 79

enum mode SavedFan1State;
enum mode SavedFan2State;
enum mode SavedFan3State;
enum mode SavedFan4State;

uint8_t chosenModeIsManual;
uint8_t chosenFan;

int manual_rpm_value;
int auto_rpm_value;
int temperature;

uint16_t pwm_to_rpm1();
uint16_t pwm_to_rpm2();
void printHomeScreen();


void fanOff();
void fanAuto();
void fanManual();
int autoRPMmode(uint16_t temperature);
void handleFansInAuto();
void startFansAfterDiagnose();
void saveFanModes();


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








#endif /* FANSTATES_H_ */