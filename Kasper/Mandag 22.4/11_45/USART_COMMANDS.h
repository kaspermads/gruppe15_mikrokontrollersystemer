/*
 * USART_COMMANDS.h
 *
 * Created: 4/21/2024 12:59:46 PM
 *  Author: Kaspe
 */ 


#ifndef USART3_COMMANDS_H_
#define USART3_COMMANDS_H_


void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char *str);
char USART3_readChar(void);


void automaticMode();
void manualMode(char *command);
void executeCommand(uint8_t command_number, char *command);
void read_commands(void);
uint8_t compareCommands (char *src);


#endif /* USART3_COMMANDS_H_ */