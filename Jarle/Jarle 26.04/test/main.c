
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
#include "MenySystem.h"
#include "fanStates.h"
#include "TCn.h"
#include "I2C_temperature.h"


// Preprocessing
uint16_t pwm_to_rpm1();
uint16_t pwm_to_rpm2();

uint16_t pulseWidthReadings1[100];
uint16_t pulseWidthReadings2[100];


uint8_t pulseWidthIndex1 = 0;
uint8_t pulseWidthIndex2 = 0;


uint16_t rpm1;
uint16_t rpm2;



ISR(USART3_RXC_vect)
{
	read_commands();
}

ISR(TCB0_INT_vect)// starting interrupt for reading pwm from fan1
{
	TCB0.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	pulseWidthReadings1[pulseWidthIndex1] = TCB0.CCMP;
	pulseWidthIndex1++;
	if (pulseWidthIndex1 >= 100) {
		pulseWidthIndex1 = 0; // Reset index when all readings are stored
	}
	
	// DO SOMETHING, maybe read from TCB0.CCMP????
	//TCB0.CCMP // holds the compare, capture, and top value
	//readPulseWidth = TCB0.CCMP;
	
}


ISR(TCB1_INT_vect)// starting interrupt for reading pwm from fan1
{
	TCB1.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	pulseWidthReadings2[pulseWidthIndex2] = TCB1.CCMP;
	pulseWidthIndex2++;
	if (pulseWidthIndex2 >= 100) {
		pulseWidthIndex2 = 0; // Reset index when all readings are stored
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


uint16_t pwm_to_rpm1()
{
	uint16_t pulseWidthAverage1 = average(pulseWidthReadings1, 100);
	uint32_t rpm1 = ((F_CPU*60)/(4*pulseWidthAverage1*2));
	return (uint16_t)rpm1;
	
}


uint16_t pwm_to_rpm2()
{
	uint16_t pulseWidthAverage2 = average(pulseWidthReadings2, 100);
	uint32_t rpm2 = ((F_CPU*60)/(4*pulseWidthAverage2*2));
	return (uint16_t)rpm2;
	
}



int main(void)
{
	
	USART3_init();
	sei(); // Enable Global Interrupts

	// PWM initialize
	PORT_init();
	TCA0_init();
	TCB0_init();
	TCB1_init();
		
	// I2C initialize 

	TWI0_M_init();
	AHT10_init();
	
	
	
	printHomeScreen(); 
	
		//TCA0_SPLIT_LCMP0 = 60;

	
	while(1){
		
		handleFansInAuto(); //changes the rpm of the fans in mode auto based on temperature
		_delay_ms(10); //få på en delay i handlefans in auto så sjekker om 100ms er gått før den sjekker og evt endrer rpm 
	
		
		//printf("Temperature: °C%d\n", AHT10_read());
		
		
	}
		
}

