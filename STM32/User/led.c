#include "led.h"

void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_SetBits(GPIOC,GPIO_Pin_1);
}
void LED_Blink(void)
{
	static uint8_t flag=0;
	if(flag){
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}else{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
	flag=!flag;
}
