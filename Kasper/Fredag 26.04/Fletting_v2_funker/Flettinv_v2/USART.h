/*
 * USART.h
 *
 * Created: 4/25/2024 3:20:55 PM
 *  Author: Kaspe
 */ 


#ifndef USART_H_
#define USART_H_

uint8_t compareCommands(char *src );
void USART3_sendChar(char c);
static int USART3_printChar(char c,  FILE *stream);
static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);
void USART3_sendString(char *str);
char USART3_readChar(void);
void USART3_init(void);
void read_commands();





#endif /* USART_H_ */