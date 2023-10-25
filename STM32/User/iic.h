#ifndef _IIC_H
#define _IIC_H
#include "includes.h"

#define IIC_SDA  GPIOB
#define IIC_SCL  GPIOB

#define IIC_SDA_PIN GPIO_Pin_9
#define IIC_SCL_PIN GPIO_Pin_8

#define IIC_SDA_H GPIO_SetBits(IIC_SDA,IIC_SDA_PIN)
#define IIC_SDA_L GPIO_ResetBits(IIC_SDA,IIC_SDA_PIN)

#define IIC_SCL_H GPIO_SetBits(IIC_SCL,IIC_SCL_PIN)
#define IIC_SCL_L GPIO_ResetBits(IIC_SCL,IIC_SCL_PIN)

void IIC_Config(void);
void IIC_Set_Delay_Us(uint8_t us);
void IIC_OUT(void);
void IIC_IN(void);
void IIC_DELAY(void);
void IIC_START(void);
void IIC_STOP(void);
void IIC_ACK(void);
void IIC_NACK(void);
unsigned char IIC_WAIT_ACK(void);
void IIC_SEND_BYTE(unsigned char byte);
unsigned char IIC_RECEIVE_BYTE(unsigned char ack);

uint8_t IIC_READ_BYTE(uint8_t addr,uint8_t reg);
uint8_t IIC_WRITE_BYTE(uint8_t addr,uint8_t reg,uint8_t data);
uint8_t IIC_READ(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data);
uint8_t IIC_WRITE(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data);
#endif
