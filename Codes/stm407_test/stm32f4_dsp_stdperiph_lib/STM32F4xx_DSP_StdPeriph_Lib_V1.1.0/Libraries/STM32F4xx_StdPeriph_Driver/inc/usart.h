#include "stm32f4xx_usart.h"
#ifndef __usart_H
#define __usart_H

void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void USART3_Config(void);
void USART2_Config(void);
void USART1_Config(void);
void send_string3(char *s);
void send_string2(char*s);
void send_string(char*s);
void sendchar3(char ch);
void sendchar2(char ch);
void dbg(char*string);
#endif
