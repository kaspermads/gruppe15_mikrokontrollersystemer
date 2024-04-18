/*
 * TCn.h
 *
 * Created: 18.04.2024 12:00:36
 *  Author: larsi
 */ 


#ifndef TCN_H_
#define TCN_H_

void TCA0_init(void);
void TCB0_init(void);


void TCA0_init(void)
{
	//set waveform output on PORT D
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTD_gc;
	
	// enable compare channel 0, 1 and set single-slope PWM mode
	TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	
	 // enable split mode
    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm; 
	
	// enable compare channel 0 for the higher byte, enable compare channel 0 for the lower byte
	TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm | TCA_SPLIT_LCMP0EN_bm | TCA_SPLIT_HCMP1EN_bm | TCA_SPLIT_LCMP1EN_bm;    
	
	
	// L0 = PIN0, L1=PIN1, H0=PIN3, H1=PIN4
	// set the PWM frequencies and duty cycles
    TCA0.SPLIT.LPER = PERIOD_FREQUENCY;                          
    TCA0.SPLIT.LCMP0 = DUTY_CYCLE_MIN_VALUE;                           
    TCA0.SPLIT.HPER = PERIOD_FREQUENCY;                             
    TCA0.SPLIT.HCMP0 = DUTY_CYCLE_MIN_VALUE;
	
	//TCA0.SPLIT.LPER = PERIOD_FREQUENCY;                          
    TCA0.SPLIT.LCMP1 = DUTY_CYCLE_MIN_VALUE;                           
    //TCA0.SPLIT.HPER = PERIOD_FREQUENCY;                             
    TCA0.SPLIT.HCMP1 = DUTY_CYCLE_MIN_VALUE; 
	
	TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV4_gc | TCA_SPLIT_ENABLE_bm;   // set clock source (sys_clk/4), and start timer         
	
	/*
	// set PWM frequency
	TCA0.SINGLE.PERBUF = PERIOD_FREQUENCY;
	
	TCA0.SINGLE.CMP0BUF = DUTY_CYCLE_MIN_VALUE; // Controls width PWM-signal

	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc | TCA_SINGLE_ENABLE_bm;// set clock source(sys_clk/4) and start timer
	*/
	
}


void TCB0_init(void)
{
	PORTB.DIR &= ~PIN4_bm;
	PORTB.PIN4CTRL |= PORT_PULLUPEN_bm;
	
	TCB0.CTRLA = TCB_ENABLE_bm | TCB_CLKSEL_TCA0_gc;	// enable timer/counter TCB0 and use clock from TCA0
	TCB0.CTRLB = TCB_CNTMODE_PW_gc;	// set timer mod to input capture frequency and PWM measurement mode
	
	TCB0.INTCTRL = TCB_CAPT_bm; // interrupt enabled for capture
	
	TCB0.EVCTRL = TCB_CAPTEI_bm; // Capture event input enable
	
	//EVSYS.SWEVENTA = EVSYS_SWEVENTA0_bm;
	EVSYS.CHANNEL0 = EVSYS_CHANNEL0_PORTB_PIN4_gc; //	input event channel 0, port b on pin 4
	EVSYS.USERTCB0CAPT = EVSYS_USER_CHANNEL0_gc; // connects channel 0(portb pin0????) specific to TCB0 input capture mode
	//   CHANNEL0
	
}


#endif /* TCN_H_ */