/*
 * RTC_start.c
 *
 * Created: 24.04.2024 15:38:27
 * Author : larsi
 */ 

#define F_CPU 4000000UL	// Define CPU-frequency

// Including libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC
#include <avr/interrupt.h>
//#include <avr/eeprom.h>

#include "USARTn.h"

void RTC_init(void);

volatile uint16_t counter;

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

ISR(RTC_PIT_vect)
{
	
	counter ++;
	if(counter == 60)
	{
		counter = 0;
	}
	RTC.PITINTFLAGS = RTC_PI_bm; // Clears the interrupt flag
}


int main(void)
{
	
	file_stream();  // Create file stream for USART
	USART3_init();	// USART3 initialize
	
	RTC_init();
	
	sei(); // Enable Global Interrupts
    /* Replace with your application code */
    while (1) 
    {
		
		printf("Teller: %d\r\n", counter);
		printf("\r\n");
		printf("\r\n");
		//_delay_ms(500);
    }
}

