#ifndef __USART_H
#define __USART_H
#include "includes.h"

void USART1_Config(uint32_t bound);
void USART2_Config(uint32_t bound);
void USART1_SendStr(char* str);
void USART2_SendStr(char* str);
void USART1_SendHex(uint8_t *data,uint16_t len);
void USART2_SendHex(uint8_t *data,uint16_t len);


#endif


