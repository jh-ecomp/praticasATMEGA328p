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
#include <asf.h>
#define cpl_bit(y,bit) (y^=(1<<bit))
int cont = 0;


ISR(TIMER0_OVF_vect){
	
	if(!cont){
		PORTB = 0b11111011;
		PORTD = 0b00000110; 
	}else{
		PORTB = 0b11111101;
		PORTD = 0b10111111; 
	}
	cont = (cont + 1) % 2;
}

ISR(TIMER1_OVF_vect){
	cpl_bit(PORTC, PC0);
}


void setup(){
	// Timer0 Config
	TCCR0A = 0x00;
	TCCR0B = 0x04;
	TIMSK0 = 0x01;
	TCNT0 = 0x0000;
	
	// Timer1 Config
	TCCR1A = 0x00;
	TCCR1B = 0x04;
	TIMSK1 = 0x01;
	TCNT1 = 0x0BDC;
	
	// Port Config
	DDRD = 0xff;
	PORTD = 0b00000000;
	
	DDRB = 0x06;
	PORTB = 0b11111001;
	
	DDRC = 0x01;
	PORTC = 0b11111110;
	
	
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	setup();
	sei();
	
	while (1)
	{
	}

	/* Insert application code here, after the board has been initialized. */
}
