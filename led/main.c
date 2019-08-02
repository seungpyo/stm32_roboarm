#include "regs.h"

/**
  * For now, we use CN10, pin 2.
  */
int blink_led(int duration) {
	
	// Enable Clocks
	*(RCC_AHB2ENR) |= RCC_GPIOCEN_MASK;

	// Set GPIO PC8 to 01 (output)
	*(GPIOC_MODER) &= ~GPIO_PIN_MASK(8);
	*(GPIOC_MODER) |= (0x1 << (2*8));

	// Set Pin 8.
	*(GPIOC_BSRR) |= (1<<8);
	for (int i = 0 ; i < (1<<duration); i++);
	// Reset Pin 8.
	*(GPIOC_BRR) |= (1<<8);

	return 0;
}

int main (void) {
		
	while (1) {
		blink_led(15);
		for (int i = 0 ; i < (1<<15); i++);
	}

	return 0;
}
