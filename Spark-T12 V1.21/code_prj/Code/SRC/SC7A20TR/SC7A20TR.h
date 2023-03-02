#ifndef __SC7A20_H
#define __SC7A20_H

#include "config.h"
//#include	"STC8A_Delay.h"
//#include "delay.h"

#define SC7A20_I2C_WADDR 0x32	//I2Cд��ַ��7λI2C��ַ+һλд
#define SC7A20_I2C_RADDR 0x33	//I2Cд��ַ��7λI2C��ַ+һλ��

void SC7A20_Init(void);  //SC7A20��ʼ��
int SC7A20_Get_ACCD(u8 accd_lsb,u8 accd_msb);  //SC7A20��ȡ���ٶ�ֵ
void SC7A20_GetInfo(void);  //SC7A20��ȡxyz����ٶ�ֵ
int SC7A20_Get_ACCD(u8 accd_lsb,u8 accd_msb);


void SC7A20_Read_Byte(u8 RAddr, u8 *RData);

#endif  
