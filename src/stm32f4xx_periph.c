#include "stm32f4xx.h"
#include "stm32f4xx_periph.h"

uint32_t Timer_Frequency = 42000;
uint32_t prescaler;

void TIM2_Init(void)														// Timer triggers every 1 sec
{
	RCC		->	APB1ENR	|=	RCC_APB1ENR_TIM2EN;			// Enable TIM2 clock
	//TIM2 	->	CR1			|=	TIM_CR1_CKD_1;				// ClockDivision 10b=tDTS = 4 × tCK_INT
	prescaler = SystemCoreClock/2;
	prescaler /=Timer_Frequency;
	prescaler -=1;
	TIM2	->	PSC			=		Timer_Frequency;
	TIM2	->	ARR			=		prescaler;
	TIM2	->  DIER		|=	TIM_DIER_UIE;
	TIM2	->	CR1			|=	TIM_CR1_CEN;					// Enable counter
	NVIC_EnableIRQ(TIM2_IRQn);
}

void LED_Init(void)
{
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
		//LED init PD12
			GPIOD	->	MODER			|=	(1<<24);						//PD12 = output
			GPIOD ->	OTYPER		&=	(1<<12);						//Output Push/pull
			GPIOD ->	OSPEEDR		&=	~(3<<24);						//50Mhz fast speed
			GPIOD ->	OSPEEDR		|=	(2<<24);		
			GPIOD ->	PUPDR			&=	~(3<<24);						//PD12 pull up
			GPIOD	->	PUPDR			|=	(1<<24);
		//LED init PD12	

		//LED init PD14
			GPIOD	->	MODER			|=	(1<<28);						//PD12 = output
			GPIOD ->	OTYPER		&=	(1<<14);						//Output Push/pull
			GPIOD ->	OSPEEDR		&=	~(3<<28);						//50Mhz fast speed
			GPIOD ->	OSPEEDR		|=	(2<<28);		
			GPIOD ->	PUPDR			&=	~(3<<28);						//PD12 pull up
			GPIOD	->	PUPDR			|=	(1<<28);
		//LED init PD14	
	
		//LED init PD13
			GPIOD	->	MODER			|=	(1<<26);						//PD12 = output
			GPIOD ->	OTYPER		&=	(1<<13);						//Output Push/pull
			GPIOD ->	OSPEEDR		&=	~(3<<26);						//50Mhz fast speed
			GPIOD ->	OSPEEDR		|=	(2<<26);		
			GPIOD ->	PUPDR			&=	~(3<<26);						//PD12 pull up
			GPIOD	->	PUPDR			|=	(1<<26);
		//LED init PD13
}

void ADC_Init(void)
{
	RCC -> APB2ENR |=(1<<8);			// Enable ADC1 clock
	ADC1-> CR1 |=ADC_CR1_SCAN;		// Enable channel scan
	ADC1-> CR2 |=ADC_CR2_CONT;		//continous
	ADC1-> SQR1 |=(1<<20);				// 0001: 2 conversion
	ADC1-> SQR3	|=(16<<0);				// Channel 16 first sequence
	ADC1-> SQR3	|=(18<<5);				// Channel 18 second sequence
	ADC1-> CR2 |=ADC_CR2_ADON;		// Enable ADC1
	ADC1->CR2 &=~ADC_CR2_DMA;
	ADC1->CR2 |=ADC_CR2_DMA;
	ADC1->SMPR1 |= ADC_SMPR1_SMP16_1 | ADC_SMPR1_SMP16_2;	// sampling time 144 cycles
	ADC1->SMPR1 |= ADC_SMPR1_SMP18_1 | ADC_SMPR1_SMP18_2;	// sampling time 144 cycles
	ADC -> CCR |=ADC_CCR_TSVREFE;	// Enable TSVREFE
	ADC -> CCR |=ADC_CCR_VBATE;		// Enable VBATE
	ADC1 -> DR = 0;
}
