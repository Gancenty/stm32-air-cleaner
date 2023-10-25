#include "ui.h"
static char temp[100];
void AIR_DATA_UI(void)
{
	uint8_t offset=0;
	OLED_GReset();
	
	offset=0;
	OLED_GDrawBitmap(offset,0,ch[6],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[8],CH_SIZE,CH_SIZE);
	sprintf(temp,":%.1f",air.temperature);
	OLED_GDrawString(24,0,temp,WIDTH,HIGHT);
	OLED_GDrawBitmap(24+strlen(temp)*6,0,ch[8],CH_SIZE,CH_SIZE);
	
	offset=79;
	if(air.mode==MANUAL_MODE)
	{	
		OLED_GDrawBitmap(offset,0,ch[11],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[13],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[9],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[10],CH_SIZE,CH_SIZE);
	}else if(air.mode==AUTO_MODE){
		OLED_GDrawBitmap(offset,0,ch[12],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[13],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[9],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[10],CH_SIZE,CH_SIZE);
	}
	
	offset=0;
	OLED_GDrawBitmap(offset,15,ch[7],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,15,ch[8],CH_SIZE,CH_SIZE);
	sprintf(temp,":%.1f%%RH",air.humidity);
	OLED_GDrawString(24,15,temp,WIDTH,HIGHT);

	sprintf(temp,"PM2.5:%.1fug/m3",air.pm25_num);
	OLED_GDrawString(0,30,temp,WIDTH,HIGHT);
	
	offset=0;
	OLED_GDrawBitmap(offset,45,ch[14],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,45,ch[15],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*2,45,ch[16],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*3,45,ch[17],CH_SIZE,CH_SIZE);
	sprintf(temp,":%.1fv",air.air_quality);
	OLED_GDrawString(12*4,45,temp,WIDTH,HIGHT);
	
	OLED_Display();	
	
}
void AIR_CTL_UI(void)
{
	uint8_t offset=0;
	OLED_GReset();
	//空气净化装置
	offset=0;
	OLED_GDrawBitmap(offset,0,ch[0],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[1],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[2],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[3],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*4,0,ch[4],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*5,0,ch[5],CH_SIZE,CH_SIZE);
	
	offset=79;
	if(air.mode==MANUAL_MODE)
	{	
		OLED_GDrawBitmap(offset,0,ch[11],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[13],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[9],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[10],CH_SIZE,CH_SIZE);
	}else if(air.mode==AUTO_MODE){
		OLED_GDrawBitmap(offset,0,ch[12],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[13],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[9],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[10],CH_SIZE,CH_SIZE);
	}
	//风扇
	offset=0;
	OLED_GDrawBitmap(offset,15,ch[18],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,15,ch[19],CH_SIZE,CH_SIZE);
	sprintf(temp,":%s",air.fan_sta?"ON":"OFF");
	OLED_GDrawString(24,15,temp,WIDTH,HIGHT);
	//加湿器
	offset=0;
	OLED_GDrawBitmap(offset,30,ch[24],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,30,ch[25],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*2,30,ch[26],CH_SIZE,CH_SIZE);
	sprintf(temp,":%s",air.humi_sta?"ON":"OFF");
	OLED_GDrawString(12*3,30,temp,WIDTH,HIGHT);
	//紫外线灯
	offset=0;
	OLED_GDrawBitmap(offset,45,ch[20],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,45,ch[21],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*2,45,ch[22],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*3,45,ch[23],CH_SIZE,CH_SIZE);
	sprintf(temp,":%s",air.violed_sta?"ON":"OFF");
	OLED_GDrawString(12*4,45,temp,WIDTH,HIGHT);
	
	OLED_Display();	
}
void AIR_SET_UI(void)
{
	uint8_t offset=0;
	OLED_GReset();
	//关闭时间
	offset=0;
	OLED_GDrawBitmap(offset,0,ch[27],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[28],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[29],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[30],CH_SIZE,CH_SIZE);
	
	offset=79;
	if(air.mode==MANUAL_MODE)
	{	
		OLED_GDrawBitmap(offset,0,ch[11],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[13],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[9],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[10],CH_SIZE,CH_SIZE);
	}else if(air.mode==AUTO_MODE){
		OLED_GDrawBitmap(offset,0,ch[12],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*1,0,ch[13],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*2,0,ch[9],CH_SIZE,CH_SIZE);
		OLED_GDrawBitmap(offset+CH_SIZE*3,0,ch[10],CH_SIZE,CH_SIZE);
	}
	
	offset=10;
	OLED_GDrawString(0,15,"-",WIDTH,HIGHT);
	OLED_GDrawBitmap(offset,15,ch[20],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*1,15,ch[21],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*2,15,ch[22],CH_SIZE,CH_SIZE);
	OLED_GDrawBitmap(offset+CH_SIZE*3,15,ch[23],CH_SIZE,CH_SIZE);
	sprintf(temp,":%dmin",air.violed_offtime);
	OLED_GDrawString(12*4+offset,15,temp,WIDTH,HIGHT);
	
	OLED_Display();
}
void Air_UI(void)
{
	switch(air.ui_mode)
	{
		case CTR_UI:AIR_CTL_UI();break;
		case DATA_UI:AIR_DATA_UI();break;
		case SET_UI:AIR_SET_UI();break;
		default:break;
	}
}

