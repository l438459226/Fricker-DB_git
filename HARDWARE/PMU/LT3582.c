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


//LT3582 DRIVER

/******************************************
Vin 2.55 -- 5.5V
Voutp  3.2v -- 12.775v   Imax 350ma

Voutp = 3.2+ Vp*50mv + Vpuls*25mv

Vp : 0--191
Vpulse : 0 or 1



Voutn  -1.2v -- 13.95v  (-50mv) Imax 600ma
 
Voutn = -1.2v - (Vn*50mv)

Vn:0--255
******************************************/




/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "LT3582.h"
#include "i2c1_bitbang.h"
#include "SysTick.h"
#include "LoadConfig.h"
#include "PCA9557.h"







#define	REG_CMDR		0x04
#define	REG_VP_VAL		0x00
#define	REG_VN_VAL		0x01
#define	REG_PARA		0x02





static u8 LT3582_Address = LT3582_ADD;


/*******************************************************************************
* 文件名	     : LT3582_I2c_write
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
static int LT3582_I2c_write(u8 reg,u8 val)
{
	int retval;
	
	retval = i2c1_start(LT3582_Address<<1);
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


static int LT3582_I2c_Read(uint8_t reg, uint8_t* pBuffer)
{
	int retval;
	retval = i2c1_start(LT3582_Address<<1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	retval = i2c1_write_u8((uint8_t)(reg));
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_write_u8(reg) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_start_repeat((LT3582_Address<<1)|0x01);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c1_start_repeat Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_read_u8(pBuffer, 1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c_read_data Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_stop();
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c1_stop Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	return I2C_OK;
}




/*******************************************************************************
* 文件名	     : LT3582_Reset
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_Reset(void)
{	
	LT3582_I2c_write(REG_CMDR,0x70);	//清除错误标志，复位，并关闭输出			
}






/*******************************************************************************
* 文件名	     : LT3582_On
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_On(void)
{
	PCA9557_Sel_Dev(0);
	PCA9557_Dev0_Set(IO_DEV0_LCD_VMTP_ON,IO_SWITCH_ON);
	LT3582_I2c_write(REG_CMDR,0x07);	
}



	

/*******************************************************************************
* 文件名	     : LT3582_Off
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_Off(void)
{
	LT3582_I2c_write(REG_CMDR,0x57);
	PCA9557_Sel_Dev(0);
	PCA9557_Dev0_Set(IO_DEV0_LCD_VMTP_ON,IO_SWITCH_OFF);
}


u16 Get_VoutP_Val(u16 volt)
{
	float temp = 0;

	temp = ((float)volt/1000 -3.2)*100/5;

	return(temp);
}


u16 Get_VoutN_Val(u16 volt)
{
	float temp = 0;

	temp = ((float)volt/1000 -1.2)*100/5;

	return(temp);
}








/*******************************************************************************
* 文件名	     : LT3582_Init
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_Init(void)
{
	u8 temp = 0;
	LT3582_Reset();
	Delay_ms(1);
	LT3582_I2c_write(REG_CMDR,0x57);			//选择REG0
	LT3582_I2c_Read(REG_CMDR,&temp);			//返回值
	//printf("reg_cmdr = 0x%x\r\n",temp);
	
	Delay_ms(1);
	
	LT3582_I2c_write(REG_VP_VAL,Get_VoutP_Val(Get_Volt_Val(PWR10_VMTP_INDEX)));			//设置VP电压
	LT3582_I2c_Read(REG_VP_VAL,&temp);
	//printf("reg_vp = 0x%x\r\n",temp);

	//LT3582_I2c_write(REG_CMDR,0x12);			//选择REG1
	Delay_ms(1);
	
	LT3582_I2c_write(REG_VN_VAL,Get_VoutN_Val(Get_Volt_Val(PWR11_VNEG_INDEX)));			//设置VN电压
	
	LT3582_I2c_Read(REG_VN_VAL,&temp);
	//printf("REG_VN_VAL = 0x%x\r\n",temp);

	//LT3582_I2c_write(REG_CMDR,0x14);			//选择REG2
	Delay_ms(1);
	LT3582_I2c_write(REG_PARA,0x07);			//设置参数，
	LT3582_I2c_Read(REG_PARA,&temp);
	//printf("REG_PARA = 0x%x\r\n",temp);

	LT3582_Off();								//默认关闭

}

















