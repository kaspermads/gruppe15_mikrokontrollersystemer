
/*
* GccApplication1.c
*
* Created: 31.01.2024 12:23:48
* Author : jarle
*/
#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) 
// PWM config
#define PERIOD_FREQUENCY 79 // 0x01A0
// Calculated based on duty cycle(%) = Ton/(Tperiod)*100%
#define DUTY_CYCLE_MIN_VALUE 0 // MIN value
#define DUTY_CYCLE_MAX_VALUE 79 // MAX value

// ADC config
#define RTC_PERIOD 511 // RTC Period


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h> // For ADC
#include "Usart.h"
#include "Menysystem.h"
#include "fanStates.h"
#include "TCn.h"


//USART
static int USART3_printChar(char c, FILE *stream);
void USART3_sendChar(char c);
void USART3_sendString(char *str);
char USART3_readChar(void);
void USART3_init(void);
void executeCommand(uint8_t command_number, char *command);
void read_commands();

//MenySystemBib
void printHomeScreen();
void PrintSelectFanMode();
void printOverview();
void printSelectFan();

//states
void fanOff();
void fanAuto();
void fanManual();
void handleFansInAuto();
int returnRpmInModeAuto(int temperature);
	
char command[MAX_COMMAND_LEN];
int temperature = 25; //test temperature


// Preprocessing
void PORT_init(void);
uint16_t pwm_to_rpm();
uint16_t pulseWidthReadings[100];
uint8_t pulseWidthIndex = 0;
uint16_t readPulseWidth;
int readFreq;
uint16_t rpm;



ISR(USART3_RXC_vect)
{
	read_commands();
}

ISR(TCB0_INT_vect)
{
	TCB0.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	pulseWidthReadings[pulseWidthIndex] = TCB0.CCMP;
	pulseWidthIndex++;
	if (pulseWidthIndex >= 100) {
		pulseWidthIndex = 0; // Reset index when all readings are stored
	}
	
	// DO SOMETHING, maybe read from TCB0.CCMP????
	//TCB0.CCMP // holds the compare, capture, and top value
	//readPulseWidth = TCB0.CCMP;
	
}

// Function to calculate the average of an array of uint16_t values
uint16_t average(uint16_t* array, uint8_t size) {
	uint32_t sum = 0;
	for (uint8_t i = 0; i < size; i++) {
		sum += array[i];
	}
	return (uint16_t)(sum / size);
}


uint16_t pwm_to_rpm()
{
	uint16_t pulseWidthAverage = average(pulseWidthReadings, 100);
	uint32_t rpm = ((F_CPU*60)/(4*pulseWidthAverage*2));
	
	return (uint16_t)rpm;
	
}


int main(void)
{
	
	USART3_init();
	sei(); // Enable Global Interrupts
	PORTB.DIRSET = PIN3_bm; //LED builtin = output
	
	// PWM initialize
	PORT_init();
	TCA0_init();
	TCB0_init();
	
	
	
	
	printHomeScreen(); 
	
	
	
	while(1){
		
		handleFansInAuto(); //changes the rpm of the fans in mode auto based on temperature
		_delay_ms(10); //whyyy? funker ikkje uten delay pga handleFansInAuto();
		int delayTime = fan1.rpm*10;
		 PORTB.OUTSET = PIN3_bm; // Turn on the LED
		 for (int i = 0; i < delayTime; i++) {
			 _delay_ms(1); // Delay for 1 millisecond
		 }
		 PORTB.OUTCLR = PIN3_bm; // Turn off the LED
		 for (int i = 0; i < delayTime; i++) {
			 _delay_ms(1); // Delay for 1 millisecond
		 }
	}
		
}

