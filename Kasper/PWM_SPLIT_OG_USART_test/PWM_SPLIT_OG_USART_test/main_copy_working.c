
#define F_CPU 4000000UL	// Define CPU-frequency

// USART
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) // Macro for baud rate

// PWM config
#define PERIOD_FREQUENCY 79 // 0x01A0
// Calculated based on duty cycle(%) = Ton/(Tperiod)*100%
#define DUTY_CYCLE_MIN_VALUE 0 // MIN value
#define DUTY_CYCLE_MAX_VALUE 79 // MAX value

// ADC config
#define RTC_PERIOD 511 // RTC Period


// Including libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC
#include <avr/interrupt.h>
#include "TCn.h" // Include own library with TCA and TCB functions
#include "ADCn.h" // Include own library with ADC
#include "USART_COMMANDS.h"

// Preprocessing
void USART3_init(void);	// Initialize USART
void USART3_printChar(char c, FILE *stream); // Send characters
void PORT_init(void);
uint16_t pwm_to_rpm();
uint16_t pulseWidthReadings[100];
uint8_t pulseWidthIndex = 0;
uint16_t readPulseWidth;
int readFreq;
uint16_t rpm;

// Function to calculate the average of an array of uint16_t values
uint16_t average(uint16_t* array, uint8_t size) {
	uint32_t sum = 0;
	for (uint8_t i = 0; i < size; i++) {
		sum += array[i];
	}
	return (uint16_t)(sum / size);
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


void PORT_init(void)
{
	// Setter PIN0 som output (PWM-out)
	PORTD.DIRSET = PIN0_bm | PIN1_bm | PIN4_bm | PIN3_bm;

	// Setter PIN1 som input (TACH)
	//PORTD.DIRCLR = PIN1_bm;
}

void USART3_init(void)
{
	// RX input and TX output
	PORTB.DIR &= ~PIN1_bm;
	PORTB.DIR |= PIN0_bm;

	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);	// Baud rate for USART3
	USART3.CTRLB |= USART_TXEN_bm;	// Aktiverer USART3-senderen i kontrollregister B med bitmasken til TX
}
void USART3_printChar(char c, FILE *stream)
{
	// Sjekker om bufferet er tomt f?r ny sending
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;	// Sender en karakter
	return 0;
}

// Lager en egendefinert output stream som skal h?ndteres av funksjonen USART3_printChar
static FILE USART3_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);

uint16_t pwm_to_rpm()
{
	uint16_t pulseWidthAverage = average(pulseWidthReadings, 100);
	uint32_t rpm = ((F_CPU*60)/(4*pulseWidthAverage*2));
	
	return (uint16_t)rpm;
	
}

int main(void)
{
	USART3_init();	// Funksjonskall for ? sette i gang USART
	stdout = &USART3_stream;	// Erstatter standard output stream med den egen definerte
	
	// PWM initialize
	PORT_init();
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
			uint16_t fanPeriod = ((79.0/1023.0)*adc_value);
			printf("ADC Value: %d\r\n", adc_value);
			printf("Fan speed/period: %d\r\n", fanPeriod);
			printf("PWM read: %d\r\n", readPulseWidth);
			printf("RPM: %d\r\n", pwm_to_rpm());
			printf("\r\n");
			printf("\r\n");
			TCA0.SPLIT.LCMP0 = fanPeriod;
			
		}
		
	}
}