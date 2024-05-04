/*
 * states.h
 *
 * Created: 17.04.2024 14:50:26
 *  Author: jarle
 */ 


#ifndef FANSTATES_H_
#define FANSTATES_H_
#include <stdint.h>
void printHomeScreen();
void fanOff();
void fanAuto();
void fanManual();
int autoRPMmode(uint16_t temperature);
void handleFansInAuto();




//saved states
extern enum mode SavedFan1State;
extern enum mode SavedFan2State;
extern enum mode SavedFan3State;
extern enum mode SavedFan4State;

extern uint8_t chosenModeIsManual;
extern uint8_t chosenFan;
extern int temperature;
extern int manual_rpm_value;
extern int auto_rpm_value;



#define FAN1_bm 0b00000001
#define FAN2_bm 0b00000010
#define FAN3_bm 0b00000100
#define FAN4_bm 0b00001000

#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 60
#define FAN_RPM_HIGH 79

     
enum mode { //definerer states for vifter
	MANUAL,
	OFF,
	AUTO
};

extern const char *state_names[] = { "MANUAL", "OFF", "AUTO" }; //Defines as names to print to Uart
	
	
typedef struct rpm {
	enum mode State;
	uint16_t rpm; 
	}Fan;
	

	extern Fan fan1 = {OFF, 0 };
	extern Fan fan2 = {OFF, 0 };
	extern Fan fan3 = {OFF, 0 };
	extern Fan fan4 = {OFF, 0 };





#endif // FANSTATES_H_ 


