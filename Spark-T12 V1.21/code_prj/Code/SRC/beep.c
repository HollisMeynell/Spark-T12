#include	"beep.h"

/************* ���غ����ͱ������� *****************/

bit beep_fig;
/***************************************************/

/***********************************************************
*@fuction	:Beep
*@brief		:������Դ������
*@param		:ON/OFF
*@return	:void
*@author	:--xptx
*@date		:2022-10-02
***********************************************************/

void Beep(bit x)
{
    if(!x)  //�򿪷�����
    {
        //beep_fig=1;
        TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����
        TIM_InitStructure.TIM_Interrupt = ENABLE;					//�ж��Ƿ�����,   ENABLE��DISABLE. (ע��: Timer2�̶�Ϊ16λ�Զ���װ, �жϹ̶�Ϊ�����ȼ�)
        TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
        TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
        TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 8000UL);				//��ֵ
        TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
        Timer_Inilize(Timer2, &TIM_InitStructure);					//��ʼ��Timer2	  Timer0,Timer1,Timer2,Timer3,Timer4
    }
    else  if(x)//�رշ�����
    {
        //beep_fig=0;
        TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����
        TIM_InitStructure.TIM_Interrupt = DISABLE;					//�ж��Ƿ�����,   ENABLE��DISABLE. (ע��: Timer2�̶�Ϊ16λ�Զ���װ, �жϹ̶�Ϊ�����ȼ�)
        TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
        TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
        TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 8000UL);				//��ֵ
        TIM_InitStructure.TIM_Run       = DISABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
        Timer_Inilize(Timer2, &TIM_InitStructure);					//��ʼ��Timer2	  Timer0,Timer1,Timer2,Timer3,Timer4
        //delay_ms(1);
        beep = 0;
    }

}
