/*
 * pwm_to_rpm.h
 *
 * Created: 5/4/2024 11:38:33 AM
 *  Author: Kaspe
 */ 


#ifndef PWM_TO_RPM_H_
#define PWM_TO_RPM_H_


#include <stdint.h>
#include <stdint-gcc.h>

uint16_t pulseWidthReadings1[100];
uint16_t pulseWidthReadings2[100];
uint16_t pwm_to_rpm(uint16_t* pulseWidthReadings);

uint8_t pulseWidthIndex1 = 0;
uint8_t pulseWidthIndex2 = 0;
uint16_t rpm;

uint16_t average(uint16_t* array, uint8_t size) {
	uint32_t sum = 0;
	for (uint8_t i = 0; i < size; i++) {
		sum += array[i];
	}
	return (uint16_t)(sum / size);
}


uint16_t pwm_to_rpm(uint16_t* pulseWidthReadings)
{
	uint16_t pulseWidthAverage1 = average(pulseWidthReadings, 100);
	uint32_t rpm = ((F_CPU*60)/(4*pulseWidthAverage1*2));
	return (uint16_t)rpm;
	
}




#endif /* PWM_TO_RPM_H_ */