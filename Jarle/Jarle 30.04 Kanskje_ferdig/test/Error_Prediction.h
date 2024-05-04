/*
 * Error_Prediction.h
 *
 * Created: 29.04.2024 15:04:40
 *  Author: jarle
 */ 


#ifndef ERROR_PREDICTION_H_
#define ERROR_PREDICTION_H_
#include <stdint.h>

// Variables
volatile uint16_t RPM_fan1;
volatile uint16_t RPM_fan2;
volatile uint16_t counter;


uint16_t previous_RPM_fan1 = 12400;
uint16_t previous_RPM_fan2 = 12400;

//get value from UART
uint8_t threshold_percentage = 90;

uint16_t read_value_EE_fan1;
uint16_t read_value_EE_fan2;


typedef struct {
	uint16_t fan1;
	uint16_t fan2;
} FanSpeeds;

// Function init
void RTC_init(void);
FanSpeeds average_values_for_tach(void);
void predict_error();
uint8_t alarm(uint8_t percentage, uint8_t threshold);
uint8_t calculate_percentage(float old_value, uint16_t current_value);

// Calculating average RPM at max fan speed


#endif //ERROR_PREDICTION_H_ 