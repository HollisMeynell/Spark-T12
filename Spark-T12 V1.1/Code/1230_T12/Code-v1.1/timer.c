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

/*************	����˵��	**************

���ļ�ΪSTC8ϵ�еĶ�ʱ����ʼ�����жϳ���,�û�����������ļ����޸��Լ���Ҫ���жϳ���.

******************************************/

#include	"timer.h"
#include	"PWM.h"

//#define adc1 Get_ADCResult(1)
//#include "Spark_T12_PID.h"
//#include	"beep.h"
//extern bit fig1;
//extern beep P37

/*************	���ر�������	**************/
//bit PWM1_Flag;
//bit PWM2_Flag;
//bit PWM3_Flag;
//bit PWM4_Flag;

/*************  �ⲿ�����ͱ������� *****************/
extern PWMx_Duty PWMA_Duty;
extern u16 present_temp, expect_temp; //��ǰ�¶ȣ������¶�
extern u16 Ch1_vinput, NTC_temp_v, temp_T12;
extern bit sleep_fig;
extern u16 sleep_time_s, set_sleep_time;  //���߼�ʱ
extern u16 sleep_io_v;    //����IO�ڵ�ѹ
static u16 temporary_Duty;


/********************* Timer0�жϺ��� 1ms�ж�һ��************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
    static u16 i = 0;
    i++;
    if(i == 10)
    {
        sleep_io_v = (3300 / 1024) * Get_ADCResult(10); //��sleep���ŵ�ѹ
        NTC_temp_v = (3300 / 1024) * Get_ADCResult(14) / 1; //���㵱ǰNTC�¶�
    }

    if(i == 82)
    {
        PWMA_Duty.PWM2_Duty = 0;            //ռ�ձ�����
        UpdatePwm(PWMA, &PWMA_Duty);        //PWM ռ�ձ����ú���


    }
    if(i == 85)
    {
        Ch1_vinput = 11 * (3300 / 1024) * Get_ADCResult(1) / 1; //���㵱ǰ�����ѹ
        i = 0;

        present_temp = filter(10); //�˲�

        //       Vcc_ref =  ( 1190 / Get_ADCResult(15) ) * 1024 ;
        //      present_temp = ( Vcc_ref / 1024 ) * Get_ADCResult(0);

        //  present_temp =Vcc_ref;
        // 			present_temp =  3300/1024  * Get_ADCResult(0);  (  (1190 * 1024) / Get_ADCResult(15)  )
        temp_T12 = present_temp;
        present_temp = order_fitting(present_temp);  //�������¶�

        if(present_temp <= 450 && sleep_fig==0)  //�¶����Ʊ���
            PWMA_Duty.PWM2_Duty = PID(present_temp, expect_temp); //���㲢���ռ�ձ�
        else
            PWMA_Duty.PWM2_Duty = 0;

        if(present_temp < 70)  PWMA_Duty.PWM2_Duty = 58982; //������ADC�޷���ȷ�������ݣ���Ҫ���ȵ�һ���¶ȡ�

        UpdatePwm(PWMA, &PWMA_Duty);             //PWM ռ�ձ����ú���
    }
}

/********************* Timer1�жϺ���--���߿��� *******************2ms�ж�һ��*****/
void timer1_int (void) interrupt TIMER1_VECTOR
{
    static u16 ms2 = 0;
    ms2++;
    if(ms2 == 500)
    {
        sleep_time_s++;    //���߼�ʱ
        ms2 = 0;
    }
    if(sleep_io_v >= 28) sleep_time_s = 0; //��ѹ����ֵ˵�������������������߼�ʱ����
		
    if(sleep_time_s >= 65535) sleep_time_s = 60000; //�����

    //if((sleep_time_s/60)>=set_sleep_time)  sleep_mode();  //��ʱʱ�������������ʱ�䣬��������ģʽ���ڼ�ر�PWM
}

/********************* Timer2�жϺ���--���������� ************************/
void timer2_int (void) interrupt TIMER2_VECTOR
{ 
    beep = !beep;
}

///********************* Timer3�жϺ���************************/
//void timer3_int (void) interrupt TIMER3_VECTOR
//{
//
//}

///********************* Timer4�жϺ���************************/
//void timer4_int (void) interrupt TIMER4_VECTOR
//{
//
//}


//========================================================================
// ����: u8	Timer_Inilize(u8 TIM, TIM_InitTypeDef *TIMx)
// ����: ��ʱ����ʼ������.
// ����: TIMx: �ṹ����,��ο�timer.h��Ķ���.
// ����: �ɹ�����0, �ղ�������1,���󷵻�2.
// �汾: V1.0, 2012-10-22
//========================================================================
u8	Timer_Inilize(u8 TIM, TIM_InitTypeDef *TIMx)
{
    if(TIM > Timer4)	return 1;	//�ղ���

    if(TIM == Timer0)
    {
        Timer0_Stop();		//ֹͣ����
        if(TIMx->TIM_Interrupt == ENABLE)		Timer0_InterruptEnable();	//�����ж�
        else		Timer0_InterruptDisable();	//��ֹ�ж�
        if(TIMx->TIM_Priority > Priority_3)	return 2;	//����
        Timer0_Priority(TIMx->TIM_Priority);	//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

        if(TIMx->TIM_Mode >= TIM_16BitAutoReloadNoMask)	return 2;	//����
        TMOD = (TMOD & ~0x30) | TIMx->TIM_Mode;	//����ģʽ,0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ
        if(TIMx->TIM_ClkSource == TIM_CLOCK_12T)	Timer0_12T();	//12T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_1T)		Timer0_1T();	//1T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_Ext)	Timer0_AsCounter();	//����������Ƶ
        else		Timer0_AsTimer();	//��ʱ
        if(TIMx->TIM_ClkOut == ENABLE)	Timer0_CLKO_Enable();	//���ʱ��
        else		Timer0_CLKO_Disable();	//�����ʱ��

        T0_Load(TIMx->TIM_Value);
        if(TIMx->TIM_Run == ENABLE)	Timer0_Run();	//��ʼ����
        return	0;		//�ɹ�
    }

    if(TIM == Timer1)
    {
        Timer1_Stop();		//ֹͣ����
        if(TIMx->TIM_Interrupt == ENABLE)		Timer1_InterruptEnable();	//�����ж�
        else		Timer1_InterruptDisable();	//��ֹ�ж�
        if(TIMx->TIM_Priority > Priority_3)	return 2;	//����
        Timer1_Priority(TIMx->TIM_Priority);	//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
        if(TIMx->TIM_Mode >= TIM_16BitAutoReloadNoMask)	return 2;	//����
        TMOD = (TMOD & ~0x30) | TIMx->TIM_Mode;	//����ģʽ,0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ
        if(TIMx->TIM_ClkSource == TIM_CLOCK_12T)	Timer1_12T();	//12T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_1T)		Timer1_1T();	//1T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_Ext)	Timer1_AsCounter();	//����������Ƶ
        else		Timer1_AsTimer();	//��ʱ
        if(TIMx->TIM_ClkOut == ENABLE)	Timer1_CLKO_Enable();	//���ʱ��
        else		Timer1_CLKO_Disable();	//�����ʱ��

        T1_Load(TIMx->TIM_Value);
        if(TIMx->TIM_Run == ENABLE)	Timer1_Run();	//��ʼ����
        return	0;		//�ɹ�
    }

    if(TIM == Timer2)		//Timer2,�̶�Ϊ16λ�Զ���װ, �ж������ȼ�
    {
        Timer2_Stop();	//ֹͣ����
        if(TIMx->TIM_Interrupt == ENABLE)		Timer2_InterruptEnable();	//�����ж�
        else		Timer2_InterruptDisable();	//��ֹ�ж�
        if(TIMx->TIM_ClkSource >  TIM_CLOCK_Ext)	return 2;
        if(TIMx->TIM_ClkSource == TIM_CLOCK_12T)	Timer2_12T();	//12T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_1T)		Timer2_1T();	//1T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_Ext)	Timer2_AsCounter();	//����������Ƶ
        else		Timer2_AsTimer();		//��ʱ
        if(TIMx->TIM_ClkOut == ENABLE)	Timer2_CLKO_Enable();		//���ʱ��
        else		Timer2_CLKO_Disable();	//�����ʱ��

        T2_Load(TIMx->TIM_Value);
        if(TIMx->TIM_Run == ENABLE)	Timer2_Run();	//��ʼ����
        return	0;		//�ɹ�
    }

    if(TIM == Timer3)		//Timer3,�̶�Ϊ16λ�Զ���װ, �ж������ȼ�
    {
        Timer3_Stop();	//ֹͣ����
        if(TIMx->TIM_Interrupt == ENABLE)		Timer3_InterruptEnable();	//�����ж�
        else		Timer3_InterruptDisable();	//��ֹ�ж�
        if(TIMx->TIM_ClkSource >  TIM_CLOCK_Ext)	return 2;
        if(TIMx->TIM_ClkSource == TIM_CLOCK_12T)	Timer3_12T();	//12T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_1T)		Timer3_1T();	//1T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_Ext)	Timer3_AsCounter();	//����������Ƶ
        else		Timer3_AsTimer();		//��ʱ
        if(TIMx->TIM_ClkOut == ENABLE)	Timer3_CLKO_Enable();		//���ʱ��
        else		Timer3_CLKO_Disable();	//�����ʱ��

        T3_Load(TIMx->TIM_Value);
        if(TIMx->TIM_Run == ENABLE)	Timer3_Run();	//��ʼ����
        return	0;		//�ɹ�
    }

    if(TIM == Timer4)		//Timer3,�̶�Ϊ16λ�Զ���װ, �ж������ȼ�
    {
        Timer4_Stop();	//ֹͣ����
        if(TIMx->TIM_Interrupt == ENABLE)		Timer4_InterruptEnable();	//�����ж�
        else		Timer4_InterruptDisable();	//��ֹ�ж�
        if(TIMx->TIM_ClkSource >  TIM_CLOCK_Ext)	return 2;
        if(TIMx->TIM_ClkSource == TIM_CLOCK_12T)	Timer4_12T();	//12T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_1T)		Timer4_1T();	//1T
        if(TIMx->TIM_ClkSource == TIM_CLOCK_Ext)	Timer4_AsCounter();	//����������Ƶ
        else		Timer4_AsTimer();		//��ʱ
        if(TIMx->TIM_ClkOut == ENABLE)	Timer4_CLKO_Enable();		//���ʱ��
        else		Timer4_CLKO_Disable();	//�����ʱ��

        T4_Load(TIMx->TIM_Value);
        if(TIMx->TIM_Run == ENABLE)	Timer4_Run();	//��ʼ����
        return	0;		//�ɹ�
    }
    return 2;	//����
}
