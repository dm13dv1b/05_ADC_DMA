#include "system_stm32f4xx.c"
#include "main.h"

#include "misc.h"
#include "stm32f4xx_periph.h"
#include "stm32f4xx_it.h"

uint8_t status;

#define TS_CAL1    ((uint32_t)0x1FFF7A2C
#define v25 = (0.76/3.3)*0xFFF										// DM00037051
#define	avg_slope = (0.0025/3.3)*0xFFF						// DM00037051

void TextOut(const char *str)											//ITM Stimulus port 0 -> 1-be
{
	do {
		if(*str=='\n') ITM_SendChar('\r');
		ITM_SendChar(*str);														//ITM Send char printf helyett
	} while (*str++);
}

void DMA_Init(void)
{
	RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN;				// Enable DMA1 clcok
	DMA2_Stream0 -> CR &= ~DMA_SxCR_EN;					// disable Stream0
	DMA2_Stream0 -> PAR = (uint32_t)&ADC1->DR;							// from periph port register
	DMA2_Stream0 -> M0AR = (uint32_t) &adc_buffer;					// write to memory
	DMA2_Stream0 -> NDTR = ARRAYSIZE;						// number of data items
	DMA2_Stream0 -> CR |= DMA_SxCR_MINC;				// memory increment
	DMA2_Stream0 -> CR &= ~(DMA_SxCR_CHSEL);		// select chanel0
	DMA2_Stream0 -> CR |= DMA_SxCR_MSIZE_0;			// half-word (16bit) memory data size
	DMA2_Stream0 -> CR |= DMA_SxCR_PSIZE_0;			// half-word (16bit) peripherial data size
	DMA2_Stream0 -> CR |= DMA_SxCR_TCIE;				// transaction complete interrupt
	DMA2_Stream0 -> CR |= DMA_SxCR_CIRC;				// non enable continous mode
	DMA2_Stream0 -> CR |= DMA_SxCR_EN;					// enable Stream0
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);					// enable IRQ	
}

int main(void)
{
	LED_Init();
	ADC_Init();
	DMA_Init();
	TIM2_Init();
	
	while(1)
	{
	}
}
