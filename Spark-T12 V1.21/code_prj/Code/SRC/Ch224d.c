#include "Ch224d.h"


/*************************�ⲿ��������******************************/
extern u16 Start_addrs;
extern u8  xdata  eeprom_dat[10];
/*******************************************************************/


/* 0x0040 eeprom Ϊ��ѹѡ���¼��ַ*/
/***********************************************************
*@fuction	:power
*@brief		:���ƹ����ѹ
*@param		:5/9/12/15/20
*@return	:void
*@author	:--xptx
*@date		:2022-10-02
***********************************************************/
void power_Init(void)
{
    u8 i;
    i = eeprom_dat[0]; //��ȡ����

    if(i == 5 || i == 9 || i == 12 || i == 15 || i == 20) //����У��
        power(i);
    else
    {
        eeprom_dat[0] = 5;
        Eeprom_data_Init(Start_addrs, &eeprom_dat, 4);
        power(i);
    }
}

/***********************************************************
*@fuction	:power_record
*@brief		:  ���¼��乩���ѹ
*@param		:--
*@return	:void
*@author	:--xptx
*@date		:2022-11-13
***********************************************************/

void power_record(void)
{
    u8 i;  //��ʱ����
    EA = 0; //�ر��ж�
    //    i = (u8)read_rom(power_addrs); //��ȡ����//��ȡ��¼����
    if( (v_data_out() != i) && (v_data_out() != 0))    //�Ա�,io���õ�ѹֵ������ѹֵ�������io���õ�ѹֵ������0
        //        write_rom(power_addrs, (u16)v_data_out()); //д��io���õ�ѹֵ
        EA = 1; //���ж�
    //���¼���
}

void power(u8 x)
{
    if(x == 5)
    {
        CFG1 = 1;
        CFG2 = CFG3 = 0;
    }
    else if(x == 9)
    {
        CFG1 = CFG2 = CFG3 = 0;
    }
    else if(x == 12)
    {
        CFG1 = CFG2 = 0;
        CFG3 = 1;
    }
    else if(x == 15)
    {
        CFG1 = 0;
        CFG2 = CFG3 = 1;
    }
    else if(x == 20)
    {
        CFG1 = 0;
        CFG2 = 1;
        CFG3 = 0;
    }
}

/***********************************************************
*@fuction	:v_data_out
*@brief		:����GPIO״̬���ص�ǰ��ѹ����
*@param		:--void
*@return	:Current supply voltage
*@author	:--xptx
*@date		:2022-11-20
***********************************************************/

u8 v_data_out(void)
{
    u8 i;
    if( CFG1 == 0 && CFG2 == 1 && CFG3 == 0)  i = 20;
    else if( CFG1 == 0 && CFG2 == 1 && CFG3 == 1)  i = 15;
    else if( CFG1 == 0 && CFG2 == 0 && CFG3 == 1)  i = 12;
    else if( CFG1 == 0 && CFG2 == 0 && CFG3 == 0)  i = 9;
    else if( CFG1 == 1 && CFG2 == 0 && CFG3 == 0)  i = 5;
    else
        i = 0;
    return i;
}
