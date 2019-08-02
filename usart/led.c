#include "regs.h"
#include "led.h"

/**
  * For now, we use CN10, pin 2.
  */

void led_init() {
	// Enable Clocks
	*(RCC_AHB2ENR) |= RCC_GPIOCEN_MASK;
}

void led_write_pin(int pin, int value) {
	// Set GPIO pin to 01 (output)
	*(GPIOC_MODER) &= ~GPIO_PIN_MASK(pin);
	*(GPIOC_MODER) |= (0x1 << (2 * pin));
	// Set pin value.
	if (value == LED_ON) {
		*(GPIOC_BSRR) |= (1<<pin);
	} else {
		*(GPIOC_BRR) |= (1<<pin);
	}
}

void led_blink(int pin, int duration) {
	led_write_pin(pin, LED_ON);
	for (int i = 0 ; i < (1<<duration); i++);
	led_write_pin(pin, LED_OFF);
}

void led_4states(int state) {
	
	// Set GPIO PC8 to 01 (output)
	*(GPIOC_MODER) &= ~GPIO_PIN_MASK(6);
	*(GPIOC_MODER) |= (0x1 << (2*6));
	*(GPIOC_MODER) &= ~GPIO_PIN_MASK(8);
	*(GPIOC_MODER) |= (0x1 << (2*8));

	led_write_pin(8, state&0x2 ? LED_ON : LED_OFF);
	led_write_pin(6, state&0x1 ? LED_ON : LED_OFF);

	for (int i = 0; i < (1<<19); i++);
	led_write_pin(8, LED_OFF);
	led_write_pin(6, LED_OFF);

	for (int i = 0; i < (1<<13); i++);
}
