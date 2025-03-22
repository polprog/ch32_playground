#include "ch32fun.h"
#include <stdio.h>

#define plpg_fclk 8000000
#define BRR_MANTISSA 4
#define BRR(x) ((plpg_fclk)/(16*(x)) << BRR_MANTISSA)

void doSetupUart1(int br){

  //Enable uart1 in rcc
  RCC->APB2PCENR |= RCC_APB2Periph_USART1;

  
  // 115200, 8n1.  Note if you don't specify a mode, UART remains off even when UE_Set.
  USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Tx;
  USART1->CTLR2 = USART_StopBits_1;
  USART1->CTLR3 = USART_HardwareFlowControl_None;

  USART1->BRR = br;
  USART1->CTLR1 |= CTLR1_UE_Set;
  
}


int main()
{
	SystemInit();
	// Enable GPIOs
	//RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC;
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;
	
	doSetupUart1(BRR(9600));

	// GPIO A3 Push-Pull
	GPIOA->CFGLR &= ~((uint32_t)0x0F<<(4*3));
	GPIOA->CFGLR |= (uint32_t)(GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*3);


	//GPIO A9 uart1 tx push-pull, alt fun
	GPIOA->CFGHR &= ~(0xf<<(4*1));  //clear existing setup
	GPIOA->CFGHR |= (uint32_t)(GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*1);

	
	while(1)
	{
		GPIOA->BSHR = (1<<3);	 // Turn on GPIO
		Delay_Ms( 1000 );
		GPIOA->BSHR = (1<<(16+3)); // Turn off GPIO
		Delay_Ms( 1000 );
		//printf("aaaa\n");
		USART1->DATAR = 0x55;
	}
}
