#include "sleep.h"
u16 sleep_io_v;
bit sleep_fig=0;
u16 sleep_time_s = 0, set_sleep_time = 5; //���߼�ʱ����������ʱ��
extern PWMx_Duty PWMA_Duty;
extern u8 z = 0;
/***********************************************************
*@fuction	:sleep_PWM_RST
*@brief		:�ر�PWM������
*@param		:--
*@return	:void
*@author	:--xptx
*@date		:2022-10-24
***********************************************************/
void sleep_PWM_RST(void)
{
    PWMx_InitDefine		PWMx_InitStructure;
    PWMx_InitStructure.PWM2_Mode       =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM2_SetPriority  = Priority_1;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    PWMx_InitStructure.PWM_Period      = 65535;							//����ʱ��,   0~65535    Լ11.6khz
    PWMx_InitStructure.PWM2_Duty       = PWMA_Duty.PWM2_Duty;	//PWM2ռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_DeadTime    = 0;								//��������������, 0~255
    PWMx_InitStructure.PWM_EnoSelect   = ENO2P;//ENO1P | ENO1N | ENO2P | ENO2N | ENO3P | ENO3N | ENO4P | ENO4N;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWMx_InitStructure.PWM_PS_SW       = PWM2_SW_P12_P13;//PWM1_SW_P60_P61| PWM2_SW_P62_P63 | PWM3_SW_P64_P65 | PWM4_SW_P66_P67;	//�л��˿�,		PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
    PWMx_InitStructure.PWM_CC2Enable   = DISABLE;				//����PWM2P���벶��/�Ƚ����,  ENABLE,DISABLE
    PWMx_InitStructure.PWM_MainOutEnable = DISABLE;				//�����ʹ��, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = DISABLE;				//ʹ�ܼ�����, ENABLE,DISABLE
    PWM_Configuration(PWMA, &PWMx_InitStructure);				//��ʼ��PWM,  PWMA,PWMB

}

/***********************************************************
*@fuction	:sleep_PWM_SET
*@brief		:����PWM������
*@param		:--
*@return	:void
*@author	:--xptx
*@date		:2022-10-24
***********************************************************/

void sleep_PWM_SET(void)
{
    PWMx_InitDefine		PWMx_InitStructure;

    PWMx_InitStructure.PWM2_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM2_SetPriority  = Priority_1;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    PWMx_InitStructure.PWM_Period   = 65535;							//����ʱ��,   0~65535    Լ11.6khz
    PWMx_InitStructure.PWM2_Duty    = PWMA_Duty.PWM2_Duty;	//PWM2ռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_DeadTime = 0;								//��������������, 0~255
    PWMx_InitStructure.PWM_EnoSelect   = ENO2P;//ENO1P | ENO1N | ENO2P | ENO2N | ENO3P | ENO3N | ENO4P | ENO4N;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWMx_InitStructure.PWM_PS_SW       = PWM2_SW_P12_P13;//PWM1_SW_P60_P61| PWM2_SW_P62_P63 | PWM3_SW_P64_P65 | PWM4_SW_P66_P67;	//�л��˿�,		PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
    PWMx_InitStructure.PWM_CC2Enable   = ENABLE;				//����PWM2P���벶��/�Ƚ����,  ENABLE,DISABLE
    PWMx_InitStructure.PWM_MainOutEnable = ENABLE;				//�����ʹ��, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;				//ʹ�ܼ�����, ENABLE,DISABLE
    PWM_Configuration(PWMA, &PWMx_InitStructure);				//��ʼ��PWM,  PWMA,PWMB

}

/***********************************************************
*@fuction	:sleep_show
*@brief		:
*@param		:--
*@return	:void
*@author	:--xptx
*@date		:2022-10-24
***********************************************************/

void sleep_show(void)
{
    static u16 i;
	i+=10;
			if(sleep_io_v >= 28) sleep_time_s = 0; //��ѹ����ֵ˵�������������������߼�ʱ����
			
			if((i % 1000) == 0)
			{
        OLED_Clear();//����
        OLED_ShowChinese( i/100 + 0, 1, 6, 12); //��
        OLED_ShowChinese(i/100 + 14, 1, 7, 12); //��
        OLED_ShowChinese(i/100 + 28, 1, 12, 12); //��
			}
			delay_ms(10);
			
		if(i>=12001) i=0;
}



/***********************************************************
*@fuction	:sleep_mode
*@brief		:����ģʽ
*@param		:--
*@return	:void
*@author	:--xptx
*@date		:2022-10-24
***********************************************************/

void sleep_mode(void)
{
    OLED_Clear();//����
    sleep_PWM_RST();  //�ر�PWM
	  sleep_fig=1; //���߱�־λ��һ
		P12=0;  //����pwm���IO��
    while(1)
    {
        sleep_show();  //������ʾ����
        if(sleep_io_v >= 28) sleep_time_s = 0; //��ѹ����ֵ˵�������������������߼�ʱ����

        if((sleep_time_s / 60) < set_sleep_time) //���߼�ʱС���趨ֵ����⵽�������������ã���������ģʽ
        {
            sleep_PWM_SET();
					  if(z==0)  //���ؽ���1
					interface_show1_Init();  //��ʼ������1
            //interface_show();
						// OLED_Clear();  //����
            break;
        }
			//if(P12) P12=0;  //�ظ���Ᵽ��
    }
	sleep_fig=0; //���߽�������־λ��0

}
