/*
 * TCn.h
 *
 * Created: 18.04.2024 12:00:36
 *  Author: larsi
 */ 


#ifndef TCN_H_
#define TCN_H_

#define PERIOD_FREQUENCY 79 // 0x01A0
// Calculated based on duty cycle(%) = Ton/(Tperiod)*100%
#define DUTY_CYCLE_MIN_VALUE 0 // MIN value
#define DUTY_CYCLE_MAX_VALUE 79 // MAX value

void TCA0_init(void);
void TCB0_init(void);
void TCB1_init(void);
void PORT_init(void);



#endif // TCN_H_ 