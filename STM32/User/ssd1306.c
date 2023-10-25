#include "ssd1306.h"

#define OLED_ADR 0X3C
#define SSD1306_DELAY_US 2
#define pixel_height 64
#define pixel_width  128

static unsigned char isInit=0;
unsigned char GDDRAM[8][128]={0};//最小位是在最前行
const unsigned char  OLED_InitCmd[25]=
{
       0xAE,//关闭显示
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率

       0xA8,//设置驱动路数
       0X3F,//默认0X3F(1/64)
       0xD3,//设置显示偏移
       0X00,//默认为0
       0x40,//设置显示开始行 [5:0],行数.                              
       0x8D,//电荷泵设置
       0x14,//bit2，开启/关闭
	
       0x20,//设置内存地址模式
       0x00,//[1:0],00，水平地址模式   01,垂直地址模式   10,页地址模式;默认10;
	
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;  左右反转
	
			 0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 上下反转
	
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置   
			 
       0x81,//对比度设置
       0xEF,//1~255;默认0X7F (亮度设置,越大越亮)
			 
       0xD9,//设置预充电周期
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
			 
       0xDB,//设置VCOMH 电压倍率
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
			 
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
			 
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示   
			 
       0xAF,//开启显示     
};
/*.........................................GDDRAM操作.......................................*/
void OLED_GReset(void)//复位全部GDDRAM
{
	unsigned char i,j;
	for(i=0;i<128;i++){
		for(j=0;j<8;j++){
			GDDRAM[j][i]=0;
		}
	}
}
void OLED_GSet(void) //置位全部GDDRAM
{
	unsigned char i,j;
	for(i=0;i<128;i++){
		for(j=0;j<8;j++){
			GDDRAM[j][i]=0xff;
		}
	}	
}
static void OLED_GSetP(unsigned char x,unsigned char y)//置位单个点
{
	if(x>pixel_width-1||y>pixel_height-1)return;
	GDDRAM[y/8][x]|=(0x01<<(y%8));
}
static void OLED_GResetP(unsigned char x,unsigned char y)//复位单个点
{
	if(x>pixel_width-1||y>pixel_height-1)return;
	GDDRAM[y/8][x]&=~(0x01<<(y%8));
}
static unsigned char OLED_GGetP(unsigned char x,unsigned char y)//获取某个点
{
	if(x>pixel_width-1||y>pixel_height-1)return 0;
	return (GDDRAM[y/8][x]>>(y%8))&0x01;
}
static void OLED_GSetByte(unsigned char data,unsigned char page,unsigned char column)
{
	if(page>7||column>127)return;
	GDDRAM[page][column]=data;
}
/*.......................................................................................*/
/*.........................................IIC操作.......................................*/
static void OLED_SendCmd(unsigned char cmd)
{
	IIC_Set_Delay_Us(SSD1306_DELAY_US);
	IIC_START();
	IIC_SEND_BYTE(OLED_ADR<<1|0);//WRITE
	IIC_WAIT_ACK();
	IIC_SEND_BYTE(0x00);//   DC位为0 表示是cmd
	IIC_WAIT_ACK();
	IIC_SEND_BYTE(cmd);
	IIC_WAIT_ACK();
	IIC_STOP();	
}
static void OLED_SendData(unsigned char data)
{
	IIC_Set_Delay_Us(SSD1306_DELAY_US);
	IIC_START();
	IIC_SEND_BYTE(OLED_ADR<<1|0);
	IIC_WAIT_ACK();
	IIC_SEND_BYTE(0x40);//   DC位为1 表示是data
	IIC_WAIT_ACK();
	IIC_SEND_BYTE(data);
	IIC_WAIT_ACK();
	IIC_STOP();	
}
static void OLED_SendCmdCo(const unsigned char * cmd,unsigned int len)
{
	IIC_Set_Delay_Us(SSD1306_DELAY_US);
	IIC_START();
	IIC_SEND_BYTE(OLED_ADR<<1|0);//WRITE
	IIC_WAIT_ACK();
	for(unsigned int i=0;i<len;i++){
		IIC_SEND_BYTE(0x80);//   DC位为0 表示是cmd
		IIC_WAIT_ACK();
		IIC_SEND_BYTE(cmd[i]);
		IIC_WAIT_ACK();
	}
	IIC_STOP();	
}
static void OLED_SendDataCo(const unsigned char * data,unsigned int len)
{
	IIC_Set_Delay_Us(SSD1306_DELAY_US);
	IIC_START();
	IIC_SEND_BYTE(OLED_ADR<<1|0);//WRITE
	IIC_WAIT_ACK();
	unsigned int i;
	for(i=0;i<len;i++)
	{   
		IIC_SEND_BYTE(0xC0);//DC位为1 表示是data
		IIC_WAIT_ACK();
		IIC_SEND_BYTE(data[i]);
		IIC_WAIT_ACK();
	}
	IIC_STOP();	
}
static void OLED_Set_MemAddrMode(uint8_t mode)//0x00 水平地址  0x01垂直地址  0x02页地址
{
	OLED_SendCmd(0x20);
	OLED_SendCmd(mode);
}

//页模式寻址
static void Column_Set_P(unsigned char column)//列地址0-127
{
	if(column>127)return;
	OLED_SendCmd(0x10|(column>>4));    //设置列地址高位
	OLED_SendCmd(0x00|(column&0x0f));   //设置列地址低位         	 
}
static void Page_Set_P(unsigned char page)//页地址
{
	OLED_SendCmd(0xB0+page);
}
//水平寻址 垂直寻址
static void Column_Set_HV(uint8_t Column_Begin,uint8_t Column_End)//0-127
{
	if(Column_End>127||Column_Begin>Column_End)return;
	OLED_SendCmd(0x21);
	OLED_SendCmd(Column_Begin); 
	OLED_SendCmd(Column_End); 
}
static void Page_Set_HV(uint8_t Page_Begin,uint8_t Page_End)//0-7
{
	if(Page_End>7||Page_Begin>Page_End)return;
	OLED_SendCmd(0x22);
	OLED_SendCmd(Page_Begin); 
	OLED_SendCmd(Page_End); 
}
/*....................................................................................*/
void OLED_Config(void)
{	
	if(isInit==1)return;
	OLED_SendCmdCo(OLED_InitCmd,25);
}
void OLED_SetContrast(unsigned char constrast)//1-255
{
	OLED_SendCmd(0x81);
	OLED_SendCmd(constrast);
}
void OLED_Full(void)
{
	OLED_GSet();
	OLED_Display();
}
void OLED_Clean(void)
{
	OLED_GReset();
	OLED_Display();
}
void OLED_OFF(void)
{
	OLED_SendCmd(0xAE);//关闭
}
void OLED_ON(void)
{
	OLED_SendCmd(0xAF);//开启
}
void OLED_FlipAll(uint8_t mode)
{
	if(mode){
		OLED_SendCmd(0xA6);//1亮0灭
	}else{	
		OLED_SendCmd(0xA7);//1灭0亮
	}
}

/*..............................GDDRAM绘制函数..............................................*/
void OLED_GFlipPart(unsigned char x,unsigned char y,unsigned char width,unsigned char height)
{	
	unsigned char i,j;
	for(i=x;i<x+width;i++){
		for(j=y;j<y+height;j++){
			if(OLED_GGetP(i,j)){OLED_GResetP(i,j);}else{OLED_GSetP(i,j);}
		}
	}
}
void OLED_GModifyPart(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char color)
{
	unsigned char i,j;
	for(i=x1;i<x2;i++)
	{
		for(j=y1;j<y2;j++)
		{
			if(color)OLED_GSetP(i,j);else OLED_GResetP(i,j);
		}
	}
}
void OLED_GDrawBitmap(unsigned char x,unsigned char y,const unsigned char*data,unsigned char width,unsigned char height)
{
/*阴码 逐列式 逆向*/
	unsigned char i,j,offset=0,index=0,fast_fill=0;
	const unsigned char *p=data;
	offset=y%8;
	if(offset==0)fast_fill=1;
	offset=0;
	for(i=x;i<x+width;i++){
		for(j=y;j<y+height;j++){
			if(fast_fill){
				OLED_GSetByte(*p,j/8,i);
				j+=8;
				p++;
			}else{
				if((*p>>offset)&0x01){
					OLED_GSetP(i,j);
				}else{
					OLED_GResetP(i,j);
				}
				offset++;
				if(offset%8==0){p++;offset=0;};
			}
		}
		if(offset!=0){
			offset=0;p++;
		}
	}
}
void OLED_GDrawChar(unsigned char x,unsigned char y,const char data,unsigned char width,unsigned char height)
{
	/*阴码 逐列式 逆向*/
	unsigned char i,j,offset=0,index=0,fast_fill=0;
	const unsigned char *p;
	index=(int)data;
	if(index>126||index<32)return;//不在可绘制列表内
	p=&ascii[index-32][0];
	offset=y%8;
	if(offset==0)fast_fill=1;
	offset=0;
	for(i=x;i<x+width;i++){
		for(j=y;j<y+height;j++){
			if(fast_fill){
				OLED_GSetByte(*p,j/8,i);
				j+=8;
				p++;
			}else{
				if((*p>>offset)&0x01){
					OLED_GSetP(i,j);
				}else{
					OLED_GResetP(i,j);
				}
				offset++;
				if(offset%8==0){p++;offset=0;};
			}
		}
		if(offset!=0){
			offset=0;p++;
		}
	}
}
void OLED_GDrawString(unsigned char x,unsigned char y,const char *data,unsigned char width,unsigned char height)
{
	unsigned char x_pos=x;
	while(*data!=0){
		if(*data=='\n'){y+=height;data++;x_pos=x;continue;}
		if(x_pos>pixel_width-1||y>pixel_height-1){data++;continue;}
		OLED_GDrawChar(x_pos,y,*data,width,height);
		x_pos+=width;
		data++;
	}
}
void OLED_GDrawL_V(unsigned char X_B,unsigned char Y_B,unsigned char height)
{
	unsigned char Y;
	for(Y=Y_B;Y<Y_B+height;Y++)
	{
		OLED_GSetP(X_B,Y);
	}
}
void OLED_GDrawL_R(unsigned char X_B,unsigned char Y_B,unsigned char width)
{
	unsigned char X;
	for(X=X_B;X<X_B+width;X++)
	{
		OLED_GSetP(X,Y_B);
	}
}
void OLED_GDrawHollowRect(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight)
{
	OLED_GDrawL_R(xpos,ypos,iwidth);
	OLED_GDrawL_R(xpos,ypos+iheight-1,iwidth);
	
	OLED_GDrawL_V(xpos,ypos,iheight);
	OLED_GDrawL_V(xpos+iwidth-1,ypos,iheight);
}
void OLED_GDrawSolidRect(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight)
{
	unsigned char i,j;
	for(i=xpos;i<xpos+iwidth;i++){
		for(j=ypos;j<ypos+iheight;j++){
			OLED_GSetP(i,j);
		}
	}
}
void OLED_GDrawNetRect(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight)
{
	unsigned char i,j;
	for(i=xpos;i<xpos+iwidth;i++){
		for(j=ypos;j<ypos+iheight;j++){
			if(i%2==0||j%2==0)OLED_GResetP(i,j);else OLED_GSetP(i,j);
		}
	}
}

void OLED_GDrawRectdotO(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight,unsigned char density)
{
		unsigned char i;
    for(i=xpos+2;i<xpos+iwidth-2;i+=density){
      OLED_GSetP(i, ypos+2);
    }
    for(i=xpos+2;i<xpos+iwidth-2;i+=density){
      OLED_GSetP(i, ypos+iheight-3);
    }
    for(i=ypos+2;i<ypos+iheight-2;i+=density){
      OLED_GSetP(xpos+2, i);
    }
    for(i=ypos+2;i<ypos+iheight-2;i+=density){
      OLED_GSetP(xpos+iwidth-3, i);
    }
    OLED_GDrawHollowRect(xpos,ypos,iwidth,iheight);   
}
void OLED_GDrawRectdotI(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight,unsigned char density)
{
		unsigned char i;
    for(i=xpos;i<xpos+iwidth;i+=density){
      OLED_GSetP(i, ypos);
    }
    for(i=xpos;i<xpos+iwidth;i+=density){
      OLED_GSetP(i, ypos+iheight-1);
    }
    for(i=ypos;i<ypos+iheight;i+=density){
      OLED_GSetP(xpos, i);
    }
    for(i=ypos;i<ypos+iheight;i+=density){
      OLED_GSetP(xpos+iwidth-1, i);
    }
    OLED_GDrawHollowRect(xpos+2,ypos+2,iwidth-4,iheight-4);
}
void OLED_Display(void)
{
	unsigned char page,column;
	OLED_Set_MemAddrMode(0);
	Column_Set_HV(0,127);
	Page_Set_HV(0,7);
	OLED_SendDataCo((const unsigned char *)GDDRAM,128*8);
}
/*.........................................................................................*/
void OLED_DrawFrame(unsigned char data[8][128])
{
	unsigned char i,j;
	OLED_Set_MemAddrMode(0);
	Column_Set_HV(0,127);
	Page_Set_HV(0,7);
	OLED_SendDataCo((const unsigned char *)data,128*8);
}

void OLED_DrawP(unsigned char x,unsigned char y,unsigned char color)
{
	if(x>pixel_width-1||y>pixel_height-1)return;
	OLED_Set_MemAddrMode(0x02);//页模式
	Page_Set_P(y/8);
	Column_Set_P(x);
	if(color)GDDRAM[y/8][x]|=(0x01<<(y%8));else GDDRAM[y/8][x]&=~(0x01<<(y%8));
	OLED_SendData(GDDRAM[y/8][x]);
}
void OLED_Part_Display(unsigned char x,unsigned char y,unsigned char width,unsigned char height)
{
	unsigned char index;
	unsigned char max_page=(y+height-1)/8;
	unsigned char max_col=x+width-1;
	if(x>pixel_width-1||y>pixel_height-1)return;
	OLED_Set_MemAddrMode(0x0);//水平模式
	if(max_page>=8){
		max_page=7;
	}
	if(max_col>=pixel_width){
		max_col=127;
	}	
	Page_Set_HV(y/8,max_page);
	Column_Set_HV(x,x+width-1);
	for(index=y/8;index<=max_page;index++){
		OLED_SendDataCo((const unsigned char *)&GDDRAM[index][x],max_col-x+1);	
	}
}
void OLED_HoriScroll(unsigned char page_begin,unsigned char page_end,unsigned char dir,unsigned char frame)
{
	uint8_t cmd[]={0X26|dir,0x00,page_begin,frame,page_end,0x00,0xff};
	OLED_SendCmdCo(cmd,7);
}
void OLED_Scroll_Activate(void)
{
	OLED_SendCmd(0X2F);
	
}
void OLED_Scroll_Deactivate(void)
{
	OLED_SendCmd(0X2E);
	
}










