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
#define FOSC 16 000 000 // Clock Speed
#define BAUD 9600
#define MYUBRR ((FOSC / 16) / BAUD) - 1


unsigned int baud_rate = 0;


void setup();
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );



int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	setup();
	sei();
	unsigned char carac;
	
	while(1){
		USART_Receive();
		carac = 'a';
		USART_Transmit(carac);
	}
	

	/* Insert application code here, after the board has been initialized. */
}


void setup(){
	
	//	PORTD Config
	DDRD = 0x02;
	PORTD = 0xfc;
	
	//	TIMER 0 Config
	TCCR0A = 0x00;
	TCCR0B = 0x04;
	TIMSK0 = 0x01;
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

