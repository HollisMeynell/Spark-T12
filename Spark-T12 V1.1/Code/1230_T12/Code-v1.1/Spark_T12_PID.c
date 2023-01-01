#include "Spark_T12_PID.h"
#include <math.h>
/**
    Kp:��������    Ek:����ƫ��ֵ   Ek_1:�ϴ�ƫ��ֵ   Ek_2:���ϴ�ƫ��ֵ
    Ti:����ʱ��    T:����ʱ��      Td:΢��ʱ��
**/
h16 Kp = 150, Ti = 5, T = 8, Td = 2, Ek;
static h16 Ek_1, Ek_2 ;
u16 present_temp, expect_temp = 300; //��ǰ�¶ȣ������¶�


/***********************************************************
*@fuction	:PID
*@brief		:����ʽPID
*@param		:E:��ǰֵ  K:����ֵ
*@return	:Output��Ҫ������ռ�ձȡ�
*@author	:--xptx
*@date		:2022-10-08
***********************************************************/

u16 PID(u16 E, u16 K)
{
    h16 Output;
    Ek = E - K;     //���㱾��ƫ���ǰֵ-����ֵ
    Output = Kp * (Ek - Ek_1) + Kp * (T / Ti) * Ek + Kp * (Td / T) * (Ek - 2 * Ek_1 + Ek_2) ;  //���㱾��PID
    Ek_2 = Ek_1;    //�������ϴ�ƫ��
    Ek_1 = Ek;      //������һ��ƫ��
    Output = abs(Output); //
    //if(Output<0) Output=0;
    return Output;  //���ؼ�����
}



//void data_send(u16 j)  //4λ���ݷ���
//{
//    PrintString1("\r\n");
//    //    PrintString1("T12��ADCֵΪ:\r\n");    //UART1����һ���ַ���
//    //    //Get_ADCResult(i);		//����0~15,��ѯ��ʽ��һ��ADC, ����һ��
//    //    //j = 3300 / 1024 * j;
//    //    TX1_write2buff('A');
//    //    TX1_write2buff('D');
//    //    //TX1_write2buff(i+'0');
//    //    TX1_write2buff('=');
//    TX1_write2buff(j / 1000 + '0');
//    TX1_write2buff(j % 1000 / 100 + '0');
//    TX1_write2buff(j % 100 / 10 + '0');
//    TX1_write2buff(j % 10 + '0');
//    //    TX1_write2buff(' ');
//    //    TX1_write2buff(' ');

//    PrintString1("\n");
//}

/***********************************************************
*@fuction	:order_fitting
*@brief		:���ݴ���������ADC����ĵ�ѹ����һ��4�׶���ʽ��ϣ������Ӧ�¶�
*@param		:--Thermocouple voltage of T12
*@return	:temp
*@author	:--xptx
*@date		:2022-11-10
***********************************************************/

u16 order_fitting(u16 dat)
{
    u16 temp;  //����һ������
    temp = 2 * pow(10, -11) * pow(dat, 4) + 2 * pow(10, -8) * pow(dat, 3) - 0.0004 * pow(dat, 2) + 0.7985 * dat + 24.271 ;  //4�׶���ʽ���
    return temp; //����
}
/***********************************************************
*@fuction	:filter
*@brief		:�˲�n�Σ�ȥ��һ�����ֵ��ȥ��һ����Сֵ�����µ���ƽ��ֵ
*@param		:--u8 Num �˲�����  Vcc_Adc �ڲ�16ͨ����ADCֵ  u16 dat ��������
*@return	:mean
*@author	:--xptx
*@date		:2022-11-11
***********************************************************/
u16 filter(u8 Num)
{ 
    u16 temp [20] ; //��ֵ����ֵ
    u16 max, mini, mean, sum = 0; //���ֵ ��Сֵ ƽ��ֵ
    u8 i;
	  u16 Vref; //MCU�����ѹ/��׼��ѹ��mV��
    for(i = 0; i < Num; i++)
    {
			  Vref=( 1190 / Get_ADCResult(15) ) * 1024 ;  //��������Դ��ѹ/��׼��ѹ
        temp [i] = ( Vref / 1024 ) * Get_ADCResult(0);
    }

    max  = temp[0];  //д���ֵ
    mini = temp[0];

    for(i = 0; i < Num; i++) //���������������Сֵ
    {
        if(max < temp[i])
            max = temp[i];
        if(mini > temp[i])
            mini = temp[i];
    }
    for(i = 0; i < Num; i++)
    {
        sum += temp[i];
    }
    mean = (sum - max - mini) / (Num - 2);
    return 	mean;
}
//u16 filter(u8 Num,u16 Vcc_Adc, u16 dat)
//{ 
//    u16 temp [20] ; //��ֵ����ֵ
//    u16 max, mini, mean, sum = 0; //���ֵ ��Сֵ ƽ��ֵ
//    u8 i;
//	  u16 Vcc; //MCU�����ѹ��mV��
//    for(i = 0; i < Num; i++)
//    {
//			Vcc=1190/1024 * Vcc_Adc;  //��������Դ��ѹ 
//        temp [i] = ( Vcc / 1024 ) * dat;
//    }

//    max  = temp[0];  //д���ֵ
//    mini = temp[0];

//    for(i = 0; i < Num; i++) //���������������Сֵ
//    {
//        if(max < temp[i])
//            max = temp[i];
//        if(mini > temp[i])
//            mini = temp[i];
//    }
//    for(i = 0; i < Num; i++)
//    {
//        sum += temp[i];
//    }
//    mean = (sum - max - mini) / (Num - 2);
//    return 	mean;
//}
