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
#include "tps65651.h"
#include "delay.h"



void AMOLED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	/* config the extiline(PB0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOF,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}



void AMOLED_Set_Vng(AMOLED_VNEG_TypeDef val)
{
	u8 i = 0;

	GPIO_SetBits(GPIOD,GPIO_Pin_7);
	Delay_ms(30);

	for(i=0;i<val;i++)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
		Delay_us(1);
		GPIO_SetBits(GPIOD,GPIO_Pin_7);
		Delay_us(1);
	}		
}


void AMOLED_Set_Avdd(AMOLED_AVDD_TypeDef val)
{
	u8 i = 0;

	GPIO_SetBits(GPIOD,GPIO_Pin_7);
	Delay_ms(30);

	for(i=0;i<val;i++)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
		Delay_us(1);
		GPIO_SetBits(GPIOD,GPIO_Pin_7);
		Delay_us(1);
	}		
}



void AMOLED_Set_Vpos(AMOLED_VPOS_TypeDef val)
{
	u8 i = 0;

	GPIO_SetBits(GPIOD,GPIO_Pin_7);
	Delay_ms(30);

	for(i=0;i<val;i++)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
		Delay_us(1);
		GPIO_SetBits(GPIOD,GPIO_Pin_7);
		Delay_us(1);
	}		
}



void tps65651_Init(void)
{	
	AMOLED_Config();
	Delay_ms(10);
	AMOLED_DISABLE();
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	AMOLED_FD_DISABLE();

}



void tps65651_On(void)
{	
	AMOLED_ENABLE();		//avdd
	AMOLED_Set_Vpos(AMOLED_VPOS_46);		//VPOS
	AMOLED_Set_Vng(AMOLED_VNEG_29);			//VNG
	AMOLED_Set_Avdd(AMOLED_AVDD_58);		//AVDD
}




void tps65651_Off(void)
{	
	AMOLED_DISABLE();		//VPOS
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
	AMOLED_FD_ENABLE();
}
























