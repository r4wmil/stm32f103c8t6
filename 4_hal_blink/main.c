#include "stm32f1xx_hal.h"

void SysTick_Handler(void){
	HAL_IncTick();
}

int main(void) {
	HAL_Init();

	// START
	SystemCoreClock = 8000000;//Частота контроллера
	uwTickFreq = 1;//Задержка в миллисекундах
	// Interrupt:
	//uint32_t count = SystemCoreClock / (1000U / uwTickFreq);//Значение для регистра Reload
	//WRITE_REG(SysTick->LOAD, count);
	//HAL_NVIC_SetPriority(SysTick_IRQn, 15U, 0U);
	//SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);//Устанавливаем предделитель в 1
	//SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);//Включаем прерывание
	//SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);//Включаем таймер
	//HAL_NVIC_DisableIRQ(SysTick_IRQn);

	// END
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	while (1) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(500);
	}
}
