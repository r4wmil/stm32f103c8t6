#include "stdint.h"
#include "stm32f1xx.h"

#define LED_PIN 13

void _init(void) {} // dummy

int main(void) {

	// GPIOC   - General Purpose In Input Output
	// APB2ENR - Advanced Peripheral Bus 2 ENable Register
	// IOPCEN  - Input/Output Port C ENable

	// Enable the clock for GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	// CRH   - port Configuration Register High (8-15)
	// PC13  - Port C 13
	// MODE  - mode
	// -> 00 - input
	// -> 01 - output 10 MHz
	// -> 10 - output 2 MHz
	// -> 11 - output 50 MHz)
	// CNF13 - CoNFiguration bits for PC13
	// -> 00 - general purpose push/pull
	// -> 01 - general purpose open/drain
	// -> 10 - alternate function push/pull
	// -> 11 - alternate function open/drain
	// General purpose - used as a normal GPIO
	// Alternate function - controlled by a peripheral
	// Push-pull - pin can drive high (1) and low (0)
	// Open-drain - pin can only drive low (0), ensuring no logic noise from chip on pin

	// Configuring pin PC13 as output
	// 1) Clear MODE & CNF bits
	// [ CNF1 | CNF0 | MODE1 | MODE0 ]
	//   ^ 0    ^ 0    ^ 0     ^ 0
	// 2) Moving 0b10 to
	// [ CNF1 | CNF0 | MODE1 | MODE0 ]
	//                 ^ 1     ^ 0
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOC->CRH |=  (0x2 << GPIO_CRH_MODE13_Pos);

	while (1) {
		// BSRR - Bits Set/Reset Register
		// - used for seting/reseting without affecting other pins
		// OFF (shift << 16 bits)
		GPIOC->BSRR = (1U << (LED_PIN + 16));
		for (volatile uint32_t i = 0; i < 500000; i++);
		// ON
		GPIOC->BSRR = (1U << LED_PIN);
		for (volatile uint32_t i = 0; i < 500000; i++);
	}
}
