/*********************************************************************************************************
* �ļ�����: pwm_input.c
* �ļ�����: zq
* ��ǰ�汾: V1.01
* ����ģ��: 
* �������: 2016��09��06��
* �ļ�ժҪ: 
* ע������:	
* ����˵��: 
**********************************************************************************************************/

/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "pwm_input.h"
#include "tester_debug.h"
#include "LoadConfig.h"
#include "SysTick.h"
#include "AD715.h"
#include "key.h"


vu32 IC1Value = 0,IC2Value = 0;
vu16 DutyCycle = 0;
vu32 Frequency = 0;
u16 DutyCycle_Valid = 0;
u32 Frequency_Valid = 0;
u16 TIM2_UPDATE_Cnt = 0;
u8 Flag_Duty_Err = 0,Flag_Duty1_Err = 0;
u8 Flag_Seq_Err = 0,Flag_Seq1_Err = 0;
u8 PWM_Test_Cnt = 0;


extern uint8_t G_Select_Port;

//������ΧDUTY 2% -- 98%,FREQ 500HZ -- 50KHZ �Ƚ�׼

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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM2_PWM_IN_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	TIM2_GPIO_Configuration();
	
	NVIC_Configuration2();
	//	��ʱ��BASE���ã�
	TIM_TimeBaseStructure.TIM_Period = 65535;     //����0��FFFF
	TIM_TimeBaseStructure.TIM_Prescaler = 2;       //ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//������ʼ��
	//���벶�����ã�
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
* �ļ���	    : bubble_sort
* ����	           : ð������
* ����           : ��
* ���           : ��
* ����           : ��
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
		IC1Value = TIM_GetCapture1(TIM2) + 3*24 + 4;		// 3Ϊ����ϵ�� 

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
				DutyCycle = ((IC2Value)*100/IC1Value) + 1;			//����Բ���	
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
				if(flag_test_one_time == 0)							//���������㷨��ȥ�������м��ֵ��ֻ����һ��
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

#define PRINT_PWM_INFO	0x00
u8  PWM_Detect_Test(void)
{	
	u8 err = 0;
	u16 temp_seq = 0,temp_duty = 0;
	u16 option_temp1 = 0,option_temp2 = 0;
	
	Get_Option_Para(PWM_EN_INDEX,&option_temp1);
	if(option_temp1 == 1)						//ʹ��pwm���
	{
		PWM_Test_Cnt++;
		if(G_Select_Port == 3)		//A+B PORT
		{	
			switch(PWM_Test_Cnt)					
			{
				case 1:
					ad715_i2c_write2(DETE_PORTA_ID_TE_PWM);
					PWM_ON();
				break;
				
				case 4:
					Get_Option_Para(PWM_SEQ_HIGH_INDEX,&option_temp1);
					Get_Option_Para(PWM_SEQ_LOW_INDEX,&option_temp2);
					temp_seq = Get_PWM_Freq();
					if(( temp_seq >= option_temp1) || (temp_seq <= option_temp2))
					{
						//pwm Ƶ�ʼ�����
						Flag_Seq_Err = 1;
					}
					
					Get_Option_Para(PWM_DUTY_HIGH_INDEX,&option_temp1);
					Get_Option_Para(PWM_DUTY_LOW_INDEX,&option_temp2);
					temp_duty = Get_PWM_Duty();
					if((temp_duty >= option_temp1) || (temp_duty <= option_temp2))
					{
						//pwm ռ�ձȼ�����
						Flag_Duty_Err = 1;
					}	

					#if PRINT_PWM_INFO
					printf("1FREQ = %dHZ\r\n",temp_seq);
					printf("1DUTY = %d%%\r\n",temp_duty);
					printf("1CCR = %d %d\r\n",IC1Value,IC2Value);		
					#endif			
				break;

				case 5:
					//Clear_PWM_Test_Cnt();
					Frequency_Valid = 0;
					DutyCycle_Valid = 0;
					flag_test_one_time = 0;
					test_one_time_cnt = 0;
					times = 0;
					ad715_i2c_write2(DETE_PORTB_ID_TE_PWM);
				break;

				case 8:
					Get_Option_Para(PWM_SEQ_HIGH_INDEX,&option_temp1);
					Get_Option_Para(PWM_SEQ_LOW_INDEX,&option_temp2);
					temp_seq = Get_PWM_Freq();
					if(( temp_seq >= option_temp1) || (temp_seq <= option_temp2))
					{
						//pwm Ƶ�ʼ�����
						Flag_Seq1_Err = 1;
					}
					
					Get_Option_Para(PWM_DUTY_HIGH_INDEX,&option_temp1);
					Get_Option_Para(PWM_DUTY_LOW_INDEX,&option_temp2);
					temp_duty = Get_PWM_Duty();
					if((temp_duty >= option_temp1) || (temp_duty <= option_temp2))
					{
						//pwm ռ�ձȼ�����
						Flag_Duty1_Err = 1;
					}
					
					#if PRINT_PWM_INFO
					printf("2FREQ = %dHZ\r\n",temp_seq);
					printf("2DUTY = %d%%\r\n",temp_duty);
					printf("2CCR = %d %d\r\n",IC1Value,IC2Value);		
					#endif				
				break;
					
				case 9:
					PWM_OFF();
					flag_test_one_time = 0;
					test_one_time_cnt = 0;
					times = 0;
				break;
				
				case 10:
					PWM_Test_Cnt = 9;
				break;
			}	
		}
		else			//PORTA
		{
			switch(PWM_Test_Cnt)					
			{
				case 1:
					ad715_i2c_write2(DETE_PORTA_ID_TE_PWM);
					PWM_ON();
				break;
				
				case 4:
					Get_Option_Para(PWM_SEQ_HIGH_INDEX,&option_temp1);
					Get_Option_Para(PWM_SEQ_LOW_INDEX,&option_temp2);
					temp_seq = Get_PWM_Freq();
					if(( temp_seq >= option_temp1) || (temp_seq <= option_temp2))
					{
						//pwm Ƶ�ʼ�����
						Flag_Seq_Err = 1;
					}
					
					Get_Option_Para(PWM_DUTY_HIGH_INDEX,&option_temp1);
					Get_Option_Para(PWM_DUTY_LOW_INDEX,&option_temp2);
					temp_duty = Get_PWM_Duty();
					if((temp_duty >= option_temp1) || (temp_duty <= option_temp2))
					{
						//pwm ռ�ձȼ�����
						Flag_Duty_Err = 1;
					}	

					#if PRINT_PWM_INFO
					printf("1FREQ = %dHZ\r\n",temp_seq);
					printf("1DUTY = %d%%\r\n",temp_duty);
					printf("1CCR = %d %d\r\n",IC1Value,IC2Value);		
					#endif			
				break;

				case 5:
					PWM_OFF();
					flag_test_one_time = 0;
					test_one_time_cnt = 0;
					times = 0;
				break;

				case 6:
					PWM_Test_Cnt = 5;
				break;
			}
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


u8 Get_Seq1_Err_Flag(void)
{
	return(Flag_Seq1_Err);
}



u8 Get_Duty1_Err_Flag(void)
{
	return(Flag_Duty1_Err);
}


void Clear_PWM_Err_Flag(void)
{
	Flag_Seq_Err = 0;
	Flag_Duty_Err = 0;
	Flag_Seq1_Err = 0;
	Flag_Duty1_Err = 0;
}


void Clear_PWM_Test_Cnt(void)
{
	flag_test_one_time = 0;
	PWM_Test_Cnt = 0;
	Frequency_Valid = 0;
	DutyCycle_Valid = 0;
}


//tp���������   PWM TE���
//����ֵ�: -1---err  1---pass
int TP_PWM_TE_Detect(void)
{
	u8 cnt = 8;
	
	while(cnt--)
	{
		PWM_Detect_Test();
		Te_Para_Test();
		Delay_ms(20);
	}

	if(Get_TE_Seq_Err_Flag() || Get_TE_Duty_Err_Flag())
	{		
		Set_Sys_Err_Flag(1);
		return(-1);		//���ش���
		//TE ERROR
	}

	if(Get_Seq_Err_Flag() || Get_Duty_Err_Flag())
	{	
		Set_Sys_Err_Flag(2);
		return(-1);
		//PWM ERROR
	}
	
	return(1);		//���� pass
	
}



