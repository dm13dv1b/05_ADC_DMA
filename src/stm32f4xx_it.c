//stm32f4xx_it.c
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

void DMA2_Stream0_IRQHandler(void);
void TIM2_IRQHandler(void);

void DMA2_Stream0_IRQHandler(void)
{
	if (DMA2 -> LISR & DMA_LISR_HTIF0)
	{
		DMA2 -> LIFCR |= DMA_LIFCR_CHTIF0;	
	}
	if (DMA2 -> LISR & DMA_LISR_TCIF0)
	{
		// clear DMA2 interrupt pending bit
		DMA2 -> LIFCR |= DMA_LIFCR_CTCIF0 | DMA_LIFCR_CHTIF0;
		GPIOD -> ODR ^= (1<<12);
	}
	NVIC->ICPR[0] |=(1<<7);
}

void TIM2_IRQHandler(void)
{
	if ( TIM2->SR & TIM_SR_UIF)
		{
			NVIC->ICER[1] &=~(1<<27);
			Read_ADC();
			delay_ms(50);
			GPIOD -> ODR ^= (1<<12);
			Average();	
			TIM2	->	SR	&= ~TIM_SR_UIF;
			NVIC->ICPR[1] |= (1<<27);
			NVIC->ISER[1] |= (1<<27);
		}
}
