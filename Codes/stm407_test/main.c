#define HSE_VALUE    ((uint32_t)8000000)  
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "usart.h"
#include "hal.h"

#include <stdio.h>


//------------------------------------globals---------------------------------------------------------
//E:\workspace\projects\humanoid\mcu source\online control\stm407-mcp-fpga\PILOT.hex
//G:\sensor logger\mass_and_hid - ala\hid.hex

char temp_main[300];

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{  GPIOD->ODR^=1<<12;
  /* SPI in Receiver mode */
  if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET)
  {

  }

}
//void SPI2_IRQHandler(void)
//{
//GPIOD->ODR^=1<<12;
//  /* Store SPI2 received data */
// // SPI2_Buffer_Rx[RxIdx++] = SPI_I2S_ReceiveData(SPI2);
//}
int main()
{
	SystemInit();
	USART3_Config();
  // 	plateform_spi2_cs_gpio_init(); 
  
  	platform_spi2_init();
   	dbg("debug mode\n");



	plateform_leds_init();

sprintf(temp_main,"CR1=%x\n",SPI2->CR1);
  	dbg(temp_main);
sprintf(temp_main,"CR2=%x\n",SPI2->CR2);
  	dbg(temp_main);


	while(1)
	{	
	   GPIOD->ODR^=1<<13;
	   sprintf(temp_main,"SR=%x\n",SPI2->SR);
  	dbg(temp_main);
		   sprintf(temp_main,"DR=%d\n",SPI2->DR);
  	dbg(temp_main);

	delay_ms(1000);

	}//end while
}//end main
