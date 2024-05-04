/*
 * MenySystem.h
 *
 * Created: 4/28/2024 10:29:46 AM
 *  Author: Kaspe
 */ 


#ifndef MENYSYSTEM_H_
#define MENYSYSTEM_H_


#include <avr/io.h>

//predeclaring TCB0/TCB1 calculating rpm value
uint16_t pwm_to_rpm1();
uint16_t pwm_to_rpm2();

uint16_t pulseWidthReadings1[100];
uint16_t pulseWidthReadings2[100];
uint16_t duty_cycle_input;

int temperature;

void printHomeScreen();
void PrintSelectFanMode();
void printOverview();
void printSelectFan();
void executeCommand(uint8_t command_number, char *command);








#endif /* MENYSYSTEM_H_ */