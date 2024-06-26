/*
 * ADCn.h
 *
 * Created: 18.04.2024 12:27:39
 *  Author: larsi
 */ 


#ifndef ADCN_H_
#define ADCN_H_

// ADC config
#define RTC_PERIOD 511 // RTC Period

void ADC0_init(void);
uint16_t ADC0_read(void);
void ADC0_start(void);
bool ADC0_conersionDone(void);

void ADC0_init(void)
{
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;

	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;

	ADC0.CTRLC = ADC_PRESC_DIV4_gc; /* CLK_PER divided by 4 */
	
	VREF.ADC0REF = VREF_REFSEL_VDD_gc ; /* VDD as reference */

	ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc; /* 10-bit mode */

	/* Select ADC channel */
	ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;

	/* Enable FreeRun mode */
	ADC0.CTRLA |= ADC_FREERUN_bm;
}
uint16_t ADC0_read(void)
{
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;

	return ADC0.RES;
}
void ADC0_start(void)
{
	/* Start conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
}
bool ADC0_conersionDone(void)
{
	return (ADC0.INTFLAGS & ADC_RESRDY_bm);
}


// For testing

// if (adc_value > 950)
// {
// 	TCA0.SPLIT.HCMP0 = 15;
// }
// else if (adc_value > 800)
//{
	//TCA0.SPLIT.LCMP1 = 10;
//}
// else if (adc_value > 700)
// {
// 	TCA0.SPLIT.HCMP1 = 16;
// }
// else
// {
// 	TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MIN_VALUE;
// 	TCA0.SPLIT.HCMP0 = DUTY_CYCLE_MIN_VALUE;
// 	TCA0.SPLIT.HCMP1 = DUTY_CYCLE_MIN_VALUE;
// }


#endif /* ADCN_H_ */