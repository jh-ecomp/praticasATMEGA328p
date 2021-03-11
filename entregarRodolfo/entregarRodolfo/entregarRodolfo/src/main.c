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
#define PARE 0
#define SEGUNDOS 1
#define PRATICA2 0
#define PRATICA3 1
#define POTH1 0
#define UNITPOTH1 1
#define POTH2 2
#define UNITPOTH2 3

void global_setup();
void pratica2_port_setup();
void pratica3_port_setup();


//	variaveis inteiras que controlam o comportamento
int muxcont = 0, select_behavior = 0;
int pinc_value = 0;

char display[2];

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
	0b01100011, // º
	0b00111001, // C
	0b00111000, // L
	0b00011100};// u

char mux_portc[] = {
	0b00110100,	//	PC1 e PC0 em low level, side 1 aceso, os dois butões em pull up
	0b00111000,	//	PC1 e PC0 em low level, side 2 aceso, os dois butões em pull up
	0b00100100,	//	PC1 e PC0 em low level, side 1 aceso, botão de comportamento em pull up botão pedestre desabilitado
	0b00101000,	//	PC1 e PC0 em low level, side 2 aceso, botão de comportamento em pull up botão pedestre desabilitado
};


/*-------------------------------------------------------*/

//	variáveis para a pratica 2
int display_type = 0, cronometro = 0;
int flag_pare = 0, flag_segundos = 0;
int conta_sec = 0, selecionaSEM = 0;
int ped_btn = 0, led_ped = 0;
int tempos[] = {5, 5, 5, 5}; // os tempos seguem a ordem: GRE via1, YEL via1, GRE via2, YEL via2

char SEM[] = {
	0b00001100, // GRE via1 and RED via2
	0b00001010, // YEL via1 and RED via2
	0b00100001, // GRE via2 and RED via1
	0b00010001, // YEL via2 and RED via1
	0b00001001, // RED via1 and RED via2
};

	
/*-------------------------------------------------------*/

//	variáveis inteiras para a pratica 3
int sensorvalue = 0, showvalue = 0, state = 0;
/*-------------------------------------------------------*/









/*  ----------                   PCINT1 Interruption                  -----------------*/
ISR(PCINT1_vect){
	pinc_value = PINC;
	if(PINC5 == 0){	//	checa se o valor do PORTC5 é 0
		select_behavior = (select_behavior + 1) % 2;
		if(select_behavior == PRATICA2)
			pratica2_port_setup();
		else
			pratica3_port_setup();
	}else{
		if(pinc_value & 0x10 == 0){	//	checa se o valor do PORTC4 é 0
			if (display_type == PARE){
				if(selecionaSEM == 0 || selecionaSEM == 2){
					ped_btn = 1;
					conta_sec = tempos[selecionaSEM];
				}
			}
		}
	}
}
/*  ----------                   OVERFLOW OF TIMER 0                  -----------------*/
ISR(TIMER0_OVF_vect){
	
	PORTC = select_behavior == PRATICA2 ? mux_portc[muxcont]: mux_portc[muxcont + 2];
	
	
	PORTD = display[muxcont];
	if(led_ped == PARE)
		PORTD |= (1 << PD7);
	
	muxcont = (muxcont + 1) % 2; //	LIMIT THE VALUE OF muxcont IN 0 OR 1	
}


/*  ----------                   OVERFLOW OF TIMER 1                  -----------------*/
ISR(TIMER1_OVF_vect){
	
	switch(select_behavior){
		case PRATICA2:	//	Prática 2 executando
		
			switch(display_type){
				case PARE:
					led_ped = PARE;
					display[0] = DS7[10 + flag_pare];
					display[1] = DS7[11 + flag_pare];
					PORTD = (1 << PD7);
					flag_pare = (flag_pare + 2) % 4;
					
					PORTB = SEM[selecionaSEM];
					
					
					if (conta_sec >= tempos[selecionaSEM]){
						selecionaSEM = (selecionaSEM + 1) % 4;
						conta_sec = 0;
						
						if(flag_segundos){
							display_type = SEGUNDOS;
							flag_segundos = 0;
						}
						
						if(ped_btn){
							flag_segundos = 1;
							ped_btn = 0;
						}
					}
					conta_sec++;	
				break;
				case SEGUNDOS:
					led_ped = SEGUNDOS;
					PORTB = SEM[4];
					
					display[0] = DS7[cronometro / 10];
					display[1] = DS7[cronometro % 10];
					PORTD = (0 << PD7);
					
					if(cronometro >= 10){
						cronometro = 0;
						display_type = PARE;
						flag_pare = 0;
					}else{
						cronometro++;
					}
					
				break;
			}
				
		break;
		case PRATICA3:	//	Prática 3 executando
				
				switch(state){
					case POTH1:
					display[0] = DS7[showvalue / 10];
					display[1] = DS7[showvalue % 10];
					//	ON FIRST SECOND THE DISPLAY PRINTS THE VALUE OF SENSOR1
					break;
					case UNITPOTH1:
					display[0] = DS7[14];
					display[1] = DS7[15];
					ADMUX  = 0b11100001;
					break;
					//	HERE THE DISPLAY PRINTS ºC
					case POTH2:
					display[0] = DS7[showvalue / 10];
					display[1] = DS7[showvalue % 10];
					//	ON THIRD SECOND THE DISPLAY PRINTS THE VALUE OF SENSOR2
					break;
					case UNITPOTH2:
					display[0] = DS7[16];
					display[1] = DS7[17];
					ADMUX  = 0b11100000;
					//	HERE THE DISPLAY PRINTS Lu
					break;
				}
				state = (state + 1) % 4;
		break;
	}
}

/*------------		ADC Interruption		--------------*/

ISR(ADC_vect){
	sensorvalue =  (99 * ADCH) / 255;
}

void global_setup(){
	select_behavior = PRATICA2;
	
	// Timer0 Config - 244Hz
	TCCR0A = 0x00;
	TCCR0B = 0x04;
	TIMSK0 = 0x01;
	TCNT0 = 0x0000;
	
	// Timer1 Config - 1Hz
	TCCR1A = 0x00;
	TCCR1B = 0x04;
	TIMSK1 = 0x01;
	TCNT1 = 0x0000;
	
	// Port Config
	
	DDRD = 0xff;	//	todos os pinos como saída
	
	//	Pin Changing Interruption Config
	PCICR = 0x02;
	PCIFR = 0x07;
	PCMSK1 = 0x30;
	
	//	ADC CONFIG	
	ADMUX  = 0b11100000;	/*	INTERNAL 1.1V VOLTAGE REFERENCE, LEFT ADJUST, INPUT CHANEL SELECT = ADC0	*/
	ADCSRB = 0b00000100;	/*	ANALOG COMPARATOR OFF, TRIGGER BY TIMER0 OVERFLOW	*/
	ADCSRA = 0b11101000;	/*	ENABLE ADC, ENABLE ADC AUTO TRIGGER, SET 2 TO DIVISION FACTOR BETWEEN SYSTEM CLOCK FREQUENCY
							AND INPUT CLOCK TO THE ADC*/
	
}

void pratica2_port_setup(){

	DDRB = 0x3f;	//	todos os pinos como saída exceto pelos pinos do cristal - PB6 e PB7
	PORTB = 0x00;
	
	DDRC = 0x0c;	//	pinos PC3 e PC2 como saída, demais como entrada
	
	PORTC = mux_portc[0];

	display_type == PARE;
}


void pratica3_port_setup(){
	
	
	DDRB = 0x00;	//	todos os pinos como entrada
	PORTB = 0x00;
	
	DDRC = 0x1c;	//	pino PC4, PC3 e PC2 como saída, demais como entrada
	PORTC = mux_portc[2];
	
	PORTD = display[0];
	
	display_type == SEGUNDOS;
	muxcont = 0;
	
}


int main (void)
{

	global_setup();
	pratica2_port_setup();
	
	sei();
	
	while(1){
		
	}
}
