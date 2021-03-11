/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#define cpl_bit(y,bit) (y^=(1<<bit))
#define POTH1 0
#define UNITPOTH1 1
#define POTH2 2
#define UNITPOTH2 3

/*	GLOBAL VARIABLES	*/
int muxsensor = 0;
int muxcont = 0;	/*	SELECT muxPB AND display POSITION	*/
int state = POTH1;	/*	SELECT WHICH WILL BE PRINTED. SENSOR1, UNIT OF SENSOR1, SENSOR2, UNIT OF SENSOR2	*/
int sensorValue = 0, showValue = 0;
char D7S[] = {	0b00111111, // 0
				0b00000110, // 1
				0b01011011, // 2
				0b01001111, // 3
				0b01100110, // 4
				0b01101101, // 5
				0b01111101, // 6
				0b00000111, // 7
				0b01111111, // 8
				0b01101111, // 9
				0b01100011, // º
				0b00111001, // C
				0b00111000, // L
				0b00011100};// u

char muxPB[] = {
	0b11110100,		// PB[7-4] IN PULL UP, PB2 (D7S-Side2) OFF, PB1 (D7S-Side1) ON, PB0 PULL UP
	0b11111000		// PB[7-4] IN PULL UP, PB2 (D7S-Side2) ON, PB1 (D7S-Side1) OFF, PB0 PULL UP
};

char display[2];	/* THE D7S IN PORTD WILL PRINT THE VALUES IN THIS VECTOR. SIDE1 PRINTS THE display[0]'s VALUE
					   AND SIDE2 PRINTS display[1]'s VALUE*/
			
/*	FUNCTIONS	*/				


/*  ----------                   OVERFLOW OF TIMER 0                  -----------------*/

ISR(TIMER0_OVF_vect){
	
	PORTC = muxPB[muxcont];
	
	PORTD = display[muxcont];
	
	muxcont = (muxcont + 1) % 2; /*	LIMIT THE VALUE OF muxcont IN 0 OR 1	*/
}
//*/


/*------------		ADC Interruption		--------------*/

ISR(ADC_vect){
	sensorValue = (50 * ADCH) / 255;	
}

/*  ----------                   OVERFLOW OF TIMER 1                  -----------------*/
//*
ISR(TIMER1_OVF_vect){
	
	showValue = sensorValue;
	
	switch(state){
		case POTH1:
			display[0] = D7S[showValue / 10];
			display[1] = D7S[showValue % 10];
			//	ON FIRST SECOND THE DISPLAY PRINTS THE VALUE OF SENSOR1	
			break;
		case UNITPOTH1:
			display[0] = D7S[10];
			display[1] = D7S[11];
			ADMUX  = 0b11100001;
			break;
			//	HERE THE DISPLAY PRINTS ºC	
		case POTH2:
			display[0] = D7S[showValue / 10];
			display[1] = D7S[showValue % 10];
			//	ON THIRD SECOND THE DISPLAY PRINTS THE VALUE OF SENSOR2	
			break;
		case UNITPOTH2:
			display[0] = D7S[12];
			display[1] = D7S[13];
			ADMUX  = 0b11100000;
			//	HERE THE DISPLAY PRINTS Lu	
		break;		
	}
	state = (state + 1) % 4;
}
//*/

void setup(){
/*	PORT CONFIG	*/

/*	PORTD - USED TO PRINT ON 7 SEGMENT DISPLAY*/
	DDRD = 0x7f; /*	SET PINS D6-D0 AS OUTPUT	*/
	PORTD = 0b00000000; /*	TURN OFF DISPLAY	*/
	
/*	PORTC - USED TO RECEIVE THE SENSORS'S DATA	*/
	DDRC  = 0b00001100; /*	SET ALL PINS AT INPUT	*/
	PORTC = 0b0011 0100; /*	PUT ALL PINS IN LOW LEVEL	*/
	
/*	ADC CONFIG	*/
	ADMUX  = 0b00100000;	/*	INTERNAL 1.1V VOLTAGE REFERENCE, LEFT ADJUST, INPUT CHANEL SELECT = ADC1	*/
	ADCSRB = 0b00000100;	/*	ANALOG COMPARATOR OFF, TRIGGER BY TIMER0 OVERFLOW	*/
	ADCSRA = 0b11101000;	/*	ENABLE ADC, ENABLE ADC AUTO TRIGGER, SET 2 TO DIVISION FACTOR BETWEEN SYSTEM CLOCK FREQUENCY
							AND INPUT CLOCK TO THE ADC*/

/*	TIMERS CONFIG	*/
	
	/************TIMER 0 CONFIG************/
	//*
	TCCR0A = 0x00;	//OC0A and OC0B disconnected;
	TCCR0B = 0x04;	// TC0 with Prescaler = 256 and capture at the falling edge. 
	TCNT0  = 0x00;	// Start timer 0 with zero. 
	TIMSK0 = 0x01;


/************TIMER 1 CONFIG************/
//Timer/Counter Control Register A | COM0A1 COM0A0 COM0B1 COM0B0 -- WGM01 WGM00
TCCR1A = 0x00;	//OC0A and OC0B disconnected;
//Timer/Counter Control Register B | FOC0A FOC0B -- WGM02 CS02 CS01 CS00
TCCR1B = 0x04;	/* TC1 with Prescaler = 256 and capture at the falling edge. */
TCNT1 = 0x00;	/* Start timer 1 with zero. */
TIMSK1 = 0x01;
	
}


int main (void)
{
	setup();
	sei();
	/* Insert application code here, after the board has been initialized. */
	display[0] = D7S[0];
	display[1] = D7S[0];
	
	while(1){
	};
}
