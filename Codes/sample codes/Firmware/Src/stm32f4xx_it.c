/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "stm32f4xx_it.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CircularBuffer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// CircularBuffer SpiBuffer;
// uint8_t Response[50];
// int ResponseIndex=0;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}
//void BufferInit()
//{
//	SpiBuffer=CircularBufferCreate(6);
//}
//void SendSSID()
//{
//
//	ResponseIndex=0;
//
//	Response[0]=0x7e;
//	Response[1]=8;
//	Response[2]=1;
//	Response[3]='N';
//	Response[4]='A';
//	Response[5]='M';
//	Response[6]='E';
//	Response[7]=0x7e;
//	HAL_UART_Transmit(&huart3, "name\n", 5, HAL_MAX_DELAY);
//
//}
//void SendPassword()
//{
//	ResponseIndex=0;
//
//	Response[0]=0x7e;
//	Response[1]=12;
//	Response[2]=2;
//	Response[3]='P';
//	Response[4]='A';
//	Response[5]='S';
//	Response[6]='S';
//	Response[7]='W';
//	Response[8]='O';
//	Response[9]='R';
//	Response[10]='D';
//	Response[11]=0x7e;
//	HAL_UART_Transmit(&huart3, "pass\n", 5, HAL_MAX_DELAY);
//}
//int PacketCompleted(uint8_t data)
//{
//	 uint8_t commandBuffer[6];
//	 char temp[50];
//
//    CircularBufferPush(SpiBuffer,&data, 1 );
//    CircularBufferRead(SpiBuffer,CircularBufferGetDataSize(SpiBuffer),&commandBuffer[0]);
//
//       sprintf(temp,"Data=%x %x %x %x %x %x\n",commandBuffer[0],commandBuffer[1],commandBuffer[2],commandBuffer[3],commandBuffer[4],commandBuffer[5]);
//   	HAL_UART_Transmit(&huart3, temp, strlen(temp), HAL_MAX_DELAY);
//	if(commandBuffer[0]==0x7e && commandBuffer[5]==0x7e ){
//		if(commandBuffer[2]==1)SendSSID();
//		if(commandBuffer[2]==2)SendPassword();
//
//
//
//	}
//
//
//}
/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles SPI2 global interrupt.
  */
void SPI2_IRQHandler(void)
{
  /* USER CODE BEGIN SPI2_IRQn 0 */
//	uint8_t rxdata;
	//uint8_t txdata;


	GPIOD->ODR^=1<<12;


	//txdata=Response[ResponseIndex];
	//if(ResponseIndex<=50)ResponseIndex++;
	//HAL_SPI_TransmitReceive(&hspi2,&txdata,&rxdata,1,1);
	hspi2.Instance->DR=WIFIGetChar();
	WIFIPutChar(hspi2.Instance->DR);


	//hspi2.Instance->DR=txdata;
	//PacketCompleted(hspi2.Instance->DR);


	//sprintf(temp,"ESP32Data=%x\n",rxdata);
	//HAL_UART_Transmit(&huart3, temp, sizeof(temp), HAL_MAX_DELAY);

  	  /* USER CODE END SPI2_IRQn 0 */
 // HAL_SPI_IRQHandler(&hspi2);

  /* USER CODE BEGIN SPI2_IRQn 1 */

  HAL_SPI_Abort(&hspi2);
  /* USER CODE END SPI2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
