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


//TPS22993 DRIVER




/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "TPS22993.h"
#include "myiic.h"
#include "delay.h"



#define	REG_ALL_CONTROL		0x05
#define	REG_CH1_CONTROL		0x01
#define	REG_CH2_CONTROL		0x02
#define	REG_CH3_CONTROL		0x03
#define	REG_CH4_CONTROL		0x04



/*********************************************************************************************************
*                                              变量定义
*********************************************************************************************************/
static u8 TPS22993_Address = TPS22993_DEVEICE0_ADD;
static u8 TPS22993_Mode = 0;						//0---IO控制，1---I2C控制
static u8 TPS22993_Swtich = 0;
static u8 TPS22993_Swtich1 = 0;


/*******************************************************************************
* 文件名	     : TPS22993_I2c_write
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
static int TPS22993_I2c_write(u8 reg,u8 val)
{
	int retval;
	
	retval = i2c1_start(TPS22993_Address<<1);
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





/*******************************************************************************
* 文件名	     : TPS22993_Sel_Dev
* 描述	         : TPS22993设备选择
* 输入           : 0---选择设备0 1---选择设备1
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_Sel_Dev(u8 val)
{
	if(val == 0)
	{
		TPS22993_Address = TPS22993_DEVEICE0_ADD;
	}
	else
	{
		TPS22993_Address = TPS22993_DEVEICE1_ADD;
	}		
}






/*******************************************************************************
* 文件名	     : TPS22993_Set_Mode
* 描述	         : mode 0--IO,1--I2C
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_Set_Mode(TPS22993_CHN_TypeDef chn,u8 mode)
{

	if(mode)
	{
		TPS22993_Mode |= (mode << (chn+4)); 
	}
	else
	{
		mode = 1;
		mode = ~mode;
		TPS22993_Mode &= (mode << (chn+4)); 
	}

	TPS22993_Mode &= 0xF0;		//取高4位 

	TPS22993_I2c_write(REG_ALL_CONTROL,TPS22993_Mode);
}



/*******************************************************************************
* 文件名	     : TPS22993_On
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_On(TPS22993_CHN_TypeDef chn)
{
	u8 temp = 1;

	if(TPS22993_Address == TPS22993_DEVEICE0_ADD)
	{
		TPS22993_Swtich = (TPS22993_Swtich | (temp << chn)) & 0x0F;	//低4位
		TPS22993_Swtich = TPS22993_Mode | TPS22993_Swtich;
		TPS22993_I2c_write(REG_ALL_CONTROL,TPS22993_Swtich);
	}
	else if(TPS22993_Address == TPS22993_DEVEICE1_ADD)
	{
		TPS22993_Swtich1 = (TPS22993_Swtich1 | (temp << chn)) & 0x0F;	//低4位
		TPS22993_Swtich1 = TPS22993_Mode | TPS22993_Swtich1;
		TPS22993_I2c_write(REG_ALL_CONTROL,TPS22993_Swtich1);
	}
}

	

/*******************************************************************************
* 文件名	     : TPS22993_Off
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_Off(TPS22993_CHN_TypeDef chn)
{
	u8 temp = 1;

	if(TPS22993_Address == TPS22993_DEVEICE0_ADD)
	{
		TPS22993_Swtich = (TPS22993_Swtich & (~(temp << chn))) & 0x0F;	//低4位	
		TPS22993_Swtich = TPS22993_Mode | TPS22993_Swtich;
		TPS22993_I2c_write(REG_ALL_CONTROL,TPS22993_Swtich);

	}
	else if(TPS22993_Address == TPS22993_DEVEICE1_ADD)
	{
		TPS22993_Swtich1 = (TPS22993_Swtich1 & (~(temp << chn))) & 0x0F;	//低4位	
		TPS22993_Swtich1 = TPS22993_Mode | TPS22993_Swtich1;
		TPS22993_I2c_write(REG_ALL_CONTROL,TPS22993_Swtich1);


	}	
}





/*******************************************************************************
* 文件名	     : TPS22993_Set_Fun
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_Set_Fun(TPS22993_CHN_TypeDef chn,TPS22993_ONDELAY_TypeDef ondelay_val,TPS22993_SLEWRATE_TypeDef slewrate_val,TPS22993_QOD_TypeDef qod_val)
{
	u8 temp = 0;

	temp = (ondelay_val<<5) | (slewrate_val<<2) | qod_val;

	TPS22993_I2c_write((u8)chn+1,temp);

}








/*******************************************************************************
* 文件名	     : TPS22993_Init
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_Init(void)
{
	TPS22993_Sel_Dev(0);				//选择设备0
#if 0
	TPS22993_Set_Mode(TPS22993_CHN1,0);
	TPS22993_Set_Mode(TPS22993_CHN2,0);
#else
	TPS22993_Set_Mode(TPS22993_CHN1,1);
	TPS22993_Set_Mode(TPS22993_CHN2,1);
#endif
	
	TPS22993_Set_Fun(TPS22993_CHN1,ONDELAY11US,SLEWRATE460USV,QOD110R);
	TPS22993_Set_Fun(TPS22993_CHN2,ONDELAY11US,SLEWRATE460USV,QOD110R);
	Delay_ms(1);
	TPS22993_Off(TPS22993_CHN1);
	TPS22993_Off(TPS22993_CHN2);
	
	Delay_ms(10);
	TPS22993_Sel_Dev(1);				//选择设备1
#if 0
	TPS22993_Set_Mode(TPS22993_CHN1,0);
	TPS22993_Set_Mode(TPS22993_CHN2,0);
	TPS22993_Set_Mode(TPS22993_CHN3,0);
	TPS22993_Set_Mode(TPS22993_CHN4,0);
#else
	TPS22993_Set_Mode(TPS22993_CHN1,1);
	TPS22993_Set_Mode(TPS22993_CHN2,1);
	TPS22993_Set_Mode(TPS22993_CHN3,1);
	TPS22993_Set_Mode(TPS22993_CHN4,1);
#endif
	TPS22993_Set_Fun(TPS22993_CHN1,ONDELAY11US,SLEWRATE460USV,QOD110R);
	TPS22993_Set_Fun(TPS22993_CHN2,ONDELAY11US,SLEWRATE460USV,QOD110R);
	TPS22993_Set_Fun(TPS22993_CHN3,ONDELAY11US,SLEWRATE460USV,QOD110R);
	TPS22993_Set_Fun(TPS22993_CHN4,ONDELAY11US,SLEWRATE460USV,QOD110R);
	Delay_ms(1);
	TPS22993_Off(TPS22993_CHN1);
	TPS22993_Off(TPS22993_CHN2);
	TPS22993_Off(TPS22993_CHN3);
	TPS22993_Off(TPS22993_CHN4);
}





/*******************************************************************************
* 文件名	     : TPS22993_Init
* 描述	         : val:1--gpio  0--I2C
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void TPS22993_Set_All_Mode(u8 val)
{
	TPS22993_Sel_Dev(0);				//选择设备0
	if(val)
	{
		TPS22993_Set_Mode(TPS22993_CHN1,0);
		TPS22993_Set_Mode(TPS22993_CHN2,0);
		TPS22993_Set_Mode(TPS22993_CHN3,0);
		TPS22993_Set_Mode(TPS22993_CHN4,0);
	}
	else
	{
		TPS22993_Set_Mode(TPS22993_CHN1,1);
		TPS22993_Set_Mode(TPS22993_CHN2,1);
		TPS22993_Set_Mode(TPS22993_CHN3,1);
		TPS22993_Set_Mode(TPS22993_CHN4,1);
	}


	TPS22993_Sel_Dev(1);				//选择设备1
	if(val)
	{
		TPS22993_Set_Mode(TPS22993_CHN1,0);
		TPS22993_Set_Mode(TPS22993_CHN2,0);
		TPS22993_Set_Mode(TPS22993_CHN3,0);
		TPS22993_Set_Mode(TPS22993_CHN4,0);

	}
	else
	{
		TPS22993_Set_Mode(TPS22993_CHN1,1);
		TPS22993_Set_Mode(TPS22993_CHN2,1);
		TPS22993_Set_Mode(TPS22993_CHN3,1);
		TPS22993_Set_Mode(TPS22993_CHN4,1);
	}



}








