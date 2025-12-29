#include "stm32f1xx.h"

#define LED_PIN 13

void SysTick_Handler(void) {
	static uint32_t count = 0;
	// BSRR - Bits Set/Reset Register
	if (count > 750) {
		GPIOC->BSRR ^= (1U << LED_PIN);
	} else {
		GPIOC->BRR  ^= (1U << LED_PIN);
	}
	count = (count + 1) % 1000;
}

int main(void) {

	// RCC     - Reset & Clock Control
	// GPIOC   - General Purpose In Input Output Port C
	// APB2ENR - Advanced Peripheral Bus 2 ENable Register
	// IOPCEN  - Input/Output Port C ENable

	// Enable clock for GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	// CRH   - port Configuration Register High (8-15)
	// PC13  - Port C 13
	// MODE  - mode
	// -> 00,01,10,11 - input, output 10, 2, 5 MHz
	// CNF13 - CoNFiguration bits for PC13
	// -> 00,01 - general purpose push/pull, open/drain
	// -> 10,11 - alternate function push/pull, open/drain
	// General purpose - used as a normal GPIO
	// Alternate function - controlled by a peripheral
	// Push-pull - pin can drive high (1) and low (0)
	// Open-drain - pin can only drive low (0), ensuring no logic noise from chip on pin

	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOC->CRH |=  (0x2 << GPIO_CRH_MODE13_Pos);

	// SysTick - System timer
	// ->LOAD  - reload value (8 MHz - 1 second)
	// ->VAL   - starting counter value
	SysTick->LOAD = 8000000 / 1000 - 1; // every ms
	SysTick->VAL = 0;

	// ->CTRL  - ConTRoL and status register
	// SysTick_CTRL_TICKINT_Msk    - enabling SysTick_Handler interrupt
	// SysTick_CTRL_ENABLE_Msk     - enabling timer
	// SysTick_CTRL_CLKSOURCE_Msk  - select processor clock
	SysTick->CTRL = 0;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	while (1) {
		__WFI(); // Wait For Interrupt (sleep)
	}
}
