
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

// Preprocessing
void USART3_init(void);	// Initialize USART
void USART3_printChar(char c, FILE *stream); // Send characters
void TCA0_init(void);
void PORT_init(void);
void ADC0_init(void);
uint16_t ADC0_read(void);
void ADC0_start(void);
bool ADC0_conersionDone(void);

// TCB0 TESTING
void TCB0_init(void);

uint16_t readPulseWidth;
int readFreq;

void TCB0_init(void)
{
	PORTB.DIR &= ~PIN4_bm;
	PORTB.PIN4CTRL = PORT_PULLUPEN_bm;
	
	TCB0.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_TCA0_gc;	// enable timer/counter TCB0 and use clock from TCA0
	TCB0.CTRLB = TCB_CNTMODE_PW_gc;	// set timer mod to input capture frequency and PWM measurement mode
	
	TCB0.INTCTRL = TCB_CAPT_bm; // interrupt enabled for capture
	
	TCB0.EVCTRL = TCB_CAPTEI_bm; // Capture event input enable
	
	//EVSYS.SWEVENTA = EVSYS_SWEVENTA0_bm;
	EVSYS.CHANNEL0 = EVSYS_CHANNEL0_PORTB_PIN4_gc; //	input event channel 0, port b on pin 4
	EVSYS.USERTCB0CAPT = EVSYS_USER_CHANNEL0_gc; // connects channel 0(portb pin0????) specific to TCB0 input capture mode
	//   CHANNEL0
	
}

ISR(TCB0_INT_vect)
{
	TCB0.INTFLAGS = TCB_CAPT_bm; // Clear the interrupt flag
	
	// DO SOMETHING, maybe read from TCB0.CCMP????
	//TCB0.CCMP // holds the compare, capture, and top value
	readPulseWidth = TCB0.CCMP;
	
}

// KAN FJERNES NED TIL HIT

void TCA0_init(void)
{
	//set waveform output on PORT D
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
	
	// enable compare channel 0 and set single-slope PWM mode
	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	
	// set PWM frequency
	TCA0.SINGLE.PERBUF = PERIOD_FREQUENCY;
	
	TCA0.SINGLE.CMP0BUF = DUTY_CYCLE_MIN_VALUE; // Controls width PWM-signal

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc | TCA_SINGLE_ENABLE_bm;// set clock source(sys_clk/4) and start timer
	
}
void PORT_init(void)
{
	// Setter PIN0 som output (PWM-out)
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
			//printf("%d\r\n", adc_value);
			printf("%d\r\n", fanSpeed);
			printf("%d\r\n", readPulseWidth);
			printf("\r\n");
			printf("\r\n");
			TCA0.SINGLE.CMP0BUF = fanSpeed; // Controls width PWM-signal
			
			//_delay_ms(1000);
		}
	}
}
