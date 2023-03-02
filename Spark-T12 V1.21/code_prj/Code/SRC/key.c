#include "key.h"

/****�������ƺ���****/


u8 key_mode(void)
{
    u16 i = 0; //��ʱ����
    if(!k2)
    {

        Beep(ON);
        while(!k2) //��⵽�������£�����ѭ��
        {
            delay_ms(1);
            i++;  //��ʼ��ʱ
        }
        Beep(OFF);
    }
    if(i > 10 && i < 500) return 1; //��⵽�̰�
    else if(i > 500)      return 2;	 //��⵽����
    else return 0;
}


u8 key_add(void)
{
    u16 i = 0; //��ʱ����
    static u8 r;
    if(!k3)
    {
        Beep(ON);
        while(!k3) //��⵽�������£�����ѭ��
        {
            delay_ms(1);
            i++;  //��ʼ��ʱ
            //if(i>1500) break;
        }
        Beep(OFF);
    }
    if(i > 10 && i < 500) r = 1; //��⵽�̰�
    else if(i > 500)      r = 2;	 //��⵽����
    else r = 0;
    //		if(i>0) data_send(r);  //4λ���ݷ���
    return r;
}

u8 key_lessen(void)
{
    u16 i = 0; //��ʱ����
    u8 r;
    if(!k1)
    {
        Beep(ON);
        while(!k1) //��⵽�������£�����ѭ��
        {
            delay_ms(1);
            i++;  //��ʼ��ʱ
            //if(i>1500) break;
        }
        Beep(OFF);
    }
    if(i > 10 && i < 500) r = 1; //��⵽�̰�
    else if(i > 500)      r = 2;	 //��⵽����
    else r = 0;
    //if(i>0) data_send(r);  //4λ���ݷ���
    return r;
}