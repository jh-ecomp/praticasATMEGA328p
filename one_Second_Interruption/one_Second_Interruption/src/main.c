/**

	Esta aplica��o tem como objetivo provocar uma interrup��o por timer.
	O timer dever� prover uma interrup��o a cada segundo.
	Para isso usaremos o TIMER 1.

 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#define cpl_bit(y,bit) (y^=(1<<bit))	//	essa marcar� faz um toogle no valor do pino indicado



void setup_Global(){
	
//	Configurando TIMER1
	TCCR1A = 0x00;	//	modo de compara��o desligado
	TCCR1B = 0x04;	//	sem PWM e prescale 256
	TIMSK1 = 0x01;	//	interrup��o por overflow habilitada
	TCNT1  = 0x0000;	//	valor de onde o contador vai come�ar
/*
	Temos um cristal de 16 MHz. Os Timers funcioanam como divisores de frequ�ncia, portanto temos o resultado da divis�o em Hertz.
	O nosso primeiro divisor � o prescale. Portanto vamos dividir a frequencia do cristal por 1024. 16 Mi dividido por 1024 �
	igual a 15.625. Agora o segundo divisor � o nosso contador.
	O nosso contador � crescente e conta at� o valor m�ximo para um registrador de 16 bits, 65.536.
	Para que ele conte apenas 15.625 precisamos dar um valor inicial diferente de 0 e que ao final da contagem o total contado
	seja 15.625. Para isso vamos subtrair o valor total do valor que queremos contar e assim teremos um valor inicial.
	65.536 - 15.625 = 49.911. 49.911 em Hexadecimal  = C2F7.
	Assim temos um gerador de interrup��o na frequ�ncia de 1 Hz
*/

//	Configurando Portas
	DDRB = 0x02;	//	Estamos usando apenas o PB1 como sa�da
	PORTB = 0b11111101;	//	Vamos colocar o valor da saida pra 0, assim nosso LED ficar� apagado. As outras portas est�o setadas
//	como entrada, vamos aproveitar para coloca-las em pull up.
	
}

int cont = 0;
//	Agora vamos definir a fun��o de interrup��o por overflow do timer1

ISR(TIMER1_OVF_vect){

		cpl_bit(PORTB, PB1);
//	vamos simplesmente fazer um xor do valor do PB1 com 1, assim se ele for 0 vai pra 1, e se for 1
//	vai pra 0. Isso dever� nos mostrar um LED que fica acesso por 1 segundo e depois fica apagado por 1 segundo.
}

int main (void)
{
	
	setup_Global();
	sei();

	while(1){
		
	}
}
