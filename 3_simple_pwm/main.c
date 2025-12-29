#include "stm32f1xx.h"
#include "math.h"

#define LED_PIN 13

// SysTick divider (for 100 us interrupts)
#define SYSTICK_DIVIDER (10000)
// 25'000 us ~= 40 Hz
#define PULSE_WIDTH (250)
// 1'000'000 us ~= 1 sec
#define TIME_PERIOD (10 * 1000)

void SysTick_Handler(void) {
	static uint32_t pulse = 0;
	static uint32_t time = 0;
	static float duty = 0.0f;
	// BSRR - Bits Set/Reset Register
	// BRR  - Bits Reset Register (+ 16 bits)
	if (pulse % PULSE_WIDTH == 0) { // Every PULSE_WIDTH
		float T = 2 * M_PI * (float)time / TIME_PERIOD;
		duty = sinf(T) * 0.5f + 0.5f;
		//duty = (float)time / TIME_PERIOD;
	}
	if (pulse > PULSE_WIDTH * duty) {
		GPIOC->BSRR |= (1U << LED_PIN);
	} else {
		GPIOC->BRR  |= (1U << LED_PIN);
	}
	pulse = (pulse + 1) % PULSE_WIDTH;
	time = (time + 1) % TIME_PERIOD;
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
	SysTick->LOAD = 8000000 / SYSTICK_DIVIDER - 1; // every 100 us
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
