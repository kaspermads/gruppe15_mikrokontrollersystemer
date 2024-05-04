/*
 * Usart.h
 *
 * Created: 24.04.2024 10:14:06
 *  Author: jarle
 */ 


#ifndef USART_H_
#define USART_H_

#include <stdint.h>
#define F_CPU 4000000UL
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)) + 0.5)
#define INIT_DELAY 10
#define MAX_COMMAND_LEN 100
#define NUMBER_OF_COMMANDS 13



void read_commands();
void USART3_init(void);
char USART3_readChar(void);
void USART3_sendString(char *str);
void USART3_sendChar(char c);
uint8_t compareCommands (char *src );

int rpm_value;
uint8_t manualFlag;

uint8_t FAN1Flag;
uint8_t FAN2Flag;
uint8_t FAN3Flag;
uint8_t FAN4Flag;


static char commands [ NUMBER_OF_COMMANDS ][ MAX_COMMAND_LEN ] = { //defines the valid Uart commands 
	"fan1",
	"fan2",
	"fan3",
	"fan4",
	"off",
	"manual",
	"auto",
	"overview",
	"controller",
	"low",
	"medium",
	"high",
	"alarm"

	
	
	};

#endif /* USART_H_ */