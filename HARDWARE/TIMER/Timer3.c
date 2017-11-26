/*********************************************************************************************************
* 文件名称: Time.c
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: Cth
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/



/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "Common.h"
#include "Timer.h"
#include "Uart.h"



/*********************************************************************************************************
*                                              宏定义定义
*********************************************************************************************************/
#define 	START_TIME 		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);\
							TIM_Cmd(TIM3, ENABLE)
#define 	STOP_TIME  		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);\
							TIM_Cmd(TIM3, DISABLE)





/*********************************************************************************************************
*                                              变量定义
*********************************************************************************************************/
static bool sFlag_1ms  = FALSE;









/*******************************************************************************
* 文件名	    : TIM3_NVIC_Configuration
* 描述	           : TIM3_NVIC_Configuration 中断配置
* 输入           : 无
* 输出           : 无
* 返回           : 无
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
* 文件名	    : TIM3_Configuration
* 描述	           :TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
static void TIM3_Configuration(u32 inttime)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    TIM_DeInit(TIM3);
#ifdef USE_HSE	
    TIM_TimeBaseStructure.TIM_Period = inttime - 1;	//999	 					//1ms		//自动重装载寄存器周期的值(计数值)    
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);				    //时钟预分频数 72M/72 
#else
	TIM_TimeBaseStructure.TIM_Period = inttime;		 					//1ms		//自动重装载寄存器周期的值(计数值)    
    TIM_TimeBaseStructure.TIM_Prescaler = (56 - 1);				    //时钟预分频数 56M/56 
#endif
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//采样分频 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数模式 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							// 清除溢出中断标志 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3, ENABLE);											//开启时钟 
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);			//先关闭等待使用   
}


/*******************************************************************************
* 文件名	    : TIM2_Init
* 描述	           : TIM2初始化
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TIM3_Init(void)//100us  
{
	TIM3_NVIC_Configuration();
	TIM3_Configuration(500);//125us
	START_TIME;								//开启定时器
}




extern u32 time3value;
extern uint16_t  ADCvalue;
extern __IO uint16_t  ADCBuffer[2100];
extern u32 ii;


/*******************************************************************************
* 文件名	    : TIM3_IRQHandler
* 描述	           : TIM2中断处理函数
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM3 , TIM_IT_Update); 	
		
		//time3value++;
		//if(time3value>0xfffffff) time3value = 0;
		//定时中断处理
		//USARTx_Timer_Proc();
		if(ii<2100)	ADCBuffer[ii++] = ADCvalue;//125us 采样时间	
			
	}		
}




/*******************************************************************************
* 文件名	    : GetTimerFlag
* 描述	           : 获取时间标志位
* 输入           : temp: TIMER_FLAG_1MS or else
* 输出           : 无
* 返回           : bool: TRUE OR FALSE
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
* 文件名	    : ClrTimerFlag
* 描述	           : 获取时间标志位
* 输入           : temp: TIMER_FLAG_1MS or else ,  value:TRUE OR FALSE
* 输出           : 无
* 返回           : bool: TRUE OR FALSE
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








