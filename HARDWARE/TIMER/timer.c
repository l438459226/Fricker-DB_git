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
#include "Timer.h"
#include "adc.h"
#include "Fricker.h"

#define  TIM3_IRQ_PrePriority		1		//定时器中断抢占优先级设置

u32 itime=0;

/*********************************************************************************************************
*                                              宏定义定义
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

    TIM_TimeBaseStructure.TIM_Period = inttime - 1;	//999	 					//1ms		//自动重装载寄存器周期的值(计数值)    
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);				    //时钟预分频数 72M/72 

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
void TIM3_Init(u32 time)//100us  
{
	TIM3_NVIC_Configuration();
	TIM3_Configuration(time);//125us
	stop_time3();								//
}

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
		TIM_ClearITPendingBit(TIM3 , TIM_IT_Update); 	//清中断标志
		collection_data();	//采集数据
		itime++;
	}		
}



