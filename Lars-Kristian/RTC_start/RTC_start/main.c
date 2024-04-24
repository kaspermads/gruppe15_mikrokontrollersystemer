/*
 * RTC_start.c
 *
 * Created: 24.04.2024 15:38:27
 * Author : larsi
 */ 

#include <avr/io.h>

void RTC_init(void);


void RTC_init(void)
{
	RTC.CTRLA = RTC_RTCEN_bm | ;
}


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

