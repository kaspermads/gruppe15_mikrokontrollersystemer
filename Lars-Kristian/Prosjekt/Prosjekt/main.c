
#define F_CPU 4000000UL	// Define CPU-frequency

// USART
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) // Macro for baud rate

// PWM config
#define PERIOD_FREQUENCY 20 // 0x01A0
// Calculated based on duty cycle(%) = Ton/(Tperiod)*100%
#define DUTY_CYCLE_MIN_VALUE 0 // MIN value for 0 degrees
#define DUTY_CYCLE_MAX_VALUE 10 // MAX value for 90 degrees

// ADC config
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) // Macro for å beregne baudrate
#define RTC_PERIOD 511 // RTC Period


// Including libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>	// For printf
#include <stdbool.h> // For ADC

// Preprocessing
void USART3_init(void);	// Initialize USART
void USART3_printChar(char c, FILE *stream); // Send characters
void TCA0_init(void);	// 
void PORT_init(void);
void ADC0_init(void);
uint16_t ADC0_read(void);
void ADC0_start(void);
bool ADC0_conersionDone(void);

void TCA0_init(void)
{
	/* set waveform output on PORT D */
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;

	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm /* enable compare
	channel 0 */
	| TCA_SINGLE_WGMODE_DSBOTTOM_gc; /* set dual-slope PWM
	mode */

	// set PWM frequency
	TCA0.SINGLE.PERBUF = PERIOD_FREQUENCY;
	
	TCA0.SINGLE.CMP0BUF = DUTY_CYCLE_MIN_VALUE; // *Controls width PWM-signal

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc /* set clock source
	(sys_clk/4) */
	| TCA_SINGLE_ENABLE_bm; /* start timer */
	
}
void PORT_init(void)
{
	// Setter PIN0 som output
	PORTD.DIRSET = PIN0_bm;

	// Setter PIN1 som input (TACH)
	PORTD.DIRCLR = PIN1_bm;

}
void ADC0_init(void)
{
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;

	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;

	ADC0.CTRLC = ADC_PRESC_DIV4_gc; /* CLK_PER divided by 4 */
	
	VREF.ADC0REF = VREF_REFSEL_VDD_gc ; /* VDD as reference */

	ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc; /* 10-bit mode */

	/* Select ADC channel */
	ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;

	/* Enable FreeRun mode */
	ADC0.CTRLA |= ADC_FREERUN_bm;
}
uint16_t ADC0_read(void)
{
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;

	return ADC0.RES;
}
void ADC0_start(void)
{
	/* Start conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
}
bool ADC0_conersionDone(void)
{
	return (ADC0.INTFLAGS & ADC_RESRDY_bm);
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
	
	// ADC initialize
	ADC0_init();
	ADC0_start();
	
	while (1)
	{
		if (ADC0_conersionDone())
		{
			// Read potentiometer
			uint16_t adc_value = ADC0_read();
			// Convert ADC-value to PWM signal for angle between 0 and 90
			uint16_t fanSpeed = ((20.0/1023.0)*adc_value);
			printf(adc_value);
			printf("\r\n");
			TCA0.SINGLE.CMP0BUF = fanSpeed; // *Controls width PWM-signal
		}
		
		/*
		TCA0.SINGLE.CMP0BUF = DUTY_CYCLE_MAX_VALUE; // *Controls width PWM-signal
		_delay_ms(5000);
		*/
	}
}