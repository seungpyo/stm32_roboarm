#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

void prints(const char *str);
void printx(uint32_t word);
void printb(uint32_t word);
void printd(int num);
void printud(uint32_t num);

void * memset(void *b, int c, uint32_t len);

#endif /* _UTILS_H */
