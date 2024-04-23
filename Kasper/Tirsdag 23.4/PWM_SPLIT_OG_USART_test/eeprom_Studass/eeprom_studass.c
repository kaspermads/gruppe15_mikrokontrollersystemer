#include "eeprom.h"

// The start address for the controller data
uint8_t EEMEM start_address_controller = 0x00;

// Where the writing of the fans points start
uint8_t EEMEM start_address_fan1 = 0x30;
uint8_t EEMEM start_address_fan2 = 0x60;

// The placement for the next datapoint form the fans.
uint8_t EEMEM current_address_fan1 = 0x30; 
uint8_t EEMEM current_address_fan2 = 0x60;

// Checks if the EEPROM memory is ready to be written in. 
void check_eeprom_is_ready(){
    while(1){
        if (eeprom_is_ready()){
            break;
        }else{
            ;
        }
    }
}


// Takes inn a struct by the form of config_t
// Checks if the eeprom is ready to be written in
// Checks if it has been written information at the address
// If true, the infromation is replaced with the intaken struct
// else the intaken struct is written at the address. 
void write_struct_from_EEPROM(config_t write_struct){
    uint8_t struct_size = sizeof(write_struct);
    
    check_eeprom_is_ready();
    
    eeprom_update_block((void*) &write_struct,(void*) &start_address_controller, struct_size);
   
}

// Reads the memory block at the address start_address_controller
// returns a struct in form of config_t
config_t read_struct_from_EEPROM(){
    //is eeprom ready??
    config_t read_struct;
    uint8_t struct_size = sizeof(read_struct);
    
    check_eeprom_is_ready();
    
    eeprom_read_block((void *) &read_struct,(void*) &start_address_controller, struct_size);
    return read_struct;
}

// Takes inn a dataPoint and what data set it belongs to
// checks if EEPROM is ready
// If the dataset is 1, the datapoint is written at the address. 
// If the dataset is 2 its written at another point. 
int write_data_point_in_EEPROM(uint8_t byte, uint8_t fan_num){
    
    check_eeprom_is_ready();
    if (fan_num == 1){
        eeprom_update_byte(current_address_fan1, byte);
        current_address_fan1++;
        return 1; 
    } else if (fan_num == 2){
        eeprom_update_byte(current_address_fan2, byte);
        current_address_fan2++;
        return 1;
    } else{
        return 0;
    }
}

// Reads all the datapoints to the choosen data. 
// it writes the data points in the USART stream.
uint8_t read_data_point_speed_info(uint8_t fan_num, uint8_t *array){
    uint8_t byte = 0;
    
    if (fan_num == 1){
        uint8_t len = current_address_fan1 - start_address_fan1;

        check_eeprom_is_ready();

        for (uint8_t i = 0; i <len; i++){
            byte = eeprom_read_byte(i + start_address_fan1);
            array[i] = byte;
            printf("Fanspeed nr %u : %u", i, byte);
        }
    } else if (fan_num == 2){
        uint8_t len = current_address_fan2 - start_address_fan2;
        check_eeprom_is_ready();

        for (uint8_t i = 0; i <len; i++){
            byte = eeprom_read_byte(i + start_address_fan2);
            array[i] = byte;
            printf("Fanspeed nr %u : %u", i, byte);
        }
    }
    return sizeof(array);
}
