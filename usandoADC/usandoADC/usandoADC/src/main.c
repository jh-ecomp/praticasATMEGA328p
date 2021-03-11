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
int muxcont = 0, wtp = 0, sensor_value = 0, contADC = 0;
char display[2];

char MUX[] = {	//display a ou b, setado no PORTB
	0b11111011,
	0b11111101 
};

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
	

ISR(TIMER1_OVF_vect){

	switch(wtp){
		case 0:
			display[0] = D7S[sensor_value / 10];
			display[1] = D7S[sensor_value % 10];
			break;
		case 1:
			display[0] = D7S[10];
			display[1] = D7S[11];
			break;		
	}
	
	wtp = (wtp + 1) % 2;

}
	

ISR(TIMER0_OVF_vect){
	
	PORTB = MUX[muxcont]; // Aqui a variável muxcont é usada para escolher qual lado do display 
	
	
	PORTD = display[muxcont]; // O vetor display contém os caracteres a serem exibidos no display
	
	muxcont = (muxcont + 1) % 2; // controle da variável muxcont, valores possíveis: 0 e 1
	
	if(contADC >= 8){
		contADC = 0;
		sensor_value = ADCH;
	}else{
		contADC++;
	}
}
void setup(){
	
	//*	Timer0 Config
		TCCR0A = 0x00;
		TCCR0B = 0x04;
		TIMSK0 = 0x01;
		TCNT0 = 0x0000;
	//*/	
	
		// Timer1 Config
		TCCR1A = 0x00;
		TCCR1B = 0x04;
		TIMSK1 = 0x01;
		TCNT1 = 0x0BDC;
		
		/*	Timer2 Config
		TCCR2A = 0x00;
		TCCR2B = 0x04;
		TIMSK2 = 0x01;
		TCNT2 = 0x0000;
		//*/
		
		//*	ADC Config
		ADMUX  = 0b00100000;	//	INTERNAL 1.1V VOLTAGE REFERENCE, LEFT ADJUST, INPUT CHANEL SELECT = ADC0	
		ADCSRB = 0b0000100;	//	ANALOG COMPARATOR OFF, TRIGGER BY TIMER0 OVERFLOW	
		ADCSRA = 0b11111000;	//	ENABLE ADC, ENABLE ADC AUTO TRIGGER, SET 2 TO DIVISION FACTOR BETWEEN SYSTEM CLOCK FREQUENCY
								//	AND INPUT CLOCK TO THE ADC
		//*/
		
		
		
		// Port Config
		DDRD = 0xff;
		PORTD = 0b00000000;
		
		DDRB = 0x06;
		PORTB = 0b11111001;
		
		DDRC = 0x00;
		PORTC = 0b11111110;
		
		display[0] = D7S[0];
		display[1] = D7S[1];
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	setup();
	sei();
	/* Insert application code here, after the board has been initialized. */
	
	while (1){
		
	}
	
}
