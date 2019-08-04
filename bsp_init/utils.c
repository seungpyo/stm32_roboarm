#include "utils.h"

void * memset(void *b, int c, uint32_t len) {
	for(int i = 0; i < len; i++) {
		((char *)b)[i] = c;
	}
	return b;
}
