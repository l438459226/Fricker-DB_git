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
#include "Timer.h"
#include "adc.h"
#include "Fricker.h"

#define  TIM3_IRQ_PrePriority		1		//��ʱ���ж���ռ���ȼ�����

u32 itime=0;

/*********************************************************************************************************
*                                              �궨�嶨��
*********************************************************************************************************/
void start_time3(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void stop_time3(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);
	TIM_Cmd(TIM3, DISABLE);
}

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

    TIM_TimeBaseStructure.TIM_Period = inttime - 1;	//999	 					//1ms		//�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ)    
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);				    //ʱ��Ԥ��Ƶ�� 72M/72 

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
void TIM3_Init(u32 time)//100us  
{
	TIM3_NVIC_Configuration();
	TIM3_Configuration(time);//125us
	stop_time3();								//
}

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
		TIM_ClearITPendingBit(TIM3 , TIM_IT_Update); 	//���жϱ�־
		collection_data();	//�ɼ�����
		itime++;
	}		
}



