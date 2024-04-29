/*
 * Error_Prediction.h
 *
 * Created: 29.04.2024 15:04:40
 *  Author: jarle
 */ 


#ifndef ERROR_PREDICTION_H_
#define ERROR_PREDICTION_H_


// Variables
volatile uint16_t RPM_fan1;
volatile uint16_t RPM_fan2;
volatile uint16_t counter;


uint16_t new_fan_RPM_fan1 = 12400;
uint16_t new_fan_RPM_fan2 = 12400;
uint16_t EEPROM_RPM_fan1 = 12400;
uint16_t EEPROM_RPM_fan2 = 12400;


typedef struct {
	uint16_t fan1;
	uint16_t fan2;
} FanSpeeds;

// Preprocessing
uint16_t PWM_freq_to_RPM(uint16_t PWM_onperiode);
void RTC_init(void);
FanSpeeds average_values_for_tach(void);
void predict_error();
uint8_t alarm(uint8_t percentage);
uint8_t calculate_percentage(float old_value, uint16_t current_value);

/*
uint16_t PWM_freq_to_RPM(uint16_t PWM_onperiode)
{
	return ((F_CPU * 60)/(4 * PRESCALER * PWM_onperiode));
}
*/

// Calculating average RPM at max fan speed
FanSpeeds average_values_for_tach()
{
	// SETT DETTE I IF MED FLAGG???
	
	// Set value to all fans
	TCA0.SPLIT.LCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP0 = DUTY_CYCLE_MAX_VALUE;
	TCA0.SPLIT.HCMP1 = DUTY_CYCLE_MAX_VALUE;
	
	printf("Diagnose startet! \r\n");
	
	// For average read values for two fans
	uint32_t sum_RPM_fan1 = 0;
	uint32_t sum_RPM_fan2 = 0;
	uint16_t average_read_value_fan1;
	uint16_t average_read_value_fan2;
	
	uint16_t num_of_readings = 0;
	uint16_t compare_counter_time = counter; // reset counter for compare
	
	while (counter - compare_counter_time <= 2)
	{
		;
	}
	compare_counter_time = counter;
	
	while (counter - compare_counter_time <= 10)
	{
		sum_RPM_fan1 += pwm_to_rpm1();
		sum_RPM_fan2 += pwm_to_rpm2();
		
		//printf("RPM fan 1: %d\r\n", RPM_fan1);
		num_of_readings ++;
	}
	
	average_read_value_fan1 = sum_RPM_fan1 / num_of_readings;
	average_read_value_fan2 = sum_RPM_fan2 / num_of_readings;
	
	printf("Number of readings: %d\r\n", num_of_readings);
	
	FanSpeeds average_read_values;
	average_read_values.fan1 = average_read_value_fan1;
	average_read_values.fan2 = average_read_value_fan2;
	
	TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MIN_VALUE;
	
	return average_read_values;
}

// Predicting failures with fans
void predict_error()
{
	// Read value from EEPROM, maybe somewhere else
	
	
	// Local struct for current RPM speeds fan1 and fan2
	FanSpeeds current_RPM_values;
	// Call function to get the average fan speeds and save in variables
	current_RPM_values = average_values_for_tach();
	uint16_t current_RPM_value_fan1 = current_RPM_values.fan1;
	uint16_t current_RPM_value_fan2 = current_RPM_values.fan2;
	
	printf("Gjennomsnitt fan 1: %d\r\n", current_RPM_value_fan1);
	printf("Gjennomsnitt fan 2: %d\r\n", current_RPM_value_fan2);
	printf("\r\n");
	printf("\r\n");
	
	uint16_t  old_value_fan1 = new_fan_RPM_fan1;
	uint16_t  old_value_fan2 = new_fan_RPM_fan2;
	printf("old fan 2: %d\r\n", old_value_fan1);
	
	// Formula for prediction of error
	uint8_t percentage_to_compare_fan1 = calculate_percentage(old_value_fan1, current_RPM_value_fan1); // send old value and atm value
	uint8_t percentage_to_compare_fan2 = calculate_percentage(old_value_fan2, current_RPM_value_fan2); // send old value and atm value
	
	// For testing
	printf("PROSENT fan 1: %d\r\n", percentage_to_compare_fan1);
	printf("PROSENT fan 2: %d\r\n", percentage_to_compare_fan2);
	
	// Under 90% set of a alarm
	uint8_t fan1_alarm =  alarm(percentage_to_compare_fan1);
	uint8_t fan2_alarm = alarm(percentage_to_compare_fan2);
	// For testing
	printf("STATUS fan 1: %d\r\n", fan1_alarm);
	printf("STATUS fan 2: %d\r\n", fan2_alarm);
	
	if (fan1_alarm == 1)
	{
		// What alarm do we want?
		printf("ALARM fan 1!! \r\n");
		
	}
	if (fan2_alarm == 1)
	{
		//what alarm do we want?
		printf("ALARM fan 2!! \r\n");
	}
	
	// Updates the short term value to predict instant failure
	new_fan_RPM_fan1 = current_RPM_value_fan1;
	new_fan_RPM_fan2 = current_RPM_value_fan2;
	
}

uint8_t alarm(uint8_t percentage)
{
	if (percentage < 90)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t calculate_percentage(float old_value, uint16_t current_value)
{
	return (current_value / old_value) * 100.0;
}


#endif /* ERROR_PREDICTION_H_ */