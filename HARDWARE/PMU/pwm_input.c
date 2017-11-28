/*********************************************************************************************************
//                              COMPANY CONFIDENTIAL
//                               INTERNAL USE ONLY
//
// Copyright (C) 2017  Comshare Technology Co.,Ltd.  All right reserved.
//
// This document contains information that is proprietary to Comshare Technology Co.,Ltd. 
// The holder of this document shall treat all information contained herein as confidential, 
// shall use the information only for its intended purpose, and shall protect the information 
// in whole or part from duplication, disclosure to any other party, or dissemination in 
// any media without the written permission of Comshare Technology Co.,Ltd.
//
// Comshare Technology Co.,Ltd
// www.comshare-sz.com
**********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "pwm_input.h"
#include "tester_debug.h"
#include "LoadConfig.h"
#include "SysTick.h"





vu32 IC1Value = 0,IC2Value = 0;
vu16 DutyCycle = 0;
vu32 Frequency = 0;
u16 DutyCycle_Valid = 0;
u32 Frequency_Valid = 0;
u16 TIM2_UPDATE_Cnt = 0;
u8 Flag_Duty_Err = 0;
u8 Flag_Seq_Err = 0;
u8 PWM_Test_Cnt = 0;






//测量范围DUTY 2% -- 98%,FREQ 500HZ -- 50KHZ 比较准


static void TIM2_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* GPIOA clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* TIM2 channel 2 pin (PA.01) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void NVIC_Configuration2(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	 /* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM2_PWM_IN_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	TIM2_GPIO_Configuration();
	
	NVIC_Configuration2();
	//	定时器BASE配置：
	TIM_TimeBaseStructure.TIM_Period = 65535;     //周期0～FFFF
	TIM_TimeBaseStructure.TIM_Prescaler = 2;       //时钟分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//基本初始化
	//输入捕获配置：
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);
	//TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	//TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);	
	//TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);	
	TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
}


/*******************************************************************************
* 文件名	    : bubble_sort
* 描述	           : 冒泡排序
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void bubble_sort(uint16_t a[], uint8_t n)
{
    u16 i, j, temp;
    for (j = 0; j < n - 1; j++)
        for (i = 0; i < n - 1 - j; i++)
        {
            if(a[i] > a[i + 1])
            {
                temp = a[i];
                a[i] = a[i + 1];
                a[i + 1] = temp;
            }
        }
}


#define BUBBLE_NUM	16
u16 temp_arr_seq[BUBBLE_NUM],temp_arr_duty[BUBBLE_NUM];
u8 test_one_time_cnt = 0,flag_test_one_time = 0;
u32 times = 0;

void TIM2_IRQHandler(void)
{
  /* Clear TIM2 Capture compare interrupt pending bit */
  
  	//if((TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)&&(TIM_GetITStatus(TIM2,TIM_IT_CC2) == RESET))
  	
  	
  	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)  
  	{
  		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  		 
  		TIM2_UPDATE_Cnt++;    		
  	}  
  	
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		
		IC2Value = TIM_GetCapture2(TIM2) + 3*24 + 4;
		IC1Value = TIM_GetCapture1(TIM2) + 3*24 + 4;		// 3为补偿系数 

		if(TIM_GetCapture1(TIM2) != 0)
		{		
			//DutyCycle = (TIM_GetCapture1(TIM2) * 100) /(IC2Value + 65535*TIM2_UPDATE_Cnt);
			if(IC1Value > IC2Value)
			{
				#if 0
				DutyCycle = TIM_GetCapture1(TIM2);
				Frequency = 1000000 / (IC2Value + 719*TIM2_UPDATE_Cnt);			
				Frequency += 1;
				#else			
				DutyCycle = ((IC2Value)*100/IC1Value) + 1;			//针对性补偿	
				#endif
			}
			else
			{
				DutyCycle = ((IC2Value-IC1Value)*100/IC1Value) + 1;
			}

			Frequency = (u16)((24000000)/IC1Value);	

			times++;
			if(times >= 3000)
			{
				if(flag_test_one_time == 0)							//做个排序算法，去掉保留中间的值，只运行一次
				{
					temp_arr_seq[test_one_time_cnt] = Frequency;
					temp_arr_duty[test_one_time_cnt] = DutyCycle;
					test_one_time_cnt++;						
					if(test_one_time_cnt >= BUBBLE_NUM)
					{
						test_one_time_cnt = 0;
						bubble_sort(temp_arr_seq,BUBBLE_NUM);
						bubble_sort(temp_arr_duty,BUBBLE_NUM);
						flag_test_one_time = 1;									
						Frequency_Valid = temp_arr_seq[7];
						DutyCycle_Valid = temp_arr_duty[8];
					}
				}
			}
		
		}
		else
		{
			DutyCycle = 0;
			Frequency = 0;
		}	
		
		
		TIM2_UPDATE_Cnt = 0;
		
	
		
		TIM_SetCounter(TIM2,0);
	}			
}



u16 Get_PWM_Freq(void)
{
	//return(Frequency);
	return(Frequency_Valid);
}


u16 Get_PWM_Duty(void)
{
	//return(DutyCycle);
	return(DutyCycle_Valid);	
}


u8  PWM_Detect_Test(void)
{	
	u8 err = 0;
	u16 temp_seq = 0,temp_duty = 0;
	u16 option_temp1 = 0,option_temp2 = 0;
	
	Get_Option_Para(PWM_EN_INDEX,&option_temp1);
	//if(option_temp1 == 1)						//使能pwm检测
	if(1)
	{
		PWM_Test_Cnt++;
		if(PWM_Test_Cnt == 1)			//运行一次
		{
			PWM_ON();
		}
		else if(PWM_Test_Cnt == 4)
		{
			//Get_Option_Para(PWM_SEQ_HIGH_INDEX,&option_temp1);
			//Get_Option_Para(PWM_SEQ_LOW_INDEX,&option_temp2);
			temp_seq = Get_PWM_Freq();
			if(( temp_seq >= option_temp1) || (temp_seq <= option_temp2))
			{
				//pwm 频率检测错误
				Flag_Seq_Err = 1;
				
			}
			else
			{
			
			}
			
			
			//Get_Option_Para(PWM_DUTY_HIGH_INDEX,&option_temp1);
			//Get_Option_Para(PWM_DUTY_LOW_INDEX,&option_temp2);
			temp_duty = Get_PWM_Duty();	
			if((temp_duty >= option_temp1) || (temp_duty <= option_temp2))
			{
				//pwm 占空比检测错误
				Flag_Duty_Err = 1;
				
			}
			else
			{
			
			}	

			#if 1
			printf("FREQ = %dHZ\r\n",temp_seq);
			printf("DUTY = %d%%\r\n",temp_duty);
			printf("CCR = %d %d\r\n",IC1Value,IC2Value);		
			#endif

			#if 0		
			TESTER_MAIN_DEBUG("seq0 = %d %d %d %d %d %d %d %d \r\n",temp_arr_seq[0],temp_arr_seq[1],temp_arr_seq[2],temp_arr_seq[3],temp_arr_seq[4],temp_arr_seq[5],temp_arr_seq[6],temp_arr_seq[7]);		
			TESTER_MAIN_DEBUG("seq8 = %d %d %d %d %d %d %d %d \r\n",temp_arr_seq[8],temp_arr_seq[9],temp_arr_seq[10],temp_arr_seq[11],temp_arr_seq[12],temp_arr_seq[13],temp_arr_seq[14],temp_arr_seq[15]);		
			TESTER_MAIN_DEBUG("duty0 = %d %d %d %d %d %d %d %d \r\n",temp_arr_duty[0],temp_arr_duty[1],temp_arr_duty[2],temp_arr_duty[3],temp_arr_duty[4],temp_arr_duty[5],temp_arr_duty[6],temp_arr_duty[7]);		
			TESTER_MAIN_DEBUG("duty8 = %d %d %d %d %d %d %d %d \r\n",temp_arr_duty[8],temp_arr_duty[9],temp_arr_duty[10],temp_arr_duty[11],temp_arr_duty[12],temp_arr_duty[13],temp_arr_duty[14],temp_arr_duty[15]);		
			#endif

		}
		else if(PWM_Test_Cnt == 5)
		{
			PWM_OFF();
			flag_test_one_time = 0;
			times = 0;
		}
		else if(PWM_Test_Cnt >= 6)
		{
			PWM_Test_Cnt = 5;
		}
	}
}



u8 Get_Seq_Err_Flag(void)
{
	return(Flag_Seq_Err);
}



u8 Get_Duty_Err_Flag(void)
{
	return(Flag_Duty_Err);
}

void Clear_PWM_Err_Flag(void)
{
	Flag_Seq_Err = 0;
	Flag_Duty_Err = 0;
}


void Clear_PWM_Test_Cnt(void)
{
	PWM_Test_Cnt = 0;
	Frequency_Valid = 0;
	DutyCycle_Valid = 0;
}






