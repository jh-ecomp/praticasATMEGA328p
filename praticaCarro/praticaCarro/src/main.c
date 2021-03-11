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
#define ONLINE 1;
#define OUTLINE 0;
#define cpl_bit(y,bit) (y^=(1<<bit))
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR ((FOSC / 16) / BAUD) - 1

unsigned int baud_rate = 0;
int senon = 1;
volatile uint8_t last_PINB = 0x00;
uint8_t changed_bits;
int sensorE = OUTLINE, sensorR = ONLINE;



void setup();
void USART_Transmit( unsigned char data);
unsigned char USART_Receive();

ISR(PCINT0_vect){
	changed_bits = PINB ^ last_PINB;
	last_PINB = PINB;
	
	//sensores de obstáculos
	if ((changed_bits & (1 << PINB0)) ||  (changed_bits & (1 << PINB1)) ){
		if ((PINB & (1 << PINB0)) && (PINB & (1 <<PINB1))) {
			//sensores frente não detectam obstáculo as rodas obedecem aos sensores seguidores de linha
			senon = 1;
		}else{
			//se ao menos um dos sensores de obstáculos detectar um obstáculo as rodas param
			OCR0A =  0xff;
			OCR0B =  0xff;
			senon = 0;
		}
	}
	
	//sensores seguidores de linha
	if(senon){
		if (changed_bits & (1 << PINB4)){
			if (PINB & (1 << PINB4)) 	// sensor esquerda na linha
				sensorE = ONLINE
			else
				sensorE = OUTLINE;
		}
	
		if (changed_bits & (1 << PINB5)){
			if (PINB & (1 << PINB5))	// sensor direita na linha
				sensorR = ONLINE;
			else
				sensorR = OUTLINE;
		}
		
		if((sensorE == OUTLINE) && (sensorR == OUTLINE)){
			OCR0A = 0x00;
			OCR0B = 0xff;
		}
		
		if(sensorE == OUTLINE)
			OCR0B = 0x80;	//com o sensor da esquerda fora da linha a roda da direita tem velocidade de 50%
		else{
			if(sensorR == OUTLINE)
				OCR0A = 0x80;	//com o sensor da direita fora da linha a roda da esquerda tem velocidade de 50%
			else
				OCR0A = 0x4b;
				OCR0B = 0x4b;
		}
	
	}

}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	setup();
	
	unsigned char caractere;
	
	while(1){
		caractere = USART_Receive();
		switch(caractere){
			case 49:
				OCR0A = 0x4b;
				break;
			case 48:
				OCR0A = 0xff;
				break;
			default:
				USART_Transmit(caractere + 1);
		}
		
	}


	/* Insert application code here, after the board has been initialized. */
}

void setup(){
	cli();
	
	DDRB = 0x00
	PORTB = 0x00;
	
	
	DDRD = 0xfd;
	PORTD = 0x00;
	
	
	
	//	TIMER 0 Config, PWM fast
	TCCR0A = 0b11000011;
	TCCR0B = 0x04;
	TIMSK0 = 0x00;
	OCR0A = 0xff;
	TCNT0 = 0x00;
	
	
	
	// USART Config
	baud_rate = MYUBRR;
	/*setando o baud rate*/
	UBRR0H = (unsigned char) (baud_rate>>8);
	UBRR0L = (unsigned char) baud_rate;
	/*habilitando o TX e RX*/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	/*setando o formato do frame*/
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
	
	
	sei();
}



void USART_Transmit( unsigned char data ){
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void ){
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}
