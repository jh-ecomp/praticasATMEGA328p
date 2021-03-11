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
#define FAST 0
#define MEDIUM 1
#define SLOW 2
#define STOPPED 3
  
int intensity = FAST;

void setup();


ISR(TIMER1_OVF_vect){
	//*
	switch(intensity){
		case FAST:
			OCR0A = 0x00;
			break;
		case MEDIUM:
			OCR0A = 0xd5;
			break;
		case SLOW:
			OCR0A = 0xfa;
			break;
		case STOPPED:
			OCR0A = 0xff;
			break;
	}
	intensity = (intensity + 1) % 4;
	//*/
	
	cpl_bit(PORTD,PD2);
	
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	setup();
	sei();
	while(1){
			/* Insert application code here, after the board has been initialized. */
	}


}

void setup(){
	//*	Timer 2 Config - Fast PWM Mode
	TCCR0A = 0b11000011;
	TCCR0B = 0x01;
	TIMSK0 = 0x00;
	OCR0A = 0xff;
	TCNT2 = 0x00;
	//*/
	
	//	Timer 1 Config
	TCCR1A = 0x00;
	TCCR1B = 0x04;
	TIMSK1 = 0x01;
	TCNT1 = 0x0BDC;
	
	// PORTD CONFIG
	DDRD = 0b01000100;
	PORTD = 0b10111011;

	
}
