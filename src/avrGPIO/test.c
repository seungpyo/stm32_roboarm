#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>


int main(){

	DDRC |= (1<<PC2);
	DDRG |= (1<<PG0);
		
	while(1){
		PORTC |= (1<<PC2);
		PORTG &= ~(1<<PG0);
		_delay_ms(60);

		PORTC &= ~(1<<PC2);
		PORTG |= (1<<PG0);
		_delay_ms(100);
	}
	
	return 0;
}	
