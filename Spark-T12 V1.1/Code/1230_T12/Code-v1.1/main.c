#include	"config.h"



#define Sleep_Fig P32
PWMx_Duty PWMA_Duty;

//PWMx_Duty PWMA_Duty_rst;
u16 thermocouple_v, temp_T12, NTC_temp_v; //�ȵ�ż��ѹ������ͷ�¶ȣ�NTC�����ѹ��ѹ

u8  xdata   tmp[Max_Length];        //EEPROM��������
extern u16 sleep_time_s;  //���߼�ʱ
extern u16 sleep_time_s, set_sleep_time;  //���߼�ʱ
u16 Ch1_vinput;  //�����ѹ
/************************ IO������ ****************************/
void	GPIO_config(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;				//�ṹ����

    GPIO_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;				//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
    GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);	//��ʼ��

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_HighZ;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);	//��ʼ��

    GPIO_InitStructure.Pin  = GPIO_Pin_6;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
    GPIO_InitStructure.Mode = GPIO_HighZ;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);	//��ʼ��

    GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;				//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
    GPIO_InitStructure.Mode = GPIO_OUT_PP;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);	//��ʼ��

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;				//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
    GPIO_InitStructure.Mode = GPIO_OUT_PP;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);	//��ʼ��

    GPIO_InitStructure.Pin  = GPIO_Pin_All;			//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7, �����
    GPIO_InitStructure.Mode = GPIO_PullUp;			//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);	//��ʼ��
}


/******************* AD���ú��� *******************/
void	ADC_config(void)
{
    ADC_InitTypeDef		ADC_InitStructure;		//�ṹ����
    ADC_InitStructure.ADC_SMPduty   = 10;		//ADC ģ���źŲ���ʱ�����, 0~31��ע�⣺ SMPDUTY һ����������С�� 10��
    ADC_InitStructure.ADC_CsSetup   = 0;		//ADC ͨ��ѡ��ʱ����� 0(Ĭ��),1
    ADC_InitStructure.ADC_CsHold    = 1;		//ADC ͨ��ѡ�񱣳�ʱ����� 0,1(Ĭ��),2,3
    ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X1T;		//���� ADC ����ʱ��Ƶ��	ADC_SPEED_2X1T~ADC_SPEED_2X16T
    ADC_InitStructure.ADC_Power     = ENABLE;				//ADC��������/�ر�	ENABLE,DISABLE
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC�������,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
    ADC_InitStructure.ADC_Priority    = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����	ENABLE,DISABLE
    ADC_Inilize(&ADC_InitStructure);					//��ʼ��
    ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
}

/***************  ���ڳ�ʼ������ *****************/
//void	UART_config(void)
//{
//	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
//	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
//	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
//	COMx_InitStructure.UART_BaudRate  = 9600ul;			//������, һ�� 110 ~ 115200
//	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
//	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
//	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
//	COMx_InitStructure.UART_Priority    = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
//	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
//	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
//}

/************************ ��ʱ������ ****************************/
void	Timer_config(void)
{
    TIM_InitTypeDef		TIM_InitStructure;						//�ṹ����

    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_Priority  = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    TIM_InitStructure.TIM_Interrupt = ENABLE;					//�ж��Ƿ�����,   ENABLE��DISABLE
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 10000UL);		//�ж�Ƶ��, 1000��/��
    TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    Timer_Inilize(Timer0, &TIM_InitStructure);					//��ʼ��Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4


    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
    TIM_InitStructure.TIM_Priority    = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    TIM_InitStructure.TIM_Interrupt = ENABLE;					//�ж��Ƿ�����,   ENABLE��DISABLE
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (500 * 12));			//��ֵ,2ms�ж�һ��
    TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    Timer_Inilize(Timer1, &TIM_InitStructure);					//��ʼ��Timer1	  Timer0,Timer1,Timer2,Timer3,Timer4

    TIM_InitStructure.TIM_Interrupt = DISABLE;					//�ж��Ƿ�����,   ENABLE��DISABLE. (ע��: Timer2�̶�Ϊ16λ�Զ���װ, �жϹ̶�Ϊ�����ȼ�)
    TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
    TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 8000UL);				//��ֵ
    TIM_InitStructure.TIM_Run       = DISABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    Timer_Inilize(Timer2, &TIM_InitStructure);					//��ʼ��Timer2	  Timer0,Timer1,Timer2,Timer3,Timer4

    //	TIM_InitStructure.TIM_Interrupt = ENABLE;					//�ж��Ƿ�����,   ENABLE��DISABLE. (ע��: Timer2�̶�Ϊ16λ�Զ���װ, �жϹ̶�Ϊ�����ȼ�)
    //	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;	//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    //	TIM_InitStructure.TIM_ClkOut    = ENABLE;					//�Ƿ������������, ENABLE��DISABLE
    //	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (4000*12));		//��ֵ
    //	TIM_InitStructure.TIM_Run      = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    //	Timer_Inilize(Timer3,&TIM_InitStructure);					//��ʼ��Timer3	  Timer0,Timer1,Timer2,Timer3,Timer4

    //	TIM_InitStructure.TIM_Interrupt = ENABLE;					//�ж��Ƿ�����,   ENABLE��DISABLE. (ע��: Timer2�̶�Ϊ16λ�Զ���װ, �жϹ̶�Ϊ�����ȼ�)
    //	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;	//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
    //	TIM_InitStructure.TIM_ClkOut    = ENABLE;					//�Ƿ������������, ENABLE��DISABLE
    //	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / (1000));		//��ֵ
    //	TIM_InitStructure.TIM_Run       = ENABLE;					//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
    //	Timer_Inilize(Timer4,&TIM_InitStructure);					//��ʼ��Timer4	  Timer0,Timer1,Timer2,Timer3,Timer4
}

/***************  PWM��ʼ������ *****************/
void	PWM_config(void)
{
    PWMx_InitDefine		PWMx_InitStructure;

    //PWMx_InitStructure.PWM1_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    PWMx_InitStructure.PWM2_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    //PWMx_InitStructure.PWM3_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
    //PWMx_InitStructure.PWM4_Mode    =	CCMRn_PWM_MODE1;	//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2

    //PWMx_InitStructure.PWM1_SetPriority  = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    PWMx_InitStructure.PWM2_SetPriority  = Priority_1;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    //PWMx_InitStructure.PWM3_SetPriority  = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
    //PWMx_InitStructure.PWM4_SetPriority  = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3

    PWMx_InitStructure.PWM_Period   = 65535;							//����ʱ��,   0~65535    Լ460hz
    //PWMx_InitStructure.PWM1_Duty    = PWMA_Duty.PWM1_Duty;	//PWM1ռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM2_Duty    = PWMA_Duty.PWM2_Duty;	//PWM2ռ�ձ�ʱ��, 0~Period
    //PWMx_InitStructure.PWM3_Duty    = PWMA_Duty.PWM3_Duty;	//PWM3ռ�ձ�ʱ��, 0~Period
    //PWMx_InitStructure.PWM4_Duty    = PWMA_Duty.PWM4_Duty;	//PWM4ռ�ձ�ʱ��, 0~Period
    PWMx_InitStructure.PWM_DeadTime = 0;								//��������������, 0~255

    PWMx_InitStructure.PWM_EnoSelect   = ENO2P;//ENO1P | ENO1N | ENO2P | ENO2N | ENO3P | ENO3N | ENO4P | ENO4N;	//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
    PWMx_InitStructure.PWM_PS_SW       = PWM2_SW_P12_P13;//PWM1_SW_P60_P61| PWM2_SW_P62_P63 | PWM3_SW_P64_P65 | PWM4_SW_P66_P67;	//�л��˿�,		PWM1_SW_P10_P11,PWM1_SW_P20_P21,PWM1_SW_P60_P61
    //						PWM2_SW_P12_P13,PWM2_SW_P22_P23,PWM2_SW_P62_P63
    //						PWM3_SW_P14_P15,PWM3_SW_P24_P25,PWM3_SW_P64_P65
    //						PWM4_SW_P16_P17,PWM4_SW_P26_P27,PWM4_SW_P66_P67,PWM4_SW_P34_P33

    //	PWMx_InitStructure.PWM_CC1Enable   = ENABLE;				//����PWM1P���벶��/�Ƚ����,  ENABLE,DISABLE
    //	PWMx_InitStructure.PWM_CC1NEnable  = ENABLE;				//����PWM1N���벶��/�Ƚ����,  ENABLE,DISABLE
    PWMx_InitStructure.PWM_CC2Enable   = ENABLE;				//����PWM2P���벶��/�Ƚ����,  ENABLE,DISABLE
    //	PWMx_InitStructure.PWM_CC2NEnable  = ENABLE;				//����PWM2N���벶��/�Ƚ����,  ENABLE,DISABLE
    //	PWMx_InitStructure.PWM_CC3Enable   = ENABLE;				//����PWM3P���벶��/�Ƚ����,  ENABLE,DISABLE
    //	PWMx_InitStructure.PWM_CC3NEnable  = ENABLE;				//����PWM3N���벶��/�Ƚ����,  ENABLE,DISABLE
    //	PWMx_InitStructure.PWM_CC4Enable   = ENABLE;				//����PWM4P���벶��/�Ƚ����,  ENABLE,DISABLE
    //	PWMx_InitStructure.PWM_CC4NEnable  = ENABLE;				//����PWM4N���벶��/�Ƚ����,  ENABLE,DISABLE

    PWMx_InitStructure.PWM_MainOutEnable = ENABLE;				//�����ʹ��, ENABLE,DISABLE
    PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;				//ʹ�ܼ�����, ENABLE,DISABLE
    PWM_Configuration(PWMA, &PWMx_InitStructure);				//��ʼ��PWM,  PWMA,PWMB
}


/******************** ������**************************/
void main(void)
{

    PWMA_Duty.PWM2_Duty = 6000;  //ռ�ձ� x/65536
    GPIO_config();  //GPIO��ʼ��
    P3PU = 0x38;      //����P3IO����������
    //UART_config();  //���ڳ�ʼ��
    ADC_config();	    //ADC��ʼ��
    Timer_config();   //��ʱ����ʼ��
    PWM_config();     //Ӳ��PWM��ʼ��
    EA = 1;           //���ж�



    OLED_Init();          //��ʼ��OLED
    OLED_ColorTurn(0);    //0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(1);  //0������ʾ 1 ��Ļ��ת��ʾ

    UpdatePwm(PWMA, &PWMA_Duty);  //PWM ռ�ձ����ú���

    OLED_Clear();  //oled����
    show_Init();   //��ʾ��ʼ����
    OLED_Clear();  //oled����
    power_Init();  //�ָ������ѹ����
    //Beep(OFF);

    // beep = 0;
    while (1)
    {
        interface_show();  //����
    }
}



