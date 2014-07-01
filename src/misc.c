//misc.c
#include "stm32f4xx.h"
#include "misc.h"

#define ARRAYSIZE 128*4
extern uint16_t adc_buffer[ARRAYSIZE];

float temperature;
float voltage, temp_adc;
uint8_t step;
uint16_t i;

void delay_ms(uint32_t ms)
{
uint32_t time = SystemCoreClock / 4000;
uint32_t loop_time;

  for(; ms != 0; ms--)
  {
    loop_time = time;
    for(; loop_time != 0; loop_time--);
  }
}

void delay_us(uint32_t us)
{
  uint32_t time = SystemCoreClock / 6800000;
  uint32_t loop_time;

  for(; us != 0; us--)
  {
    loop_time = time;
    for(; loop_time != 0; loop_time--);
  }
}

void Read_ADC(void)
{
	GPIOD -> ODR |= (1<<12);
	ADC1->CR2 &=~ADC_CR2_DMA;
	ADC1->CR2 |=ADC_CR2_DMA;
	ADC1->SR &=~ADC_SR_STRT;
	ADC1->SR &=~ADC_SR_OVR;
	ADC1->SR &=~ADC_SR_EOC;
	ADC1-> CR2 |=ADC_CR2_SWSTART;	//Start conversion of regular channels
	while (DMA1->LISR & DMA_LIFCR_CHTIF0){};
	ADC1-> CR2 &= ~ADC_CR2_SWSTART;
}

void Average(void)
{
	if (adc_buffer[0] > 1500)
	{ step = 0; } else step = 1;
	temp_adc = 0;
	for ( i=step; i < ARRAYSIZE; i+=2)
		{
			temp_adc += adc_buffer[i];
		}
	temp_adc /= ARRAYSIZE/2;
	temp_adc -= 760;
	temp_adc /= 2500;
	temp_adc +=25;
		
	temperature = temp_adc;
	temp_adc = 0;
		
	for ( i=step+1; i < ARRAYSIZE; i+=2)
		{
			temp_adc += adc_buffer[i];
		}
	temp_adc /= ARRAYSIZE/2;
	temp_adc *= 3300;
	temp_adc /= 0xFFF;
	temp_adc /= 1000;
	temp_adc *= 2;
	voltage = temp_adc;
}
