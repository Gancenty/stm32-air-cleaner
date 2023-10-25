#ifndef _COMM_H
#define _COMM_H
#include "includes.h"

#define MODE_STA_FUN  0X00
#define HUMI_STA_FUN     0X01
#define FAN_STA_FUN      0X02
#define VIOLED_STA_FUN   0X03

#define TEMP_DATA_FUN   0X04
#define HUMI_DATA_FUN   0X05
#define PM25_DATA_FUN   0X06
#define AIR_DATA_FUN    0X07

#define VIOLED_OFFTIME_FUN 0X08
#define RTC_TIME_FUN    	 0X09
#define P_TAIL 0XFF

void Air_Up(void);
void Air_Down(void);
void Air_Control(void);
void Air_Config(void);






#endif 
