/*
 * MenysystemBib.h
 *
 * Created: 17.04.2024 12:56:51
 *  Author: jarle
 */ 


#ifndef MENYSYSTEM_H_
#define MENYSYSTEM_H_

#include <stdint.h>


void printHomeScreen();
void PrintSelectFanMode();
void printOverview();
void executeCommand(uint8_t command_number, char *command);
void printSelectFan();


//predeclaring TCB0/TCB1 calculating rpm value
//uint16_t pwm_to_rpm(uint16_t pulseWidthReadings);

// uint16_t pulseWidthReadings1[100];
// uint16_t pulseWidthReadings2[100];
uint16_t duty_cycle_input;

int temperature;

uint8_t diagnoseIsRunning;

uint8_t changeAlarmDeviation;



#endif /* MENYSYSTEM_H_ */