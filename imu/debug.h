#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdlib.h>
#include <stdint.h>

#include "regs.h"
#include "utils.h"
#include "usart.h"

typedef int (*printk_fn_t)(const char *str);

printk_fn_t printk;

char * wtoa(uint32_t word);

#endif /* _DEBUG_H */
