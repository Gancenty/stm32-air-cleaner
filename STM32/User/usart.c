#include "usart.h"

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1,ch);	
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
	return ch;
}
static uint8_t temp=0;
void USART1_Config(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel =USART1_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_DeInit(USART1);
	USART_InitStruct.USART_BaudRate =bound;
	USART_InitStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity =USART_Parity_No;
	USART_InitStruct.USART_StopBits =USART_StopBits_1;
	USART_InitStruct.USART_WordLength =USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART2_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_DeInit(USART2);
	USART_InitStruct.USART_BaudRate =bound;
	USART_InitStruct.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity =USART_Parity_No;
	USART_InitStruct.USART_StopBits =USART_StopBits_1;
	USART_InitStruct.USART_WordLength =USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);
	USART_Cmd(USART2,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin =GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel =USART2_IRQn; 
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}
void USART1_IRQHandler(void)                
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		temp = USART_ReceiveData(USART1);
		if(temp==0XFF){
			air.rxflag=1;
			Air_Down();
		}else{
			air.rxtemp[air.rxlen++]=temp;
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	} 
} 
void USART2_IRQHandler(void)                
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	
} 
void USART1_SendStr(char* str)
{
	while(*str!='\0'){
		USART_SendData(USART1,*str++);	
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)!=SET);
	}
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
}
void USART2_SendStr(char *str)
{
	while(*str!='\0'){
		USART_SendData(USART2,*str++);	
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)!=SET);
	}	
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)!=SET);
}
void USART1_SendHex(uint8_t *data,uint16_t len)
{
	while(len--){
		USART_SendData(USART1,*data++);	
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	}	
}
void USART2_SendHex(uint8_t *data,uint16_t len)
{
	while(len--){
		USART_SendData(USART2,*data++);	
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)==RESET);
	}
}

