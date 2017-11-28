/*********************************************************************************************************
* �ļ�����: pwm_input.h
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
#ifndef __PWM_INPUT_H
#define	__PWM_INPUT_H




#include "stm32f10x.h"

#define	PWM_DETECT_EN		0x00		//1--�����⣬0---������



#define	PWM_SEQ_HIGH		(25500+2000)
#define	PWM_SEQ_LOW			(25500-2000)

#define	PWM_DUTY_HIGH		(50+3)
#define	PWM_DUTY_LOW		(50-3)




#if 0
#define	 PWM_ON()			TIM_Cmd(TIM2, ENABLE);	    	//����PWM���
#define	 PWM_OFF()			TIM_Cmd(TIM2, DISABLE);			//�ر�PWM���
#else
#define	 PWM_ON()			TIM_SetCounter(TIM2,0);TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);TIM_Cmd(TIM2, ENABLE);	    	//����PWM���
#define	 PWM_OFF()			TIM_SetCounter(TIM2,0);TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);TIM_Cmd(TIM2, DISABLE);			//�ر�PWM���
#endif




void TIM2_PWM_IN_Configuration(void);
//void TIM3_PWM_Init(void);
u8 PWM_Detect_Test(void);
u16 Get_PWM_Duty(void);
u16 Get_PWM_Freq(void);
u8 Get_Seq_Err_Flag(void);
u8 Get_Duty_Err_Flag(void);
void Clear_PWM_Err_Flag(void);
void Clear_PWM_Test_Cnt(void);
int TP_PWM_TE_Detect(void);




//˵��-> ��������PWM_ON  ʹ�ܼ�⣬500ms����Ե���Get_PWM_Duty��Get_PWM_Freq����ȡ��֮�����PWM_OFF




#endif /* __PWM_INPUT_H */




