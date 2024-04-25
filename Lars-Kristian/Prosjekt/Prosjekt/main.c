
#define F_CPU 4000000UL	// Define CPU-frequency

// Including libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC
#include <avr/interrupt.h>
//#include <avr/eeprom.h>

#include "TCn.h" // Include own library with TCA and TCB functions
#include "ADCn.h" // Include own library with ADC
#include "USARTn1.h"

// Preprocessing
void PORT_init(void);
uint16_t PWM_freq_to_RPM(uint16_t PWM_onperiode);
void RTC_init(void);

// Variables
volatile uint16_t RPM_fan1;
volatile uint16_t RPM_fan2;
volatile uint16_t counter;
volatile uint16_t delay_counter = 0;


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
	
	counter ++;
	
	RTC.PITINTFLAGS = RTC_PI_bm; // Clears the interrupt flag
}
// TCB0 interrupt function
ISR(TCB0_INT_vect)
{
	//TCB0.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	
	// DO SOMETHING, maybe read from TCB0.CCMP????
	//TCB0.CCMP // holds the compare, capture, and top value
	RPM_fan1= PWM_freq_to_RPM(TCB0.CCMP);
	TCB0.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	
}

ISR(TCB1_INT_vect)
{
	//TCB1.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	RPM_fan2= PWM_freq_to_RPM(TCB1.CCMP);
	TCB1.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
}

int main(void)
{
	file_stream();  // Create file stream for USART
	USART3_init();	// USART3 initialize
	
	// PWM initialize
	TCA0_init();
	TCB0_init();
	TCB1_init();
	
	// ADC initialize
	ADC0_init();
	ADC0_start();
	
	// RTC initialize
	RTC_init();
	
	sei(); // Enable Global Interrupts
	
	while (1)
	{
		if(counter == 20)
		{
			values_for_tach();
			counter = 0;
		}
		if (ADC0_conersionDone())
		{
			// Read potentiometer
			uint16_t adc_value = ADC0_read();
			// Convert ADC-value to PWM signal
			uint16_t fanSpeed = ((39.0/1023.0)*adc_value);
			printf("ADC Value: %d\r\n", adc_value);
			printf("Fan speed: %d\r\n", fanSpeed);
			printf("PWM read: %d\r\n", RPM_fan1);
			printf("\r\n");
			printf("\r\n");
			//TCA0.SINGLE.CMP0BUF = fanSpeed; // Controls width PWM-signal
			TCA0.SPLIT.LCMP0 = fanSpeed;
			
			//_delay_ms(1000);
		}
		
	}
}

uint16_t PWM_freq_to_RPM(uint16_t PWM_onperiode)
{
	return ((F_CPU * 60)/(4 * 4 * PWM_onperiode));
}

// Rewrite for multiple fans, with fannr, max fan speed as parameter and return average
void values_for_tach()
{
	// SETT DETTE I IF MED FLAGG
	
	// Set value to all fans
	TCA0.SPLIT.LCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP1 = DUTY_CYCLE_MAX_VALUE;
	
	printf("Satt vifter! \r\n");
	
	uint32_t sum_RPM = 0;
	uint16_t average_read_value;
	uint32_t num_of_readings = 0;
	
	volatile uint16_t compare_counter_time = counter;
	
	while (1)
	{
		//sum_RPM += PWM_freq_to_RPM(TCB0.CCMP);
		sum_RPM += RPM_fan1;
		printf("PWM read: %d\r\n", RPM_fan1);
		num_of_readings ++;
		
		if (counter - compare_counter_time >= 10)
		{
			break;
		}
	}
		
	average_read_value = sum_RPM / num_of_readings;
	
	printf("Number of readings: %d\r\n", num_of_readings);
	printf("Gjennomsnitt: %d\r\n", average_read_value);
	
	printf("\r\n");
	printf("\r\n");

	// Maybe return a value
	
}