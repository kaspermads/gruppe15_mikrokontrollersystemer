/*
 * fanStates.h
 *
 * Created: 4/25/2024 3:16:32 PM
 *  Author: Kaspe
 */ 


#ifndef FANSTATES_H_
#define FANSTATES_H_

uint8_t chosenModeIsManual;
uint8_t chosenFan;

int manual_rpm_value;
int auto_rpm_value;
int temperature;


#define FAN1_bm 0b00000001
#define FAN2_bm 0b00000010
#define FAN3_bm 0b00000100
#define FAN4_bm 0b00001000

#define FAN_RPM_LOW 35
#define FAN_RPM_MEDIUM 60
#define FAN_RPM_HIGH 79


void handleFansInAuto();
int autoRPMmode(uint16_t temperature);
void fanManual();
void fanAuto();
void fanOff();
void printHomeScreen();
uint16_t pwm_to_rpm();






#endif /* FANSTATES_H_ */