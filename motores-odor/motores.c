// this code sets up counter0 for an 8kHz Fast PWM wave @ 16Mhz Clock


#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

#include "serial.h"

void motores(int8_t vel_1, int8_t vel_2)
{
	/* 
	 *  vel : -128 a 127. 
	 *  0 es stop. 
	 *  valores negativos atras (reverso)
	 *  valores positivos adelante
	 *
	 *  Como los motores son de 12v, 
	 *  vamos a suponer un umbral minimo de 8v aprox.
	 */

	/*
	 * interfaz con la placa de potencia 
	 * motor 1: PORTD 3 pwm, PB0 in1, PB1 in2 
	 * motor 2: PORTB 3 pwm, PB2 in3, PB4 in4
	 */

	/* Los comparadores van de 0 a 255. Pero a 128 tendremos algun movimiento
	 * de motor. Entonces, buscamos valores de 128 a 255 para 
	 * los comparadores.
	 */

	/* motor 1 */
	if (vel_1 == 0) {		/* stop */
		serial_put_str("motor 1 stop");
		OCR2B = 0;
		PORTB &=  ( ~((1 << PB0) | (1 << PB1)));

	} else if (vel_1 < 0) {		/* reversa */

		serial_put_str("motor 1 reversa");
		/* comparador desde 128 */
		OCR2B = (vel_1 * (-1)) + 127;
		PORTD |= (1 << PD3);
		PORTB &= (~(1 << PB0));
		PORTB |= (1 << PB1);

	} else {			/* adelante */

		serial_put_str("motor 1 adelante");
		OCR2B = vel_1 + 127;
		PORTD |= (1 << PD3);
		PORTB |= (1 << PB0);
		PORTB &= (~(1 << PB1));
	}
	
	/* motor 2 */
	if (vel_2 == 0) {		/* stop */
		OCR2A = 0;
		PORTB &=  (~((1 << PB2) | (1 << PB4)));

	} else if (vel_2 < 0) {		/* reversa */

		/* comparador desde 128 */
		OCR2A = (vel_2 * (-1)) + 127;
		PORTB |= (1 << PB3);
		PORTB &= (~(1 << PB2));
		PORTB |= (1 << PB4);

	} else {			/* adelante */

		OCR2A = vel_2 + 127;
		PORTB |= (1 << PB3);
		PORTB |= (1 << PB2);
		PORTB &= (~(1 << PB4));
	}
	
}

void motores_init() 
{
	// this code sets up counter0 for an 8kHz Fast PWM wave @ 16Mhz Clock
	
	/* pwm signals */
    //DDRD |= (1 << DDD3); //PD3 es OC2B. En arduino uno, el pin etiquetado con ~3
    //DDRB |= (1 << DDB3); //PB3 es OC2A. En arduino uno, el pin etiquetado con ~11
    DDRD |= (1 << PD3); //PD3 es OC2B. En arduino uno, el pin etiquetado con ~3
    DDRB |= (1 << PB3); //PB3 es OC2A. En arduino uno, el pin etiquetado con ~11

    /* pb0, pb1, pb2, pb3, pb4 (ver motores) */
    DDRB |= 0b00011111;

    /* apagamos motores */
    motores(0, 0);
    //OCR2A = 0;
    //OCR2B = 0;

    TCCR2A |= (1 << COM2A1);
    TCCR2A |= (1 << COM2B1);
    // set none-inverting mode

    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    // set fast PWM Mode

    TCCR2B |= (1 << CS21);
    // set prescaler to 8 and starts PWM

}


int main(void)
{
	int8_t x, v1, v2;

	char s[80];
	serial_init();
	motores_init();
	while (1) {
		x = 0;
		while (x != 'X')
			x = (int8_t) serial_get_char();

		serial_put_str("obtuve X");

		v1 = (int8_t) serial_get_char();
		v2 = (int8_t) serial_get_char();
		sprintf(s, "v1: %i, v2: %i \n", v1, v2);
		serial_put_str(s);
		motores(v1, v2);
		_delay_ms(200);
	}
}
