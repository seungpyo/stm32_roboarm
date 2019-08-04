#ifndef _USART_H
#define _USART_H

#define USART_FREQ (4*1000000)
#define USART_BAUD (9600)

void usart_init();
void usart_putc(const char c);
char usart_getc(void);
int usart_puts(const char *str);
int usart_gets(char *buf);

void usart_println(const char *str);

#endif /* _USART_H */
