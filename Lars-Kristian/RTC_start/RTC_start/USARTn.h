/*
 * USARTn1.h
 *
 * Created: 19.04.2024 16:17:00
 *  Author: larsi
 */ 


#ifndef USARTN1_H_
#define USARTN1_H_

#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5) // Macro for baud rate

void file_stream(void);
void USART3_init(void);	// Initialize USART
void USART3_printChar(char c, FILE *stream); // Send characters

void file_stream(void)
{
	// Lager en egendefinert output stream som skal håndteres av funksjonen USART3_printChar
	static FILE USART3_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);
	stdout = &USART3_stream;	// Erstatter standard output stream med den egen definerte
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



#endif /* USARTN1_H_ */