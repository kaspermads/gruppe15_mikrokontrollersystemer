/* 
 * File:   eeprom.h
 * Author: Helle Augland Grasmo
 *
 * Created on 06 March 2024, 15:30
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <stdbool.h>



// Struct for information on the controller.
typedef struct {
    uint8_t fanSpeed;
} config_t;

// Check if EEPROM is ready to be written in
void check_eeprom_is_ready();

// Writes a struct in EEPROM
void write_struct_from_EEPROM(config_t write_struct);

// Read data from EEPROM and return it as a controller struct
config_t read_struct_from_EEPROM();

// Writes a datapoint in EEPROM
int write_data_point_from_EEPROM(uint8_t byte, uint8_t fan_num);

// Reads all the dataPoints form EEPROM
uint8_t read_data_point_speed_info(uint8_t fan_num, uint8_t *array);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

