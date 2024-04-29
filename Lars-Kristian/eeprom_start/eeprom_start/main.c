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

void status_eeprom();
void write_to_EEPROM(uint16_t value);
uint16_t read_from_EEPROM();

uint16_t EEMEM start_address_controller = 0x00;
uint16_t EEMEM start_address_fan1 = 0x30;

uint16_t EEMEM address_fan1 = 10;
uint16_t EEMEM address_fan2 = 20;


uint16_t stored_data_fan1 = 12645;
uint16_t stored_data_fan2 = 12239;

uint16_t read_value_EE_fan1;
uint16_t read_value_EE_fan2;

// Checks if memory(EEPROM) is ready.
void status_eeprom()
{
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

void write_to_EEPROM(uint16_t value){
	//uint8_t datasize = sizeof(value);
	
	status_eeprom();
	
	printf("Her skrevet: %d\r\n", value);
	eeprom_write_word((uint16_t*)address_fan1, value);
	
}

uint16_t read_from_EEPROM()
{
	uint16_t read_value;
	
	status_eeprom();
	
	read_value = eeprom_read_word((uint16_t*) address_fan1);
	return read_value;
}

int main(void)
{
	
	file_stream();  // Create file stream for USART
	USART3_init();	// USART3 initialize
	
    //uint16_t skrevet1 = skrevet;
	//write_to_EEPROM(stored_data_fan1);
	
	//read_value_EE_fan2 = read_from_EEPROM();
	status_eeprom();
	read_value_EE_fan1 = eeprom_read_word((uint16_t*) address_fan1);
	status_eeprom();
	read_value_EE_fan2 = eeprom_read_word((uint16_t*) address_fan2);
	
    while (1) 
    {
		printf("Lest verdi vifte 1: %d\r\n", read_value_EE_fan1);
		printf("Lest verdi vifte 2: %d\r\n", read_value_EE_fan2);
		printf("\r\n");
		printf("\r\n");    
	}
}

