#include "includes.h"
AIR_CLEAN air;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	IO_Config();
  USART1_Config(115200);
	DELAY_Config();
	KEY_Config();
	Air_Config();
	DHT11_Config();
	LED_Config();
	ADC_Config();
	IIC_Config();
	OLED_Config();
	TIMER_Config();
	RTC_Config();
	air.violed_offtime=1;
	while(1)
	{
		if(IS_2HZ()){
			LED_Blink();
		}
		if(IS_1HZ()){
			Air_Get_Info();	
			DHT11_GetData(&air.temperature,&air.humidity);			
		}
		if(IS_5S()){
			Air_Up();
		}
		if(IS_10HZ()){
			Air_UI();
		}
		Air_Control();
		
	}
}
