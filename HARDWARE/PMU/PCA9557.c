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


//IO EXPANDER DRIVER




/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "PCA9557.h"
#include "i2c1_bitbang.h"
#include "SysTick.h"

#define LCM_2000_MP_VERSION








/*********************************************************************************************************
*                                              变量定义
*********************************************************************************************************/
static u8 PCA9557_Address = PCA9557_DEVEICE0_ADD;






/*******************************************************************************
* 文件名	     : PCA9557_Sel_Dev
* 描述	         : PCA9557设备选择
* 输入           : 0---选择设备0 1---选择设备1
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void PCA9557_Sel_Dev(u8 val)
{
	if(val == 0)
	{
		PCA9557_Address = PCA9557_DEVEICE0_ADD;
	}
	else
	{
		PCA9557_Address = PCA9557_DEVEICE1_ADD;
	}	
}



/*******************************************************************************
* 文件名	     : PCA9557_I2c_write
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
int PCA9557_I2c_write(u8 reg,u8 val)
{
	int retval;
	
	retval = i2c1_start(PCA9557_Address<<1);
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_write_u8((u8)(reg));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_write_u8(reg) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_write_u8((u8)(val));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_write_u8(val) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_stop();
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c1_stop Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	return I2C_OK;
	
}


u8 Setbit(int num, int bit)
{  
    return num|=(1<<bit);  
}  
  
u8 Clearbit(int num, int bit)
{  
    return num&=~(1<<bit);  
}  
  
u8 Testbit(int num, int bit)
{  
    return num&=(1<<bit);  
}



/*******************************************************************************
* 文件名	     : PCA9557_Dev0_Set
* 描述	         : PCA9557 设备0设置
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 PCA9557_Dev0_Set(IO_EXPANDER_DEV0_TypeDef tpye,u8 val)					//低电平有效
{
	static u8 temp = 0xff;

	if(tpye < IO_DEV0_BL_LEDP_EN && tpye > IO_DEV0_ALL_EN)
	{
		return 0;		//返回错误
	}	

	if(tpye != IO_DEV0_ALL_EN)
	{
		//temp &= (~(val << (tpye -1)));	
		if(tpye == IO_DEV0_LCD_VMTP_ON)
		{
			if(val)
			{
				val = 0;
			}
			else
			{
				val = 1;
			}
		}
		
		if(val)
		{
			temp = Clearbit(temp,(tpye -1));
		}
		else
		{
			temp = Setbit(temp,(tpye -1));
		}
	}
	else
	{
		if(val)
		{
			temp = 0x20;
		}
		else
		{
			temp = 0xdf;
		}
	}

	PCA9557_I2c_write(PCA9557_OUTPUT_REG,temp);
}



u8 PCA9557_Set_Mode(void)	
{
	PCA9557_I2c_write(PCA9557_MODE_REG,0x00);
}




/*******************************************************************************
* 文件名	     : PCA9557_Dev1_Set
* 描述	         : PCA9557 设备1设置
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 PCA9557_Dev1_Set(IO_EXPANDER_DEV1_TypeDef tpye,u8 val)				//高电平有效
{
	static u8 temp = 0;

	if(tpye < IO_DEV1_LCD_VMTP_ON && tpye > IO_DEV1_ALL_ON)
	{
		return 0;		//返回错误
	}	

	if(tpye != IO_DEV1_ALL_ON)
	{
		//temp |= (val << (tpye -1));	
		if(val)
		{			
			temp = Setbit(temp,(tpye -1));
		}
		else
		{
			temp = Clearbit(temp,(tpye -1));
		}
	}
	else
	{
		if(val)
		{
			temp = 0xff;
		}
		else
		{
			temp = 0x00;
		}
	}

	PCA9557_I2c_write(PCA9557_OUTPUT_REG,temp);
}




/*******************************************************************************
* 文件名	     : PCA9557_Init
* 描述	         : PCA9557初始化
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void PCA9557_Init(void)
{	
	PCA9557_Sel_Dev(0);
	Delay_ms(10);
	PCA9557_Set_Mode();
	PCA9557_Dev0_Set(IO_DEV0_ALL_EN,IO_SWITCH_OFF);
	//PCA9557_Dev0_Set(IO_DEV0_BL_LEDP_EN,IO_SWITCH_ON);
	//PCA9557_Dev0_Set(IO_DEV0_ADC_LED_PWR,IO_SWITCH_OFF);

	PCA9557_Dev0_Set(IO_DEV0_ADC_LED_PWR,IO_SWITCH_OFF);
	PCA9557_Dev0_Set(IO_DEV0_LCD_VMTP_ON,IO_SWITCH_OFF);					//默认关闭
	
	Delay_ms(1);
	
	PCA9557_Sel_Dev(1);
	PCA9557_Set_Mode();
	Delay_ms(1);
	
	PCA9557_Dev1_Set(IO_DEV1_ALL_ON,IO_SWITCH_OFF);
	
	PCA9557_Dev1_Set(IO_DEV1_VSP_OUT_ON,IO_SWITCH_ON);
	#ifdef LCM_2000_MP_VERSION 	
				PCA9557_Dev1_Set(IO_DEV1_BL_ON,IO_SWITCH_ON);			//老的不能打开会烧坏mos管，新的需要打开
	#endif	


	//PCA9557_Dev1_Set(IO_DEV1_BL_ON,IO_SWITCH_ON);
	
	//PCA9557_Dev1_Set(IO_DEV1_DC9V_OUT_ON,IO_SWITCH_ON);	
	//Delay_ms(1);
	//PCA9557_Dev1_Set(IO_DEV1_LCD_VMTP_ON,IO_SWITCH_ON);		//新版用于开启ssd2828电源

	/*
	PCA9557_Dev1_Set(IO_DEV1_RL1_ON,IO_SWITCH_ON);	
	PCA9557_Dev1_Set(IO_DEV1_RL2_ON,IO_SWITCH_ON);
	PCA9557_Dev1_Set(IO_DEV1_RL3_ON,IO_SWITCH_ON);
	PCA9557_Dev1_Set(IO_DEV1_RL4_ON,IO_SWITCH_ON);
	*/	
	//PCA9557_I2c_write(PCA9557_OUTPUT_REG,0x0f);
}





























