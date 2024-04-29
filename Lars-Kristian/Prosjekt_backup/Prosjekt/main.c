
#define F_CPU 4000000UL	// Define CPU-frequency

// Including libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "TCn.h" // Include own library with TCA and TCB functions
#include "ADCn.h" // Include own library with ADC
#include "USARTn1.h"

typedef struct {
	uint16_t fan1;
	uint16_t fan2;
} FanSpeeds;

// Preprocessing
uint16_t PWM_freq_to_RPM(uint16_t PWM_onperiode);
void RTC_init(void);
FanSpeeds average_values_for_tach(void);
void predict_error();
uint8_t alarm(uint8_t percentage);
uint8_t calculate_percentage(uint16_t old_value, uint16_t current_value);

// Variables
volatile uint16_t RPM_fan1;
volatile uint16_t RPM_fan2;
volatile uint16_t counter;

uint16_t new_fan_RPM_fan1 = 12400;
uint16_t new_fan_RPM_fan2 = 12400;


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
			predict_error();
			//average_values_for_tach();
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
			printf("RPM fan 1: %d\r\n", RPM_fan1);
			printf("\r\n");
			printf("\r\n");
			
			TCA0.SPLIT.LCMP0 = fanSpeed;
			
			//_delay_ms(1000);
		}
	}
}

uint16_t PWM_freq_to_RPM(uint16_t PWM_onperiode)
{
	return ((F_CPU * 60)/(4 * PRESCALER * PWM_onperiode));
}

// Calculating average RPM at max fan speed
FanSpeeds average_values_for_tach()	
{
	// SETT DETTE I IF MED FLAGG???
	
	// Set value to all fans
	TCA0.SPLIT.LCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP1 = DUTY_CYCLE_MAX_VALUE;
	
	printf("Satt vifter! \r\n");
	
	// For average read values for two fans
	uint32_t sum_RPM_fan1 = 0;
	uint32_t sum_RPM_fan2 = 0;
	uint16_t average_read_value_fan1;
	uint16_t average_read_value_fan2;
	
	uint16_t num_of_readings = 0;
	uint16_t compare_counter_time = counter; // reset counter for compare
	
	while (counter - compare_counter_time <= 10) 
	{
		sum_RPM_fan1 += RPM_fan1;
		sum_RPM_fan2 += RPM_fan2;
			
		printf("RPM fan 1: %d\r\n", RPM_fan1);
		num_of_readings ++;
	}
	
	average_read_value_fan1 = sum_RPM_fan1 / num_of_readings;
	average_read_value_fan2 = sum_RPM_fan2 / num_of_readings;
	
 	printf("Number of readings: %d\r\n", num_of_readings);
	
	FanSpeeds average_read_values;
	average_read_values.fan1 = average_read_value_fan1;
	average_read_values.fan2 = average_read_value_fan2;
	
	return average_read_values;
}

// Predicting failures with fans
void predict_error()
{
	// Read value from EEPROM, maybe somewhere else
	
	
	// Local struct for current RPM speeds fan1 and fan2
	FanSpeeds current_RPM_values;
	// Call function to get the average fan speeds and save in variables
	current_RPM_values = average_values_for_tach();
	uint16_t current_RPM_value_fan1 = current_RPM_values.fan1;
	uint16_t current_RPM_value_fan2 = current_RPM_values.fan2;
	
	printf("Gjennomsnitt fan 1: %d\r\n", current_RPM_value_fan1);
	printf("Gjennomsnitt fan 2: %d\r\n", current_RPM_value_fan2);
	printf("\r\n");
	printf("\r\n");
	
	uint16_t  old_value_fan1 = new_fan_RPM_fan1;
	uint16_t  old_value_fan2 = new_fan_RPM_fan2;
	
	// Formula for prediction of error
	uint8_t percentage_to_compare_fan1 = calculate_percentage(old_value_fan1, current_RPM_value_fan1); // send old value and atm value
	uint8_t percentage_to_compare_fan2 = calculate_percentage(old_value_fan2, current_RPM_value_fan1); // send old value and atm value
	
	printf("PROSENT fan 1: %d\r\n", percentage_to_compare_fan1);
	
	// Under 90% set of a alarm
	uint8_t fan1_status =  alarm(percentage_to_compare_fan1);
	uint8_t fan2_status = alarm(percentage_to_compare_fan2);
	printf("STATUS fan 1: %d\r\n", fan1_status);
	
	if (fan1_status == 1)
	{
		// What alarm do we want?
		printf("ALARM!! \r\n");
		
	}
// 	else if (percentage_to_compare_fan2 < 90)
// 	{
// 		//what alarm do we want?
// 	}

}

uint8_t alarm(uint8_t percentage)
{
	if (percentage < 90)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t calculate_percentage(uint16_t old_value, uint16_t current_value)
{
	return (current_value / old_value) * 100.0;
}

