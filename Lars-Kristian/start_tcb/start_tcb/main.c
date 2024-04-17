

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC

#define F_CPU 4000000UL	// Define CPU-frequency

// PWM config
#define PERIOD_FREQUENCY 20 // 0x01A0
// Calculated based on duty cycle(%) = Ton/(Tperiod)*100%
#define DUTY_CYCLE_MIN_VALUE 0 // MIN value
#define DUTY_CYCLE_MAX_VALUE 20 // MAX value


// Preprocessing

void TCB0_init(void);

int readPulseWidth;
int readFreq;

void TCB0_init(void)
{
	TCB0.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_TCA0_gc;	// enable timer/counter TCB0 and use clock from TCA0
	TCB0.CTRLB = TCB_CNTMODE_FRQPW_gc;	// set timer mod to input capture frequency and PWM measurement mode
	
	// TCB0.EVCTRL = TCB_CAPTEI_bm; // Capture event input enable
	
	EVSYS.CHANNEL0 = EVSYS_CHANNEL0_PORTB_PIN0_gc; //	input event channel 0, port b on pin 0
	EVSYS.USERTCB0CAPT= EVSYS_USER_CHANNEL0_gc; // connects channel 0(portb pin2????) specific to TCB0 input capture mode
	
	TCB0.INTCTRL = TCB_CAPT_bm; // interrupt enabled for capture
	
	
	
	
}

ISR(TCB0_INT_vect)
{
	TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
	
	// DO SOMETHING, maybe read from TCB0.CCMP????
	//TCB0.CCMP // holds the compare, capture, and top value
	readPulseWidth = TCB0.CCMP;
	
}



int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

