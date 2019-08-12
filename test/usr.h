#include <stdlib.h>
#include <avr/io.h>
#include <util/twi.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

#define BAUD 57600
#define BUFSIZE 128


#if __GCC_VERSION__ < 34000
#define push(ptr, N)\
{	if(N==0){\
		asm volatile("in %[PTR], %[udr]\n"\
			:[PTR] "=r" (*(ptr))\
			:[udr] "I" (_SFR_IO_ADDR(UDR0))\
			:"memory");\
	}\
	else if(N==1){\
		asm volatile("lds %[PTR], 0x9c\n"\
			:[PTR] "=r" (*(ptr))\
			:\
			:"memory");\
	}\
	else{\
	}\
	ptr++;\
}
#define pop(ptr, N)\
{	while(!(UCSR0A & (1<<UDRE0)));\
	if(N==0){\
		asm volatile("out %[udr], %[PTR]\n"\
			:\
			:[PTR] "r" (*(ptr)), [udr] "I" (_SFR_IO_ADDR(UDR0))\
			:"memory");\
	}\
	else if(N==1){\
		asm volatile("sts 0x9c, %[PTR]\n"\
			:\
			:[PTR] "r" (*(ptr))\
			:"memory");\
	}\
	else{\
	}\
	ptr--;\
}

#else
#define push(ptr, N)\
{	*ptr = ((typeof(*ptr))UDR##N);\
	ptr++;\
}
#define pop(ptr, N)\
{	while(!(UCSR0A & (1<<UDRE0)));\
	UDR##N = *ptr;\
	ptr--;\
}
#endif


typedef uint8_t _size_t;

typedef struct fifo{
	unsigned char *head;
	unsigned char *tail;
	unsigned char *ptr;
	_size_t size;
}buf;

void blinkG(unsigned int times){

	DDRG |= (1<<DDG0);

	while(times){
		PORTG |= (1<<PG0);
		_delay_ms(50);
		PORTG &= ~(1<<PG0);
		_delay_ms(50);
		times--;
	}
}



void initBuf(buf a){
	a.size = BUFSIZE;
	a.head = (unsigned char *)malloc(BUFSIZE*sizeof(unsigned char));

	if(a.head != ((void *)0)){
		blinkG(1);
   		
		a.tail = a.head + a.size;
		a.ptr = a.head;
	}
	else{
		blinkG(2);
	}	
}

uint8_t __attribute__ ((destructor)) freeBuf(buf a){
	free(a.head);
	return 0;
}


void initUSART0(){
	uint8_t u = F_CPU/16/BAUD - 1;

	UBRR0H = (((uint8_t)u)>>8) & 0xFF00;
	UBRR0L = (uint8_t)u;
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

void initTWI(){
	TWSR = 0x00;
	TWBR = 0x0C;
	TWCR = (1<<TWIE) | (1<<TWEN);
}



















