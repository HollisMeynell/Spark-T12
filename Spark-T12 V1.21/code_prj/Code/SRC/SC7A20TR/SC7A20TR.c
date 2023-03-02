#include "SC7A20TR.h"
#include "IIC.h"
#include "stdio.h"	//ʹ��printf��ӡ���ݵ�����

/*************************�ⲿ��������******************************/

extern bit IIC_communication_busy;
extern u16 xx, yy, zz;

/*******************************************************************/


/*
SC7A20��Ĵ���дһ���ֽ�
RAddr���Ĵ�����ַ
WData������
*/
void SC7A20_Write_Byte(u8 RAddr, u8 WData)
{
    if(IIC_communication_busy == 0)
    {
        IIC_communication_busy = 1; //IIC���߹����־λ��æµ��

        Start();
        SendData(SC7A20_I2C_WADDR);
        RecvACK();
        SendData(RAddr);
        RecvACK();
        SendData(WData);
        RecvACK();
        Stop();

        IIC_communication_busy = 0; //IIC���߽�ű�־λ��δʹ�ã�
    }
}

/*
SC7A20�ӼĴ�����һ���ֽ�
RAddr���Ĵ�����ַ
RData���������ݵ�ַ
*/
void SC7A20_Read_Byte(u8 RAddr, u8 *RData)
{
    if(IIC_communication_busy == 0)
    {
        IIC_communication_busy = 1; //IIC���߹����־λ��æµ��

        Start();
        SendData(SC7A20_I2C_WADDR);
        RecvACK();
        SendData(RAddr);
        RecvACK();
        Start();
        SendData(SC7A20_I2C_RADDR);
        RecvACK();
        *RData = RecvData(0);
        Stop();

        IIC_communication_busy = 0; //IIC���߽�ű�־λ��δʹ�ã�
    }
}

/*
SC7A20��ʼ��
*/
void SC7A20_Init(void)
{
    SC7A20_Write_Byte(0x20, 0x77); //400Hz+����ģʽxyzʹ��
    SC7A20_Write_Byte(0x21, 0x88); //��ͨ�˲�������ģʽ,00+�˲�ʹ��
    SC7A20_Write_Byte(0x23, 0x88); //��ȡ����ٸ��¡�2g+����ģʽ
}

/*
SC7A20��ȡ���ٶ�ֵ
accd_lsb�����ٶ�ֵ�ͼĴ�����ַ
accd_msb�����ٶ�ֵ�߼Ĵ�����ַ
����ֵaccd_f�����ٶ�ֵ����λmg
*/
int SC7A20_Get_ACCD(u8 accd_lsb, u8 accd_msb)
{
    u8 accd_l, accd_m = 0;
    unsigned int temp;
    int accd;

    SC7A20_Read_Byte(accd_lsb, &accd_l);
    SC7A20_Read_Byte(accd_msb, &accd_m);
    temp &= 0x0000;
    temp |= accd_m;
    temp <<= 8;
    temp &= 0xff00;
    temp |= accd_l;
    if(temp & 0x8000)
    {
        temp >>= 4;
        temp |= 0xf000;
    }
    else
    {
        temp >>= 4;
        temp &= 0x0fff;
    }
    accd = temp * 1;
    return accd;
}

/*
SC7A20��ȡxyz����ٶ�ֵ
*/
void SC7A20_GetInfo(void)
{
    //��ȡ��������
    int x = 0, y = 0, z = 0;
    unsigned char i = 16;

    x = SC7A20_Get_ACCD(0x28, 0x29);
    y = SC7A20_Get_ACCD(0x2A, 0x2B);
    z = SC7A20_Get_ACCD(0x2C, 0x2D);

    xx = (u16)abs(x);
    yy = (u16)abs(y);
    zz = (u16)abs(z);



    //printf("x:%hd mg, y:%hd mg, z:%hd mg \r\n",x,y,z);
}
