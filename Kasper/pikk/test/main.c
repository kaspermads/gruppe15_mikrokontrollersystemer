
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
#include "Usart.h"
#include "Menysystem.h"
#include "fanStates.h"
#include "TCn.h"
#include "I2C_temperature.h"
#include "Error_Prediction.h"
#include "save_to_eeprom.h"
#include "pwm_to_rpm.h"



uint8_t diagnoseIsRunning;





void RTC_init(void)
{
	// Enables the source clock for the RTC counter
	RTC.CTRLA = RTC_RTCEN_bm | RTC_PRESCALER_DIV1_gc; // Enable RTC, select presaler
	//RTC.INTCTRL = RTC_OVF_bm;
	RTC.CLKSEL = RTC_CLKSEL_OSC1K_gc; // Select source for the RTC clock
	
	// Enables the PIT (Periodic Interrupt Timer)
	RTC.PITCTRLA = RTC_PITEN_bm | RTC_PERIOD_CYC1024_gc; // Enable PIT and select number of clock cycles between interrupt
	RTC.PITINTCTRL = RTC_PI_bm;	// Enable interrupt
}

// RTC interrupt
ISR(RTC_PIT_vect)
{
	counter ++; // Counter represent seconds
	RTC.PITINTFLAGS = RTC_PI_bm; // Clears the interrupt flag
}


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
	
	// RTC initialize
	RTC_init();
	
	// Get values for prediction from eeprom
	status_eeprom();
	read_value_EE_fan1 = eeprom_read_word((uint16_t*) address_fan1);
	status_eeprom();
	read_value_EE_fan2 = eeprom_read_word((uint16_t*) address_fan2);
	
	printHomeScreen();
	
	//TCA0_SPLIT_LCMP0 = 60;

	
// 	while(1){
// 		
// 		
// 		if(counter == 2000)
// 		{
// 			saveFanModes();
// 			diagnoseIsRunning = 1;
// 			predict_error();
// 			startFansAfterDiagnose();
// 			diagnoseIsRunning = 0;
// 			counter = 0;
// 		}
// 		
// 		
// 		
// 		handleFansInAuto(); //changes the rpm of the fans in mode auto based on temperature
// 		_delay_ms(10); //Usart er facked uten denne
// 		
// 		
// 		
// 		
// 	}
	
}