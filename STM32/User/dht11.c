#include "dht11.h"

volatile uint8_t dht11_buff[5];

void DHT11_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=DHT11_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT,&GPIO_InitStruct);

	GPIO_SetBits(DHT11_PORT,DHT11_PIN);	
}
void DHT11_OUT(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=DHT11_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT,&GPIO_InitStruct);
}
void DHT11_IN(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=DHT11_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT,&GPIO_InitStruct);

	GPIO_SetBits(DHT11_PORT,DHT11_PIN);	
}
uint8_t DHT11_START(void)
{
	
	DHT11_OUT();
	GPIO_ResetBits(DHT11_PORT,DHT11_PIN);
	delay_ms(20);
	GPIO_SetBits(DHT11_PORT,DHT11_PIN);
	delay_us(30);
	DHT11_IN();
	uint8_t rpt=0;
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==Bit_SET){		//等待响应
		delay_us(5);
		rpt++;
		if(rpt>5){
			return 0;
		}
	}
	rpt=0;
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==Bit_RESET){	//拉低80us
		delay_us(10);
		rpt++;
		if(rpt>15){
			return 0;
		}
	}
	rpt=0;
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==Bit_SET){		//拉高80us
		delay_us(10);
		rpt++;
		if(rpt>15){
			return 0;
		}
	}
	return 1;
}
uint8_t DHT11_GET_BIT(void)
{
	uint8_t rpt=0,bit=0;
	rpt=0;
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==Bit_RESET){		//拉低50us
		delay_us(5);
		rpt++;
		if(rpt>12){
			return 255;
		}
	}
	delay_us(35);
	bit=GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN);
	rpt=0;
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==Bit_SET){		//拉低50us
		delay_us(5);
		rpt++;
		if(rpt>10){
			return 255;
		}
	}
	return bit;
}
uint8_t DHT11_GET_BYTE(void)
{
	uint8_t ret,bit;
	for(uint8_t i=0;i<8;i++){
		bit=DHT11_GET_BIT();
		if(bit<2){
			ret=ret<<1;
			ret+=bit;
		}else{
			return 255;
		}
	}
	return ret;
}

uint8_t DHT11_GetData(float *temper,float *humi)
{
	uint8_t check=0;
	if(DHT11_START()==0){
		return 0;
	}
	for(uint8_t i=0;i<5;i++){
		uint8_t byte=DHT11_GET_BYTE();
		if(byte!=255){
			dht11_buff[i]=byte;
		}else{
			return 0;
		}
	}
	for(uint8_t i=0;i<4;i++){
		check+=dht11_buff[i];
	}
	if(check==dht11_buff[4]){
		*humi=(float)dht11_buff[0]+(float)dht11_buff[1]/10;
		*temper=(float)dht11_buff[2]+(float)dht11_buff[3]/10;
		return 1;
	}else{
		return 0;
	}
}



