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


//LM36923 DRIVER




/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "LM36923.h"
#include "myiic.h"
#include "delay.h"
#include "usart.h"



#define	REG_RESET		0x01
#define	REG_MODE		0x11
#define	REG_DEVEICE_ON	0x10
#define	REG_CODE_H		0x19
#define	REG_CODE_L		0x18







static u8 LM36923_Address = LM36923_ADD;






/*******************************************************************************
* 文件名	     : LM36923_I2c_write
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
int LM36923_I2c_write(u8 reg,u8 val)
{
	int retval;
	
	retval = i2c1_start(LM36923_Address<<1);
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_write_u8((u8)(reg));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_write_u8(reg) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_write_u8((u8)(val));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_write_u8(val) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_stop();
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c1_stop Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	return I2C_OK;
}





static int LM36923_I2c_Read(uint8_t reg, uint8_t* pBuffer)
{
	int retval;
	retval = i2c1_start(LM36923_Address<<1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	retval = i2c1_write_u8((uint8_t)(reg));
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_write_u8(reg) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_start_repeat((LM36923_Address<<1)|0x01);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c1_start_repeat Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_read_u8(pBuffer, 1);
	//*pBuffer = IIC_Read_Byte();
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c_read_data Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_stop();
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c1_stop Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	return I2C_OK;
}



u8 TL36923_Read(u8 reg)
{
	u8 date = 0x5a;
 	IIC_Start();	
 	IIC_Send_Byte(LM36923_Address<<1);  
	IIC_Wait_Ack();
	
 	IIC_Send_Byte(reg);   	
	IIC_Wait_Ack();  
 	IIC_Start();  	 	   
	IIC_Send_Byte((LM36923_Address<<1)|0x01);   //读 - 地址最低位为1    写- 地址最低位为0
	IIC_Wait_Ack();	

	date = IIC_Read_Byte(); 

	IIC_Stop();
	return date;
}


/*******************************************************************************
* 文件名	     : LM36923_Reset
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_Reset(void)
{
	//LM36923_I2c_write(REG_RESET,0x00); 
	Delay_ms(10);
	LM36923_I2c_write(REG_DEVEICE_ON,0x01); 
	Delay_ms(10);
	LM36923_Set_Mode(BL_MODE_I2C,LIN_MAP);
	Delay_ms(10);
}






/*******************************************************************************
* 文件名	     : LM36923_Set_Mode
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_Set_Mode(BL_MODE_TypeDef mode,MAP_TypeDef map)
{
	u8 temp = 0x00;
		
	temp  = (u8)(map << 7) | 0x01;			//bj_adi 0--关闭，1--输出
	LM36923_I2c_write(REG_MODE,temp | (mode<<5));  //00 I2C控制
}


u8 Setbit1(int num, int bit)
{  
    return num|=(1<<bit);  
}  
  
u8 Clearbit1(int num, int bit)
{  
    return num&=~(1<<bit);  
}  




u8 LM36923_Val = 0;

/*******************************************************************************
* 文件名	     : LM36923_On
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_On(BL_CHN_TypeDef chn)
{
	LM36923_Val = Setbit1(LM36923_Val,chn) | 0x01;
	LM36923_I2c_write(REG_DEVEICE_ON,LM36923_Val); 	
}



	

/*******************************************************************************
* 文件名	     : LM36923_Off
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_Off(BL_CHN_TypeDef chn)
{
	u8 temp = 0;
	LM36923_Val = Clearbit1(LM36923_Val,chn) | 0x01;
	LM36923_I2c_write(REG_DEVEICE_ON,LM36923_Val); 	
	LM36923_I2c_Read(REG_DEVEICE_ON,&temp);
	//printf("temp = %d\r\n",temp);
}


void LM36923_AllOn(void)
{
	LM36923_I2c_write(REG_DEVEICE_ON,0x0f); 	
}


void LM36923_AllOff(void)
{
	LM36923_I2c_write(REG_DEVEICE_ON,0x01); 	
}





/*******************************************************************************
* 文件名	     : LM36923_Set_Current
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
//liner map 计算公式 I = 37.806ua + 12.195ua*val,max = 25ma
//val 1 -- 2047
void LM36923_Set_Current(u16 val)
{
	LM36923_I2c_write(REG_CODE_L,val&0x07);	
	LM36923_I2c_write(REG_CODE_H,(u8)(val>>3));	
}






//电感也可能要设置，先预留

/*******************************************************************************
* 文件名	     : LM36923_Init
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_Init(u16 val)
{	
	u8 temp;
	float value = 0;
	u16 value1 = 0;
	LM36923_Reset();
	//LM36923_I2c_write(0x17,0xFF); Disable BL_ADJ, Debug Only
	LM36923_I2c_Read(0x11,&temp);
	TESTER_MAIN_DEBUG("REG_11 = 0x%x\r\n",temp);
	Delay_ms(10);
	LM36923_I2c_Read(0x13,&temp);
	LM36923_I2c_write(0x13,temp | (0x01<<4)); 	//设置电感为22uh

	LM36923_I2c_write(REG_DEVEICE_ON,0x0F); 

	/*
	LM36923_On(BL_CHN1);
	LM36923_On(BL_CHN2);
	LM36923_On(BL_CHN3);
	*/

	TESTER_MAIN_DEBUG("\r\nbl ma = %d\r\n",val);
	value = ((float)2047)/25;
	//TESTER_MAIN_DEBUG("\r\nbl ma = %f\r\n",value);
	value *= 100;
	//TESTER_MAIN_DEBUG("\r\nbl ma = %f\r\n",value);
	value *= val;
	//TESTER_MAIN_DEBUG("\r\nbl ma = %f\r\n",value);
	value1 = (u16)(((u32)(value))/100);


	TESTER_MAIN_DEBUG("\r\nbl value = %d\r\n",value1);
	
	LM36923_Set_Current(value1);
	
	LM36923_I2c_Read(0x19,&temp);
	TESTER_MAIN_DEBUG("REG_19 = 0x%x\r\n",temp);
	LM36923_I2c_Read(0x18,&temp);
	TESTER_MAIN_DEBUG("REG_18 = 0x%x\r\n",temp);	
	

	LM36923_I2c_Read(0x1f,&temp);
	TESTER_MAIN_DEBUG("REG_ERR = 0x%x\r\n",temp);	

	
	//LM36923_On(BL_CHN1);	
	//LM36923_On(BL_CHN2);
	//LM36923_Off(BL_CHN3);
	
}




































