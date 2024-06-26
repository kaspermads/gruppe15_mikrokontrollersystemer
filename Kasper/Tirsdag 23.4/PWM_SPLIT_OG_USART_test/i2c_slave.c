/*
 * i2c_slave.c
 *
 * Created: 4/22/2024 3:17:57 PM
 *  Author: Kaspe
 */ 

# define F_CPU 4000000 UL
# include < avr / io .h >
# include < avr / interrupt .h >
# include < string .h >
# include < util / delay .h >
# include < stdio .h >
# include < stdbool .h >
# include " twi_target .h"
# include " usart .h"
# define DATA_SIZE 8
volatile uint8_t data_buffer [ DATA_SIZE ];
volatile uint8_t buffer_index = 0;
volatile bool was_read = false ;
volatile bool data_ready = false ;
volatile uint8_t current_cnt ;
void write_handler ( uint8_t data_w ) {
	if( buffer_index < DATA_SIZE ) {
		data_buffer [ buffer_index ] = data_w ;
		buffer_index ++;
	}
}
uint8_t read_handler ( void ) {
	was_read = true ;
	uint8_t data_rd = current_cnt ;
	return data_rd ;
}
void stop_handler ( void ) {
	buffer_index = 0;
	if (! was_read ) {
		data_ready = true ;
	}
	else
	{
		was_read = false ;
	}
}
int main ( void )
{
	USART3_init () ;
	TWI_initPins () ;
	TWI_initClient (0 x6 ) ;
	// Assign handlers
	TWI_assignByteWriteHandler (& write_handler );
	TWI_assignByteReadHandler (& read_handler );
	TWI_assignStopHandler (& stop_handler ) ;
	sei () ;
	uint8_t data [ DATA_SIZE +1];
	while (1)
	{
		if( data_ready ) {
			cli () ; /* Clear interrupts while handling data_buffer */
			memcpy ( data , ( const uint8_t *) data_buffer , DATA_SIZE ) ;
			sei () ; /* Re - enable interrupts */
			data [ DATA_SIZE ] = �\0 �; /* Zero termination of C- string */
			printf (" Received : %s\n", data );
		}
		current_cnt ++;
		_delay_ms (1000) ;
	}
}