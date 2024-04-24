/*
 * eeprom_start.c
 *
 * Created: 21.04.2024 15:33:23
 * Author : larsi
 */ 

#define F_CPU 4000000UL	// Define CPU-frequency
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>	// For printf and sizeof
#include "USARTn1.h"

uint8_t EEMEM start_address_controller = 0x00;
uint8_t EEMEM start_address_fan1 = 0x30;

uint8_t skrevet = 20;
uint8_t lest;

// Checks if memory(EEPROM) is ready.
void status_eeprom(){
	while(1)
	{
		if (eeprom_is_ready())
		{
			break;
		}
		else
		{
			;
		}
	}
}

void write_to_EEPROM(uint8_t value){
	uint8_t datasize = sizeof(value);
	
	status_eeprom();
	
	eeprom_update_block((void*) &value,(void*) &start_address_fan1, datasize);
	
}

uint8_t read_from_EEPROM()
{
	uint8_t test_lest;
	uint8_t datasize = sizeof(test_lest);
	printf("Størrelse: %d\r\n", datasize);
	
	status_eeprom();
	
	eeprom_read_block((void *) &test_lest,(void*) &start_address_controller, datasize);
	return test_lest;
}

int main(void)
{
	
	file_stream();  // Create file stream for USART
	USART3_init();	// USART3 initialize
    
	write_to_EEPROM(skrevet);
	
	lest = read_from_EEPROM();
	
    while (1) 
    {
		printf("Skrevet verdi: %d\r\n", skrevet);
		printf("Lest verdi: %d\r\n", lest);
		printf("\r\n");
		printf("\r\n");
    }
}

