#define F_CPU 32000000UL
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "avr_compiler.h"
#include "clksys_driver.h"
#include "usart_driver.h"
#include "TC_driver.h"
#include "hires_driver.h"
#include "awex_driver.h"
#include "adc_driver.h"
#include "usart_driver.h"
#include "spi_driver.h"

//#include "/usr/lib/avr/include/avr/iox128a3.h"

#define SET_phaseOutputsEHigh(value) phaseOutputsEHigh = (value)
#define SET_phaseOutputsELow(value) phaseOutputsELow = (value)
#define SET_phaseOutputsFHigh(value) phaseOutputsFHigh = (value)
#define SET_phaseOutputsFLow(value) phaseOutputsFLow = (value)
#define SET_phaseOutputsDHigh(value) phaseOutputsDHigh = (value)
#define SET_phaseOutputsDLow(value) phaseOutputsDLow = (value)

#define SET_PHASE_STATE_0_MOT2() \
	SET_phaseOutputsEHigh(phaseOutputsEHigh & 0b01111111); \
	SET_phaseOutputsEHigh(phaseOutputsEHigh | 0b01000000);  /* set bits 6-7 of phaseOutputsEHigh to 01 */ \
															\
	SET_phaseOutputsELow(phaseOutputsELow & 0b10111111); \
	SET_phaseOutputsELow(phaseOutputsELow | 0b10000000); /* and 10 for phaseOutputsELow */ \
															\
	SET_phaseOutputsFHigh(phaseOutputsFHigh & 0b11110000); /* set bits 0-3 of phaseOutputsFHigh and phaseOutputsFLow to 0 */ \
	SET_phaseOutputsFLow(phaseOutputsFLow & 0b11110000); \
															\
	PORTF.OUT &= 0b11110010; /* set b2 floating -- important to do this in this order to avoid shoot through!! (and set c2 floating)  */ \
	PORTF.OUT |= 0b00000010; /* set b2 low */

#define SET_PHASE_STATE_1_MOT2() \
	SET_phaseOutputsEHigh(phaseOutputsEHigh & 0b01111111); \
	SET_phaseOutputsEHigh(phaseOutputsEHigh | 0b01000000); \
															\
	SET_phaseOutputsELow(phaseOutputsELow & 0b10111111); \
	SET_phaseOutputsELow(phaseOutputsELow | 0b10000000); \
															\
	SET_phaseOutputsFHigh(phaseOutputsFHigh & 0b11110000); \
	SET_phaseOutputsFLow(phaseOutputsFLow & 0b11110000); \
															\
	PORTF.OUT &= 0b11111000; /* set c2 floating -- important to do this in this order to avoid shoot through!! (and set b2 floating) */ \
	PORTF.OUT |= 0b00001000; /* set c2 low */
	

#define SET_PHASE_STATE_2_MOT2() \
	SET_phaseOutputsEHigh(phaseOutputsEHigh & 0b00111111); \
	SET_phaseOutputsELow(phaseOutputsELow & 0b00111111); \
															\
	SET_phaseOutputsFHigh(phaseOutputsFHigh & 0b11110001); \
	SET_phaseOutputsFHigh(phaseOutputsFHigh | 0b00000001); \
															\
	SET_phaseOutputsFLow(phaseOutputsFLow & 0b11110010); \
	SET_phaseOutputsFLow(phaseOutputsFLow | 0b00000010); \
															\
	PORTF.OUT &= 0b11111011; /* set c2 floating -- important to do this in this order to avoid shoot through!! */ \
	PORTF.OUT |= 0b00001000; /* set c2 low */ \
															\
	PORTE.OUT &= 0b00111111; /* set a2 floating */

#define SET_PHASE_STATE_3_MOT2() \
	SET_phaseOutputsEHigh(phaseOutputsEHigh & 0b00111111); \
	SET_phaseOutputsELow(phaseOutputsELow & 0b00111111); \
															\
	SET_phaseOutputsFHigh(phaseOutputsFHigh & 0b11110001); \
	SET_phaseOutputsFHigh(phaseOutputsFHigh | 0b00000001); \
															\
	SET_phaseOutputsFLow(phaseOutputsFLow & 0b11110010); \
	SET_phaseOutputsFLow(phaseOutputsFLow | 0b00000010); \
															\
	PORTE.OUT &= 0b10111111; /* set a2 floating -- important to do this in this order to avoid shoot through!! */ \
	PORTE.OUT |= 0b10000000; /* set a2 low */ \
															\
	PORTF.OUT &= 0b11110011; /* set c2 floating */

#define SET_PHASE_STATE_4_MOT2() \
	SET_phaseOutputsEHigh(phaseOutputsEHigh & 0b00111111); \
	SET_phaseOutputsELow(phaseOutputsELow & 0b00111111); \
															\
	SET_phaseOutputsFHigh(phaseOutputsFHigh & 0b11110100); \
	SET_phaseOutputsFHigh(phaseOutputsFHigh | 0b00000100); \
															\
	SET_phaseOutputsFLow(phaseOutputsFLow & 0b11111000); \
	SET_phaseOutputsFLow(phaseOutputsFLow | 0b00001000); \
															\
	PORTE.OUT &= 0b10111111; /* set a2 floating -- important to do this in this order to avoid shoot through!! */ 	\
	PORTE.OUT |= 0b10000000; /* set a2 low */			\
															\
	PORTF.OUT &= 0b11111100; /* set b2 floating */

#define SET_PHASE_STATE_5_MOT2() \
	SET_phaseOutputsEHigh(phaseOutputsEHigh & 0b00111111); \
	SET_phaseOutputsELow(phaseOutputsELow & 0b00111111); \
															\
	SET_phaseOutputsFHigh(phaseOutputsFHigh & 0b11110100); \
	SET_phaseOutputsFHigh(phaseOutputsFHigh | 0b00000100); \
															\
	SET_phaseOutputsFLow(phaseOutputsFLow & 0b11111000); \
	SET_phaseOutputsFLow(phaseOutputsFLow | 0b00001000); \
															\
	PORTF.OUT &= 0b11111110; /* set b2 floating -- important to do this in this order to avoid shoot through!! */ \
	PORTF.OUT |= 0b00000010; /* set b2 low */ \
															\
	PORTE.OUT &= 0b00111111; /* set a2 floating */

void configClock (void);
void configPWM (volatile TC0_t * tc, HIRES_t * hires, uint16_t period); // configure timer used to generate pwm
void configDelayTimer (volatile TC0_t * tc); // configure timer used to generate delays
void configHalfPWMTimer (volatile TC0_t * tc, HIRES_t * hires, uint16_t period);
void initAdc (ADC_t * adc); // use to initialize adcs
void spiInit (void);

void setMotor1State (uint8_t state); // motors to their a commutation state based on an integ 
void setMotor2State (uint8_t state); // -- used instead of macros to allow numerical indexing of states
void setMotor3State (uint8_t state);
void setMotor4State (uint8_t state);

void startup(void);

// holds current motor state (1-6) -- set by setMotorXState function call - !if you use macro - make sure to manually set!
volatile uint8_t motor1State = 0;
volatile uint8_t motor2State = 0;
volatile uint8_t motor3State = 0;
volatile uint8_t motor4State = 0;

// these are constants that define which direction the back emf slope goes during each state
// 0 = falling
// 1 = rising
volatile uint8_t stateSlope[6] = {0,1,0,1,0,1};

// these are constants that define the index of the pin to look at for back emf during each state
volatile uint8_t motor2StateSenseIndex[6] = {5,4,3,5,4,3};

// middle thresholds for back emf "zero" crossing for each motor.  I'm not sure if this needs to vary with pwm or not...
volatile uint8_t motor1Thresh = 0;
volatile uint8_t motor2Thresh = 92;
volatile uint8_t motor3Thresh = 0;
volatile uint8_t motor4Thresh = 0;

volatile uint8_t autoCommutationFlag = 0;
//~ #define autoCommutationFlag GPIOC

volatile uint8_t missedCommFlag = 0;

volatile uint8_t spiBuffer[8];

// these variables keep track of which pins should be driven during the high and low parts of the pwm cycle

	#define phaseOutputsEHigh GPIO0 // keep track of which pins should be driven high during high part of pwm
	#define phaseOutputsFHigh GPIO1 // keep track of which pins should be driven high during high part of pwm
	#define phaseOutputsDHigh GPIO2 // keep track of which pins should be driven high during high part of pwm

	#define phaseOutputsELow GPIO3 // keep track of which pins should be driven high during low part of pwm
	#define phaseOutputsFLow GPIO4 // keep track of which pins should be driven high during low part of pwm
	#define phaseOutputsDLow GPIO5 // keep track of which pins should be driven high during low part of pwm

#define flag GPIO6
#define firstSampleFlag GPIO7
#define passedCenterFlag GPIO8

#define risingReadyFlag GPIO9
#define fallingReadyFlag GPIOA

#define risingCount GPIOB
#define fallingCount GPIOC


volatile uint8_t getSpiPktFlag = 0;

volatile uint8_t spiIndex = 0;


volatile uint8_t writeBackFlag = 0;
// here is the pinout of the motor phases and sense lines for reference

volatile uint8_t spiWriteBuffer[9];

volatile uint8_t globalResult;

//~ PORTE:
	//~ AH1
	//~ AL1
	//~ BH1
	//~ BL1
	//~ CH1
	//~ CL1
	//~ AH2
	//~ AL2
//~ 
//~ PORTF:
	//~ BH2
	//~ BL2
	//~ CH2
	//~ CL2
	//~ AH3
	//~ AL3
	//~ BH3
	//~ BL3
//~ 
//~ PORTD:
	//~ CH3
	//~ CL3
	//~ AH4
	//~ AL4
	//~ BH4
	//~ BL4
	//~ CH4
	//~ CL4
	
// ADCA:
	//~ A1
	//~ B1
	//~ C1
	//~ A2
	//~ B2
	//~ C2
	//~ A3
	//~ B3
	
// ADCB:
	//~ C3
	//~ A4
	//~ B4
	//~ C4
	
// intercommutation delays from atmel app note AVR444
//~ startupDelays[0] = 200;
//~ startupDelays[1] = 150;
//~ startupDelays[2] = 100;
//~ startupDelays[3] = 80;
//~ startupDelays[4] = 70;
//~ startupDelays[5] = 65;
//~ startupDelays[6] = 60;
//~ startupDelays[7] = 55;

#define STARTUP_PWM 500
//~ 
uint16_t startupPwms[6] = {800, 800, 900, 900, 1200, 1200};
uint8_t startupDelays[6] = {250, 150, 105, 80, 75, 75}; // 630

//~ uint16_t startupPwms[6] = {800, 800, 800, 800, 800, 800};
//~ uint8_t startupDelays[6] = {180, 120, 105, 85, 75, 65}; // 630

//~ uint8_t startupDelays[6] = {200, 145, 90, 70, 57, 50}; // bad
//~ uint8_t startupDelays[6] = {250, 150, 105, 80, 71, 65}; // 630
//~ uint8_t startupDelays[6] = {200, 150, 104, 84, 74, 70}; // better // 500
//~ uint8_t startupDelays[6] = {200, 150, 107, 88, 79, 75};
//~ uint8_t startupDelays[6] = {200, 150, 109, 92, 83, 80};
//~ uint8_t startupDelays[6] = {200, 150, 112, 96, 88, 85};
//~ uint8_t startupDelays[6] = {200, 150, 115, 100, 92, 90};
	
int main (void) {
	
	passedCenterFlag = 0;
	
	configClock ();
	
	PORTE.DIR = 0b11000000;
	PORTF.DIR = 0b00001111;
	
	PMIC.CTRL |= PMIC_HILVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	TCF0.CCABUF = STARTUP_PWM;
	TCF0.CCBBUF = STARTUP_PWM;
	TCF0.CCCBUF = STARTUP_PWM;
	TCF0.CCDBUF = STARTUP_PWM;
	
	TCE0.CCABUF = STARTUP_PWM/2;
	TCE0.CCBBUF = STARTUP_PWM/2;
	TCE0.CCCBUF = STARTUP_PWM/2;
	TCE0.CCDBUF = STARTUP_PWM/2;
	
	configPWM (&TCF0, &HIRESF, 5000);
	configHalfPWMTimer (&TCE0, &HIRESE, 5000);
	
	TCF0.CNT = 0;
	TCE0.CNT = 0;
	
	configDelayTimer (&TCC0);
	
	initAdc (&ADCA);
	initAdc (&ADCB);
	
	sei();
	
	startup();
	//~ autoCommutationFlag = 1;
	// float everything
	
	//~ SET_phaseOutputsEHigh(0);
	//~ SET_phaseOutputsEHigh(0);
//~ 
	//~ SET_phaseOutputsELow(0);
	//~ SET_phaseOutputsELow(0);
//~ 
	//~ SET_phaseOutputsFHigh(0);
	//~ SET_phaseOutputsFHigh(0);
//~ 
	//~ SET_phaseOutputsFLow(0);
	//~ SET_phaseOutputsFLow(0);
	//~ 
	//~ PORTE.OUT = 0;
	//~ PORTF.OUT = 0;
	
    _delay_ms(1000);
    _delay_ms(1000);
	TCF0.CCBBUF = 1600;
	TCE0.CCBBUF = 1600/2;

	spiInit ();

	while (1) {}
}

void configClock (void) {
	// enable 32mhz oscillator 
	CLKSYS_Enable( OSC_RC32MEN_bm );
	do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );

	// configure pll source = 32mhz oscillator/4 * 16 = 4*32mhz = 128mhz output
	CLKSYS_PLL_Config( OSC_PLLSRC_RC32M_gc, 16 );

	// enable pll
	CLKSYS_Enable( OSC_PLLEN_bm );
	do {} while ( CLKSYS_IsReady( OSC_PLLEN_bm ) == 0 );

	// enable prescale by 2 and 2 again to generate 2x and 4x clocks
	CCP = CCP_IOREG_gc;
	CLK.PSCTRL = CLK_PSBCDIV_2_2_gc;

	// select main clock source as pll output
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );
}

void spiInit () {
	SPIC.CTRL = SPI_ENABLE_bm | SPI_MODE_2_gc;
	SPIC.INTCTRL = SPI_INTLVL_HI_gc;
	PORTC.DIRSET = SPI_MISO_bm;
    PORTC.DIRCLR = 0b10111111;
}

ISR(SPIC_INT_vect) {
    static uint8_t spi_index = 0;
    static uint8_t readPacketFlag = 0;
    static uint8_t writePacketFlag = 0;
	uint8_t data = SPIC.DATA;
    if(data == 0xB5)
    {
        readPacketFlag = 1;
        writePacketFlag = 0;
        spi_index = 0;
    }
    if(readPacketFlag)
    {
        if(data != 0xB5)
        {
            spiBuffer[spi_index] = data;
            spi_index++;
            SPIC.DATA = 0;
            if (spi_index == 9) {
				TCF0.CCABUF = *((uint16_t *)(spiBuffer+0));
				TCF0.CCBBUF = *((uint16_t *)(spiBuffer+2));
				TCF0.CCCBUF = *((uint16_t *)(spiBuffer+4));
				TCF0.CCDBUF = *((uint16_t *)(spiBuffer+6));
				
				TCE0.CCBBUF = TCF0.CCBBUF/2;
				
				if (TCF0.CCABUF == 0 || TCF0.CCBBUF == 0 || TCF0.CCCBUF == 0 || TCF0.CCDBUF == 0)
				{
					PORTD.DIR = 0;
					PORTE.DIR = 0;
					PORTF.DIR = 0;
					cli();
					while(1);
				}
			}
        }
    }
    if(readPacketFlag && spi_index >= 9)
    {
        readPacketFlag = 0;
        writePacketFlag = 1;
        spi_index = 0;
    }
    if(writePacketFlag && spi_index <= 8)
    {
        SPIC.DATA = spiWriteBuffer[spi_index];
        //SPIC.DATA = 0xAA;
        spi_index++;
    }
}


// *************** PWM **********************

// Configures PWM output on compare a b and c for single slope pwm, with hires, and clk source as sys clk
void configPWM (volatile TC0_t * tc, HIRES_t * hires, uint16_t period) {
	TC_SetPeriod (tc, period );
	TC0_ConfigWGM (tc, TC_WGMODE_NORMAL_gc ); // set to single slope pwm generation mode
	TC0_EnableCCChannels (tc, TC0_CCAEN_bm); // enable compare A
	TC0_EnableCCChannels (tc, TC0_CCBEN_bm); // enable compare B
	TC0_EnableCCChannels (tc, TC0_CCCEN_bm); // enable compare C
	TC0_EnableCCChannels (tc, TC0_CCDEN_bm); // enable compare D

	//~ TC0_SetCCAIntLevel (tc, TC_CCAINTLVL_HI_gc);
	TC0_SetCCBIntLevel (tc, TC_CCBINTLVL_LO_gc);
	//~ TC0_SetCCCIntLevel (tc, TC_CCCINTLVL_HI_gc);
	//~ TC0_SetCCDIntLevel (tc, TC_CCDINTLVL_HI_gc);

	TC0_SetOverflowIntLevel (tc, TC_OVFINTLVL_LO_gc);
	
	PMIC.CTRL |= PMIC_HILVLEN_bm;

	TC0_ConfigClockSource (tc, TC_CLKSEL_DIV1_gc);
	HIRES_Enable (hires, HIRES_HREN_TC0_gc);
}

// When TCF0 compare interrupts are triggered, set the set of 6 outputs which
// controls the motor assigned to that interrupt to low

ISR (TCF0_CCA_vect) {	
	// motor 1 on pe0-5
	
	// force bits 6 and 7 to 1 (for phaseOutputsEHighInv) and 0 (for phaseOutputsEHighInv) so that only pins for motor 1 are turned off 
	PORTE.OUTCLR = (phaseOutputsEHigh | 0b11000000); // turn off all high bits in phaseOutputsELowInv
	PORTE.OUTSET = (phaseOutputsELow & 0b00111111); // turn on all low bits in phaseOutputsEHigh	
}

ISR (TCF0_CCB_vect) {
	// motor 2 on pe6-7 and pf0-3
	
	PORTE.OUTCLR = (phaseOutputsEHigh | 0b00111111); // turn off all high bits in phaseOutputsELowInv
	PORTE.OUTSET = (phaseOutputsELow & 0b11000000); // turn on all low bits in phaseOutputsEHigh	
	
	PORTF.OUTCLR = (phaseOutputsFHigh | 0b11110000); // turn off all high bits in phaseOutputsELowInv
	PORTF.OUTSET = (phaseOutputsFLow & 0b00001111); // turn on all low bits in phaseOutputsEHigh
}

ISR (TCF0_CCC_vect) {
	// motor 3 on pf4-7 and pd0-1
	
	PORTF.OUTCLR = (phaseOutputsFHigh | 0b00001111); // turn off all high bits in phaseOutputsELowInv
	PORTF.OUTSET = (phaseOutputsFLow & 0b11110000); // turn on all low bits in phaseOutputsEHigh	
	
	PORTD.OUTCLR = (phaseOutputsDHigh | 0b11111100); // turn off all high bits in phaseOutputsELowInv
	PORTD.OUTSET = (phaseOutputsDLow & 0b00000011); // turn on all low bits in phaseOutputsEHigh	
}

ISR (TCF0_CCD_vect) {
	// motor 4 on pd2-7
	
	PORTD.OUTCLR = (phaseOutputsDHigh | 0b00000011); // turn off all high bits in phaseOutputsELowInv
	PORTD.OUTSET = (phaseOutputsDLow & 0b11111100); // turn on all low bits in phaseOutputsEHigh	
}

// When TCF0 overflows, set all phase outputs to their commutation value
ISR (TCF0_OVF_vect) {
	PORTE.OUTCLR = phaseOutputsELow; // turn off all low bits in phaseOutputsELowInv
	PORTE.OUTSET = phaseOutputsEHigh; // turn on all high bits in phaseOutputsEHigh
	PORTF.OUTCLR = phaseOutputsFLow; // turn off all low bits in phaseOutputsFLowInv
	PORTF.OUTSET = phaseOutputsFHigh; // turn on all high bits in phaseOutputsFHigh
	PORTD.OUTCLR = phaseOutputsDLow; // turn off all low bits in phaseOutputsDLowInv
	PORTD.OUTSET = phaseOutputsDHigh; // turn on all high bits in phaseOutputsDHigh
}

// *************** /PWM **********************

// *************** PWM/2 **********************

// Configures PWM output on compare a b and c for single slope pwm, with hires, and clk source as sys clk
void configHalfPWMTimer (volatile TC0_t * tc, HIRES_t * hires, uint16_t period) {
	TC_SetPeriod (tc, period );
	TC0_ConfigWGM (tc, TC_WGMODE_NORMAL_gc ); // set to single slope pwm generation mode
	TC0_EnableCCChannels (tc, TC0_CCAEN_bm); // enable compare A
	TC0_EnableCCChannels (tc, TC0_CCBEN_bm); // enable compare B
	TC0_EnableCCChannels (tc, TC0_CCCEN_bm); // enable compare C
	TC0_EnableCCChannels (tc, TC0_CCDEN_bm); // enable compare D

	//~ TC0_SetCCAIntLevel (tc, TC_CCAINTLVL_HI_gc);
	TC0_SetCCBIntLevel (tc, TC_CCBINTLVL_LO_gc);
	//~ TC0_SetCCCIntLevel (tc, TC_CCCINTLVL_HI_gc);
	//~ TC0_SetCCDIntLevel (tc, TC_CCDINTLVL_HI_gc);
	
	TC0_SetOverflowIntLevel (tc, TC_OVFINTLVL_LO_gc);
	
	PMIC.CTRL |= PMIC_HILVLEN_bm;

	TC0_ConfigClockSource (tc, TC_CLKSEL_DIV1_gc);
	HIRES_Enable (hires, HIRES_HREN_TC0_gc);
	
}

ISR (TCE0_CCA_vect) {
	// half way into high part of motor 1 pwm
	//
}

ISR (TCE0_CCB_vect) {
	// half way into high part of motor 2 pwm
	// start adc conversion on adca ch1 (this channel is dedicated to this motor)
	ADC_Ch_InputMux_Config (&ADCA.CH1, motor2StateSenseIndex[motor2State]<<3, 0);
	ADC_Ch_Conversion_Start (&(ADCA.CH1));
}

ISR (TCE0_CCC_vect) {
	// half way into high part of motor 3 pwm
	//
}

ISR (TCE0_CCD_vect) {
	// half way into high part of motor 4 pwm
	//
}

ISR (TCE0_OVF_vect) {

}

// *************** /PWM/2 **********************

// *************** Commutation Timing **********************

void configMotTimer0 (volatile TC0_t * tc) {

}

void configMotTimer1 (volatile TC1_t * tc) {

}

ISR (TCC1_CCA_vect) {
// time to change motor1 state
}

ISR (TCD1_OVF_vect) {
	if (missedCommFlag) {
		spiWriteBuffer[2] = 0xFF;
		spiWriteBuffer[4] = globalResult;
	}
	
	missedCommFlag = 1; // should be set back to 0 before we get here again
	
	TCD1.PER = 65535;
	// time to change motor2 state
	if (motor2State < 5)
		motor2State++;
	else
		motor2State=0;

	risingReadyFlag = 0;
	fallingReadyFlag = 0;
	risingCount = 0;
	fallingCount = 0;
	passedCenterFlag = 0;
	setMotor2State(motor2State);
}

ISR (TCE1_CCA_vect) {
// time to change motor3 state
}

ISR (TCD0_CCA_vect) {
// time to change motor4 state
}

// *************** /Commutation Timing **********************

void initAdc (ADC_t * adc) {
	ADC_CalibrationValues_Load (adc);
  	ADC_ConvMode_and_Resolution_Config (adc, ADC_ConvMode_Unsigned, ADC_RESOLUTION_8BIT_gc);
	ADC_Prescaler_Config (adc, ADC_PRESCALER_DIV16_gc); // Fadc = 250khz
	ADC_Reference_Config (adc, ADC_REFSEL_INT1V_gc); // vref = internal 1v
    
    /* Setup channel 0, 1, 2 and 3 to have single ended input and 1x gain. */
	ADC_Ch_InputMode_and_Gain_Config (&(adc->CH0),
	                                 ADC_CH_INPUTMODE_SINGLEENDED_gc,
	                                 ADC_CH_GAIN_1X_gc);

	ADC_Ch_InputMode_and_Gain_Config (&(adc->CH1),
	                                 ADC_CH_INPUTMODE_SINGLEENDED_gc,
	                                 ADC_CH_GAIN_1X_gc);

	ADC_Ch_InputMode_and_Gain_Config (&(adc->CH2),
	                                 ADC_CH_INPUTMODE_SINGLEENDED_gc,
	                                 ADC_CH_GAIN_1X_gc);

	ADC_Ch_InputMode_and_Gain_Config (&(adc->CH3),
	                                 ADC_CH_INPUTMODE_SINGLEENDED_gc,
	                                 ADC_CH_GAIN_1X_gc);

	/* Enable high level sample complete interrupt for channel 3 */
	ADC_Ch_Interrupts_Config (&(adc->CH0), ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
	ADC_Ch_Interrupts_Config (&(adc->CH1), ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
	ADC_Ch_Interrupts_Config (&(adc->CH2), ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
	ADC_Ch_Interrupts_Config (&(adc->CH3), ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);

	PMIC.CTRL |= PMIC_HILVLEN_bm; // Enable low level interrupts
	ADC_Enable (adc); // Enable ADC A with free running mode
	ADC_Wait_32MHz (adc); // Wait until common mode voltage is stable
}

#define min(a,b) ((a)<(b))?(a):(b)

ISR (ADCA_CH1_vect) {
	ADCA.CH1.INTFLAGS = ADC_CH_CHIF_bm; // clear interrupt flag
	uint8_t result = ADCA.CH1.RES;
	globalResult = result;
	
	if (!passedCenterFlag) {
        /* rising */
		if (stateSlope[motor2State])
        { 
			if (risingCount > 0) 
            {
				if (result > motor2Thresh) 
                {
					if( TCD1.CNT < 32768)
						TCD1.PER = TCD1.CNT * 2;
					else
						TCD1.PER = 65535;
						
					missedCommFlag = 0;
					passedCenterFlag = 1;
				}
			} else { risingCount++; }
		} 
        /* falling */
        else 
        { 
			if (fallingCount > 0) 
            {
				if (result < motor2Thresh) 
                {
					if( TCD1.CNT < 32768)
						TCD1.PER = TCD1.CNT * 2;
					else
						TCD1.PER = 65535;
					
					missedCommFlag = 0;
					passedCenterFlag = 1;
				}
			} else { fallingCount++; }
		}
	}
}

void configDelayTimer (volatile TC0_t * tc) {
	TC_SetPeriod (tc, (uint16_t)65535); // set tc period
	TC0_ConfigWGM (tc, TC_WGMODE_NORMAL_gc); // normal timer countermode
	TC0_ConfigClockSource (tc, TC_CLKSEL_DIV1024_gc);
}

void startup(void) {
	
	SET_PHASE_STATE_5_MOT2();
	TCC0.CNT = 0;
	while (TCC0.CNT < 65535) {}
	
    /*
	TCF0.CCBBUF = startupPwms[0];
	SET_PHASE_STATE_0_MOT2();
	TCC0.CNT = 0;
	while (TCC0.CNT < startupDelays[0]) {}
	
	TCF0.CCBBUF = startupPwms[1];
	SET_PHASE_STATE_1_MOT2();
	TCC0.CNT = 0;
	while (TCC0.CNT < startupDelays[1]) {}
	
	TCF0.CCBBUF = startupPwms[2];
	SET_PHASE_STATE_2_MOT2();	
	TCC0.CNT = 0;
	while (TCC0.CNT < startupDelays[2]) {}
		
	TCF0.CCBBUF = startupPwms[3];
	SET_PHASE_STATE_3_MOT2();
	TCC0.CNT = 0;
	while (TCC0.CNT < startupDelays[3]) {}
	
	TCF0.CCBBUF = startupPwms[4];
	SET_PHASE_STATE_4_MOT2();
	TCC0.CNT = 0;
	while (TCC0.CNT < startupDelays[4]) {}
    */

	TCF0.CCBBUF = startupPwms[5];
	TCE0.CCBBUF = startupPwms[5]/2;

	TC_SetPeriod( &TCD1, 65535 );
	//~ xxx TC1_ConfigClockSource( &TCD1, TC_CLKSEL_DIV64_gc );
	TC1_ConfigClockSource( &TCD1, TC_CLKSEL_DIV4_gc );
	//TC1_ConfigClockSource( &TCD1, TC_CLKSEL_DIV64_gc);
	TC1_SetOverflowIntLevel (&TCD1, TC_OVFINTLVL_LO_gc);
	
	missedCommFlag = 1;
	
    SET_PHASE_STATE_0_MOT2();
	motor2State = 0;
	
	//~ while (TCC0.CNT < startupDelays[5]) {}
	//~ while (1);
}

void setMotor1State (uint8_t state) {
	
}

void setMotor2State (uint8_t state) {
	if (state == 0) {
		SET_PHASE_STATE_0_MOT2();
	} else if (state == 1) {
		SET_PHASE_STATE_1_MOT2();
	} else if (state == 2) {
		SET_PHASE_STATE_2_MOT2();
	} else if (state == 3) {
		SET_PHASE_STATE_3_MOT2();
	} else if (state == 4) {
		SET_PHASE_STATE_4_MOT2();
	} else if (state == 5) {
		SET_PHASE_STATE_5_MOT2();
	}
}

void setMotor3State (uint8_t state) {
	
}

void setMotor4State (uint8_t state) {
	
}
