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
#define PARE 0
#define SECS 1

char TMR2_soft_postscaller = 0;

char DS7[] = {  //-gfedcba
	0b00111111, //0
	0b00000110, //1
	0b01011011, //2
	0b01001111, //3
	0b01100110, //4
	0b01101101, //5
	0b01111101, //6
	0b00000111, //7
	0b01111111, //8
	0b01100111, //9
	0b01110011, //p
	0b01110111, //a
	0b01010000, //r
	0b01111001, //e
	
};

char SEM[] = {
	0b00001100, // GRE via1 and RED via2
	0b00001010, // YEL via1 and RED via2
	0b00100001, // GRE via2 and RED via1
	0b00010001, // YEL via2 and RED via1
	0b00001001, // RED via1 and RED via2
};

char display[2];
int contsec = 0;
int muxcont = 0; // acende o Side1 do display
int wtp = PARE;
int flagPARE = 0;
int flagBTN = 0;
int flagSECS = 0;
int led_ped = SECS;

int selecionaSEM = 0;
int contsecSEM = 0;

int tempos[] = {5, 5, 5, 5}; /* os tempos seguem a ordem: GRE via1, YEL via1,
										GRE via2, YEL via2						*/


char MUX[] = {	//display a ou b, setado no PORTC
	0b00110100,
	0b00111000
};



//SELF RELOAD TIMER 0 interrupt


/*  ----------                   FUNÇÃO QUE O PARNAÍBA FEZ                  -----------------*/
ISR(TIMER0_OVF_vect){
	
	PORTC = MUX[muxcont];// Aqui a variável muxcont é usada para 
	//escolher qual lado do display será aceso e se o LEDped estará aceso ou apagado
	
	
	PORTD = display[muxcont]; // O vetor display contém os caracteres a serem exibidos no display
	if(led_ped == PARE)
		PORTD |= (1 << PD7); 
	
	muxcont = (muxcont + 1) % 2; // controle da variável muxcont, valores possíveis: 0 e 1
}


// Quando apertar o Botão
ISR(PCINT1_vect){
		if(wtp == PARE){
			if(selecionaSEM == 0 || selecionaSEM == 2){
				flagBTN = 1;
				contsecSEM = tempos[selecionaSEM];
			}
			
		}
}


//SELF RELOAD TIMER 2 interrupt

//*  ----------                   FUNÇÃO QUE O PARNAÍBA FEZ                  -----------------*
ISR(TIMER1_OVF_vect){

		if(wtp == PARE){
			led_ped = PARE;
			display[0] = DS7[10 + flagPARE];
			display[1] = DS7[11 + flagPARE];
			flagPARE = (flagPARE + 2) % 4;
			
			PORTB = SEM[selecionaSEM];
			
			contsecSEM++;
			if (contsecSEM >= tempos[selecionaSEM]){
				selecionaSEM = (selecionaSEM + 1) % 4;
				contsecSEM = 0;
				if(flagSECS){
					wtp = SECS;
					flagSECS = 0;
					flagBTN = 0;
				}
				if(flagBTN)
					flagSECS = 1;
			}
				
			
				
		}else{
			PORTB = SEM[4];
			led_ped = SECS;
			display[0] = DS7[contsecSEM / 10];
			display[1] = DS7[contsecSEM % 10];
			contsecSEM++;
			
			if(contsecSEM >= 20)
				cpl_bit(PORTD, PD7);
			
			if (contsecSEM >= 30){
				contsecSEM = 0;
				wtp = PARE;
				flagPARE = 2;
			}
			
			
		}
		contsec = 0;

		
}


void setup_uc(void){
		DDRB =  0x3f; // setando as portas estão sendo usadas pb0 até pb5
		DDRC =  0b00001100; 
		DDRD =  0xff; // Setando do pd0 até o pd6
		
		PORTC = 0b11110100;
		
		PORTC = MUX[0];
		PORTB = SEM[0];

		
	PCICR = 0x02;
	PCIFR = 0x07;
	PCMSK1 = 0x10;
	
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
	led_ped = PARE;
}


int main(void){
	setup_uc();

	sei();

	while(1){
		
	}
}
