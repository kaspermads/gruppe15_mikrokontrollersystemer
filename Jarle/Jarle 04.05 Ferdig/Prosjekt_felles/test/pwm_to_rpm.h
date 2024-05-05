/*
 * pwm_to_rpm.h
 *
 * Created: 5/4/2024 1:04:37 PM
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



/**
 * \brief Finds the average of an array of values
 * 
 * \param array Expects an array filled with 100 tach readings
 * \param size Expects the size of the array, which will be 100 in this case
 * 
 * \return uint16_t average
 */
uint16_t average(uint16_t* array, uint8_t size) {
	uint32_t sum = 0;
	for (uint8_t i = 0; i < size; i++) {
		sum += array[i];
	}
	return (uint16_t)(sum / size);
}


/**
 * \brief Converts the tach readings to actual rotations per minute by using formula from fan datasheet
 * 
 * \param pulseWidthReadings Expects an array (points to an uint16_t array)
 * 
 * \return uint16_t rpm
 */
uint16_t pwm_to_rpm(uint16_t* pulseWidthReadings)
{
	uint16_t pulseWidthAverage1 = average(pulseWidthReadings, 100);
	uint32_t rpm = ((F_CPU*60)/(4*pulseWidthAverage1*2));
	return (uint16_t)rpm;
	
}




#endif /* PWM_TO_RPM_H_ */