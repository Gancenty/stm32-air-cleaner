#ifndef _CONFIG_H
#define _CONFIG_H
#include "stm32f10x.h"

#define MAX_RX_TEMP     1024
#define MAX_TX_TEMP     1024

typedef enum{
	MANUAL_MODE,
	AUTO_MODE,
	MAX_MODE_COUNT
}MODE;
typedef enum{
	CTR_UI,
	DATA_UI,
	SET_UI,
	MAX_UI_COUNT
}UI;
typedef struct{
	u32 timestamp;
	u32 lastupdate;
	u8 	timezone;
	u16 year;
	u8 	month;
	u8 	day;
	u8 	weekday;
	u8 	hour;
	u8 	minute;
	u8 	second;	
}TIME;

typedef struct CanFly_Control{	
	uint8_t rxtemp[MAX_RX_TEMP];
	uint8_t txtemp[MAX_TX_TEMP];
	volatile uint16_t rxlen;
	volatile uint16_t txlen;
	uint8_t rxflag;
	
	uint8_t net;
	uint8_t mode;
	uint8_t ui_mode;
	uint32_t violed_offtime;
	uint32_t violed_start_time;
	uint8_t humi_sta,fan_sta,violed_sta;
	
	float temperature;
	float humidity;
	float pm25_num;
	float air_quality;
	
	uint8_t need_update;
	volatile uint32_t tim_ms;
	TIME time;
}AIR_CLEAN;

extern AIR_CLEAN air;



#endif
