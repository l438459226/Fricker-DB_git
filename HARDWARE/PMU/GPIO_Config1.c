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
#include "GPIO_Config.h"
#include "INA226.h"
#include "tester_debug.h"












/*******************************************************************************
* 文件名	     : PMU_GPIO_Config
* 描述	         : 内部系统时钟配置
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void PMU_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);		


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;	
  	GPIO_Init(GPIOE, &GPIO_InitStructure);	


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	
  	GPIO_Init(GPIOF, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
  	GPIO_Init(GPIOD, &GPIO_InitStructure);		
		
	//power ON sequence
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	GPIO_SetBits(GPIOG, GPIO_Pin_8);
	GPIO_ResetBits(GPIOF,GPIO_Pin_10);		//oled control
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}


void VSP_VSN_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOC, &GPIO_InitStructure);			

	GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);	


	GPIO_ResetBits(GPIOG,GPIO_Pin_1);
}



void OTP_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure);			

	GPIO_SetBits(GPIOE,GPIO_Pin_11);	
}



void BL_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure);			

	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	GPIO_ResetBits(GPIOE,GPIO_Pin_4);

	//////////////////////////////////////检测部分
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|\
								  GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	

	GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);
	GPIO_SetBits(GPIOE,GPIO_Pin_14);
	GPIO_ResetBits(GPIOE,GPIO_Pin_13);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);		
	GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9);
}




void VA_Err_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOC, &GPIO_InitStructure);			

	GPIO_SetBits(GPIOC,GPIO_Pin_5);
}	




void Usr_Led_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOB, &GPIO_InitStructure);			

	GPIO_SetBits(GPIOB,GPIO_Pin_3);
}	





void TPS22993_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|\
								  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);			


	GPIO_ResetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	GPIO_ResetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8);
}



void RELAY_UA_GPIO_Config(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);			


	GPIO_ResetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}



void  System_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//禁止jtag 使能swd
	
	PMU_GPIO_Config();
	VSP_VSN_GPIO_Config();
	OTP_GPIO_Config();
	BL_GPIO_Config();
	VA_Err_GPIO_Config();
	TPS22993_GPIO_Config();
	RELAY_UA_GPIO_Config();
	Usr_Led_GPIO_Config();
}



/*******************************************************************************
* 文件名	     : Set_PMU_RST_IO
* 描述	         : 内部系统时钟配置
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Set_PMU_RST_IO(u8 value)
{			
	if(value)
	{
		GPIO_SetBits(GPIOG, GPIO_Pin_8);
	}
	else
	{
		GPIO_ResetBits(GPIOG, GPIO_Pin_8);
	}
}






/*******************************************************************************
* 文件名	     : Set_PMU_PWREN_IO
* 描述	         : 内部系统时钟配置
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Set_PMU_PWREN_IO(u8 value)
{			
	if(value)
	{
		GPIO_SetBits(GPIOE,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	}
}


/*******************************************************************************
* 文件名	     : ACT8846_DCDC_Switch
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void ACT8846_DCDC_Switch(ACT8846_DCDC_CHN_TypeDef chn,ACT8846_SWITCH_TypeDef onoff)
{
	if(onoff == ACT8846_SWITCH_OFF)
	{
		act8846_dcdc_disable(chn);
	}
	else
	{
		act8846_dcdc_enable(chn);
	}
}




/*******************************************************************************
* 文件名	     : ACT8846_LDO_Switch
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void ACT8846_LDO_Switch(ACT8846_LDO_CHN_TypeDef chn,ACT8846_SWITCH_TypeDef onoff)
{
	if(onoff == ACT8846_SWITCH_OFF)
	{
		act8846_ldo_disable(chn);
	}
	else
	{
		act8846_ldo_enable(chn);
	}
}




/*******************************************************************************
* 文件名	     : TPS22993_PWR_ON_Cfg
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_PWR_ON_Cfg(void)		//负载开关上电配置	
{
	TPS22993_Init();							//上电后设置为I2C控制方式，通道全部关闭
}


/*******************************************************************************
* 文件名	     : LT3582_PWR_ON_Cfg
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_PWR_ON_Cfg(void)		//OTP上电配置	
{
	LT3582_Init();							//关闭
}



/*******************************************************************************
* 文件名	     : LM36923_PWR_ON_Cfg
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_PWR_ON_Cfg(u16 val)		//背光上电配置	
{
	LM36923_Init(val);							//3个通道都关闭
}


/*******************************************************************************
* 文件名	     : LM36923_PWR_ON_Cfg
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void AD715_PWR_ON_Cfg(void)		
{
	ad715_Init();					
}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
* 文件名	     : I2C_Detect
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void I2C_Detect(void)
{
	int retval;
	u8 i = 0;

	/*
	i=0x64;
	retval = i2c1_start(i<<1);
	if(retval<0)
	{
		TESTER_MAIN_DEBUG("i2c start error!!\r\n");
	}
	else
	{
		TESTER_MAIN_DEBUG("i2c address = 0x%x detected.\r\n",i);
	}
	Delay_ms(10); 
	i2c1_stop();
	Delay_ms(10); 
	*/
	
	TESTER_MAIN_DEBUG("Scan i2c address:\r\n");	
	for(i=10;i<0x7F;i++)
	{
		retval = i2c1_start(i<<1);
		if(retval<0)
		{
			//TESTER_MAIN_DEBUG("i2c start error!!\r\n",i);
		}
		else
		{
			TESTER_MAIN_DEBUG("i2c address = 0x%x detected.\r\n",i);
		}
		Delay_ms(10); 
		i2c1_stop();
		Delay_ms(10); 
	}
}



/*******************************************************************************
* 文件名	     : Print_VoltCurrent
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Print_VoltCurrent(void)
{
	////////////////////////////////////////////////////////////////////////////////	
	TESTER_MAIN_DEBUG("Current Measure:\r\n");
	/////////////////////////////////////////////////////////////
	INA226_selectDevice(0);
	// Configure INA226
	//INA226_configure(INA226_AVERAGES_1024, INA226_BUS_CONV_TIME_8244US, INA226_SHUNT_CONV_TIME_8244US, INA226_MODE_SHUNT_BUS_CONT);
	INA226_configure(INA226_AVERAGES_128, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
	
	// Calibrate INA226. Rshunt = 1 ohm, Max excepted current = 0.4A
	INA226_calibrate(1, 0.1);//

	Delay_ms(10);
	
	TESTER_MAIN_DEBUG("Bus voltage:   ");
	TESTER_MAIN_DEBUG("%f", INA226_readBusVoltage());
	TESTER_MAIN_DEBUG(" V\r\n");
	

	TESTER_MAIN_DEBUG("Shunt voltage: ");
	TESTER_MAIN_DEBUG("%f", INA226_readShuntVoltage());
	TESTER_MAIN_DEBUG(" V\r\n");
	

	TESTER_MAIN_DEBUG("Shunt current: ");
	TESTER_MAIN_DEBUG("%f", INA226_readShuntCurrent());
	TESTER_MAIN_DEBUG(" A\r\n");
	

	TESTER_MAIN_DEBUG("Bus power:	   ");																															 
	TESTER_MAIN_DEBUG("%f", INA226_readBusPower());
	TESTER_MAIN_DEBUG(" W\r\n");
	
}













//////////////////////////////////////////////////////////////////////////////////////////////////////////////////









