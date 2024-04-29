/*
 * I2C.h
 *
 * Created: 22.04.2024 14:50:17
 *  Author: larsi
 */ 


#ifndef I2C_H_
#define I2C_H_

void TWI0_M_init ( void ) {
	PORTA.DIRSET = PIN2_bm | PIN3_bm ;
	PORTA.PINCONFIG = PORT_PULLUPEN_bm ;
	PORTA.PINCTRLUPD = PIN2_bm | PIN3_bm ;
	// TWI0.CTRLA = TWI_SDAHOLD_50NS_gc ; // Setting hold time to 50 NS.
	TWI0.MCTRLA = TWI_ENABLE_bm ; // Enable TWI /I2C as controller .
	
}



#endif /* I2C_H_ */