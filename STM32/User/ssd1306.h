#ifndef _SSD1306_H
#define _SSD1306_H
#include "iic.h"
#include "fonts.h"
void OLED_Config(void);

void OLED_ON(void);
void OLED_OFF(void);
void OLED_SetContrast(unsigned char constrast);
void OLED_Full(void);
void OLED_Clean(void);
void OLED_FlipAll(unsigned char mode);
void OLED_DrawFrame(unsigned char data[8][128]);
void OLED_DrawP(unsigned char x,unsigned char y,unsigned char color);

void OLED_GReset(void);
void OLED_GSet(void);
void OLED_GFlipPart(unsigned char x,unsigned char y,unsigned char width,unsigned char height);
void OLED_GModifyPart(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char color);
void OLED_GDrawBitmap(unsigned char x,unsigned char y,const unsigned char*data,unsigned char width,unsigned char height);
void OLED_GDrawChar(unsigned char x,unsigned char y,const char data,unsigned char width,unsigned char height);
void OLED_GDrawString(unsigned char x,unsigned char y,const char *data,unsigned char width,unsigned char height);
void OLED_GDrawL_V(unsigned char X_B,unsigned char Y_B,unsigned char height);
void OLED_GDrawL_R(unsigned char X_B,unsigned char Y_B,unsigned char width);
void OLED_GDrawHollowRect(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight);
void OLED_GDrawSolidRect(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight);
void OLED_GDrawNetRect(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight);
void OLED_GDrawRectdotO(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight,unsigned char density);
void OLED_GDrawRectdotI(unsigned char xpos,unsigned char ypos,unsigned char iwidth,unsigned char iheight,unsigned char density);
void OLED_Display(void);
void OLED_Part_Display(unsigned char x,unsigned char y,unsigned char width,unsigned char height);

void OLED_HoriScroll(unsigned char page_begin,unsigned char page_end,unsigned char dir,unsigned char frame);
void OLED_Scroll_Activate(void);
void OLED_Scroll_Deactivate(void);
#endif
