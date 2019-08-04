#include "utils.h"
/*
#include "usart.h"

// "0x"(2) + word(8) + '\0'(1) = 11 bytes
#define WTOA_HEXBUFSIZE 11
#define WTOA_BUFSIZE 32
char wtoa_hexbuf[WTOA_HEXBUFSIZE] = "0x";
char wtoa_buf[WTOA_BUFSIZE];
char wtoa_stk[WTOA_BUFSIZE];
static uint16_t wtoa_stk_top = 0;

#define WTOA_HEX 16
#define WTOA_DEC 10
static char hex_lookup[] = { 
	'0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


char * wtoa(uint32_t word, uint32_t radix) {
	if (radix == 16) {
		char *ptr = wtoa_hexbuf + 2;
		
		return wtoa_hexbuf;
	} else {
		return NULL;
	}
}
*/

void * memset(void *b, int c, uint32_t len) {
	for(int i = 0; i < len; i++) {
		((char *)b)[i] = c;
	}
	return b;
}



