#ifndef _DHT11_H
#define _DHT11_H
#include "includes.h"
#define DHT11_PORT GPIOB
#define DHT11_PIN  GPIO_Pin_10

void DHT11_Config(void);
uint8_t DHT11_GetData(float *temper,float *humi);




#endif
