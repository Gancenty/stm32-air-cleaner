#ifndef _ADC_H
#define _ADC_H
#include "includes.h"


void ADC_Config(void);
unsigned short ADC_Get(uint8_t ch);
unsigned short ADC_Get_Aver(uint8_t ch);
void Air_Get_Info(void);
#endif
