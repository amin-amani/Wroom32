#ifndef __HAL_H
#define __HAL_H
#define left_is_up		(GPIOA->IDR&1<<4) // is 32 when up
#define right_is_up		(GPIOA->IDR&1<<5)  //is 16 when up
#define led_r_on()		{GPIOD->ODR|=(1<<12);}  //is 16 when up
#define led_b_on()		{GPIOD->ODR|=(1<<13);}  //is 16 when up
#define led_w_on()		{GPIOD->ODR|=(1<<14);}  //is 16 when up
#define led_g_on()		{GPIOD->ODR|=(1<<15);}  //is 16 when up

#define led_r_off()		{GPIOD->ODR&=~(1<<12);}  //is 16 when up
#define led_b_off()		{GPIOD->ODR&=~(1<<13);}  //is 16 when up
#define led_w_off()		{GPIOD->ODR&=~(1<<14);}  //is 16 when up
#define led_g_off()		{GPIOD->ODR&=~(1<<15);}  //is 16 when up

#define led_r_blink()		{GPIOD->ODR^=1<<12;}  //is 16 when up
#define led_b_blink()		{GPIOD->ODR^=1<<13;}  //is 16 when up
#define led_w_blink()		{GPIOD->ODR^=1<<14;}  //is 16 when up
#define led_g_blink()		{GPIOD->ODR^=1<<15;}  //is 16 when up

//////////////////////////////////////////hardware abstraction layer 
void delay_us(int delay);
//=================================================================================
void delay_ms(int delay);
//=================================================================================
void platform_spi1_init(void);
//=================================================================================
u8 platform_spi1_SendByte(u8 byte);
//=================================================================================
void platform_spi2_init(void);
//=================================================================================
u8 platform_spi2_SendByte(u8 byte);
//=================================================================================
void cs2_hi(void);
//=================================================================================
void cs2_low(void);
//=================================================================================
void USART_Config(void);
//=================================================================================
 void platform_spi_other_low(void);
  void platform_spi_other_high(void);
//=================================================================================
void cs1_config(u16 CS_ADR);
//=================================================================================
void plateform_spi2_cs_gpio_init(void);
//=================================================================================
void plateform_spi1_cs_gpio_init(void);
//=================================================================================
void timer_init(int interval);
//=================================================================================
void timer_start(int count);
void plateform_leds_init(void);

#endif
