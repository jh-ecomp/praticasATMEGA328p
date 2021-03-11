#include <avr/io.h>
#include <avr/interrupt.h>
#define cpl_bit(y,bit) (y^=(1<<bit))

unsigned int tempoMOD;

char vetorDisplay[] = {	0b00111111, // 0
						0b00000110, // 1
						0b01011011, // 2
						0b01001111, // 3
						0b01100110, // 4
						0b01101101, // 5
						0b01111101, // 6
						0b00000111, // 7
						0b01111111, // 8
						0b01101111};// 9

ISR(TIMER1_CAPT_vect){
	
	tempoMOD = ICR1%10; // pega o tempo do sistema e pega o resto da divisão por 10
	
	cpl_bit(PORTB, PB1);
	
	switch (tempoMOD){
		case 0:
		PORTD = vetorDisplay[0];
		PORTD &= ~(1 << PORTD6);
		break;
		case 1:
		PORTD = vetorDisplay[1];
		PORTD &= ~(1 << PORTD6);
		break;
		case 2:
		PORTD = vetorDisplay[2];
		PORTD |= (1 << PORTD6);
		break;
		case 3:
		PORTD = vetorDisplay[3];
		PORTD |= (1 << PORTD6);
		break;
		case 4:
		PORTD = vetorDisplay[4];
		PORTD |= (1 << PORTD6);
		break;
		case 5:
		PORTD = vetorDisplay[5];
		PORTD |= (1 << PORTD6);
		break;
		case 6:
		PORTB = vetorDisplay[6];
		PORTD |= (1 << PORTD6);
		break;
		case 7:
		PORTD = vetorDisplay[7];
		PORTD &= ~(1 << PORTD6);
		break;
		case 8:
		PORTD = vetorDisplay[8];
		PORTD |= (1 << PORTD6);
		break;
		case 9:
		PORTD = vetorDisplay[9];
		PORTD |= (1 << PORTD6);
		break;
	}
}

ISR(TIMER1_OVF_vect){
	cpl_bit(PORTB, PB5);
}

int main(void)
{
	DDRB  = 0x22; /* Set PB5 and PB1 as output */
	PORTB = 0b11011101; /* Turn off LEDS and enable pull-up on the other pins */
	
	
	
	
	
	TCNT1 = 0x00;	/* Start timer 1 with zero. */
	TCCR1B = 0x03;	/* TC1 with Prescaler = 64 and capture at the falling edge. */
	TIMSK1 = 0x21;

	
	// aqui que o programa começa a fazer algo
	DDRD  = 0xFF; /* Set all PORTD as output */
	
	PORTD = 0b00111111; // initialize dysplay with '0'
	PORTD &= ~(1 << PORTD6);
	sei();

	while(1){}
}