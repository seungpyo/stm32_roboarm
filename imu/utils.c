#include "utils.h"
#include "usart.h"

void prints(const char *str) {
	usart_puts(str);
}

char hexmap[] = "0123456789abcdef";

void printx(uint32_t word) {
	unsigned char buf[] = "0x00000000";
	unsigned char byte;

	byte = ((word & 0xFF000000) >> 24);
	buf[2] = hexmap[(byte>>4)&0xF];
	buf[3] = hexmap[byte&0xF];
	
	byte = ((word & 0x00FF0000) >> 16);
	buf[4] = hexmap[(byte>>4)&0xF];
	buf[5] = hexmap[byte&0xF];

	byte = ((word & 0x0000FF00) >> 8);
	buf[6] = hexmap[(byte>>4)&0xF];
	buf[7] = hexmap[byte&0xF];
	
	byte = ((word & 0x000000FF) >> 0);
	buf[8] = hexmap[(byte>>4)&0xF];
	buf[9] = hexmap[byte&0xF];

	prints((const char *)buf);
}

void printb(uint32_t word) {
	prints("0b");
	for(int i = 0; i < 32; i++) {
		if (!(i&0x3)) {
			usart_putc(' ');
		}
		usart_putc((char)(((word >> (31-i)) & 0x1) + '0'));
	}
}

void printd(int num) {
	return;
}

void printud(uint32_t num) {
	return;
}

void * memset(void *b, int c, uint32_t len) {
	for(int i = 0; i < len; i++) {
		((char *)b)[i] = c;
	}
	return b;
}

void delay(uint32_t duration) {
	for (int i = 0; i < duration; i++);
}
void exp_delay(unsigned char exp) {
	delay(1<<exp);
}
