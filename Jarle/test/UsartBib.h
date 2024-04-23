/*
 * BIB.h
 *
 * Created: 15.04.2024 19:22:51
 *  Author: jarle
 */ 


#ifndef BIB_H_
#define BIB_H_
#define MAX_COMMAND_LEN 100



char command[MAX_COMMAND_LEN];

void USART3_init(void)
{
	PORTB.DIRSET = PIN0_bm ; //TX -pin as output
	PORTB.DIRCLR = PIN1_bm ; //RX -pin as input
	USART3.BAUD = (uint16_t)
	USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm ; // Enable both TX and Rx
}




void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}

static int USART3_printChar(char c, FILE *stream)
{
	USART3_sendChar(c);
	return 0;
}



void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

uint8_t USART3_read()
{
	while (!(USART3.STATUS & USART_RXCIF_bm))
	{
		;
	}
	return USART3.RXDATAL;
}

/*

char USART3_readChar(void)
{
	while (!(USART3.STATUS & USART_RXCIF_bm))
	{
		;
	}
	return USART3.RXDATAL;
}

*/ 

char USART3_readChar(void)
{
	while (!(USART3.STATUS & USART_RXCIF_bm))
	{
		;
	}
	//return USART3.RXDATAL;
	uint8_t src = USART3.RXDATAL ;
	USART3_sendChar(src);
	return src;
}

/*
void returnUart(char *command) {
uint8_t index = 0;
char c;


while ((c = USART3_readChar()) != '\r') {
	if (c != '\n') {
		command[index++] = c;
		if (index >= MAX_COMMAND_LEN) {
			printf("Command too long\n");
			return;
		}
	}
}

command[index] = '\0';
}
*/
void returnUart(char *command)
{
	//char command[MAX_COMMAND_LEN];
	static uint8_t index = 0;
	char c = USART3_readChar();
	
	if(c != '\n' && c != '\r' && index < MAX_COMMAND_LEN - 1)
	{
		command[index++] = c;
		
	}
	
	if(c == '\r'|| c=='\n')
	{
		command[index] = '\0';
		index = 0;
		//printf("Command: %s\r\n",command);
		
		
		
		
	}
}





#endif /* BIB_H_ */