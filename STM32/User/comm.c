#include "comm.h"
void Air_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//加湿器控制引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;           
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//风扇控制引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;           
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//紫外线灯控制引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;           
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Air_Up(void)
{
	//1
	air.txtemp[0]=HUMI_STA_FUN;
	air.txtemp[1]=air.humi_sta;
	air.txtemp[2]=0xff;
	USART1_SendHex(air.txtemp,3);
	//2
	air.txtemp[0]=FAN_STA_FUN;
	air.txtemp[1]=air.fan_sta;
	air.txtemp[2]=0xff;
	USART1_SendHex(air.txtemp,3);
	//3
	air.txtemp[0]=VIOLED_STA_FUN;
	air.txtemp[1]=air.violed_sta;
	air.txtemp[2]=0xff;
	USART1_SendHex(air.txtemp,3);
	//4
	uint16_t temp=air.temperature*100;
	air.txtemp[0]=TEMP_DATA_FUN;
	air.txtemp[1]=(uint8_t)(temp>>8);
	air.txtemp[2]=temp;
	air.txtemp[3]=0xff;
	USART1_SendHex(air.txtemp,4);
	//5
	temp=air.humidity*100;
	air.txtemp[0]=HUMI_DATA_FUN;
	air.txtemp[1]=(uint8_t)(temp>>8);
	air.txtemp[2]=temp;
	air.txtemp[3]=0xff;
	USART1_SendHex(air.txtemp,4);
	//6
	temp=air.pm25_num*100;
	air.txtemp[0]=PM25_DATA_FUN;
	air.txtemp[1]=(uint8_t)(temp>>8);
	air.txtemp[2]=temp;
	air.txtemp[3]=0xff;
	USART1_SendHex(air.txtemp,4);
	//7
	temp=air.air_quality*100;
	air.txtemp[0]=AIR_DATA_FUN;
	air.txtemp[1]=(uint8_t)(temp>>8);
	air.txtemp[2]=temp;
	air.txtemp[3]=0xff;
	USART1_SendHex(air.txtemp,4);
	//8
	air.txtemp[0]=MODE_STA_FUN;
	air.txtemp[1]=air.mode;
	air.txtemp[2]=0xff;
	USART1_SendHex(air.txtemp,3);
	//9
	air.txtemp[0]=VIOLED_OFFTIME_FUN;
  air.txtemp[1]=air.violed_offtime>>24;
  air.txtemp[2]=air.violed_offtime>>16;
  air.txtemp[3]=air.violed_offtime>>8;
  air.txtemp[4]=air.violed_offtime>>0;
	air.txtemp[5]=0xff;
	USART1_SendHex(air.txtemp,6);
}
void Air_Down(void)
{
	uint32_t timestamp=0;
	if(air.rxflag==1){
		if(air.rxlen<1){
			air.rxflag=0;
			air.rxlen=0;
			return;
		}
		switch(air.rxtemp[0]){
			case HUMI_STA_FUN:air.humi_sta=air.rxtemp[1];break;
			case FAN_STA_FUN:air.fan_sta=air.rxtemp[1];break;
			case VIOLED_STA_FUN:air.violed_sta=air.rxtemp[1];break;
			case RTC_TIME_FUN:
				timestamp=(uint32_t)(air.rxtemp[1]<<24)|(uint32_t)(air.rxtemp[2]<<16)|(uint32_t)(air.rxtemp[3]<<8)|(uint32_t)(air.rxtemp[4]<<0);
				RTC_set(timestamp);
				break;
			case VIOLED_OFFTIME_FUN:
				air.violed_offtime=(uint32_t)(air.rxtemp[1]<<24)|(uint32_t)(air.rxtemp[2]<<16)|(uint32_t)(air.rxtemp[3]<<8)|(uint32_t)(air.rxtemp[4]<<0);
				break;
			case MODE_STA_FUN:
				air.mode=air.rxtemp[1];
				if(air.mode==MANUAL_MODE){
					air.humi_sta=0;
					air.fan_sta=0;
					air.violed_sta=0;
				}
				break;
			default:break;
		}
		air.rxflag=0;
		air.rxlen=0;
		Air_Up();
	}
}
void Air_Control(void)
{
	static uint8_t violed_last_state=0;
	if(violed_last_state==0){
		if(air.violed_sta==1){
			air.violed_start_time=air.time.timestamp;
		}
	}
	violed_last_state=air.violed_sta;
	if(air.violed_sta==1){
		if(air.time.timestamp>air.violed_start_time+air.violed_offtime*60){
			air.violed_sta=0;
		}
	}
	if(air.mode==AUTO_MODE){
		if(air.temperature>26){
			air.fan_sta=1;
		}
		if(air.pm25_num>50){
			air.fan_sta=1;
		}
		if(air.humidity<40){
			air.humi_sta=1;
		}
	}
	if(air.need_update){
		air.need_update=0;
		Air_Up();
	}
	if(air.humi_sta){
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);	
	}else{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}
	if(air.fan_sta){
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
	if(air.violed_sta){
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
}
