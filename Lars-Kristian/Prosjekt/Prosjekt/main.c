
#define F_CPU 4000000UL	// Define CPU-frequency

// USART
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) // Macro for baud rate

// PWM config
#define PERIOD_FREQUENCY 20 // 0x01A0
// Calculated based on duty cycle(%) = Ton/(Tperiod)*100%
#define DUTY_CYCLE_MIN_VALUE 0 // MIN value
#define DUTY_CYCLE_MAX_VALUE 20 // MAX value

// ADC config
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) // Macro for å beregne baudrate
#define RTC_PERIOD 511 // RTC Period


// Including libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC
#include <avr/interrupt.h>
#include "TCn.h" // Include own library with TCA and TCB functions
#include "ADCn.h" // Include own library with ADC

// Preprocessing
void USART3_init(void);	// Initialize USART
void USART3_printChar(char c, FILE *stream); // Send characters
void PORT_init(void);



uint16_t readPulseWidth;
int readFreq;



ISR(TCB0_INT_vect)
{
	TCB0.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	
	// DO SOMETHING, maybe read from TCB0.CCMP????
	//TCB0.CCMP // holds the compare, capture, and top value
	readPulseWidth = TCB0.CCMP;
	
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
	// Sjekker om bufferet er tomt før ny sending
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;	// Sender en karakter
	return 0;
}

// Lager en egendefinert output stream som skal håndteres av funksjonen USART3_printChar
static FILE USART3_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);

int main(void)
{
	USART3_init();	// Funksjonskall for å sette i gang USART
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
			uint16_t fanSpeed = ((20.0/1023.0)*adc_value);
			printf("%d\r\n", adc_value);
			printf("%d\r\n", fanSpeed);
			printf("%d\r\n", readPulseWidth);
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
