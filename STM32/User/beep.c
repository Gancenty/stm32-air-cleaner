#include "beep.h"

void BEEP_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
void BEEP_Blink(void)
{
	static uint8_t flag=0;
	if(flag){
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	}
	flag=!flag;
}


