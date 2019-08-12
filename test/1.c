#include "usr.h"
#include <avr/interrupt.h>

volatile unsigned char *ptr;

ISR(USART0_RX_vect){
	cli();

	UCSR0A &= ~(1<<RXC0);
	*ptr = UDR0;
	
	blinkG(1);

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = *ptr;

	blinkG(2);
}

int main(){

	initUSART0();
	
	while(1){}

	return 0;
}
