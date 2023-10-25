#ifndef _PROTOCOL_H
#define _PROTOCOL_H
#include "includes.h"

#define ANO_HEAD        0XAA
#define ANO_ADDR        0XFF
#define ADDI_BYTE_NUM   6

#define HEAD_LOC     0
#define ADDR_LOC     1
#define FUN_LOC      2
#define LEN_LOC      3


typedef enum ANO_FUN_T{
    VALID_SET=0X00,
    INERTIAL_SENSOR,
    OTHER_SENSOR,
    EULER_ARC,
    QUATERNION,
    ATTITUDE,
    FLY_MODE,
    FLY_SPEED,
    
    TARGET_ARC=0X0A,
    TARGET_SPEED=0X0B,

    BAT_INFO=0X0D,

    PWM_INFO=0X20,
    CONTROL_INFO=0X21,
    RGB_INFO=0X0F,
}ANO_T;


void ano_data_make_isensor(uint8_t *data, uint16_t *len);
void ano_data_make_euler_arc(uint8_t *data, uint16_t *len);
void ano_data_make_target_euler_arc(uint8_t *data, uint16_t *len);
void ano_data_make_ctrl_euler_arc(uint8_t *data, uint16_t *len);
void ano_data_make_rgb(uint8_t *data, uint16_t *len);
void ano_data_make_pwm(uint8_t *data, uint16_t *len);
void ano_data_make_atti(uint8_t *data, uint16_t *len);
void ano_data_make_flymode(uint8_t *data, uint16_t *len);
void ano_data_make_bat(uint8_t *data,uint16_t *len);
void ano_data_make_valid(uint8_t *data, uint16_t *len, char *rec, uint16_t rec_len);
void ano_data_decode(uint8_t *data,uint16_t *len);
void CanFly_Upload(void);
void CanFly_Decode(void);

#endif

