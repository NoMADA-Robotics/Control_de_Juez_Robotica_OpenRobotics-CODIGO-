/*
 * Control_Juez_tutorial.c
 *
 * Created: 2/27/2021 12:20:59 PM
 * Author : Lenovo-i7
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>		// Encabezado para utilizar las funciones de delay ms y us

int RESET[] = {0,1,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0};	//Tramas de comandos
int READY[] = {0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,0,0};
int GO[]	= {0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,0};

//	Configurar los pines de entrada y salida
//	Generar se�al cuadrada a 38khz a partir del timer 1 (CTC) se�al cuadrada
//	Generar las tramas de RESET READY y GO (comandos)



void conf_puertos(void)
{
	DDRB |= (1<<1);			// Salida de la se�al a los LED infrarrojos 
	
	DDRD &= ~(1<<0);		// Entrada Bot�n de RESET (comando)
	DDRD &= ~(1<<1);		// Entrada Bot�n de GO (comando)
	DDRD &= ~(1<<3);		// Entrada bot�n de READY (comando)
}


void conf_timer1(void)
{
	//TCCR1A |= (1<<COM1A0);		// Salida del timer habilitada para CTC
	TCCR1B |= (1<<WGM12);		// Modo de operaci�n #4 CTC (se�al cuadrada)
	OCR1A =	104;				// TOP para generar  38khz
}

void ir_on(void)
{
	TCCR1A |= (1<<COM1A0);		// Salida del timer habilitada para CTC
}

void ir_off(void)
{
	TCCR1A &= ~(1<<COM1A0);		// Salida del timer apagada para CTC
}


void send_trama(int *trama)
{
	ir_off();				//salida apagada
	
	for (int i=0; i<19; i++)
	{
		switch(trama[i])
		{
			case 0: ir_on(); _delay_us(400); break;
			case 1: ir_off(); _delay_us(400); break;
			default: ir_off(); break;
		}
	}
	
	ir_off();
}


int main(void)
{
	conf_puertos();		// Configuraci�n de los puertos de entrada y salida
	conf_timer1();		// Mandamos llamar la funcion de configuraci�n del timer
	
	TCCR1B |= (1<<CS10);	// Habilita timer - > Prescalador de 1
	
	
    /* Replace with your application code */
    while (1) 
    {
		ir_off();
		
		if ((PIND & (1<<0))==0)	//Leemos el boton de RESET (comando)
		{
			DDRD |= (1<<2);
			send_trama(RESET);
			//_delay_ms(64);				//agregamos un delay de 64 mili segundos
			PORTD &= ~(1<<2);
			_delay_ms(8);
			PORTD |= (1<<2);
			_delay_ms(8);
			PORTD &= ~(1<<2);
			_delay_ms(8);
			PORTD |= (1<<2);
			_delay_ms(8);
			PORTD &= ~(1<<2);
			_delay_ms(8);
			PORTD |= (1<<2);
			_delay_ms(8);
			DDRD &= ~(1<<2);
		}
		else if((PIND & (1<<3))==0)	//Leemos el boton de READY (comando)
		{
			DDRD |= (1<<2);
			send_trama(READY);
			PORTD |= (1<<2);
			_delay_ms(64);				//agregamos un delay de 64 mili segundos
			DDRD &= ~(1<<2);
		}
		else if((PIND & (1<<1))==0)	// leemos el bot�n de GO (comando)
		{
			DDRD |= (1<<2);
			send_trama(GO);
			PORTD &= ~(1<<2);
			_delay_ms(64);				//agregamos un delay de 64 mili segundos
			DDRD &= ~(1<<2);
		}
		
		ir_off();
		
    }
}

