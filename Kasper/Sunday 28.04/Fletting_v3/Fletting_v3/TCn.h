/*
 * TCn.h
 *
 * Created: 4/28/2024 10:32:14 AM
 *  Author: Kaspe
 */ 


#ifndef TCN_H_
#define TCN_H_


#define PERIOD_FREQUENCY 79 // 0x01A0
// Calculated based on duty cycle(%) = Ton/(Tperiod)*100%
#define DUTY_CYCLE_MIN_VALUE 0 // MIN value
#define DUTY_CYCLE_MAX_VALUE 79 // MAX value

void TCA0_init(void);
void TCB0_init(void);
void PORT_init(void);
void TCB1_init(void);





#endif /* TCN_H_ */