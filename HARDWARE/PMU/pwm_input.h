/*********************************************************************************************************
* 文件名称: pwm_input.h
* 文件作者: zq
* 当前版本: V1.01
* 所属模块: 
* 完成日期: 2016年09月06日
* 文件摘要: 
* 注意事项:	
* 更改说明: 
**********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#ifndef __PWM_INPUT_H
#define	__PWM_INPUT_H




#include "stm32f10x.h"

#define	PWM_DETECT_EN		0x00		//1--允许检测，0---不允许



#define	PWM_SEQ_HIGH		(25500+2000)
#define	PWM_SEQ_LOW			(25500-2000)

#define	PWM_DUTY_HIGH		(50+3)
#define	PWM_DUTY_LOW		(50-3)




#if 0
#define	 PWM_ON()			TIM_Cmd(TIM2, ENABLE);	    	//开启PWM检测
#define	 PWM_OFF()			TIM_Cmd(TIM2, DISABLE);			//关闭PWM检测
#else
#define	 PWM_ON()			TIM_SetCounter(TIM2,0);TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);TIM_Cmd(TIM2, ENABLE);	    	//开启PWM检测
#define	 PWM_OFF()			TIM_SetCounter(TIM2,0);TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE);TIM_Cmd(TIM2, DISABLE);			//关闭PWM检测
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




//说明-> 开电后调用PWM_ON  使能检测，500ms后可以调用Get_PWM_Duty，Get_PWM_Freq，获取完之后调用PWM_OFF




#endif /* __PWM_INPUT_H */




