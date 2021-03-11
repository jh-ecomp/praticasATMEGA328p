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
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR ((FOSC / 16) / BAUD) - 1

int sensor_value[2] = {0,0};
unsigned int baud_rate = 0;
int sensor_ind = 0, tick_count = 0;


void setup();
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );

ISR(TIMER0_OVF_vect){
	
	/*
	switch(tick_count){
		case 0:
			ADMUX = 0b00100000;
			break;
		case 2:
			ADMUX = 0b00100001;
			break;
	}
	//*/
	
	tick_count = (tick_count + 1) % 4;
	
	
}

ISR(ADC_vect){
	
	sensor_value[sensor_ind] =  (99 *ADCH) / 255;

	//sensor_ind = (sensor_ind + 1) % 2;
	
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	unsigned char caractere;
	setup();
	sei();
	while(1){
		caractere = USART_Receive();
		switch(caractere){
			case 'A':
				cpl_bit(PORTB,PB0);
				break;
			case 'B':
				cpl_bit(PORTB,PB1);
				break;
			case 'C':
				cpl_bit(PORTB,PB2);
				break;
			case 'T':
				USART_Transmit((sensor_value[0] / 10) + '0');
				USART_Transmit((sensor_value[0] % 10) + '0');
				USART_Transmit(167);
				USART_Transmit('C');
				USART_Transmit(13);
				break;
			case 'L':
				USART_Transmit((sensor_value[1] / 10) + '0');
				USART_Transmit((sensor_value[1] % 10) + '0');
				USART_Transmit('L');
				USART_Transmit('u');
				USART_Transmit(13);
				break;
			case 49:
				OCR2A = 0xfa;
				break;
			case 50:
				OCR2A = 0xd5;
				break;
			case 51:
				OCR2A = 0x00;
				break;
			case 48:
				OCR2A = 0xff;
				break;
			default:
				USART_Transmit(caractere + 1);
				
		}
	}

	/* Insert application code here, after the board has been initialized. */
}


void setup(){
	
	
	
//	PORT Config
	DDRD = 0x02;
	PORTD = 0xfc;
	
	DDRB = 0x0f;
	PORTB = 0x30;
	
	DDRC = 0x00;
	PORTC = 0x3c;
	
/*	ADC CONFIG	*/
	ADMUX  = 0b01100001;	/*	LEFT ADJUST, INPUT CHANEL SELECT = ADC1	*/
	ADCSRB = 0b00000100;	/*	ANALOG COMPARATOR OFF, TRIGGER BY TIMER0 OVERFLOW	*/
	ADCSRA = 0b11101000;	/*	ENABLE ADC, ENABLE ADC AUTO TRIGGER, SET 2 TO DIVISION FACTOR BETWEEN SYSTEM CLOCK FREQUENCY
							AND INPUT CLOCK TO THE ADC*/
	
//	TIMER 0 Config
	TCCR0A = 0x00;
	TCCR0B = 0x04;
	TIMSK0 = 0x01;
	TCNT0 = 0x00;
	
//	TIMER 2 Config
	TCCR2A = 0b11000011;
	TCCR2B = 0x01;
	TIMSK2 = 0x00;
	OCR2A = 0xff;
	TCNT2 = 0x00;

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
