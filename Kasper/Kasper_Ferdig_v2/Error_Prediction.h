/*
 * Error_Prediction.h
 *
 * Created: 29.04.2024 15:04:40
 *  Author: jarle
 */ 


#ifndef ERROR_PREDICTION_H_
#define ERROR_PREDICTION_H_

#include "pwm_to_rpm.h"

// Count variable in seconds
volatile uint16_t counter;

// Declaration of variables and initial values
uint16_t previous_RPM_fan1 = 12645;
uint16_t previous_RPM_fan2 = 12400;

// Initial value, can be updated by UART
uint8_t threshold_percentage = 90;

// Declaration of variables for values from EEPROM
uint16_t read_value_EE_fan1;
uint16_t read_value_EE_fan2;

// To save fan speeds during calculation 
typedef struct {
	uint16_t fan1;
	uint16_t fan2;
} FanSpeeds;

// Function prototyping
void RTC_init(void);
FanSpeeds run_test_and_calculate_average(void);
void predict_error(void);
uint8_t alarm(uint8_t percentage, uint8_t threshold);
uint8_t calculate_percentage(float old_value, uint16_t current_value);

/**
 * @brief This function starts the test and calculates the average RPM of the fans for 10 seconds
 * 
 * @param void
 * 
 * @return FanSpeeds is a struct for average values for fans
 */
FanSpeeds run_test_and_calculate_average()
{
	// Set max values to all fans
	TCA0.SPLIT.LCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP1 = DUTY_CYCLE_MAX_VALUE;
	
	
	printf("Diagnose startet! \r\n");
	
	// Sum for calculation
	uint32_t sum_RPM_fan1 = 0;
	uint32_t sum_RPM_fan2 = 0;
	// Hold average values for two fans
	uint16_t average_value_fan1;
	uint16_t average_value_fan2;
	
	uint16_t num_of_readings = 0;
	uint16_t compare_counter = counter; // Current value to compare
	
	// Hold 2seconds for fans to get maximum speed
	while (counter - compare_counter <= 2)
	{
		;
	}
	compare_counter = counter;	// Update for new comparison
	
	
	while (counter - compare_counter <= 10)
	{
		sum_RPM_fan1 += pwm_to_rpm(pulseWidthReadings1);
		sum_RPM_fan2 += pwm_to_rpm(pulseWidthReadings2);
		
		num_of_readings ++;
	}
	
	// Actual calculation of average
	average_value_fan1 = sum_RPM_fan1 / num_of_readings;
	average_value_fan2 = sum_RPM_fan2 / num_of_readings;
	
	// Local buffer for fan speeds to be returned
	FanSpeeds average_values;
	average_values.fan1 = average_value_fan1;
	average_values.fan2 = average_value_fan2;
	
	return average_values;
}

// Predicting failures with fans
void predict_error()
{	
	// Local struct for current RPM speeds for fans
	FanSpeeds current_RPM_values;
	// Call function to get the average fan speeds and save in local variables
	current_RPM_values = run_test_and_calculate_average();
	
	printf("Gjennomsnitt fan 1: %d\r\n", current_RPM_values.fan1);
	printf("Gjennomsnitt fan 2: %d\r\n", current_RPM_values.fan2);
	printf("\r\n");
	printf("\r\n");
	
	// EEPROM values
	uint16_t  old_value_fan1 = read_value_EE_fan1;
	uint16_t  old_value_fan2 = read_value_EE_fan2;
	
	//TESTING
	//printf("forrige fan 1: %d\r\n", previous_RPM_fan1);
	
	// Percentages for prediction 
	uint8_t percentage_fan1_eeprom = calculate_percentage(old_value_fan1, current_RPM_values.fan1);
	uint8_t percentage_fan2_eeprom = calculate_percentage(old_value_fan2, current_RPM_values.fan2);
	
	uint8_t percentage_fan1_previous = calculate_percentage(previous_RPM_fan1, current_RPM_values.fan1);
	uint8_t percentage_fan2_previous = calculate_percentage(previous_RPM_fan2, current_RPM_values.fan2);
	
	// For testing
	printf("PROSENT fan 1: %d\r\n", percentage_fan1_previous);
	printf("PROSENT fan 2: %d\r\n", percentage_fan2_eeprom);
	
	// Flags for alarms
	uint8_t fan1_alarm_long_term =  alarm(percentage_fan1_eeprom, threshold_percentage);
	uint8_t fan2_alarm_long_term = alarm(percentage_fan2_eeprom, threshold_percentage);
	
	uint8_t fan1_alarm_short_term = alarm(percentage_fan1_previous, 90);
	uint8_t fan2_alarm_short_term = alarm(percentage_fan2_previous, 90);
	
	// For testing
	printf("STATUS fan 1: %d\r\n", fan1_alarm_short_term);
	printf("STATUS fan 2: %d\r\n", fan2_alarm_long_term);
	
	// Check flags
	if (fan1_alarm_long_term == 1)
	{
		printf("ALARM fan 1! Type: wear and tear \r\n");
	}
	if (fan2_alarm_long_term == 1)
	{
		printf("ALARM fan 2! Type: wear and tear \r\n");
	}
	if (fan1_alarm_short_term == 1)
	{
		printf("ALARM fan 1! Type: momentarily\r\n");
	}
	if (fan2_alarm_short_term == 1)
	{
		printf("ALARM fan 1! Type: momentarily\r\n");
	}
	
	// Updates the short term value to predict instant failure
	previous_RPM_fan1 = current_RPM_values.fan1;
	previous_RPM_fan2 = current_RPM_values.fan2;
	
}

/**
 * @brief This function takes in two parameters for comparison and returns either 1 or 0 
 * 
 * @param percentage - Percentage deviation for fans
 * @param threshold - For comparison
 * 
 * @return uint8_t flag for alarm
 */
uint8_t alarm(uint8_t percentage, uint8_t threshold)
{
	if (percentage < threshold)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief This function takes two values and calculates the percentage and returns the value
 * 
 * @param old_value is float to avoid only 1 or 0
 * @param current_value The current value
 * 
 * @return uint8_t percentage
 */
uint8_t calculate_percentage(float old_value, uint16_t current_value)
{
	return (current_value / old_value) * 100.0;
}


#endif /* ERROR_PREDICTION_H_ */