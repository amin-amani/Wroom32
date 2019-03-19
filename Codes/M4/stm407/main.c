#define HSE_VALUE    ((uint32_t)8000000)  
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "mcp2515.h"
#include "usart.h"
#include "hal.h"
#include "k9f2g08.h"
#include "epos.h"
#include "commandline.h"
#include "sensor.h"
#include <stdio.h>


//------------------------------------globals---------------------------------------------------------
//E:\workspace\projects\humanoid\mcu source\online control\stm407-mcp-fpga\PILOT.hex
//G:\sensor logger\mass_and_hid - ala\hid.hex
u8 rxdata[8],rxlen;
char temp_main[300];
char data[100];
char record[96];


float imu_rx=0,imu_ry=0,imu_rz=0;



int i,dummy=0;			  
u16 rxid;

int CCR1_Val=60000;
//=================================================================================
void USART2_IRQHandler(){
char ch=USART_ReceiveData(USART2);
//sendchar2(ch);
//return; 

if (NAND_write_mode_flag)
		{ NAND_write_mode_function_with_crc(ch);
			if(!NAND_write_mode_flag)
				{
				 usart2_buffer[0]=0;
				 buffer2_index=0; 
				 return;
				}
		}

	else if (NAND_read_mode_flag)
		{ NAND_read_mode_function();
		  usart2_buffer[0]=0;
		  buffer2_index=0; 
		  return;
		}
	
	else 
		{ sendchar2(ch); 
		}
 

if((ch==10) && (!NAND_write_mode_flag) && (!NAND_read_mode_flag))
{task_handle(usart2_buffer);
 buffer2_index=0;
 usart2_buffer[0]=0;
 return;
}

if(ch==0x08 && !NAND_write_mode_flag)
	{buffer2_index--;
	 usart2_buffer[buffer2_index]=0;
	 return;
	}

usart2_buffer[buffer2_index++]=ch;
usart2_buffer[buffer2_index]=0;
if(buffer2_index>=USART2_BUFFER_SIZE-1)
	{buffer2_index=0;}
}
//=================================================================================

void USART3_IRQHandler(){
char ch=USART_ReceiveData(USART3);

if (NAND_write_mode_flag)
		{ NAND_write_mode_function_with_crc(ch);
			if(!NAND_write_mode_flag)
				{
				 usart3_buffer[0]=0;
				 buffer3_index=0; 
				 return;
				}
		}

	else if (NAND_read_mode_flag)
		{ NAND_read_mode_function();
		  usart3_buffer[0]=0;
		  buffer3_index=0; 
		  return;
		}
	
	else 
		{ sendchar3(ch); 
		}
 

if((ch==10) && (!NAND_write_mode_flag) && (!NAND_read_mode_flag))
{task_handle(usart3_buffer);
 buffer3_index=0;
 usart3_buffer[0]=0;
 return;
}

if(ch==0x08 && !NAND_write_mode_flag)
	{buffer3_index--;
	 usart3_buffer[buffer3_index]=0;
	 return;
	}

usart3_buffer[buffer3_index++]=ch;
usart3_buffer[buffer3_index]=0;
if(buffer3_index>=USART3_BUFFER_SIZE-1)
	{buffer3_index=0;}
}
//=================================================================================
void test_io_init(){
GPIO_InitTypeDef  GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //enable io clock for portb

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOE, &GPIO_InitStructure);



}
//=================================================================================
void TIM3_IRQHandler(void)
{	 unsigned long int capture=0;
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
  //////////timer reset
 
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
  	capture = TIM_GetCapture1(TIM3);
	TIM_SetCompare1(TIM3, capture + CCR1_Val);
    if(driver_stop==1)return;
//send_string3("timer\n");

if(buffer_ovf==0){MCP2515_start_transmittion();
  //GPIOB->ODR^=0X0001<<2;
  driver_update_done=1;		   } 
   	MCP2515_CANReadMsg(&rxid,rxdata,&rxlen,8);
	if(rxid==0x181 && rxdata[0]==0x0e && rxdata[1]==0xc0){buffer_ovf=1;rxid=0;}
	if(rxid==0x181 && rxdata[0]!=0x0e){buffer_ovf=0;rxid=0;}
  }//end tim1 capt 
}//end timer
//=====================================================================================================

void work(){

	  if(driver_update_done==1){
	if(step==start_trajectory_step){
	
	 step=addfifo_step;
	 //send_string3("send start\n");
	  start_ipm_trajectory();
	  driver_update_done=0;
	  return;
	}////end start step

	if(step==addfifo_step){
	

	NAND_read_record ( start_address,nand_address_pointer++,record);
	
	 //control function

	add_to_fifo3( record,10);

	
	 	motor_packet_sent++;
	
	if(motor_packet_sent>=data_size){step=send_stop_step; }
	  driver_update_done=0;
	  return;
	}//end fifo step	 
	if(step==send_stop_step){
	   	   
			add_to_fifo(pos,0,0);
			send_string3("stop\n");
		driver_stop=1;
	  	step==do_nothing_step;
	  }//end stop step
		

		driver_update_done=0;
	   }//end driver update



}

//=====================================================================================================
	float a,b,c;
	_FORCE ft1,ft2;
	_IMU imu;

int main()
{
	SystemInit();
	priority();
	USART3_Config();
	USART2_Config();
	platform_spi1_init();
	platform_spi2_init();
	plateform_spi1_cs_gpio_init();
	plateform_spi2_cs_gpio_init(); 
	for(i=0;i<14;i++) MCP2515_CAN_Setup(baud_1M_bit,i);
	NAND_io_init();
	NAND_initial_reset_function(1); 
	USART_puts(USART3,"in the name of allah\n");
	USART_puts(USART2,"in the name of allah U2\n");



	USART_puts(USART3,"can_setup\n");
	for(i=0;i<14;i++) MCP2515_CAN_Setup(baud_1M_bit,i);
	platform_spi_other_init();
	platform_spi_other_low();
	plateform_opto_init();
	timer_init(5);//10 ms==>5
	timer_start(1);
	dbg("debug mode\n");
	plateform_leds_init();

	  /////////////////////
	while(1)
	{	
	//work();
	led_b_blink();
	if(left_is_up) {led_g_on();}
	else{led_g_off();}
	if(right_is_up) {led_w_on();}
	else{led_w_off();}

	//led_r_blink();
	//led_b_blink();
	//led_g_blink();
	read_sensors2(&imu,&ft1,&ft2);
	sprintf(temp_main,"Rx=%1.2f Ry=%1.2f Rz=%1.2f Ax=%1.2f Ay=%1.2f Az=%1.2f\n",imu.rx,imu.ry,imu.rz,imu.ax,imu.ay,imu.az);
	USART_puts(USART3,temp_main);
	sprintf(temp_main,"fx=%1.2f fy=%1.2f fz=%1.2f tx=%1.2f ty=%1.2f tz=%1.2f\n",ft1.fx,ft1.fy,ft1.fz,ft1.tx,ft1.ty,ft1.tz);
	USART_puts(USART3,temp_main);
	sprintf(temp_main,"fx2=%1.2f fy2=%1.2f fz2=%1.2f tx2=%1.2f ty2=%1.2f tz2=%1.2f\n",ft2.fx,ft2.fy,ft2.fz,ft2.tx,ft2.ty,ft2.tz);
	USART_puts(USART3,temp_main);
	USART_puts(USART3,"\n\n");
	delay_ms(300);
	//NAND_read_record ( start_address,nand_address_pointer++,record);
	
	 //control function

	//add_to_fifo3( record,10);
	//delay_ms(200);
	
	}//end while
}//end main
