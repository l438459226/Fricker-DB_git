/*********************************************************************************************************
* �ļ�����: Time.c
* �ļ�����:����δ��
* ��ǰ�汾: 01.00.00
* ����ģ��: Cth
* ��������: 2016��03��22��
* �ļ�ժҪ: 
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/



/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "Common.h"
#include "Timer.h"
#include "Uart.h"



/*********************************************************************************************************
*                                              �궨�嶨��
*********************************************************************************************************/
#define 	START_TIME 		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);\
							TIM_Cmd(TIM3, ENABLE)
#define 	STOP_TIME  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);\
							TIM_Cmd(TIM3, DISABLE)





/*********************************************************************************************************
*                                              ��������
*********************************************************************************************************/
static bool sFlag_1ms  = FALSE;









/*******************************************************************************
* �ļ���	    : TIM3_NVIC_Configuration
* ����	           : TIM3_NVIC_Configuration �ж�����
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
static void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM3_IRQ_PrePriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}






/*******************************************************************************
* �ļ���	    : TIM3_Configuration
* ����	           :TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
static void TIM3_Configuration(u32 inttime)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    TIM_DeInit(TIM3);
#ifdef USE_HSE	
    TIM_TimeBaseStructure.TIM_Period = inttime - 1;	//999	 					//1ms		//�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ)    
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);				    //ʱ��Ԥ��Ƶ�� 72M/72 
#else
	TIM_TimeBaseStructure.TIM_Period = inttime;		 					//1ms		//�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ)    
    TIM_TimeBaseStructure.TIM_Prescaler = (56 - 1);				    //ʱ��Ԥ��Ƶ�� 56M/56 
#endif
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//������Ƶ 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���ģʽ 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							// �������жϱ�־ 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3, ENABLE);											//����ʱ�� 
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);			//�ȹرյȴ�ʹ��   
}


/*******************************************************************************
* �ļ���	    : TIM2_Init
* ����	           : TIM2��ʼ��
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void TIM3_Init(void)//100us  
{
	TIM3_NVIC_Configuration();
	TIM3_Configuration(500);//125us
	START_TIME;								//������ʱ��
}




extern u32 time3value;
extern uint16_t  ADCvalue;
extern __IO uint16_t  ADCBuffer[2100];
extern u32 ii;


/*******************************************************************************
* �ļ���	    : TIM3_IRQHandler
* ����	           : TIM2�жϴ�����
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM3 , TIM_IT_Update); 	
		
		//time3value++;
		//if(time3value>0xfffffff) time3value = 0;
		//��ʱ�жϴ���
		//USARTx_Timer_Proc();
		if(ii<2100)	ADCBuffer[ii++] = ADCvalue;//125us ����ʱ��	
			
	}		
}




/*******************************************************************************
* �ļ���	    : GetTimerFlag
* ����	           : ��ȡʱ���־λ
* ����           : temp: TIMER_FLAG_1MS or else
* ���           : ��
* ����           : bool: TRUE OR FALSE
*******************************************************************************/
u8 GetTimerFlag(TimerFlagType temp)
{
	/*
	switch(temp)
	{
		case TIMER_FLAG_1MS:
			
			return(sFlag_1ms);			
			
			break;
		default:
			
			break;
	}
	*/
	if(temp == TIMER_FLAG_1MS)
	{
		return(sFlag_1ms);	
	}
	else
	{
		return(0);
	}
	
}


/*******************************************************************************
* �ļ���	    : ClrTimerFlag
* ����	           : ��ȡʱ���־λ
* ����           : temp: TIMER_FLAG_1MS or else ,  value:TRUE OR FALSE
* ���           : ��
* ����           : bool: TRUE OR FALSE
*******************************************************************************/
void SetTimerFlag(TimerFlagType temp, bool value)
{
	switch(temp)
	{
		case TIMER_FLAG_1MS:
			
			sFlag_1ms = value;
			
			break;
		default:
			
			break;
	}
}








