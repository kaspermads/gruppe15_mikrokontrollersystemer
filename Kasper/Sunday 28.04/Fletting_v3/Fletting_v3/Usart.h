/*
 * Usart.h
 *
 * Created: 4/28/2024 10:32:27 AM
 *  Author: Kaspe
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)
#define INIT_DELAY 10
#define MAX_COMMAND_LEN 100
#define NUMBER_OF_COMMANDS 12

int rpm_value;
uint8_t manualFlag;

uint8_t compareCommands (char *src );
void USART3_sendChar(char c);
static int USART3_printChar(char c,  FILE *stream);
void USART3_sendString(char *str);
char USART3_readChar(void);
void USART3_init(void);
void read_commands();









#endif /* USART_H_ */