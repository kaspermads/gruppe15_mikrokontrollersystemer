
#define F_CPU 4000000UL	// Define CPU-frequency

// Including libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC
#include <avr/interrupt.h>
#include "TCn.h" // Include own library with TCA and TCB functions
#include "ADCn.h" // Include own library with ADC
#include "USARTn1.h"

// Preprocessing
void PORT_init(void);

// Variables
uint16_t readPulseWidth;
uint16_t readFreq;

// TCB0 interrupt function
ISR(TCB0_INT_vect)
{
	TCB0.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	
	// DO SOMETHING, maybe read from TCB0.CCMP????
	//TCB0.CCMP // holds the compare, capture, and top value
	readPulseWidth = TCB0.CCMP;
	
}

int main(void)
{
	file_stream();  // Create file stream for USART
	USART3_init();	// USART3 initialize
	
	// PWM initialize
	TCA0_init();
	TCB0_init();
	
	// ADC initialize
	ADC0_init();
	ADC0_start();
	
	sei(); // Enable Global Interrupts
	
	while (1)
	{
		if (ADC0_conersionDone())
		{
			// Read potentiometer
			uint16_t adc_value = ADC0_read();
			// Convert ADC-value to PWM signal
			uint16_t fanSpeed = ((40.0/1023.0)*adc_value);
			printf("ADC Value: %d\r\n", adc_value);
			printf("Fan speed: %d\r\n", fanSpeed);
			printf("PWM read: %d\r\n", readPulseWidth);
			printf("\r\n");
			printf("\r\n");
			//TCA0.SINGLE.CMP0BUF = fanSpeed; // Controls width PWM-signal
			TCA0.SPLIT.LCMP0 = fanSpeed;
			
			if (adc_value > 950)
			{
				TCA0.SPLIT.HCMP0 = 15;
			}
			else if (adc_value > 800)
			{
				TCA0.SPLIT.LCMP1 = 10;
			}
			else if (adc_value > 700)
			{
				TCA0.SPLIT.HCMP1 = 16;
			}
			else
			{
				TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MIN_VALUE;
				TCA0.SPLIT.HCMP0 = DUTY_CYCLE_MIN_VALUE;
				TCA0.SPLIT.HCMP1 = DUTY_CYCLE_MIN_VALUE;
			}
			
			//_delay_ms(1000);
		}
		
	}
}
