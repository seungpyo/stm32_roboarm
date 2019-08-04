#include "regs.h"

#define LED_OFF 0
#define LED_ON 1

void led_init();
void led_write_pin(int pin, int value);
void led_blink(int pin, int duration);
void led_4states(int state);

