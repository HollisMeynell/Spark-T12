/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#ifndef		__CONFIG_H
#define		__CONFIG_H

/*********************************************************/

#define MAIN_Fosc		30000000L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
//#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��
//#define MAIN_Fosc		24000000L	//������ʱ��


/*********************************************************/

#include	"STC8xxxx.H"
#include	"delay.h"
#include	"timer.h"
#include	"GPIO.h"
#include "oled.h"
//#include "bmp.h"
#include	"adc.h"
#include "UART.h"
#include 	"eeprom.h"
#include	"PWM.h"


#include	"beep.h"
#include  "key.h"
#include "interface.h"
#include "Ch224d.h"
#include "Spark_T12_PID.h"
#include "sleep.h"

#include <stdio.h>
#include <stdlib.h>

#define     Max_Length          3      //��дEEPROM���峤��

#endif
