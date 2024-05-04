
/*
 * save_to_eeprom.h
 *
 * Created: 29.04.2024 17:05:52
 *  Author: jarle
 */ 


#ifndef SAVE_TO_EEPROM_H_
#define SAVE_TO_EEPROM_H_

#include <avr/eeprom.h>

void status_eeprom();
void write_to_EEPROM(uint16_t value);
uint16_t read_from_EEPROM();

uint16_t EEMEM address_fan1 = 10;
uint16_t EEMEM address_fan2 = 20;

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


/* The functions below were only used in the process to load values manually 
into EEPROM
*/
void write_to_EEPROM(uint16_t value)
{
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


#endif /* SAVE_TO_EEPROM_H_ */
