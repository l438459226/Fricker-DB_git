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
#include "LoadConfig.h"
#include "SysTick.h"
#include <string.h>
#include "TPS22993.h"
#include "LM36923.h"
#include "tps65132.h"
#include "tester_debug.h"
#include "Uart.h"
#include "GPIO_Config.h"
#include "pwm_input.h"
#include "Protocol.h"



/*********************************************************************************************************
*                                              变量定义
*********************************************************************************************************/



static PARA_STRUCT_TypeDef			ParaManage;
static PATTERN_STRUCT_TypeDef      PatternManage;



static	u8	PowerOnOffSeq = 0;			//0--默认关闭，1--打开
static  u8  Pic_Num_Max = 0;

static  u32 Info_Add = 0;
static  u16 Info_Add_Size = 0;
static  u32 Pattern_Add = 0;
static  u16 Pattern_Add_Size = 0;
static  u32 Board_Init_Add = 0;
static  u16 Board_Init_Add_Size = 0;
static  u32 PowerOn_Add = 0;
static  u16 PowerOn_Add_Size = 0;
static  u32 Code_Init_Add = 0;
static  u16 Code_Init_Add_Size = 0;
static  u32 PowerOff_Add = 0;
static  u16 PowerOff_Add_Size = 0;
static  u32 Sleep_Seq_Add = 0;
static  u16 Sleep_Seq_Add_Size = 0;
static  u32 Code_Init2_Add = 0;
static  u16 Code_Init2_Add_Size = 0;
static  u32 Code_Cmd1_Add = 0;
static  u16 Code_Cmd1_Add_Size = 0;
static  u32 Code_Cmd2_Add = 0;
static  u16 Code_Cmd2_Add_Size = 0;
static  u32 Code_Cmd3_Add = 0;
static  u16 Code_Cmd3_Add_Size = 0;
static  u32 Code_Cmd4_Add = 0;
static  u16 Code_Cmd4_Add_Size = 0;



static u8 Power_Status = 0;	
static u8 Sleep_Status = 0;	//0---正常模式，1---休眠模式
static u8 Vsn_Power_Status = 0;	//VSN 0---关掉，1---打开
static u8 Iovcc_Power_Status = 0;	//IOVCC 0---关掉，1---打开
static u8 Vsp_Power_Status = 0;
static u8 Tpvio_Power_Status = 0;
static u8 Tpvdd_Power_Status = 0;
static u8 Lcdvdd_Power_Status = 0;


static u8 Flag_Big_Current_Err = 0;

static u16 Lcd_Iovcc_A_High = 0;
static u16 Lcd_Vddh_A_High = 0;
static u16 Tp_Vio_A_High = 0;
static u16 Tp_Vdd_A_High = 0;
static u16 Vsp_A_High = 0;
static u16 Vsn_A_High = 0;



/////////////////////////////////////////////////
//结构体部分定义   PowerStatusManage
u16 PWR1_IOVCC_Off_Status;
u16 PWR2_VDDH_Off_Status;
u16 PWR3_TP_VDD_Off_Status;
u16 PWR4_TP_VIO_Off_Status;
u16 PWR5_VSP_AVDD_Off_Status;
u16 PWR6_VSN_AVEE_Off_Status;
u16 PWR7_LED1N_Off_Status;
u16 PWR8_LED2N_Off_Status;
u16 PWR9_LED3N_Off_Status;
u16 PWR10_VMTP_Off_Status;
u16 PWR11_VNEG_Off_Status;
u16 PWR12_LED_PWR_5V_Off_Status;
u16 PWR13_EXT_PWR_A_Off_Status;
u16 PWR14_EXT_PWR_B_Off_Status;
u16 PWR15_TESTER_VBUS_Off_Status;

u16 PWR1_IOVCC_On_Status; 
u16 PWR2_VDDH_On_Status; 
u16 PWR3_TP_VDD_On_Status; 
u16 PWR4_TP_VIO_On_Status;
u16 PWR5_VSP_AVDD_On_Status;
u16 PWR6_VSN_AVEE_On_Status;
u16 PWR7_LED1N_On_Status;
u16 PWR8_LED2N_On_Status;
u16 PWR9_LED3N_On_Status;
u16 PWR10_VMTP_On_Status;
u16 PWR11_VNEG_On_Status;
u16 PWR12_LED_PWR_5V_On_Status;
u16 PWR13_EXT_PWR_A_On_Status;
u16 PWR14_EXT_PWR_B_On_Status;
u16 PWR15_TESTER_VBUS_On_Status;


///////////////////////////////////////////////////////
//结构体部分定义   SetVoltageManage
u16 PWR1_IOVCC; 
u16 PWR2_VDDH; 
u16 PWR3_TP_VDD; 
u16 PWR4_TP_VIO;
u16 PWR5_VSP_AVDD;
u16 PWR6_VSN_AVEE;
u16 PWR7_LED1N;
u16 PWR8_LED2N;
u16 PWR9_LED3N;
u16 PWR10_VMTP;
u16 PWR11_VNEG;
u16 PWR12_LED_PWR_5V;
u16 PWR13_EXT_PWR_A;
u16 PWR14_EXT_PWR_B;
u16 PWR15_TESTER_VBUS;

u16 PWR1_IOVCC_HIGH; 	//电压上下线
u16 PWR1_IOVCC_LOW;
u16 PWR2_VDDH_HIGH; 
u16 PWR2_VDDH_LOW;
u16 PWR3_TP_VDD_HIGH; 
u16 PWR3_TP_VDD_LOW; 
u16 PWR4_TP_VIO_HIGH;
u16 PWR4_TP_VIO_LOW;
u16 PWR5_VSP_AVDD_HIGH;
u16 PWR5_VSP_AVDD_LOW;
u16 PWR6_VSN_AVEE_HIGH;
u16 PWR6_VSN_AVEE_LOW;
u16 PWR7_LED1N_HIGH;
u16 PWR7_LED1N_LOW;
u16 PWR8_LED2N_HIGH;
u16 PWR8_LED2N_LOW;
u16 PWR9_LED3N_HIGH;
u16 PWR9_LED3N_LOW;
u16 PWR10_VMTP_HIGH;
u16 PWR10_VMTP_LOW;
u16 PWR11_VNEG_HIGH;
u16 PWR11_VNEG_LOW;
u16 PWR12_LED_PWR_5V_HIGH;
u16 PWR12_LED_PWR_5V_LOW;
u16 PWR13_EXT_PWR_A_HIGH;
u16 PWR13_EXT_PWR_A_LOW;
u16 PWR14_EXT_PWR_B_HIGH;
u16 PWR14_EXT_PWR_B_LOW;
u16 PWR15_TESTER_VBUS_HIGH;
u16 PWR15_TESTER_VBUS_LOW;

u16 PWR1_IOVCC_A_HIGH; 	//电流上下线
u16 PWR1_IOVCC_A_LOW;
u16 PWR2_VDDH_A_HIGH; 
u16 PWR2_VDDH_A_LOW;
u16 PWR3_TP_VDD_A_HIGH; 
u16 PWR3_TP_VDD_A_LOW; 
u16 PWR4_TP_VIO_A_HIGH;
u16 PWR4_TP_VIO_A_LOW;
u16 PWR5_VSP_AVDD_A_HIGH;
u16 PWR5_VSP_AVDD_A_LOW;
u16 PWR6_VSN_AVEE_A_HIGH;
u16 PWR6_VSN_AVEE_A_LOW;
u16 PWR7_LED1N_A_HIGH;
u16 PWR7_LED1N_A_LOW;
u16 PWR8_LED2N_A_HIGH;
u16 PWR8_LED2N_A_LOW;
u16 PWR9_LED3N_A_HIGH;
u16 PWR9_LED3N_A_LOW;
u16 PWR10_VMTP_A_HIGH;
u16 PWR10_VMTP_A_LOW;
u16 PWR11_VNEG_A_HIGH;
u16 PWR11_VNEG_A_LOW;
u16 PWR12_LED_PWR_5V_A_HIGH;
u16 PWR12_LED_PWR_5V_A_LOW;
u16 PWR13_EXT_PWR_A_A_HIGH;
u16 PWR13_EXT_PWR_A_A_LOW;
u16 PWR14_EXT_PWR_B_A_HIGH;
u16 PWR14_EXT_PWR_B_A_LOW;
u16 PWR15_TESTER_VBUS_A_HIGH;
u16 PWR15_TESTER_VBUS_A_LOW;

u16 PWR1_IOVCC_UA_HIGH; 	//UA电流上下线
u16 PWR1_IOVCC_UA_LOW;
u16 PWR2_VDDH_UA_HIGH; 
u16 PWR2_VDDH_UA_LOW;
u16 PWR3_TP_VDD_UA_HIGH; 
u16 PWR3_TP_VDD_UA_LOW; 
u16 PWR4_TP_VIO_UA_HIGH;
u16 PWR4_TP_VIO_UA_LOW;
u16 PWR5_VSP_AVDD_UA_HIGH;
u16 PWR5_VSP_AVDD_UA_LOW;
u16 PWR6_VSN_AVEE_UA_HIGH;
u16 PWR6_VSN_AVEE_UA_LOW;
u16 PWR7_LED1N_UA_HIGH;
u16 PWR7_LED1N_UA_LOW;
u16 PWR8_LED2N_UA_HIGH;
u16 PWR8_LED2N_UA_LOW;
u16 PWR9_LED3N_UA_HIGH;
u16 PWR9_LED3N_UA_LOW;
u16 PWR10_VMTP_UA_HIGH;
u16 PWR10_VMTP_UA_LOW;
u16 PWR11_VNEG_UA_HIGH;
u16 PWR11_VNEG_UA_LOW;
u16 PWR12_LED_PWR_5V_UA_HIGH;
u16 PWR12_LED_PWR_5V_UA_LOW;
u16 PWR13_EXT_PWR_A_UA_HIGH;
u16 PWR13_EXT_PWR_A_UA_LOW;
u16 PWR14_EXT_PWR_B_UA_HIGH;
u16 PWR14_EXT_PWR_B_UA_LOW;
u16 PWR15_TESTER_VBUS_UA_HIGH;
u16 PWR15_TESTER_VBUS_UA_LOW;




//////////////////////////////////////////
//结构体部分定义 CtrlAddValManage
u32 General_Info_Add; 
u32 Pattern_Set_Add; 
u32 Board_Add; 
u32 Power_On_Set_Add; 
u32 Init_Code_Add; 
u32 Power_Off_Set_Add; 
u32 Sleep_Seq_Set_Add; 
u32 Init2_Code_Add; 
u32 Cmd_Code1_Add; 
u32 Cmd_Code2_Add;
u32 Cmd_Code3_Add;
u32 Cmd_Code4_Add;




u16 General_Info_Size; 
u16 Pattern_Set_Size;
u32 Board_Size; 
u16 Power_On_Set_Size;
u16 Init_Code_Size; 
u16 Power_Off_Set_Size;
u16 Sleep_Seq_Set_Size; 
u16 Init2_Code_Size; 
u16 Cmd_Code1_Size; 
u16 Cmd_Code2_Size;
u16 Cmd_Code3_Size;
u16 Cmd_Code4_Size;


///////////////////////////////////////////////////
//选项配置区域
u16 Flag_Current_En = 0;
u16 Flag_Voltage_En = 0;
u16 Flag_Sleep_Current_En = 0;
u16 Flag_Hard_ID_En = 0;
u16 Hard_ID_En_Val = 0;				//使能和限制值
u16 Flag_Soft_ID_En = 0;
u16 Soft_ID12_Val = 0;
u16 Soft_ID34_Val = 0;
u16 Flag_OTP_Times_En = 0;
u16 OTP_Times_High = 0;
u16 OTP_Times_Low = 0;
u16 Flag_DuiWei_En = 0;
u16 Flag_PWM_En = 0;
u16 PWM_Seq_High = 0;
u16 PWM_Seq_Low = 0;
u16 PWM_Duty_High = 0;
u16 PWM_Duty_Low = 0;
u16 PWM_Volt_High = 0;
u16 PWM_Volt_Low = 0;
u16 Flag_TE_En = 0;
u16 TE_Seq_High = 0;
u16 TE_Seq_Low = 0;
u16 TE_Duty_High = 0;
u16 TE_Duty_Low = 0;
u16 TE_Volt_High = 0;
u16 TE_Volt_Low = 0;
u16 FLAG_BK_En = 0;


u8 Flag_Vsp_Vsn = 0;

u8 G_DISP_FILE_BUFFER[45*1024];		//20k ram to disp。bin










/*******************************************************************************
* 文件名	  	 : Read_Start_Address
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
u8  Read_Start_Address(u32 reg_addr,u32 *data_addr,u16 *data_size)
{
	u32 temp = 0;
	u8 temparr[8];	

		
	memcpy(temparr,&G_DISP_FILE_BUFFER[reg_addr],8);
	
	temp = ((u32)temparr[3])<<24 & 0xff000000;
	temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
	temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
	temp |= (u32)temparr[0];	
	*data_addr = temp;
	temp = ((u32)temparr[7])<<24 & 0xff000000;
	temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
	temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
	temp |= (u32)temparr[4];				
	*data_size = temp;	
		

	return(0);	
}




/*******************************************************************************
* 文件名	  	 : BinSetVoltVal_Handle
* 描述	         : 设置电压值
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void BinSetVoltVal_Handle(u8 chn,u16 val)
{
	//设置电压值
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC = val;
		break;
		case 2:
			 PWR2_VDDH = val;
		break;
		case 3:
			 PWR3_TP_VDD = val;
		break;
		case 4:
			 PWR4_TP_VIO = val;
		break;
		case 5:
			 PWR5_VSP_AVDD = val;
		break;
		case 6:
			 PWR6_VSN_AVEE = val;
		break;
		case 7:
			 PWR7_LED1N = val;
		break;
		case 8:
			 PWR8_LED2N = val;
		break;
		case 9:
			 PWR9_LED3N = val;
		break;
		case 10:
			 PWR10_VMTP = val;
		break;
		case 11:
			 PWR11_VNEG = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS = val;
		break;		
	}
}


/*******************************************************************************
* 文件名	  	 : BinSetVoltLow_Handle
* 描述	         : 设置电压下限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void BinSetVoltLow_Handle(u8 chn,u16 val)
{
	//设置电压下限	
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_LOW = val;			
		break;
		case 2:
			 PWR2_VDDH_LOW = val;			
		break;
		case 3:
			 PWR3_TP_VDD_LOW = val;			
		break;
		case 4:
			 PWR4_TP_VIO_LOW = val;			
		break;
		case 5:
			 PWR5_VSP_AVDD_LOW = val;			
		break;
		case 6:
			 PWR6_VSN_AVEE_LOW = val;
		break;
		case 7:
			 PWR7_LED1N_LOW = val;
		break;
		case 8:
			 PWR8_LED2N_LOW = val;
		break;
		case 9:
			 PWR9_LED3N_LOW = val;
		break;
		case 10:
			 PWR10_VMTP_LOW = val;			
		break;
		case 11:
			 PWR11_VNEG_LOW = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_LOW = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_LOW = val;			
		break;
		case 14:
			 PWR14_EXT_PWR_B_LOW = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_LOW = val;
		break;
	}		
}
	


/*******************************************************************************
* 文件名	  	 : BinSetVoltHigh_Handle
* 描述	         : 设置电压上限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/			
void BinSetVoltHigh_Handle(u8 chn,u16 val)
{
	//设置电压上限
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_HIGH = val;			
		break;
		case 2:
			 PWR2_VDDH_HIGH = val;			
		break;
		case 3:
			 PWR3_TP_VDD_HIGH = val;			
		break;
		case 4:
			 PWR4_TP_VIO_HIGH = val;			
		break;
		case 5:
			 PWR5_VSP_AVDD_HIGH = val;			
		break;
		case 6:
			 PWR6_VSN_AVEE_HIGH = val;
		break;
		case 7:
			 PWR7_LED1N_HIGH = val;
		break;
		case 8:
			 PWR8_LED2N_HIGH = val;
		break;
		case 9:
			 PWR9_LED3N_HIGH = val;
		break;
		case 10:
			 PWR10_VMTP_HIGH = val;			
		break;
		case 11:
			 PWR11_VNEG_HIGH = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_HIGH = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_HIGH = val;			
		break;
		case 14:
			 PWR14_EXT_PWR_B_HIGH = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_HIGH = val;
		break;
	}
}



/*******************************************************************************
* 文件名	  	 : BinSetCurrLow_Handle
* 描述	         : 设置电流下限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/	
void BinSetCurrLow_Handle(u8 chn,u16 val)
{
	//设置电流下限	
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_A_LOW = val;
		break;
		case 2:
			 PWR2_VDDH_A_LOW = val;
		break;
		case 3:
			 PWR3_TP_VDD_A_LOW = val;
		break;
		case 4:
			 PWR4_TP_VIO_A_LOW = val;
		break;
		case 5:
			 PWR5_VSP_AVDD_A_LOW = val;
		break;
		case 6:
			 PWR6_VSN_AVEE_A_LOW = val;
		break;
		case 7:
			 PWR7_LED1N_A_LOW = val;
		break;
		case 8:
			 PWR8_LED2N_A_LOW = val;
		break;
		case 9:
			 PWR9_LED3N_A_LOW = val;
		break;
		case 10:
			 PWR10_VMTP_A_LOW = val;
		break;
		case 11:
			 PWR11_VNEG_A_LOW = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_A_LOW = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_A_LOW = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B_A_LOW = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_A_LOW = val;
		break;
		////////////////////////////////////////////////////////
		//ua part
		case 101:
			 PWR1_IOVCC_UA_LOW = val;
		break;
		case 102:
			 PWR2_VDDH_UA_LOW = val;
		break;
		case 103:
			 PWR3_TP_VDD_UA_LOW = val;
		break;
		case 104:
			 PWR4_TP_VIO_UA_LOW = val;
		break;
		case 105:
			 PWR5_VSP_AVDD_UA_LOW = val;
		break;
		case 106:
			 PWR6_VSN_AVEE_UA_LOW = val;
		break;
		case 107:
			 PWR7_LED1N_UA_LOW = val;
		break;
		case 108:
			 PWR8_LED2N_UA_LOW = val;
		break;
		case 109:
			 PWR9_LED3N_UA_LOW = val;
		break;
		case 110:
			 PWR10_VMTP_UA_LOW = val;
		break;
		case 111:
			 PWR11_VNEG_UA_LOW = val;
		break;
		case 112:
			 PWR12_LED_PWR_5V_UA_LOW = val;
		break;
		case 113:
			 PWR13_EXT_PWR_A_UA_LOW = val;
		break;
		case 114:
			 PWR14_EXT_PWR_B_UA_LOW = val;
		break;
		case 115:
			 PWR15_TESTER_VBUS_UA_LOW = val;
		break;
	}
}



/*******************************************************************************
* 文件名	  	 : BinSetCurrHigh_Handle
* 描述	         : 设置电流上限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void BinSetCurrHigh_Handle(u8 chn,u16 val)
{
	//设置电流上限	
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_A_HIGH = val;
			Lcd_Iovcc_A_High = val;
		break;
		case 2:
			 PWR2_VDDH_A_HIGH = val;
			Lcd_Vddh_A_High = val;
		break;
		case 3:
			 PWR3_TP_VDD_A_HIGH = val;
			Tp_Vdd_A_High = val;
		break;
		case 4:
			 PWR4_TP_VIO_A_HIGH = val;
			Tp_Vio_A_High = val;
		break;
		case 5:
			 PWR5_VSP_AVDD_A_HIGH = val;
			Vsp_A_High = val;
		break;
		case 6:
			 PWR6_VSN_AVEE_A_HIGH = val;
			Vsn_A_High = val;
		break;
		case 7:
			 PWR7_LED1N_A_HIGH = val;
		break;
		case 8:
			 PWR8_LED2N_A_HIGH = val;
		break;
		case 9:
			 PWR9_LED3N_A_HIGH = val;
		break;
		case 10:
			 PWR10_VMTP_A_HIGH = val;
		break;
		case 11:
			 PWR11_VNEG_A_HIGH = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_A_HIGH = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_A_HIGH = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B_A_HIGH = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_A_HIGH = val;
		break;
		////////////////////////////////////////////////////////
		//ua part
		case 101:
			 PWR1_IOVCC_UA_HIGH = val;
		break;
		case 102:
			 PWR2_VDDH_UA_HIGH = val;
		break;
		case 103:
			 PWR3_TP_VDD_UA_HIGH = val;
		break;
		case 104:
			 PWR4_TP_VIO_UA_HIGH = val;
		break;
		case 105:
			 PWR5_VSP_AVDD_UA_HIGH = val;
		break;
		case 106:
			 PWR6_VSN_AVEE_UA_HIGH = val;
		break;
		case 107:
			 PWR7_LED1N_UA_HIGH = val;
		break;
		case 108:
			 PWR8_LED2N_UA_HIGH = val;
		break;
		case 109:
			 PWR9_LED3N_UA_HIGH = val;
		break;
		case 110:
			 PWR10_VMTP_UA_HIGH = val;
		break;
		case 111:
			 PWR11_VNEG_UA_HIGH = val;
		break;
		case 112:
			 PWR12_LED_PWR_5V_UA_HIGH = val;
		break;
		case 113:
			 PWR13_EXT_PWR_A_UA_HIGH = val;
		break;
		case 114:
			 PWR14_EXT_PWR_B_UA_HIGH = val;
		break;
		case 115:
			 PWR15_TESTER_VBUS_UA_HIGH = val;
		break;
	}
}


		



/*******************************************************************************
* 文件名	  	 : PowerUp_Init_Handle
* 描述	         : 主板上电初始化，不是点屏上电流程
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static void  PowerUp_Init_Handle(void)
{
	u16 temp = 0;
	
#if 1
	Delay_ms(20);
	
	i2c1_init();		//I2C初始化

	Delay_ms(20);

	I2C_Detect();

	Delay_ms(100);
#endif
	
	TESTER_MAIN_DEBUG("\r\nEnable ACT8846....\r\n\r\n");
	act8846_init(); 
	Delay_ms(200);
	TESTER_MAIN_DEBUG("int ACT8846_ON();\r\n");
	
	
	ACT8846_DCDC_Switch(DCDC_SW1,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW2,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW3,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW4,ACT8846_SWITCH_ON);

	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON);
	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_OFF);
	//ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_OFF);

	AD715_PWR_ON_Cfg();
	OTP_EN_ON_OFF(1);					//打开OTP
	OTP_P_ON_OFF(1);
	LT3582_PWR_ON_Cfg();				//OTP_EN 拉高,里面设置了电压
	

	TPS22993_PWR_ON_Cfg();
	TPS22993_Set_All_Mode(1);			//设置负载开关为io控制
	TPS22993_VSP_VSN_ON_OFF(1);			//VSP_VSN POWER ON	
	Delay_ms(200);						//延时时间不能太小
	
	BL_POWER_ON(); 
	Delay_ms(100);
	//////////////////////////////////////////////////////////VSP VSN设置	
#if 1
	VSP_ON();		//BL ADJ 拉高，使能 vsp  vsn
	VSP_PWR_ON();
	VSN_ON();
	BL_ADJ_OFF();

	OLEDLCD_MAIN_POWER_OFF(); 
	BL_DETECT_ON();									//背光检测使能
	//BL_POWER_ON(); 									//背光主电源开启
	LM36923_PWR_ON_Cfg(PWR7_LED1N);					//设置背光电流
	

	/////////////////////////////////////			//开启oled电压
	#if OLED_EN
	tps65651_Init();
	tps65651_On();
	OLEDLCD_MAIN_POWER_ON(); 
	#endif
	
	
	Delay_ms(10);		
	Tps65132_Init(PWR5_VSP_AVDD, PWR6_VSN_AVEE); //设置VSP VSN电压
	Delay_ms(10);

	//I2C_Detect();
	
	VSP_OFF();		//BL ADJ 拉高，断开vsp  vsn
	VSN_OFF();
#endif

	

#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("x=%d y=%d\t\r\n",ParaManage.Resolution_X,ParaManage.Resolution_Y);
	TESTER_MAIN_DEBUG("pwr1=%d %d %d %d %d\t\r\n", PWR1_IOVCC, PWR2_VDDH, PWR3_TP_VDD, PWR4_TP_VIO, PWR5_VSP_AVDD);
	TESTER_MAIN_DEBUG("pwr6=%d %d %d %d %d\t\r\n", PWR6_VSN_AVEE, PWR7_LED1N, PWR8_LED2N, PWR9_LED3N, PWR10_VMTP);
	TESTER_MAIN_DEBUG("pwr11=%d %d %d %d \t\r\n", PWR11_VNEG, PWR12_LED_PWR_5V, PWR13_EXT_PWR_A, PWR14_EXT_PWR_B);
#endif

#if DEBUG_BIN_CFG		//电压电流上下限
	TESTER_MAIN_DEBUG("IOVCC LIMIT = %d %d %d %d %d %d\r\n",PWR1_IOVCC_HIGH,PWR1_IOVCC_LOW,PWR1_IOVCC_A_HIGH,PWR1_IOVCC_A_LOW,PWR1_IOVCC_UA_HIGH,PWR1_IOVCC_UA_LOW);
	TESTER_MAIN_DEBUG("VDDH LIMIT = %d %d %d %d %d %d\r\n",PWR2_VDDH_HIGH,PWR2_VDDH_LOW,PWR2_VDDH_A_HIGH,PWR2_VDDH_A_LOW,PWR2_VDDH_UA_HIGH,PWR2_VDDH_UA_LOW);
	TESTER_MAIN_DEBUG("TPVDD LIMIT = %d %d %d %d %d %d\r\n",PWR3_TP_VDD_HIGH,PWR3_TP_VDD_LOW,PWR3_TP_VDD_A_HIGH,PWR3_TP_VDD_A_LOW,PWR3_TP_VDD_UA_HIGH,PWR3_TP_VDD_UA_LOW);
	TESTER_MAIN_DEBUG("TPVIO LIMIT = %d %d %d %d %d %d\r\n",PWR4_TP_VIO_HIGH,PWR4_TP_VIO_LOW,PWR4_TP_VIO_A_HIGH,PWR4_TP_VIO_A_LOW,PWR4_TP_VIO_UA_HIGH,PWR4_TP_VIO_UA_LOW);
	TESTER_MAIN_DEBUG("VSP LIMIT = %d %d %d %d %d %d\r\n",PWR5_VSP_AVDD_HIGH,PWR5_VSP_AVDD_LOW,PWR5_VSP_AVDD_A_HIGH,PWR5_VSP_AVDD_A_LOW,PWR5_VSP_AVDD_UA_HIGH,PWR5_VSP_AVDD_UA_LOW);	
	TESTER_MAIN_DEBUG("VSN LIMIT = %d %d %d %d %d %d\r\n",PWR6_VSN_AVEE_HIGH,PWR6_VSN_AVEE_LOW,PWR6_VSN_AVEE_A_HIGH,PWR6_VSN_AVEE_A_LOW,PWR6_VSN_AVEE_UA_HIGH,PWR6_VSN_AVEE_UA_LOW);
	TESTER_MAIN_DEBUG("LED1 = %d %d %d %d %d %d\r\n",PWR7_LED1N_HIGH,PWR7_LED1N_LOW,PWR7_LED1N_A_HIGH,PWR7_LED1N_A_LOW,PWR7_LED1N_UA_HIGH,PWR7_LED1N_UA_LOW);
	TESTER_MAIN_DEBUG("LED2 = %d %d %d %d %d %d\r\n",PWR8_LED2N_HIGH,PWR8_LED2N_LOW,PWR8_LED2N_A_HIGH,PWR8_LED2N_A_LOW,PWR8_LED2N_UA_HIGH,PWR8_LED2N_UA_LOW);
	TESTER_MAIN_DEBUG("LED3 = %d %d %d %d %d %d\r\n",PWR9_LED3N_HIGH,PWR9_LED3N_LOW,PWR9_LED3N_A_HIGH,PWR9_LED3N_A_LOW,PWR9_LED3N_UA_HIGH,PWR9_LED3N_UA_LOW);
#endif
	
}





/*******************************************************************************
* 文件名	  	 : Info_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void Power_On_Handle(u8 chn,u8 val)
{
	u16 temp = 0;	
	switch(chn)		//电压通道
	{
		case 1:
			switch(val)
			{
				case POWER_CTRL_OFF:					
					ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_OFF);				
					TPS22993_IOVCC_ON_OFF(0);
					PWR1_IOVCC_On_Status = POWER_OFF;
					Iovcc_Power_Status = 0;
					
					ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF);
					TPS22993_EXT2_ON_OFF(0);
					PWR14_EXT_PWR_B_On_Status = POWER_OFF;										
					break;
				case POWER_CTRL_ON:							
					//TPS22993_Sel_Dev(0);	
					//TPS22993_On(TPS22993_CHN2);
					//TPS22993_IOVCC_ON_OFF(1);
					ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
					TPS22993_IOVCC_ON_OFF(1);
					PWR1_IOVCC_On_Status = POWER_ON;
					Iovcc_Power_Status = 1;	
					
					ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON); 
					TPS22993_EXT2_ON_OFF(1);
					PWR14_EXT_PWR_B_On_Status = POWER_ON;									
					break;
				case POWER_CTRL_PROTECT_ON:					
					break;
				case POWER_CTRL_PROTECT_OFF:					
					break;
			}
		break;
		
		case 2:
			switch(val)
			{
				case POWER_CTRL_OFF:					
					//ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_OFF); 
					//TPS22993_Sel_Dev(1);	
					//TPS22993_Off(TPS22993_CHN3);
					TPS22993_VDDH_ON_OFF(0);
					Lcdvdd_Power_Status = 0;
					PWR2_VDDH_On_Status = POWER_OFF;
					break;
				case POWER_CTRL_ON:				
					//ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON); 
					//TPS22993_Sel_Dev(1);	
					//TPS22993_On(TPS22993_CHN3);
					ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON);
					TPS22993_VDDH_ON_OFF(1);
					PWR2_VDDH_On_Status = POWER_ON;
					Lcdvdd_Power_Status = 1;				
					break;
				case POWER_CTRL_PROTECT_ON:
					
					break;
				case POWER_CTRL_PROTECT_OFF:
					
					break;
			}
		break;
		
		case 3:
			switch(val)
			{
				case POWER_CTRL_OFF:					 
					//ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_OFF); 
					//TPS22993_Sel_Dev(1);	
					//TPS22993_Off(TPS22993_CHN1);
					TPS22993_TPVDD_ON_OFF(0);
					PWR3_TP_VDD_On_Status = POWER_OFF;
					Tpvdd_Power_Status = 0;
					break;
				case POWER_CTRL_ON:					
					//ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON); 	
					//TPS22993_Sel_Dev(1);	
					//TPS22993_On(TPS22993_CHN1);
					ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON); 
					TPS22993_TPVDD_ON_OFF(1);
					PWR3_TP_VDD_On_Status = POWER_ON;
					Tpvdd_Power_Status = 1;					
					break;
				case POWER_CTRL_PROTECT_ON:					
					break;
				case POWER_CTRL_PROTECT_OFF:					
					break;
			}
		break;
		
		case 4:
			switch(val)
			{
				case POWER_CTRL_OFF:					
					//ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF); 
					//TPS22993_Sel_Dev(1);	
					//TPS22993_Off(TPS22993_CHN2);
					TPS22993_TPVIO_ON_OFF(0);
					PWR4_TP_VIO_On_Status = POWER_OFF;
					Tpvio_Power_Status = 0;
					break;
				case POWER_CTRL_ON:					
					//ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);
					//TPS22993_Sel_Dev(1);	
					//TPS22993_On(TPS22993_CHN2);
					ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);
					TPS22993_TPVIO_ON_OFF(1);
					PWR4_TP_VIO_On_Status = POWER_ON;
					Tpvio_Power_Status = 1;					
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 5:
			switch(val)
			{
				case POWER_CTRL_OFF:
					VSP_OFF();	
					

					if(Flag_Vsp_Vsn == 1)
					{
						//ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_OFF); 						
						TPS22993_VSP_VSN_ON_OFF(0);
					}
					Flag_Vsp_Vsn = 1;
					PWR5_VSP_AVDD_On_Status = POWER_OFF;
					Vsp_Power_Status = 0;
					break;
				case POWER_CTRL_ON:					
					//Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);					
					//ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 				
					TPS22993_VSP_VSN_ON_OFF(1);					
					VSP_ON();
					Delay_us(100);
					Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);	
					Vsp_Power_Status = 1;
					Flag_Vsp_Vsn = 0;	
					PWR5_VSP_AVDD_On_Status = POWER_ON;
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 6:
			switch(val)
			{
				case POWER_CTRL_OFF:
					VSN_OFF();					

					if(Flag_Vsp_Vsn == 1)
					{
						//ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_OFF); 
						TPS22993_VSP_VSN_ON_OFF(0);
					}
					Flag_Vsp_Vsn = 1;
					PWR6_VSN_AVEE_On_Status = POWER_OFF;
					Vsn_Power_Status = 0;
					
					break;
				case POWER_CTRL_ON:					 
					//Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);					
					//ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 
					TPS22993_VSP_VSN_ON_OFF(1);
					VSN_ON();
					Delay_us(100);
					Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);	
					PWR6_VSN_AVEE_On_Status = POWER_ON;
					Vsn_Power_Status = 1;
					Flag_Vsp_Vsn = 0;
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 7:
			switch(val)
			{
				case POWER_CTRL_OFF:
					//BL_LM36923_POWER_OFF;	//关背光主电源	
					LM36923_Off(BL_CHN1);		
					PWR7_LED1N_On_Status = POWER_OFF;	
					BL_ADJ_OFF();	
					//LM36923_AllOff();
					
		
					break;
				case POWER_CTRL_ON:				
					//BL_LM36923_POWER_ON;		//开背光主电源	
					//LM36923_PWR_ON_Cfg( PWR7_LED1N);		//重新设置背光电流
					
					PWR7_LED1N_On_Status = POWER_ON;
					BL_ADJ_ON();				
					//LM36923_AllOn();
					LM36923_On(BL_CHN1);					
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 8:
			switch(val)
			{
				case POWER_CTRL_OFF:
					//BL_LM36923_POWER_OFF;	//关背光主电源	
					LM36923_Off(BL_CHN2);
					PWR8_LED2N_On_Status = POWER_OFF;	
					
			
					
					break;
				case POWER_CTRL_ON:					
					//BL_LM36923_POWER_ON;		//开背光主电源
					//LM36923_PWR_ON_Cfg( PWR7_LED1N);		//重新设置背光电流
					
					PWR8_LED2N_On_Status = POWER_ON;
					
					LM36923_On(BL_CHN2);					
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 9:
			switch(val)
			{
				case POWER_CTRL_OFF:
					//BL_LM36923_POWER_OFF;	//关背光主电源	
					
					LM36923_Off(BL_CHN3);
					PWR9_LED3N_On_Status = POWER_OFF;
					
					
					break;
				case POWER_CTRL_ON:					
					//BL_LM36923_POWER_ON;		//开背光主电源	
					//LM36923_PWR_ON_Cfg( PWR7_LED1N);		//重新设置背光电流
					
					PWR9_LED3N_On_Status = POWER_ON;
					
					LM36923_On(BL_CHN3);				
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 10:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR10_VMTP_On_Status = POWER_OFF;
					 LT3582_Off();
					break;
				case POWER_CTRL_ON:
					 PWR10_VMTP_On_Status = POWER_ON;
					 LT3582_On();
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 11:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR11_VNEG_On_Status = POWER_OFF;
					 LT3582_Off();
					break;
				case POWER_CTRL_ON:
					 PWR11_VNEG_On_Status = POWER_ON;
					 LT3582_On();
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 12:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR12_LED_PWR_5V_On_Status = POWER_OFF;
					break;
				case POWER_CTRL_ON:
					 PWR12_LED_PWR_5V_On_Status = POWER_ON;
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 13:
			switch(val)
			{
				case POWER_CTRL_OFF:
					PWR13_EXT_PWR_A_On_Status = POWER_OFF;
					//ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF); 
					//ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_OFF); 
					TPS22993_TPVIO_ON_OFF(0);
					TPS22993_EXT1_ON_OFF(0);
					//TPS22993_Sel_Dev(1);	
					//TPS22993_Off(TPS22993_CHN4);
					break;
				case POWER_CTRL_ON:				
					PWR13_EXT_PWR_A_On_Status = POWER_ON;
					ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);
					ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON); 
					TPS22993_TPVIO_ON_OFF(1);
					TPS22993_EXT1_ON_OFF(1);
					//TPS22993_Sel_Dev(1);	
					//TPS22993_On(TPS22993_CHN4);				
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 14:
			/*
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR14_EXT_PWR_B_On_Status = POWER_OFF;
					ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF); 
					break;
				case POWER_CTRL_ON:				
					 PWR14_EXT_PWR_B_On_Status = POWER_ON;
					ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON); 					
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
			*/
		break;

		case 15:
			switch(val)
			{
				case POWER_CTRL_OFF:				
					PWR15_TESTER_VBUS_On_Status = POWER_OFF;
					ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_OFF);								 
					break;
				case POWER_CTRL_ON:					
					PWR15_TESTER_VBUS_On_Status = POWER_ON;
					ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON); 					
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
	}
	
}



/*******************************************************************************
* 文件名	  	 : MutiBuff_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static u8 MutiBuff_Handle(u32 add,u16 size)
{
	u8 *pbuf;	

	pbuf = G_DISP_FILE_BUFFER;		
	BinProctcoal_Handle(&pbuf[add],size);	
	
	TESTER_MAIN_DEBUG("Read %d bytes of data.\r\n", size);

	return(0);	
}




/*******************************************************************************
* 文件名	  	 : Initial_Code_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static void Initial_Code_Handle(void)
{
	u32 br = 0,btr = 0,j = 0;
	u8 *pbuf;
	
	uint16_t fpga_version;
	char fpga_version_char[11];

	Read_Start_Address(REG_GENERAL_INFO_ADD,&Info_Add,&Info_Add_Size);		//读取常规设置的地址和size	
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("info_add = 0x%x 0x%x\r\n", Info_Add, Info_Add_Size);	
	#endif	
	MutiBuff_Handle( Info_Add, Info_Add_Size);	
	PowerUp_Init_Handle();		//解析完了参数，然后上电初始化
	

	//ASH204A_Test(1,1);		// LCM2000 + FPGA, Need run ASH204A_Test(1,1), otherwise FPGA will not working

	
	///////////////////////////////////////////////////////////////////////////////	
	Read_Start_Address(REG_BOARD_SET_ADD,&Board_Init_Add,&Board_Init_Add_Size);		//读取board发送数据
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("board_add = 0x%x 0x%x\r\n", Board_Init_Add, Board_Init_Add_Size);
	#endif
	MutiBuff_Handle(Board_Init_Add, Board_Init_Add_Size);	


	///////////////////////////////////////////////////////////////////////////////	
	Read_Start_Address(REG_POWER_ON_ADD,&PowerOn_Add,&PowerOn_Add_Size);		//读取power on数据		
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("on_add = 0x%x 0x%x\r\n", PowerOn_Add, PowerOn_Add_Size);
	#endif


	///////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(REG_INIT_CODE_ADD,&Code_Init_Add,&Code_Init_Add_Size);		//读取lcd init数据	
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("init_add = 0x%x 0x%x\r\n", Code_Init_Add, Code_Init_Add_Size);
	#endif	


	///////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(REG_POWER_OFF_ADD,&PowerOff_Add,&PowerOff_Add_Size);		//读取power off数据		
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("off_add = 0x%x 0x%x\r\n", PowerOff_Add, PowerOff_Add_Size);
	#endif


	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(REG_SLEEP_SEQ_ADD,&Sleep_Seq_Add,&Sleep_Seq_Add_Size);		//读取休眠SLEEP数据		
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("sleep_add = 0x%x 0x%x\r\n", Sleep_Seq_Add, Sleep_Seq_Add_Size);
	#endif


	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(REG_INIT2_CODE_ADD,&Code_Init2_Add,&Code_Init2_Add_Size);		//读取用户定义数据		;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("init2_add = 0x%x 0x%x\r\n", Code_Init2_Add, Code_Init2_Add_Size);
	#endif
	
}


/*******************************************************************************
* 文件名	  	 : Bin_Option_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static void Bin_Option_Handle(u8 cmd,u16 val)
{
	switch(cmd)
	{	
		case 1:
			Flag_Current_En = val;		
			break;
		case 2:
			Flag_Voltage_En = val;		
			break;
		case 3:
			Flag_Sleep_Current_En = val;
			break;
		case 4:
			Flag_Hard_ID_En = (val&0xff00)>>8;
			Hard_ID_En_Val = (val&0x00ff);
			break;
		case 5:
			Flag_Soft_ID_En = val;
			break;
		case 6:
			Soft_ID12_Val = val;
			break;
		case 7:
			Soft_ID34_Val = val;
			break;
		case 8:
			Flag_OTP_Times_En = val;
			break;
		case 9:
			OTP_Times_High = (val&0xff00)>>8;
			OTP_Times_Low = (val&0x00ff);
			break;
		case 10:
			Flag_DuiWei_En = val;		
			break;
		case 11:
			Flag_PWM_En = val;
			break;
		case 12:
			PWM_Seq_Low = val;
			break;
		case 13:
			PWM_Seq_High= val;
			break;
		case 14:
			PWM_Duty_Low = val;
			break;
		case 15:
			PWM_Duty_High = val;
			break;
		case 16:
			PWM_Volt_Low = val;
			break;
		case 17:
			PWM_Volt_High = val;
			break;
		case 18:
			Flag_TE_En = val;
			break;
		case 19:
			TE_Seq_Low = val;
			break;
		case 20:
			TE_Seq_High= val;
			break;
		case 21:
			TE_Duty_Low = val;
			break;
		case 22:
			TE_Duty_High = val;
			break;
		case 23:
			TE_Volt_Low = val;
			break;
		case 24:
			TE_Volt_High = val;
			break;
		case 25:
			FLAG_BK_En = val;
			break;
	}
	
}



/*******************************************************************************
* 文件名	  	 : MutiBuff_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
int BinProctcoal_Handle(u8 *buf, u32 size)//Size 为4的整数倍
{
	u32 i = 0;
	u16 pDATA = 0;
	u8  pAdd = 0;
	u8  pic_index = 0;
	u8  name_arr[2];

	
	if(size%4)
	{
		TESTER_MAIN_DEBUG("File Size is incorrect.\r\n");	
		return -1;
	}
	
	
	for(i=0;i<size;i= i+4)
	{	
		 //Write data to FPGA FSMC
		 pDATA = buf[i+1]; 
		 pDATA = pDATA << 8;
		 pDATA |= buf[i];
		 //FPGA FSMC ADDRESS
		 pAdd = buf[i+2];

		 //Delay_us(10);			//必须加延时，否则屏不亮
		 
		//if(size ==  Init_Code_Size || size ==  Power_Off_Set_Size)
		//TESTER_MAIN_DEBUG("buf[%d]=%02x %02x %02x %02x\t\r\n",i,buf[i+0],buf[i+1],buf[i+2],buf[i+3]);

		#if 0
		if(size == 0x40)
		{
			TESTER_MAIN_DEBUG("i = %d\r\n",i);
		}
		#endif
		
		
		 switch (buf[i+3])
		 {		
			
			case REG_DELAY:			
		
				
				if(buf[i+2] == 0) //ms
				{
					Delay_ms((u32)(pDATA));
				}
				else if(buf[i+2] == 1) //10us
				{
					Delay_us((u32)(pDATA / 10)); //(10us*pDATA)/10us
				}
				else
				{
					return -1;
				}

				break;
			//////////////////////////////////////////////////////////161123 新添加部分
			case REG_RESOLUTION:		//分辨率获取	
			  if(pAdd == 6)		//
				{
					//DDIC = pDATA;
				}
				break;				

			case REG_SET_VOLTAGE:	     //电压参数设置
				BinSetVoltVal_Handle(pAdd,pDATA);
				break;		

			case REG_VOLTAGE_HIGH:	     //电压参数设置
				BinSetVoltHigh_Handle(pAdd,pDATA);
				break;	

			case REG_VOLTAGE_LOW:	     //电压参数设置
				BinSetVoltLow_Handle(pAdd,pDATA);
				break;

			case REG_CURRENT_HIGH:	     //电压参数设置
				BinSetCurrHigh_Handle(pAdd,pDATA);
				break;

			case REG_CURRENT_LOW:	     //电压参数设置
				BinSetCurrLow_Handle(pAdd,pDATA);
				break;

			case REG_PATTERN_CTRL:				
				break;		

			case REG_SLEEP_SCAN:						
				break;
			
			case REG_PATTERN_RGB:				
				break;	

			case REG_POWER_SEQ:					
				Power_On_Handle(pAdd,pDATA);
				break;	
				
			case REG_SHOW_PIC:						
				break;		

			case REG_OPTION:			
				//选项功能配置
				Bin_Option_Handle(pAdd,pDATA);
				break;	
			
			default:
				break;
			
		}				
	}	
	
	return 0;
}



/*******************************************************************************
* 文件名	  	 : Load_Initial_Code1
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void Load_Initial_Code(void)
{
	u32 i = 0,len = 0,len_rec = 0;	
	u8 temp[4] = {0,0,0,0};

	USR_LED_ON_OFF(0);

	
	while(1)
	{			
		len = ReadUart(USART_PORT_COM1,temp,1);
		if(len > 0)
		{
			G_DISP_FILE_BUFFER[i] = temp[0];		
			i++;

			if(i >= 5)
			{
				if((G_DISP_FILE_BUFFER[i-5] == 0x80) && (G_DISP_FILE_BUFFER[i-4] == 0x81) && (G_DISP_FILE_BUFFER[i-3] == 0x82))
				{
					len_rec = (G_DISP_FILE_BUFFER[i-2] << 8) + G_DISP_FILE_BUFFER[i-1] + 5;
					if(len_rec == i)
					{			
						//TESTER_MAIN_DEBUG("received over\r\n");						
						//TESTER_MAIN_DEBUG("last arr[]= %2x %2x\r\n",G_DISP_FILE_BUFFER[i-2],G_DISP_FILE_BUFFER[i-1]);

						Initial_Code_Handle();			//解析完了参数，然后上电初始化

						USR_LED_ON_OFF(1);

						TIM2_PWM_IN_Configuration();

						//FPGA_Key_On();
						//Delay_ms(1000);
						//FPGA_Key_Long_On();
						break;
					}
				}
			}


			if((G_DISP_FILE_BUFFER[0] == 0x80) && (G_DISP_FILE_BUFFER[1] == MACHINE_ADDR) && (G_DISP_FILE_BUFFER[2] == 0x01)&& (G_DISP_FILE_BUFFER[3] == 0x42)&& (G_DISP_FILE_BUFFER[4] == INIT_STATUS_HARD_RESTET))
			{
				SystemReset();	
			}
		}

		Send_LoadBIN_Res(0); 		//主板询问返回未完成
		
	}		
		
}




/*******************************************************************************
* 文件名	     : Get_Volt_Val
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/

u16 Get_Volt_Val(POWER_CHANNEL_INDEX_TypeDef index)
{
	u16 temp = 0;
	switch(index)
	{
		case PWR1_IOVCC_INDEX:
			temp =  PWR1_IOVCC;
			break;
		case PWR2_VDDH_INDEX:
			temp =  PWR2_VDDH;
			break;
		case PWR3_TP_VDD_INDEX:
			temp =  PWR3_TP_VDD;
			break;
		case PWR4_TP_VIO_INDEX:
			temp =  PWR4_TP_VIO;
			break;
		case PWR5_VSP_AVDD_INDEX:
			temp =  PWR5_VSP_AVDD;
			break;
		case PWR6_VSN_AVEE_INDEX:
			temp =  PWR6_VSN_AVEE;
			break;
		case PWR7_LED1N_INDEX:
			temp =  PWR7_LED1N;
			break;
		case PWR8_LED2N_INDEX:
			temp =  PWR8_LED2N;
			break;
		case PWR9_LED3N_INDEX:
			temp =  PWR9_LED3N;
			break;
		case PWR10_VMTP_INDEX:
			temp =  PWR10_VMTP;
			break;
		case PWR11_VNEG_INDEX:
			temp =  PWR11_VNEG;
			break;
		case PWR12_LED_PWR_5V_INDEX:
			temp =  PWR12_LED_PWR_5V;
			break;
		case PWR13_EXT_PWR_A_INDEX:
			temp =  PWR13_EXT_PWR_A;
			break;
		case PWR14_EXT_PWR_B_INDEX:
			//temp =  PWR14_EXT_PWR_B;
			temp =  PWR1_IOVCC;
			break;
		case PWR15_TESTER_VBUS_INDEX:
			temp =  PWR15_TESTER_VBUS;
			break;
	}

	return(temp);
}




/*******************************************************************************
* 文件名	     : FPGA_Key_On
* 描述	         : 点屏键控制流程
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void  FPGA_Key_On(void)
{
	u16 temp = 0;
	u16 j;

	//MutiBuff_Handle(Info_Add,Info_Add_Size);					//重现刷新FPGA
	//MutiBuff_Handle(Board_Init_Add, Board_Init_Add_Size);	
	//RELAY_UA_ALL_ON_OFF(0);
	//Delay_ms(50);
	
	MutiBuff_Handle(PowerOn_Add,PowerOn_Add_Size);				//执行上电程序
	Delay_ms(500);		//否则背光会先亮
	MutiBuff_Handle(Code_Init_Add,Code_Init_Add_Size);			//发送lcd_init代码

	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("pwr1sta=%d %d %d %d %d\t\r\n", PWR1_IOVCC_On_Status, PWR2_VDDH_On_Status, PWR3_TP_VDD_On_Status, PWR4_TP_VIO_On_Status, PWR5_VSP_AVDD_On_Status);
	TESTER_MAIN_DEBUG("pwr6sta=%d %d %d %d %d\t\r\n", PWR6_VSN_AVEE_On_Status, PWR7_LED1N_On_Status, PWR8_LED2N_On_Status, PWR9_LED3N_On_Status, PWR10_VMTP_On_Status);
	TESTER_MAIN_DEBUG("pwr11sta=%d %d %d %d %d\t\r\n", PWR11_VNEG_On_Status, PWR12_LED_PWR_5V_On_Status, PWR13_EXT_PWR_A_On_Status, PWR14_EXT_PWR_B_On_Status, PWR15_TESTER_VBUS_On_Status);
	#endif

	Option_Test();							//测试读出来的配置是否正确
	Set_Sleep_Status(0);					//取消休眠
	Set_Power_Status(1);					//0---关机状态，1---开机状态
}




void  Special_Init2_On(void)
{
	u16 temp = 0;

	MutiBuff_Handle(PowerOn_Add,PowerOn_Add_Size);				//执行上电程序	
	Delay_ms(50);
	MutiBuff_Handle(Code_Init2_Add,Code_Init2_Add_Size);		//发送lcd_init代码

	Set_Sleep_Status(0);										//取消休眠
	Set_Power_Status(1);										//0---关机状态，1---开机状态
}




void  FPGA_Key_Long_On(void) 
{	
	MutiBuff_Handle(Sleep_Seq_Add,Sleep_Seq_Add_Size);
	Set_Sleep_Status(1);   		//进入休眠模式
	Set_Power_Status(0);		//设置为关机状态
}



/*******************************************************************************
* 文件名	     : FPGA_Key_Off
* 描述	         : 灭亮键控制流程
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void  FPGA_Key_Off(void)
{		
	TIM_Cmd(TIM2, DISABLE);
	//Set_PowerSeq_Index(0);
	//MutiBuff_Handle( Power_Off_Set_Add, Power_Off_Set_Size);	
	TESTER_MAIN_DEBUG("\r\npower off ing,add = %x size = %x\r\n",PowerOff_Add,PowerOff_Add_Size);	
	MutiBuff_Handle(PowerOff_Add,PowerOff_Add_Size);	
	

	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("pwr1sta=%d %d %d %d %d\t\r\n", PWR1_IOVCC_On_Status, PWR2_VDDH_On_Status, PWR3_TP_VDD_On_Status, PWR4_TP_VIO_On_Status, PWR5_VSP_AVDD_On_Status);
	TESTER_MAIN_DEBUG("pwr6sta=%d %d %d %d %d\t\r\n", PWR6_VSN_AVEE_On_Status, PWR7_LED1N_On_Status, PWR8_LED2N_On_Status, PWR9_LED3N_On_Status, PWR10_VMTP_On_Status);
	TESTER_MAIN_DEBUG("pwr11sta=%d %d %d %d %d\t\r\n", PWR11_VNEG_On_Status, PWR12_LED_PWR_5V_On_Status, PWR13_EXT_PWR_A_On_Status, PWR14_EXT_PWR_B_On_Status, PWR15_TESTER_VBUS_On_Status);
	#endif
	


	Set_Sleep_Status(0);			//取消休眠
	Set_Power_Status(0);			//0---关机状态，1---开机状态
}


/*******************************************************************************
* 文件名	     : FPGA_Sleep_Off
* 描述	         : 睡眠时候的数据发送
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void  FPGA_Sleep_Off(void)
{
	//BL_POWER_OFF();				//背光在校准的时候必须要关闭
	MutiBuff_Handle(PowerOn_Add,PowerOn_Add_Size);	
	//BL_POWER_ON();
	//LM36923_PWR_ON_Cfg(PWR7_LED1N);
}





/*******************************************************************************
* 文件名	     : System_Power_On_All
* 描述	         : 电源全部打开
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void System_Power_On_All(void)
{
	;					
}




/*******************************************************************************
* 文件名	     : System_Power_On_All
* 描述	         : 电源全部关闭
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void System_Power_Off_All(void)
{
	u16 temp;

	//ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_OFF);	
	TPS22993_IOVCC_ON_OFF(0);
	Iovcc_Power_Status = 0;


	//ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_OFF); 
	TPS22993_VDDH_ON_OFF(0);
	Lcdvdd_Power_Status = 0;

	//ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_OFF); 
	TPS22993_TPVDD_ON_OFF(0);
	Tpvdd_Power_Status = 0;
					

	//ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF); 
	TPS22993_TPVIO_ON_OFF(0);
	Tpvio_Power_Status = 0;
						

	VSP_OFF();	
	Vsp_Power_Status = 0;
					

	VSN_OFF();
	Vsn_Power_Status = 0;	


	BL_ADJ_OFF();	
	LM36923_AllOff();


	//ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_OFF); 
	TPS22993_EXT1_ON_OFF(0);

	LT3582_Off();


	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF); 		//tester vbus

	//ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_OFF);
	TPS22993_EXT2_ON_OFF(0);

	PWR1_IOVCC_On_Status = POWER_OFF;
	PWR2_VDDH_On_Status = POWER_OFF;
	PWR3_TP_VDD_On_Status = POWER_OFF;
	PWR4_TP_VIO_On_Status = POWER_OFF;
	PWR5_VSP_AVDD_On_Status = POWER_OFF;
	PWR6_VSN_AVEE_On_Status = POWER_OFF;
	PWR7_LED1N_On_Status = POWER_OFF;
	PWR8_LED2N_On_Status = POWER_OFF;
	PWR9_LED3N_On_Status = POWER_OFF;
	PWR10_VMTP_On_Status = POWER_OFF;
	PWR11_VNEG_On_Status = POWER_OFF;
	PWR12_LED_PWR_5V_On_Status = POWER_OFF;
	PWR13_EXT_PWR_A_On_Status = POWER_OFF;
	PWR14_EXT_PWR_B_On_Status = POWER_OFF;
	PWR15_TESTER_VBUS_On_Status = POWER_OFF;
}










u8 Get_Detect_Power_Status(DETECT_POWER_TypeDef detect)
{
	switch(detect)
	{
		case DETECT_POWER_IOVCC:
			return(Iovcc_Power_Status);
			break;
		case DETECT_POWER_LCDVDD:
			return(Lcdvdd_Power_Status);
			break;
		case DETECT_POWER_TPVIO:
			return(Tpvio_Power_Status);
			break;
		case DETECT_POWER_TPVDD:
			return(Tpvdd_Power_Status);
			break;
		case DETECT_POWER_VSP:
			return(Vsp_Power_Status);
			break;
		case DETECT_POWER_VSN:
			return(Vsn_Power_Status);
			break;
		case DETECT_POWER_LEDN1:
			return(PWR7_LED1N_On_Status);
			break;
		case DETECT_POWER_LEDN2:
			return(PWR8_LED2N_On_Status);
			break;
		case DETECT_POWER_LEDN3:
			return(PWR9_LED3N_On_Status);
			break;
	}
	
}







void Get_Option_Para(OPTION_INDEX_TypeDef option,u16 *val)
{
	switch(option)
	{
		case CURRENT_EN_INDEX:
			*val = Flag_Current_En;
			break;
		case VOLTAGE_EN_INDEX:
			*val = Flag_Voltage_En;
			break;
		case SLEEP_CURRENT_EN_INDEX:
			*val = Flag_Sleep_Current_En;
			break;
		case HARD_ID_EN_INDEX:
			*val = Flag_Hard_ID_En;
			break;
		case HARD_ID_VAL_INDEX:
			*val = Hard_ID_En_Val;
			break;
		case SOFT_ID_EN_INDEX:
			*val = Flag_Soft_ID_En;
			break;
		case SOFT_ID12_VAL_INDEX:
			*val = Soft_ID12_Val;
			break;
		case SOFT_ID34_VAL_INDEX:
			*val = Soft_ID34_Val;
			break;
		case OTP_EN_INDEX:
			*val = Flag_OTP_Times_En;
			break;
		case OTP_HIGH_INDEX:
			*val = OTP_Times_High;
			break;
		case OTP_LOW_INDEX:
			*val = OTP_Times_Low;
			break;
		case DUIWEI_EN_INDEX:
			*val = Flag_DuiWei_En;
			break;
		case PWM_EN_INDEX:
			*val = Flag_PWM_En;
			break;
		case PWM_SEQ_HIGH_INDEX:
			*val = PWM_Seq_High;
			break;
		case PWM_SEQ_LOW_INDEX:
			*val = PWM_Seq_Low;
			break;
		case PWM_DUTY_HIGH_INDEX:
			*val = PWM_Duty_High;
			break;
		case PWM_DUTY_LOW_INDEX:
			*val = PWM_Duty_Low;
			break;
		case PWM_VOLT_HIGH_INDEX:
			*val = PWM_Volt_High;
			break;
		case PWM_VOLT_LOW_INDEX:
			*val = PWM_Volt_Low;
			break;
		case TE_EN_INDEX:
			*val = Flag_TE_En;
			break;
		case TE_SEQ_HIGH_INDEX:
			*val = TE_Seq_High;
			break;
		case TE_SEQ_LOW_INDEX:
			*val = TE_Seq_Low;
			break;
		case TE_DUTY_HIGH_INDEX:
			*val = TE_Duty_High;
			break;
		case TE_DUTY_LOW_INDEX:
			*val = TE_Duty_Low;
			break;
		case TE_VOLT_HIGH_INDEX:
			*val = TE_Volt_High;
			break;
		case TE_VOLT_LOW_INDEX:
			*val = TE_Volt_Low;
			break;
		case BK_EN_INDEX:
			*val = FLAG_BK_En;
			break;	
		case PWR1_IOVCC_V_HIGH_INDEX:
			*val = PWR1_IOVCC_HIGH;
			break;
		case PWR1_IOVCC_V_LOW_INDEX:
			*val = PWR1_IOVCC_LOW;
			break;
		case PWR1_IOVCC_A_HIGH_INDEX:
			*val = PWR1_IOVCC_A_HIGH;
			break;
		case PWR1_IOVCC_A_LOW_INDEX:
			*val = PWR1_IOVCC_A_LOW;
			break;
		case PWR1_IOVCC_UA_HIGH_INDEX:
			*val = PWR1_IOVCC_UA_HIGH;
			break;
		case PWR1_IOVCC_UA_LOW_INDEX:
			*val = PWR1_IOVCC_UA_LOW;
			break;
		case PWR2_VDDH_V_HIGH_INDEX:
			*val = PWR2_VDDH_HIGH;
			break;
		case PWR2_VDDH_V_LOW_INDEX:
			*val = PWR2_VDDH_LOW;
			break;
		case PWR2_VDDH_A_HIGH_INDEX:
			*val = PWR2_VDDH_A_HIGH;
			break;
		case PWR2_VDDH_A_LOW_INDEX:
			*val = PWR2_VDDH_A_LOW;
			break;
		case PWR2_VDDH_UA_HIGH_INDEX:
			*val = PWR2_VDDH_UA_HIGH;
			break;
		case PWR2_VDDH_UA_LOW_INDEX:
			*val = PWR2_VDDH_UA_LOW;
			break;
		case PWR3_TP_VDD_V_HIGH_INDEX:
			*val = PWR3_TP_VDD_HIGH;
			break;
		case PWR3_TP_VDD_V_LOW_INDEX:
			*val = PWR3_TP_VDD_LOW;
			break;
		case PWR3_TP_VDD_A_HIGH_INDEX:
			*val = PWR3_TP_VDD_A_HIGH;
			break;
		case PWR3_TP_VDD_A_LOW_INDEX:
			*val = PWR3_TP_VDD_A_LOW;
			break;
		case PWR3_TP_VDD_UA_HIGH_INDEX:
			*val = PWR3_TP_VDD_UA_HIGH;
			break;
		case PWR3_TP_VDD_UA_LOW_INDEX:
			*val = PWR3_TP_VDD_UA_LOW;
			break;
		case PWR4_TP_VIO_V_HIGH_INDEX:
			*val = PWR4_TP_VIO_HIGH;
			break;
		case PWR4_TP_VIO_V_LOW_INDEX:
			*val = PWR4_TP_VIO_LOW;
			break;
		case PWR4_TP_VIO_A_HIGH_INDEX:
			*val = PWR4_TP_VIO_A_HIGH;
			break;
		case PWR4_TP_VIO_A_LOW_INDEX:
			*val = PWR4_TP_VIO_A_LOW;
			break;
		case PWR4_TP_VIO_UA_HIGH_INDEX:
			*val = PWR4_TP_VIO_UA_HIGH;
			break;
		case PWR4_TP_VIO_UA_LOW_INDEX:
			*val = PWR4_TP_VIO_UA_LOW;
			break;
		case PWR5_VSP_AVDD_V_HIGH_INDEX:
			*val = PWR5_VSP_AVDD_HIGH;
			break;
		case PWR5_VSP_AVDD_V_LOW_INDEX:
			*val = PWR5_VSP_AVDD_LOW;
			break;
		case PWR5_VSP_AVDD_A_HIGH_INDEX:
			*val = PWR5_VSP_AVDD_A_HIGH;
			break;
		case PWR5_VSP_AVDD_A_LOW_INDEX:
			*val = PWR5_VSP_AVDD_A_LOW;
			break;
		case PWR5_VSP_AVDD_UA_HIGH_INDEX:
			*val = PWR5_VSP_AVDD_UA_HIGH;
			break;
		case PWR5_VSP_AVDD_UA_LOW_INDEX:
			*val = PWR5_VSP_AVDD_UA_LOW;
			break;
		case PWR6_VSN_AVEE_V_HIGH_INDEX:
			*val = PWR6_VSN_AVEE_HIGH;
			break;
		case PWR6_VSN_AVEE_V_LOW_INDEX:
			*val = PWR6_VSN_AVEE_LOW;
			break;
		case PWR6_VSN_AVEE_A_HIGH_INDEX:
			*val = PWR6_VSN_AVEE_A_HIGH;
			break;
		case PWR6_VSN_AVEE_A_LOW_INDEX:
			*val = PWR6_VSN_AVEE_A_LOW;
			break;
		case PWR6_VSN_AVEE_UA_HIGH_INDEX:
			*val = PWR6_VSN_AVEE_UA_HIGH;
			break;
		case PWR6_VSN_AVEE_UA_LOW_INDEX:
			*val = PWR6_VSN_AVEE_UA_LOW;
			break;
		case PWR7_LED1N_V_HIGH_INDEX:
			*val = PWR7_LED1N_HIGH;
			break;
		case PWR7_LED1N_V_LOW_INDEX:
			*val = PWR7_LED1N_LOW;
			break;
		case PWR7_LED1N_A_HIGH_INDEX:
			*val = PWR7_LED1N_A_HIGH;
			break;
		case PWR7_LED1N_A_LOW_INDEX:
			*val = PWR7_LED1N_A_LOW;
			break;
		case PWR7_LED1N_UA_HIGH_INDEX:
			*val = PWR7_LED1N_UA_HIGH;
			break;
		case PWR7_LED1N_UA_LOW_INDEX:
			*val = PWR7_LED1N_UA_LOW;
			break;
		case PWR8_LED2N_V_HIGH_INDEX:
			*val = PWR8_LED2N_HIGH;
			break;
		case PWR8_LED2N_V_LOW_INDEX:
			*val = PWR8_LED2N_LOW;
			break;
		case PWR8_LED2N_A_HIGH_INDEX:
			*val = PWR8_LED2N_A_HIGH;
			break;
		case PWR8_LED2N_A_LOW_INDEX:
			*val = PWR8_LED2N_A_LOW;
			break;
		case PWR8_LED2N_UA_HIGH_INDEX:
			*val = PWR8_LED2N_UA_HIGH;
			break;
		case PWR8_LED2N_UA_LOW_INDEX:
			*val = PWR8_LED2N_UA_LOW;
			break;
		case PWR9_LED3N_V_HIGH_INDEX:
			*val = PWR9_LED3N_HIGH;
			break;
		case PWR9_LED3N_V_LOW_INDEX:
			*val = PWR9_LED3N_LOW;
			break;
		case PWR9_LED3N_A_HIGH_INDEX:
			*val = PWR9_LED3N_A_HIGH;
			break;
		case PWR9_LED3N_A_LOW_INDEX:
			*val = PWR9_LED3N_A_LOW;
			break;
		case PWR9_LED3N_UA_HIGH_INDEX:
			*val = PWR9_LED3N_UA_HIGH;
			break;
		case PWR9_LED3N_UA_LOW_INDEX:
			*val = PWR9_LED3N_UA_LOW;
			break;
	}
}



void Get_Bin_Version(char **s)
{
	u8 temp[2] = 0;
	
	//TESTER_MAIN_DEBUG("ver = %d\r\n",ParaManage.Version);
#if 0		//是否和版本号一起显示
	temp[0] = ParaManage.Version + 0x30;
	strcat(ParaManage.Bin_Name,temp);
#endif
	*s = ParaManage.Bin_Name;
}



void Option_Test(void)
{
	u16 temp = 0;
	//char temp_arr[50];
	char * temp_arr;
	
#if DEBUG_BIN_CFG
	Get_Bin_Version(&temp_arr);
	TESTER_MAIN_DEBUG("Bin version = %s\r\n",temp_arr);
	Get_Option_Para(CURRENT_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("current en = %d\r\n",Flag_Current_En);
	Get_Option_Para(VOLTAGE_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("voltage en = %d\r\n",temp);
	Get_Option_Para(SLEEP_CURRENT_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("sleep en = %d\r\n",temp);
	Get_Option_Para(HARD_ID_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("hard id en = %d\r\n",temp);
	Get_Option_Para(HARD_ID_VAL_INDEX,&temp);
	TESTER_MAIN_DEBUG("hard id val = %d\r\n",temp);
	Get_Option_Para(SOFT_ID_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("soft id en = %d\r\n",temp);
	Get_Option_Para(SOFT_ID12_VAL_INDEX,&temp);
	TESTER_MAIN_DEBUG("soft id12 val = %d\r\n",temp);
	Get_Option_Para(SOFT_ID34_VAL_INDEX,&temp);
	TESTER_MAIN_DEBUG("soft id34 val = %d\r\n",temp);
	Get_Option_Para(OTP_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("opt en = %d\r\n",temp);
	Get_Option_Para(OTP_HIGH_INDEX,&temp);
	TESTER_MAIN_DEBUG("opt high = %d\r\n",temp);
	Get_Option_Para(OTP_LOW_INDEX,&temp);
	TESTER_MAIN_DEBUG("opt low = %d\r\n",temp);
	Get_Option_Para(DUIWEI_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("duiwei en = %d\r\n",temp);
	Get_Option_Para(PWM_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("pwm en = %d\r\n",temp);
	Get_Option_Para(PWM_SEQ_HIGH_INDEX,&temp);
	TESTER_MAIN_DEBUG("pwm seq high = %d\r\n",temp);
	Get_Option_Para(PWM_SEQ_LOW_INDEX,&temp);
	TESTER_MAIN_DEBUG("pwm seq low = %d\r\n",temp);
	Get_Option_Para(PWM_DUTY_HIGH_INDEX,&temp);
	TESTER_MAIN_DEBUG("pwm duty high = %d\r\n",temp);
	Get_Option_Para(PWM_DUTY_LOW_INDEX,&temp);
	TESTER_MAIN_DEBUG("pwm duty low = %d\r\n",temp);
	Get_Option_Para(PWM_VOLT_HIGH_INDEX,&temp);
	TESTER_MAIN_DEBUG("pwm volt high = %d\r\n",temp);
	Get_Option_Para(PWM_VOLT_LOW_INDEX,&temp);
	TESTER_MAIN_DEBUG("pwm volt low = %d\r\n",temp);
	Get_Option_Para(TE_EN_INDEX,&temp);
	TESTER_MAIN_DEBUG("te en = %d\r\n",temp);
	Get_Option_Para(TE_SEQ_HIGH_INDEX,&temp);
	TESTER_MAIN_DEBUG("te seq high = %d\r\n",temp);
	Get_Option_Para(TE_SEQ_LOW_INDEX,&temp);
	TESTER_MAIN_DEBUG("te seq low = %d\r\n",temp);
	Get_Option_Para(TE_DUTY_HIGH_INDEX,&temp);
	TESTER_MAIN_DEBUG("te duty high = %d\r\n",temp);
	Get_Option_Para(TE_DUTY_LOW_INDEX,&temp);
	TESTER_MAIN_DEBUG("te duty low = %d\r\n",temp);
	Get_Option_Para(TE_VOLT_HIGH_INDEX,&temp);
	TESTER_MAIN_DEBUG("te volt high = %d\r\n",temp);
	Get_Option_Para(TE_VOLT_LOW_INDEX,&temp);
	TESTER_MAIN_DEBUG("te volt low = %d\r\n",temp);
#endif

}


u8 Get_Sleep_Status(void)
{
	return(Sleep_Status);
}


void Set_Sleep_Status(u8 val)
{
	Sleep_Status = val;
}

u8 Get_Power_Status(void)
{
	return(Power_Status);
}


void Set_Power_Status(u8 val)
{
	Power_Status = val;
}


/*******************************************************************************
* 文件名	     : Big_Current_Dect
* 描述	         : 大电流检测
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Big_Current_Dect(void)
{
	u8 err = 0;

	// PWR1_IOVCC_A_HIGH = 1001;
	
	if(Lcd_Iovcc_A_High > PWR1_IOVCC_A_DECT_HIGH)
	{
		err = 1;
	}

	if(Lcd_Vddh_A_High > PWR2_VDDH_A_DECT_HIGH)
	{
		err = 1;		
	}

	if(Tp_Vio_A_High > PWR3_TP_VDD_A_DECT_HIGH)
	{
		err = 1;	
	}

	if(Tp_Vdd_A_High > PWR4_TP_VIO_A_DECT_HIGH)
	{
		err = 1;	
	}

	if(Vsp_A_High> PWR5_VSP_AVDD_A_DECT_HIGH)
	{
		err = 1;	
	}

	if(Vsn_A_High > PWR6_VSN_AVEE_A_DECT_HIGH)
	{
		err = 1;	
	}


	if(err)		//大电流被检测到了
	{
		Flag_Big_Current_Err = 1;
	}
	else
	{
		Flag_Big_Current_Err = 0;
	}
}



u8 Get_Big_Current_Err(void)
{
	return(Flag_Big_Current_Err);
}






