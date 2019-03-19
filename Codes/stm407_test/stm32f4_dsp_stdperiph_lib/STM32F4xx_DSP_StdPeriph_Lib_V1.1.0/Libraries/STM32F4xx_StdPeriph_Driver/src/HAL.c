//////////////////////////////////////////hardware abstraction layer sourc
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "hal.h"


//=====================================================================================================
void delay_ms(int delay){

unsigned long int i;
while(delay-->0)
for(i=0;i<32002;i++);

}
void delay_us(int delay){

unsigned long int i;
while(delay-->0)
for(i=0;i<32;i++);

}
//=================================================================================
void platform_spi1_init(void){

SPI_InitTypeDef  SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

/* Enable SPI1 and GPIO clocks */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE );
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /*!< SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /*!< SPI MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA =SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//SPI_BaudRatePrescaler_4;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  SPI_Init(SPI3, &SPI_InitStructure);
 

  /* Enable SPI1  */
  SPI_Cmd(SPI3, ENABLE);

}
//=================================================================================
u8 platform_spi1_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI3, ~byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return ~SPI_I2S_ReceiveData(SPI3);
}  
//=================================================================================
void platform_spi2_init(void){

SPI_InitTypeDef  SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
   /* Configure and enable SPI2 interrupt -------------------------------------*/
  NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
/* Enable SPI1 and GPIO clocks */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE );
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);

GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);  //b13 sck
GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);	   //c3	mosi
GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);	   //c2	miso
GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_SPI2);//ss

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
 
    /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

      /*!< mosi  pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

      /*!< miso  pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;//SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode =SPI_Mode_Slave;// SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA =SPI_CPHA_1Edge;// SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS =SPI_NSS_Hard;// 
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //SPI_FirstBit_LSB;//
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  SPI_Init(SPI2, &SPI_InitStructure);
    /* Enable SPI2  */
  SPI_Cmd(SPI2, ENABLE);
 /* Enable SPI2 RXNE interrupt */
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);


}
//=================================================================================
u8 platform_spi2_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}  
//=================================================================================
void plateform_spi1_cs_gpio_init(void){
GPIO_InitTypeDef  GPIO_InitStructure;
  /* GPIOC Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
//
//=================================================================================
void plateform_opto_init(void){
GPIO_InitTypeDef  GPIO_InitStructure;
  /* GPIOC Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//=================================================================================

void plateform_leds_init(void){
GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

//=================================================================================

void plateform_spi2_cs_gpio_init(void){

	GPIO_InitTypeDef  GPIO_InitStructure;
	/* GPIOC Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;// GPIO_Mode_OUT;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;		// TX0RTS PIN
  //GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//=================================================================================
void cs1_config(u16 CS_ADR)
{	
GPIOC->ODR|=(0x3C0);
GPIOC->ODR&=~(CS_ADR << 6);
}
//=====================================================================================================
void USART_Config(void)
{
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStruct;

RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
						 
	/* enable the peripheral clock for the pins used by
	 * USART1, PB6 for TX and PB7 for RX
	 */
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

  /* This sequence sets up the TX and RX pins
	 * so they work correctly with the USART1 peripheral
	 */

   /* Configure USART Tx as alternate function  */
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* The RX and TX pins are now connected to their AF
	 * so that the USART1 can take over control of the
	 * pins
	 */
	

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;

USART_Init(USART3, &USART_InitStructure);

USART_Cmd(USART3, ENABLE);

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}


//=================================================================================

void timer_init(int interval){	 //set interval in msec
__IO uint16_t CCR1_Val = 60000;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t PrescalerValue = 0;
CCR1_Val=3000*interval;
/* TIM3 clock enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

/* Enable the TIM3 gloabal Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 6000000) - 1;

/* Time base configuration */
TIM_TimeBaseStructure.TIM_Period = 65535;
TIM_TimeBaseStructure.TIM_Prescaler = 0;
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

/* Prescaler configuration */
TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

/* Output Compare Timing Mode configuration: Channel1 */
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

TIM_OC1Init(TIM3, &TIM_OCInitStructure);

TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
//interrupt enable
//TIM_ITConfig(TIM3, TIM_IT_CC1 , ENABLE);

/* TIM3 enable counter */
TIM_Cmd(TIM3, ENABLE);
}
//=================================================================================

void timer_start(int count){  //start togle depend on count
 TIM_ITConfig(TIM3, TIM_IT_CC1 , ENABLE);
  // data_cnt=count;
}
//=================================================================================
void cs2_hi(void){
GPIOB->ODR|=1<<9;
}
//=================================================================================
void cs2_low(void){
GPIOB->ODR&=~(1<<9);
}
void cs1_low(void){
GPIOB->ODR|=1<<2;
}
//=================================================================================
void cs1_hi(void){
GPIOB->ODR&=~(1<<2);
}
//=================================================================================

void priority(){

/* Configure the Priority Grouping with 1 bit */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
}
  void platform_spi_other_low(void){
 
  GPIOC->ODR&=~(1<<10);
 }
 void platform_spi_other_high(void){
 
  GPIOC->ODR|=1<<10;
 }

 void platform_spi_other_init(){
GPIO_InitTypeDef  GPIO_InitStructure;
  /* GPIOC Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
 
 }

