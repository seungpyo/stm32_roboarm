#include <stdint.h>

#define __REG volatile uint32_t *

#define RCC_APB1ENR1 ((__REG)0x40021058)
#define RCC_USART2EN_MASK (0x00020000)

#define USART2_CR1 ((__REG)0x40004400)
#define USART2_ISR ((__REG)0x4000441C)
#define USART2_TDR ((__REG)0x40004428)
#define USART_FLAG_TXE	((uint16_t) 0x0080)

struct gpio {
    uint32_t MODER; // 0x00
    uint32_t OTYPER; // 0x04
    uint32_t OSPEEDR; // 0x08
    uint32_t PUPDR; // 0x0C
    uint32_t IDR; // 0x10
    uint32_t ODR; // 0x14
    uint32_t BSRR; // 0x18
    uint32_t LCKR; // 0x1C
    uint32_t AFRL; // 0x20
    uint32_t AFRH; // 0x24
    uint32_t BRR; // 0x28
    uint32_t ASCR; // 0x2C
};

typedef enum {
    GPIO_PIN_RESET = 0u,
    GPIO_PIN_SET
} GPIO_PinState;

#define GPIOA ((volatile struct gpio *)0x48000000)
#define GPIOB ((volatile struct gpio *)0x48000400)
#define GPIOC ((volatile struct gpio *)0x48000800)
#define GPIOD ((volatile struct gpio *)0x48000C00)
#define GPIOE ((volatile struct gpio *)0x48001000)
#define GPIOH ((volatile struct gpio *)0x48001C00)

/**
 * @brief A configuration struct for a GPIO peripheral.
 * @param pin: Pin #x can be encoded as 1<<x.
 * User can specify multiple pins using bitwise OR operation.
 */
struct gpio_config {
    uint32_t pin;
    uint32_t mode;
    uint32_t pull;
    uint32_t speed;
    uint32_t alternate;
};

#define GPIO_CONFIG_MODE_AT_PIN(p) ((uint32_t)(0x3<<(2*p)))
#define GPIO_CONFIG_DEFAULT_INITIALIZER {\
    .pin = (uint32_t)1<<5, \
    .mode = GPIO_CONFIG_MODE_AT_PIN(5), \
    .pull = 0, \
    .speed = 0, \
    .alternate = 0}


/**
 * @brief Initialize every pin the user specified.
 */
static void gpio_init(volatile struct gpio * gpiox, struct gpio_config * cfg) {
    uint32_t pos;
    uint32_t tmp, mask;
    for (pos = 0; cfg->pin >> pos; pos++) {
        // Configure IO Direction mode (Input, Output, Alternate, Analog)
        mask = 0x3U << (2 * pos);
        tmp = cfg->mode & mask; // Turn on only the masked bits
        gpiox->MODER &= ~mask; // Turn off the masked bits
        gpiox->MODER |= tmp;
    }
}

typedef uint32_t gpio_pin_t;

/**
 * @brief Writes a state to the given pin of GPIO peripheral gpiox.
 * @param pin: Pin number should be given as an encoded bit vector so that function can
 * deal with multiple pins at the same time.
 */
static inline void gpio_write(volatile struct gpio * gpiox, gpio_pin_t pin, GPIO_PinState state) {
    pin &= 0x0000FFFF;
    if (state != GPIO_PIN_RESET) {
        gpiox->BSRR = (uint32_t) pin;
    } else {
        gpiox->BRR = (uint32_t) pin;
    }
}

int puts(const char *str)
{
	while (*str) {
		while (!(*(USART2_ISR) & USART_FLAG_TXE));
		*(USART2_TDR) = *str++ & 0xFF;
	}
	return 0;
}


int main (void) {

	*RCC_APB1ENR1 |= RCC_USART2EN_MASK;
	*(USART2_CR1) = 0xC;
	*(USART2_CR1) |= 0x2000;

	puts("Hello World!");
    
	// while(1);
}
