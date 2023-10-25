#include "tick.h"

uint8_t IS_200HZ(void)
{
	static uint32_t last_ms=0;
	if(air.tim_ms-last_ms>5){
		last_ms=air.tim_ms;
		return 1;
	}else{
		return 0;
	}
}
uint8_t IS_100HZ(void)
{
	static uint32_t last_ms=0;
	if(air.tim_ms-last_ms>10){
		last_ms=air.tim_ms;
		return 1;
	}else{
		return 0;
	}	
}
uint8_t IS_10HZ(void)
{
	static uint32_t last_ms=0;
	if(air.tim_ms-last_ms>100){
		last_ms=air.tim_ms;
		return 1;
	}else{
		return 0;
	}
}
uint8_t IS_2HZ(void)
{
	static uint32_t last_ms=0;
	if(air.tim_ms-last_ms>500){
		last_ms=air.tim_ms;
		return 1;
	}else{
		return 0;
	}
}
uint8_t IS_1HZ(void)
{
	static uint32_t last_ms=0;
	if(air.tim_ms-last_ms>1000){
		last_ms=air.tim_ms;
		return 1;
	}else{
		return 0;
	}
}
uint8_t IS_5S(void)
{
	static uint32_t last_ms=0;
	if(air.tim_ms-last_ms>5000){
		last_ms=air.tim_ms;
		return 1;
	}else{
		return 0;
	}
}