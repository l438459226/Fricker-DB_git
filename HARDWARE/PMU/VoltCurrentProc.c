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


//上电和采样电压电流处理流程



/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "VoltCurrentProc.h"
#include "INA226.h"
#include "ad715.h"
#include "LoadConfig.h"
#include "GPIO_Config.h"
#include "tester_debug.h"





static int	Vsp_Leak_Currrent = 0;
static int	Vsn_Leak_Currrent = 0;
static int	Iovcc_Leak_Currrent = 0;
static int	Vddh_Leak_Currrent = 0;
static u8	Flag_Vsp_Leak_Currrent = 1;
static u8	Flag_Vsn_Leak_Currrent = 1;
static u8	Flag_Iovcc_Leak_Currrent = 1;
static u8	Flag_Vddh_Leak_Currrent = 1;

static u16 	Cnt_Ma = 0;
static u16	Cnt_Ua = 0;
static u8 	flag_1time_ma = 0;
static u8 	flag_1time_ua = 0;



///////////////////////////////////////////
//测量出来的电压值
static u16 IOVCC_V_Alarm_Val = 0;
static u16 VDDH_V_Alarm_Val = 0;
static u16 TPVIO_V_Alarm_Val = 0;
static u16 TPVCC_V_Alarm_Val = 0;
static u16 VSP_V_Alarm_Val = 0;
static u16 VSN_V_Alarm_Val = 0;
static u16 VSP_Sleep_V_Alarm_Val = 0;
static u16 VSN_Sleep_V_Alarm_Val = 0;
static u16 IOVCC_Sleep_V_Alarm_Val = 0;
static u16 VDDH_Sleep_V_Alarm_Val = 0;


static u16 LEDP_V_Alarm_Val = 0;
static u16 LEDN1_V_Alarm_Val = 0;
static u16 LEDN2_V_Alarm_Val = 0;
static u16 LEDN3_V_Alarm_Val = 0;



//测量出来的电流值
static u16 IOVCC_A_Alarm_Val = 0;
static u16 VDDH_A_Alarm_Val = 0;
static u16 TPVIO_A_Alarm_Val = 0;
static u16 TPVCC_A_Alarm_Val = 0;
static u16 VSP_A_Alarm_Val = 0;
static u16 VSN_A_Alarm_Val = 0;
static u16 VSP_Sleep_A_Alarm_Val = 0;
static u16 VSN_Sleep_A_Alarm_Val = 0;
static u16 IOVCC_Sleep_A_Alarm_Val = 0;
static u16 VDDH_Sleep_A_Alarm_Val = 0;

static u16 LEDP_A_Alarm_Val = 0;
static u16 LEDN1_A_Alarm_Val = 0;
static u16 LEDN2_A_Alarm_Val = 0;
static u16 LEDN3_A_Alarm_Val = 0;



static u8 Flag_V_Alarm = 0;
static u8 Flag_A_Alarm = 0;
static u8 V_A_Detect_Over = 0;		//电流电压一轮测过之后
static u8 V_UA_Detect_Over = 0;
static u8 BL_V_A_Detect_Over = 0;	
static u8 Flag_LED_Start = 0;		//背光检测标志
static u8 Flag_Sys_Ready = 0;		//系统检测ready标志


/////////////////////////////////////////////////////////////
static u8 Flag_UA_err = 0;			//0---无效  1---ok  2---err
static u8 Flag_V_err = 0;			//0---无效  1---ok  2---err
static u8 Flag_A_err = 0;			//0---无效  1---ok  2---err






void Set_V_Alarm_Flag(u8 val)
{
	Flag_V_Alarm = val;
}


void Set_A_Alarm_Flag(u8 val)
{
	Flag_A_Alarm = val;
}



u8 Get_A_Alarm_Flag(void)
{
	return(Flag_A_Alarm);
}

u8 Get_V_Alarm_Flag(void)
{
	return(Flag_V_Alarm);
}

u8 Get_V_A_Detect_Over_Flag(void)
{
	return(V_A_Detect_Over);
}

void Set_V_A_Detect_Over_Flag(u8 val)
{
	V_A_Detect_Over = val;
}

u8 Get_V_UA_Detect_Over_Flag(void)
{
	return(V_UA_Detect_Over);
}

void Set_V_UA_Detect_Over_Flag(u8 val)
{
	V_UA_Detect_Over = val;
}




void Set_V_Alarm_Val(ALARM_V_INDEX_TypeDef v_index,u16 val)
{
	switch(v_index)
	{
		case IOVCC_ALARM_V_INDEX:
			IOVCC_V_Alarm_Val = val;
			break;
		case VDDH_ALARM_V_INDEX:
			VDDH_V_Alarm_Val = val;
			break;
		case TPVIO_ALARM_V_INDEX:
			TPVIO_V_Alarm_Val = val;
			break;
		case TPVCC_ALARM_V_INDEX:
			TPVCC_V_Alarm_Val = val;
			break;
		case VSP_ALARM_V_INDEX:
			VSP_V_Alarm_Val = val;
			break;
		case VSN_ALARM_V_INDEX:
			VSN_V_Alarm_Val = val;
			break;
		case VSP_ALARM_SLEEP_V_INDEX:
			VSP_Sleep_V_Alarm_Val = val;
			break;
		case VSN_ALARM_SLEEP_V_INDEX:
			VSN_Sleep_V_Alarm_Val = val;
			break;
		case IOVCC_ALARM_SLEEP_V_INDEX:
			IOVCC_Sleep_V_Alarm_Val = val;
			break;
		case VDDH_ALARM_SLEEP_V_INDEX:
			VDDH_Sleep_V_Alarm_Val = val;
			break;
	}
}



u16 Get_V_Alarm_Val(ALARM_V_INDEX_TypeDef v_index)
{
	switch(v_index)
	{
		case IOVCC_ALARM_V_INDEX:
			return(IOVCC_V_Alarm_Val);
			break;
		case VDDH_ALARM_V_INDEX:
			return(VDDH_V_Alarm_Val);			
			break;
		case TPVIO_ALARM_V_INDEX:
			return(TPVIO_V_Alarm_Val);				
			break;
		case TPVCC_ALARM_V_INDEX:
			return(TPVCC_V_Alarm_Val);			
			break;
		case VSP_ALARM_V_INDEX:
			return(VSP_V_Alarm_Val);				
			break;
		case VSN_ALARM_V_INDEX:
			return(VSN_V_Alarm_Val);			
			break;
		case VSP_ALARM_SLEEP_V_INDEX:
			return(VSP_Sleep_V_Alarm_Val);
			break;
		case VSN_ALARM_SLEEP_V_INDEX:
			return(VSN_Sleep_V_Alarm_Val);			
			break;
		case IOVCC_ALARM_SLEEP_V_INDEX:
			return(IOVCC_Sleep_V_Alarm_Val);				
			break;
		case VDDH_ALARM_SLEEP_V_INDEX:
			return(VDDH_Sleep_V_Alarm_Val);			
			break;
	}
}



u16 Get_A_Alarm_Val(ALARM_A_INDEX_TypeDef a_index)
{
	switch(a_index)
	{
		case IOVCC_ALARM_A_INDEX:
			return(IOVCC_A_Alarm_Val);
			break;
		case VDDH_ALARM_A_INDEX:
			return(VDDH_A_Alarm_Val);
			break;
		case TPVIO_ALARM_A_INDEX:
			return(TPVIO_A_Alarm_Val);
			break;
		case TPVCC_ALARM_A_INDEX:
			return(TPVCC_A_Alarm_Val);	
			break;
		case VSP_ALARM_A_INDEX:
			return(VSP_A_Alarm_Val);
			break;
		case VSN_ALARM_A_INDEX:
			return(VSN_A_Alarm_Val);			
			break;
		case VSP_ALARM_SLEEP_A_INDEX:
			return(VSP_Sleep_A_Alarm_Val);
			break;
		case VSN_ALARM_SLEEP_A_INDEX:
			return(VSN_Sleep_A_Alarm_Val);			
			break;
		case IOVCC_ALARM_SLEEP_A_INDEX:
			return(IOVCC_Sleep_A_Alarm_Val);				
			break;
		case VDDH_ALARM_SLEEP_A_INDEX:
			return(VDDH_Sleep_A_Alarm_Val);			
			break;
	}
}



void Set_A_Alarm_Val(ALARM_A_INDEX_TypeDef a_index,u16 val)
{
	switch(a_index)
	{
		case IOVCC_ALARM_A_INDEX:
			IOVCC_A_Alarm_Val = val;
			break;
		case VDDH_ALARM_A_INDEX:
			VDDH_A_Alarm_Val = val;
			break;
		case TPVIO_ALARM_A_INDEX:
			TPVIO_A_Alarm_Val = val;
			break;
		case TPVCC_ALARM_A_INDEX:
			TPVCC_A_Alarm_Val = val;
			break;
		case VSP_ALARM_A_INDEX:
			VSP_A_Alarm_Val = val;
			break;
		case VSN_ALARM_A_INDEX:
			VSN_A_Alarm_Val = val;
			break;
		case VSP_ALARM_SLEEP_A_INDEX:
			VSP_Sleep_A_Alarm_Val = val;
			break;
		case VSN_ALARM_SLEEP_A_INDEX:
			VSN_Sleep_A_Alarm_Val = val;
			break;
		case IOVCC_ALARM_SLEEP_A_INDEX:
			IOVCC_Sleep_A_Alarm_Val = val;
			break;
		case VDDH_ALARM_SLEEP_A_INDEX:
			VDDH_Sleep_A_Alarm_Val = val;
			break;
	}
}





void  Current_Volt_Ua_Fresh(void)
{
	static u16 cnt = 0;
	//char *S_V_vsp,*S_A_vsp;
	//char *S_V_vsn,*S_A_vsn;
	char S_temp[6];
	u16  V_val_temp = 0,A_val_temp = 0;



	if(flag_1time_ua == 0)
	{
		INA226_selectDevice(0);	
		INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
		INA226_calibrate(201, 0.001);	//

		INA226_selectDevice(1);
		INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
		INA226_calibrate(201, 0.001);

		INA226_selectDevice(2);
		INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
		INA226_calibrate(201, 0.001);

		flag_1time_ua = 1;
		flag_1time_ma = 0;
	}
	//////////////////////////////////////////////////
	Cnt_Ma = 0;
	Cnt_Ua++;			//分时读取
	Delay_ms(1);
	switch(Cnt_Ua)
	{
		case 1:
			ad715_i2c_write(VOUT_NONE);	
			INA226_selectDevice(0);						
			break;			
		case 50:		
			ad715_i2c_write(LCD_ABUS);		//lcd_dvdd					
			break;
		
		case 150:	
			V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv			
			A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua			
			if(((int)A_val_temp - Iovcc_Leak_Currrent) < 0)
			{
				A_val_temp = 0;
			}
			else
			{
				A_val_temp -= Iovcc_Leak_Currrent;
			}
			Set_V_Alarm_Val(IOVCC_ALARM_SLEEP_V_INDEX,V_val_temp);
			Set_A_Alarm_Val(IOVCC_ALARM_SLEEP_A_INDEX,A_val_temp);
			ad715_i2c_write(LCD_AVDD);		//lcd_avdd	

			TESTER_MAIN_DEBUG("iovcc v = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("iovcc ua= %d\r\n",A_val_temp);			
			break;	
			
		case 250:			
			V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv			
			A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua			
			if(((int)A_val_temp - Vddh_Leak_Currrent) < 0)
			{
				A_val_temp = 0;
			}
			else
			{
				A_val_temp -= Vddh_Leak_Currrent;
			}
			Set_V_Alarm_Val(VDDH_ALARM_SLEEP_V_INDEX,V_val_temp);
			Set_A_Alarm_Val(VDDH_ALARM_SLEEP_A_INDEX,A_val_temp);
			ad715_i2c_write(VOUT_NONE);				
			TESTER_MAIN_DEBUG("VDDH v = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("VDDH ua= %d\r\n",A_val_temp);		
			break;

		case 300:			
			INA226_selectDevice(2);	
			DETECT_VSP_ON_OFF(0);
			DETECT_LEDP_ON_OFF(0);
			DETECT_LED1N_ON_OFF(0);
			DETECT_LED2N_ON_OFF(0);
			DETECT_LED3N_ON_OFF(0);
			DETECT_OTP_P_ON_OFF(0);
			DETECT_VSP_ON_OFF(1);		//打开vsp						
			break;		
	
		case 400:				
			V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv			
			A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua				
			if(((int)A_val_temp - Vsp_Leak_Currrent) < 0)
			{
				A_val_temp = 0;
			}
			else
			{
				A_val_temp -= Vsp_Leak_Currrent;
			}	
			Set_V_Alarm_Val(VSP_ALARM_SLEEP_V_INDEX,V_val_temp);
			Set_A_Alarm_Val(VSP_ALARM_SLEEP_A_INDEX,A_val_temp);
			DETECT_VSP_ON_OFF(0);
			TESTER_MAIN_DEBUG("VSP v = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("VSP ua= %d\r\n",A_val_temp);		
			break;
	

		case 450:				
			INA226_selectDevice(1);
			INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
			INA226_calibrate(201, 0.001);
			break;

		case 550:	
			if(Get_Detect_Power_Status(DETECT_POWER_VSN))
			{
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua					
				if(((int)A_val_temp - Vsn_Leak_Currrent) < 0)
				{
					A_val_temp = 0;
				}
				else
				{
					A_val_temp -= Vsn_Leak_Currrent;
				}
				Set_V_Alarm_Val(VSN_ALARM_SLEEP_V_INDEX,V_val_temp);
				Set_A_Alarm_Val(VSN_ALARM_SLEEP_A_INDEX,A_val_temp);

				TESTER_MAIN_DEBUG("VSN v = %d\r\n",V_val_temp);
				TESTER_MAIN_DEBUG("VSN ua= %d\r\n",A_val_temp);
			}	
			break;		
			
		case 700:
			if(Get_Sleep_Status())			
			{
				Set_V_UA_Detect_Over_Flag(1);
			}
			Cnt_Ua = 0;
			break;
	}	
}






#define	PRINT_LED	0x01	//打印背光部分信息

//电流电压刷新
void  Current_Volt_Fresh(void)
{

	static u16 cnt = 0,cnt1 = 0;
	char S_temp[6];
	u16  V_val_temp = 0,A_val_temp = 0;

	cnt1++;
	if(cnt1 >= 2000)
	{
		cnt1 = 0;
		//i2c1_anti_death();
	}
	

	//////////////////////////////////////////////////
	if(flag_1time_ma == 0)
	{
		INA226_selectDevice(0);	
		INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_588US, INA226_SHUNT_CONV_TIME_588US, INA226_MODE_SHUNT_BUS_CONT);
		INA226_calibrate(1, 0.1);		
		
		INA226_selectDevice(1);
		INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_588US, INA226_SHUNT_CONV_TIME_588US, INA226_MODE_SHUNT_BUS_CONT);
		INA226_calibrate(1, 0.1);
		
		
		INA226_selectDevice(2);
		INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_588US, INA226_SHUNT_CONV_TIME_588US, INA226_MODE_SHUNT_BUS_CONT);
		INA226_calibrate(1, 0.1);
		
		flag_1time_ma = 1;
		flag_1time_ua = 0;	
	}
	

	Cnt_Ua = 0;
	Cnt_Ma++;
	Delay_ms(1);
	switch(Cnt_Ma)
	{
		case 1:
			INA226_selectDevice(0);
			ad715_i2c_write(VOUT_NONE);				
			break;		

		case 50:			
			ad715_i2c_write(LCD_ABUS);		//lcd_dvdd			
			break;
		
		case 150:	
			if(Get_Detect_Power_Status(DETECT_POWER_IOVCC))			
			{
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000);	//精确到ma				

				Set_V_Alarm_Val(IOVCC_ALARM_V_INDEX,V_val_temp);
				Set_A_Alarm_Val(IOVCC_ALARM_A_INDEX,A_val_temp);			
			}			
			ad715_i2c_write(LCD_AVDD);		//lcd_avdd	
			TESTER_MAIN_DEBUG("iovcc v = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("iovcc a= %d\r\n",A_val_temp);
		break;		
		
		case 250:	
			if(Get_Detect_Power_Status(DETECT_POWER_LCDVDD))	
			{
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000);	//精确到ma			

				Set_V_Alarm_Val(VDDH_ALARM_V_INDEX,V_val_temp);
				Set_A_Alarm_Val(VDDH_ALARM_A_INDEX,A_val_temp);
			}
			ad715_i2c_write(TP_ABUS);		//tp_dvdd		
			TESTER_MAIN_DEBUG("VDDH v = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("VDDH a= %d\r\n",A_val_temp);
		break;		
		
		case 350:
			if(Get_Detect_Power_Status(DETECT_POWER_TPVIO))
			{
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000);	//精确到ma					

				Set_V_Alarm_Val(TPVIO_ALARM_V_INDEX,V_val_temp);
				Set_A_Alarm_Val(TPVIO_ALARM_A_INDEX,A_val_temp);
			}	
			ad715_i2c_write(TP_AVDD);		//tp_avdd	
			TESTER_MAIN_DEBUG("TPVIO v = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("TPVIO a= %d\r\n",A_val_temp);
		break;
		
		case 450:
			if(Get_Detect_Power_Status(DETECT_POWER_TPVDD))
			{
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000);	//精确到ma					

				Set_V_Alarm_Val(TPVCC_ALARM_V_INDEX,V_val_temp);
				Set_A_Alarm_Val(TPVCC_ALARM_A_INDEX,A_val_temp);
			}			
			ad715_i2c_write(VOUT_NONE);	
			TESTER_MAIN_DEBUG("TPVDD v = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("TPVDD a= %d\r\n",A_val_temp);
		break;	

		case 550:
			if(Get_Detect_Power_Status(DETECT_POWER_VSN))
			{			
				INA226_selectDevice(1);	
				INA226_configure(INA226_AVERAGES_16, INA226_BUS_CONV_TIME_588US, INA226_SHUNT_CONV_TIME_588US, INA226_MODE_SHUNT_BUS_CONT);
				INA226_calibrate(1, 0.1);
			}			
		break;	

		case 650:			
			if(Get_Detect_Power_Status(DETECT_POWER_VSN))
			{
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000);	//精确到ma						

				Set_V_Alarm_Val(VSN_ALARM_V_INDEX,V_val_temp);
				Set_A_Alarm_Val(VSN_ALARM_A_INDEX,A_val_temp);

				TESTER_MAIN_DEBUG("VSN v = %d\r\n",V_val_temp);
				TESTER_MAIN_DEBUG("VSN a= %d\r\n",A_val_temp);
			}				
		break;

		case 700:
			INA226_selectDevice(2);
			DETECT_OLED_AVDD_ON_OFF(0);
			DETECT_VSP_ON_OFF(0);
			DETECT_LEDP_ON_OFF(0);
			DETECT_LED1N_ON_OFF(0);
			DETECT_LED2N_ON_OFF(0);
			DETECT_LED3N_ON_OFF(0);
			DETECT_OTP_P_ON_OFF(0);
			DETECT_VSP_ON_OFF(1);		//打开vsp
			break;
	
		case 800:
			if(Get_Detect_Power_Status(DETECT_POWER_VSP))
			{
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000);	//精确到ma					
				
				//TESTER_MAIN_DEBUG("%d %d\r\n",V_val_temp,A_val_temp);
				A_val_temp = ((u32)A_val_temp*1000)/1145;				//vsp校准系数

				Set_V_Alarm_Val(VSP_ALARM_V_INDEX,V_val_temp);
				Set_A_Alarm_Val(VSP_ALARM_A_INDEX,A_val_temp);

				TESTER_MAIN_DEBUG("VSP v = %d\r\n",V_val_temp);
				TESTER_MAIN_DEBUG("VSP a= %d\r\n",A_val_temp);
			}	

			if(Get_Power_Status())
			{
				Set_V_A_Detect_Over_Flag(1);
			}

			if(Flag_LED_Start == 0)
			{
				Cnt_Ma = 0;			
			}	
			DETECT_VSP_ON_OFF(0);
		break;
		#if OLED_EN
		case 850:	
			DETECT_OLED_AVDD_ON_OFF(1);
			break;
			
		case 950:
			V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv					
			A_val_temp = (u16)(INA226_readShuntCurrent() *1000);	//精确到ua	
			#if PRINT_LED
			TESTER_MAIN_DEBUG("OLED AVDD V+ = %d\r\n",V_val_temp);
			TESTER_MAIN_DEBUG("OLED AVDD A+ = %d\r\n",A_val_temp);
			#endif
			DETECT_OLED_AVDD_ON_OFF(0);	
			Cnt_Ma = 0;
			break;
		#else
		case 850:			
			DETECT_LEDP_ON_OFF(1);
		break;
			
		case 950:
			LEDP_V_Alarm_Val = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv					
			LEDP_A_Alarm_Val = (u16)(INA226_readShuntCurrent() *1000);	//精确到ua	
			#if PRINT_LED
			TESTER_MAIN_DEBUG("Vled+ = %d\r\n",LEDP_V_Alarm_Val);
			TESTER_MAIN_DEBUG("Aled+ = %d\r\n",LEDP_A_Alarm_Val);
			#endif
			DETECT_LEDP_ON_OFF(0);			
			break;
		#endif

		case 1000:	
			DETECT_LED1N_ON_OFF(1);			
			break;

		case 1100:
			LEDN1_V_Alarm_Val = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv					
			LEDN1_A_Alarm_Val = (u16)(INA226_readShuntCurrent() *10000);	//精确到ua	
			#if PRINT_LED
			TESTER_MAIN_DEBUG("Vled1- = %d\r\n",LEDN1_V_Alarm_Val);
			TESTER_MAIN_DEBUG("Aled1- = %d\r\n",LEDN1_A_Alarm_Val);
			#endif
			DETECT_LED1N_ON_OFF(0);	
			break;

		case 1200:	
			DETECT_LED2N_ON_OFF(1);			
			break;

		case 1300:
			LEDN2_V_Alarm_Val = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv					
			LEDN2_A_Alarm_Val = (u16)(INA226_readShuntCurrent() *10000);	//精确到ua	
			#if PRINT_LED
			TESTER_MAIN_DEBUG("Vled2- = %d\r\n",LEDN2_V_Alarm_Val);
			TESTER_MAIN_DEBUG("Aled2- = %d\r\n",LEDN2_A_Alarm_Val);
			#endif	
			DETECT_LED2N_ON_OFF(0);				
			break;

		case 1400:
			DETECT_LED3N_ON_OFF(1);			
			break;

		case 1500:
			LEDN3_V_Alarm_Val = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv					
			LEDN3_A_Alarm_Val = (u16)(INA226_readShuntCurrent() *10000);	//精确到ua	
			#if PRINT_LED
			TESTER_MAIN_DEBUG("Vled3- = %d\r\n",LEDN3_V_Alarm_Val);
			TESTER_MAIN_DEBUG("Aled3- = %d\r\n",LEDN3_A_Alarm_Val);
			#endif	
			DETECT_LED3N_ON_OFF(0);
			Flag_LED_Start = 0;
			BL_V_A_Detect_Over = 1;
			
			Cnt_Ma = 0;	
			break;

		case 1600:							//目前预留
			DETECT_OTP_P_ON_OFF(1);		
			break;

		case 1700:
			TESTER_MAIN_DEBUG("Votp- = %f\r\n",INA226_readBusVoltage());
			TESTER_MAIN_DEBUG("Aotp- = %f\r\n",INA226_readShuntCurrent());
			DETECT_OTP_P_ON_OFF(0);			
			Cnt_Ma = 0;	
			break;
	}
	
}



/*******************************************************************************
* 文件名	  	 : Sys_Err_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 0--正常，1--TE NG 2--PWM NG 3--ID NG 4--NWM -NG 5--TP NG
					18 - BK NG
				  
*******************************************************************************/
u8	Sys_Err_Handle(void)
{		
	if(Get_TE_Seq_Err_Flag() || Get_TE_Duty_Err_Flag())
	{
		Clear_TE_Err_Flag();		//保证运行一次
		Set_Sys_Err_Flag(1);		
		return(1);
	}

	if(Get_Seq_Err_Flag() || Get_Duty_Err_Flag())
	{
		Clear_PWM_Err_Flag();
		Set_Sys_Err_Flag(2);
		return(1);
	}
	
	return(0);		
}




u8 V_UA_Alarm_Handle(void)
{
	u16 temp = 0;
	u8 err = 0;	
	u16 option_temp1 = 0,option_temp2 = 0;
	static u16 err_v_cnt = 0,err_a_cnt = 0,cnt = 0;
	

	Get_Option_Para(SLEEP_CURRENT_EN_INDEX,&option_temp1);			//工作电压检测
	if(option_temp1 == 1)
	{
		if(Get_Detect_Power_Status(DETECT_POWER_IOVCC))			
		{
			Get_Option_Para(PWR1_IOVCC_UA_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR1_IOVCC_UA_LOW_INDEX,&option_temp2);	
			temp = Get_A_Alarm_Val(IOVCC_ALARM_SLEEP_A_INDEX);
			//temp /= 10;
			//TESTER_MAIN_DEBUG("iovcc = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > (option_temp1 * 10)) || (temp < (option_temp2 * 10)))
			{
				err = 1;
				Set_Sys_Err_Flag(19);
			}

			#if 0
			if(temp > 200)
			{
				TESTER_MAIN_DEBUG("IOVCC UA ERR\r\n");
			}
			#endif
		}

		if(Get_Detect_Power_Status(DETECT_POWER_LCDVDD))			
		{
			Get_Option_Para(PWR2_VDDH_UA_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR2_VDDH_UA_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(VDDH_ALARM_SLEEP_A_INDEX);
			//temp /= 10;
			//TESTER_MAIN_DEBUG("vddh = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > (option_temp1 * 10)) || (temp < (option_temp2 * 10)))
			{
				err = 1;
				Set_Sys_Err_Flag(20);
			}
		}
		
		if(Get_Detect_Power_Status(DETECT_POWER_VSP))			
		{
			Get_Option_Para(PWR5_VSP_AVDD_UA_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR5_VSP_AVDD_UA_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(VSP_ALARM_SLEEP_A_INDEX);
			//temp /= 10;
			//TESTER_MAIN_DEBUG("vsp = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > (option_temp1 * 10)) || (temp < (option_temp2 * 10)))
			{
				err = 1;
				Set_Sys_Err_Flag(21);
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_VSN))			
		{
			Get_Option_Para(PWR6_VSN_AVEE_UA_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR6_VSN_AVEE_UA_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(VSN_ALARM_SLEEP_A_INDEX);
			//temp /= 10;
			//TESTER_MAIN_DEBUG("vsn = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > (option_temp1 * 10)) || (temp < (option_temp2 * 10)))
			{
				err = 1;
				Set_Sys_Err_Flag(22);
			}
		}
		
		
		if(err)		//出错判断
		{
			Set_A_Alarm_Flag(1);
			//Set_Sys_Err_Flag(1);
			
			err = 0;
			err_a_cnt++;
		}
		else
		{
			Set_A_Alarm_Flag(0);
			err_a_cnt = 0;
		}

		if(err_a_cnt >= 4)		//重试8次
		{
			err_a_cnt = 0;			
			Flag_UA_err = 2;		// sleep ua err
			return 1;
		}
		else
		{
			Set_Sys_Err_Flag(0);
			cnt++;
			if(cnt >= 1)
			{
				cnt = 0;
				Flag_UA_err = 1;		// sleep ua ok
			}
		}
	}
	else
	{
		Flag_UA_err = 1;;		// sleep ua ok
	}
}



u8 V_A_Alarm_Handle(void)
{
	u16 temp = 0;
	u8 err = 0;	
	u16 option_temp1 = 0,option_temp2 = 0;
	static u16 err_v_cnt = 0,err_a_cnt = 0;
	

	Get_Option_Para(VOLTAGE_EN_INDEX,&option_temp1);			//工作电压检测
	if(option_temp1 == 1)
	{
		if(Get_Detect_Power_Status(DETECT_POWER_IOVCC))			
		{
			Get_Option_Para(PWR1_IOVCC_V_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR1_IOVCC_V_LOW_INDEX,&option_temp2);
			temp = Get_V_Alarm_Val(IOVCC_ALARM_V_INDEX);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(6);
				
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_LCDVDD))			
		{
			Get_Option_Para(PWR2_VDDH_V_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR2_VDDH_V_LOW_INDEX,&option_temp2);
			temp = Get_V_Alarm_Val(VDDH_ALARM_V_INDEX);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(7);
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_TPVDD))			
		{
			Get_Option_Para(PWR3_TP_VDD_V_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR3_TP_VDD_V_LOW_INDEX,&option_temp2);
			temp = Get_V_Alarm_Val(TPVCC_ALARM_V_INDEX);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(8);
			}	
		}

		if(Get_Detect_Power_Status(DETECT_POWER_TPVIO))			
		{
			Get_Option_Para(PWR4_TP_VIO_V_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR4_TP_VIO_V_LOW_INDEX,&option_temp2);
			temp = Get_V_Alarm_Val(TPVIO_ALARM_V_INDEX);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(9);
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_VSP))			
		{
			Get_Option_Para(PWR5_VSP_AVDD_V_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR5_VSP_AVDD_V_LOW_INDEX,&option_temp2);
			temp = Get_V_Alarm_Val(VSP_ALARM_V_INDEX);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(10);
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_VSN))			
		{
			Get_Option_Para(PWR6_VSN_AVEE_V_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR6_VSN_AVEE_V_LOW_INDEX,&option_temp2);
			temp = Get_V_Alarm_Val(VSN_ALARM_V_INDEX);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(11);
			}
		}

		if(err)		//出错判断
		{
			Set_V_Alarm_Flag(1);				
			err = 0;
			err_v_cnt++;
		}
		else
		{
			Set_V_Alarm_Flag(0);
			err_v_cnt = 0;
		}

		if(err_v_cnt >= 8)			//重试8次
		{
			err_v_cnt = 0;
			Flag_V_err = 2;
			return 1;
		}
		else
		{
			Set_Sys_Err_Flag(0);
		}
	}


	Get_Option_Para(CURRENT_EN_INDEX,&option_temp1);			//工作电流检测
	if(option_temp1 == 1)
	{
		if(Get_Detect_Power_Status(DETECT_POWER_IOVCC))			
		{
			Get_Option_Para(PWR1_IOVCC_A_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR1_IOVCC_A_LOW_INDEX,&option_temp2);	
			temp = Get_A_Alarm_Val(IOVCC_ALARM_A_INDEX);
			temp /= 10;
			//TESTER_MAIN_DEBUG("iovcc = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(12);
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_LCDVDD))			
		{
			Get_Option_Para(PWR2_VDDH_A_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR2_VDDH_A_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(VDDH_ALARM_A_INDEX);
			temp /= 10;
			//TESTER_MAIN_DEBUG("vddh = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(13);
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_TPVIO))			
		{
			Get_Option_Para(PWR4_TP_VIO_A_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR4_TP_VIO_A_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(TPVCC_ALARM_A_INDEX);
			temp /= 10;
			//TESTER_MAIN_DEBUG("tpvdd = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(14);
			}
		}
		
		if(Get_Detect_Power_Status(DETECT_POWER_TPVDD))			
		{
			Get_Option_Para(PWR3_TP_VDD_A_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR3_TP_VDD_A_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(TPVIO_ALARM_A_INDEX);
			temp /= 10;
			//TESTER_MAIN_DEBUG("tpvio = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(15);
			}
		}


		if(Get_Detect_Power_Status(DETECT_POWER_VSP))			
		{
			Get_Option_Para(PWR5_VSP_AVDD_A_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR5_VSP_AVDD_A_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(VSP_ALARM_A_INDEX);
			temp /= 10;
			//TESTER_MAIN_DEBUG("vsp = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(16);
			}
		}

		if(Get_Detect_Power_Status(DETECT_POWER_VSN))			
		{
			Get_Option_Para(PWR6_VSN_AVEE_A_HIGH_INDEX,&option_temp1);
			Get_Option_Para(PWR6_VSN_AVEE_A_LOW_INDEX,&option_temp2);
			temp = Get_A_Alarm_Val(VSN_ALARM_A_INDEX);
			temp /= 10;
			//TESTER_MAIN_DEBUG("vsn = %d %d %d\r\n",temp,option_temp1,option_temp2);
			if((temp > option_temp1) || (temp < option_temp2))
			{
				err = 1;
				Set_Sys_Err_Flag(17);
			}
		}
		
		
		if(err)		//出错判断
		{
			Set_A_Alarm_Flag(1);
			//Set_Sys_Err_Flag(1);
			
			err = 0;
			err_a_cnt++;
		}
		else
		{
			Set_A_Alarm_Flag(0);
			err_a_cnt = 0;
		}

		if(err_a_cnt >= 8)		//重试8次
		{
			err_a_cnt = 0;
			Flag_A_err = 2;			
			return 1;
		}
		else
		{
			Set_Sys_Err_Flag(0);
		}
	}	
	
}



void Consum_V_A_Clear(void)
{
	Set_V_Alarm_Val(VSP_ALARM_V_INDEX,0);			
	Set_A_Alarm_Val(VSP_ALARM_A_INDEX,0);
	Set_V_Alarm_Val(IOVCC_ALARM_V_INDEX,0);
	Set_A_Alarm_Val(IOVCC_ALARM_A_INDEX,0);
	Set_V_Alarm_Val(VDDH_ALARM_V_INDEX,0);
	Set_A_Alarm_Val(VDDH_ALARM_A_INDEX,0);
	Set_V_Alarm_Val(TPVIO_ALARM_V_INDEX,0);
	Set_A_Alarm_Val(TPVIO_ALARM_A_INDEX,0);
	Set_V_Alarm_Val(TPVCC_ALARM_V_INDEX,0);
	Set_A_Alarm_Val(TPVCC_ALARM_A_INDEX,0);
	Set_V_Alarm_Val(VSN_ALARM_V_INDEX,0);
	Set_A_Alarm_Val(VSN_ALARM_A_INDEX,0);
}




void Volt_Current_Detect(void)
{	
	static u8 flag0 = 0,flag1 = 0,flag2 = 0;
	static u8 status_old = 0,connect_old = 0,temp = 0;	
	u16 option_temp = 0;
	u8 buf[1];
	
	temp = Get_Power_Status();
	if(temp != status_old)
	{
		Cnt_Ma = 0;	
		Cnt_Ua = 0;	
	
		if(temp == 0)
		{	
			if(Get_Sleep_Status() == 0) 
			{
				Consum_V_A_Clear();
				
			}			
		}
		else
		{
			
		}

		status_old = temp;
			
	}
	
	
	if(Get_Sleep_Status() == 1)
	{
		if(flag0 == 0)
		{
			flag0 = 1;
			flag1 = 0;	

			Delay_ms(800);
			
			RELAY_UA_ALL_ON_OFF(1);
			Consum_V_A_Clear();
			
		}		

		Current_Volt_Ua_Fresh();	
	
		Delay_us(10);		
	}
	else
	{
		if(flag1 == 0)
		{
			flag1 = 1;
			flag0 = 0;	
			RELAY_UA_ALL_ON_OFF(0);
			Consum_V_A_Clear();			
		}
		
		if(Get_Power_Status())		
		{
			//if(Get_TE_Over_Flag())			//确保TE已经检查过了，才能关闭总中断
			if(1)
			{				
				Current_Volt_Fresh();		
			}		
		}
		else
		{	
			flag1 = 0;
			Cnt_Ma = 0;	
			Cnt_Ua = 0;	
			flag_1time_ma = 0;
			flag_1time_ua = 0;			
		}
	}
}





u8 Volt_Current_Ua_Calibration(void)
{
	static u16 cnt = 0;	
	char S_temp[6];
	u16  V_val_temp = 0,A_val_temp = 0;
	//////////////////////////////////////////////////

	FPGA_Sleep_Off();	//先上一次电

	Delay_ms(50);

	while(1)			//执行完成后，自动跳出
	{	
		cnt++;
		Delay_ms(1);
		switch(cnt)
		{
			case 1:
				ad715_i2c_write(VOUT_NONE);	
				INA226_selectDevice(0);	
				INA226_configure(INA226_AVERAGES_128, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);		
				INA226_calibrate(201, 0.001);

				RELAY_UA_ALL_ON_OFF(1);				//relay all on		
				break;				
			
			case 50:		
				ad715_i2c_write(LCD_ABUS);		//lcd_dvdd				
				break;
		
			case 150:
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv
				TESTER_MAIN_DEBUG("iovcc ua vcali= %d\r\n",V_val_temp);				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua	
				TESTER_MAIN_DEBUG("iovcc ua acali =%f\r\n",INA226_readShuntCurrent());
			
				Iovcc_Leak_Currrent = A_val_temp;		//记录下漏电流
			
				ad715_i2c_write(LCD_AVDD);		//lcd_avdd			
			break;		
			
			case 250:	
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv
				TESTER_MAIN_DEBUG("vddh ua vcali= %d\r\n",V_val_temp);				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua	
				TESTER_MAIN_DEBUG("vddh ua acali =%f\r\n",INA226_readShuntCurrent());
			
				Vddh_Leak_Currrent = A_val_temp;			

				ad715_i2c_write(VOUT_NONE);					
			break;

			case 350:				
				INA226_selectDevice(1);
				INA226_configure(INA226_AVERAGES_128, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
				INA226_calibrate(201, 0.001);						
			break;

			case 450:	
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000); 	//精确到mv
				TESTER_MAIN_DEBUG("vsn ua vcali= %d\r\n",V_val_temp);				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua	
				TESTER_MAIN_DEBUG("vsn ua acali =%f\r\n",INA226_readShuntCurrent());
		
				Vsn_Leak_Currrent = A_val_temp;			
			break;

			case 550:			
				INA226_selectDevice(2);
				INA226_configure(INA226_AVERAGES_128, INA226_BUS_CONV_TIME_204US, INA226_SHUNT_CONV_TIME_332US, INA226_MODE_SHUNT_BUS_CONT);
				INA226_calibrate(201, 0.001);
				DETECT_OLED_AVDD_ON_OFF(0);
				DETECT_VSP_ON_OFF(0);
				DETECT_LEDP_ON_OFF(0);
				DETECT_LED1N_ON_OFF(0);
				DETECT_LED2N_ON_OFF(0);
				DETECT_LED3N_ON_OFF(0);
				DETECT_OTP_P_ON_OFF(0);
				DETECT_VSP_ON_OFF(1);
			break;		
		
			case 650:					
				V_val_temp = (u16)(INA226_readBusVoltage() * 1000); 	//精确到mv
				TESTER_MAIN_DEBUG("vsp ua vcali= %d\r\n",V_val_temp);				
				A_val_temp = (u16)(INA226_readShuntCurrent() *10000000);	//精确到ua	
				TESTER_MAIN_DEBUG("vsp ua acali =%f\r\n",INA226_readShuntCurrent());
			
				Vsp_Leak_Currrent = A_val_temp;
				
				cnt = 0;
				System_Power_Off_All();
				Delay_ms(100);
				RELAY_UA_ALL_ON_OFF(0);				//relay all off					
				Delay_ms(200);
				TESTER_MAIN_DEBUG("Ua Current Calibration done!!\r\n");	

				DETECT_VSP_ON_OFF(0);
				return OK;
			break;							
		}
	}
}



void LED_DETECT_Hanlde(void)
{
	static u8 flag_1times = 0,flag1_ok = 0,flag2_ok = 0,flag3_ok = 0;
	static u16 cnt_1times = 0;
	u16 option_temp1 = 0,option_temp2 = 0;
	static u16	led_a_low = 0,led_a_high = 0;

	
	
	if(Get_Power_Status())
	{			
		if(flag_1times == 0)
		{			
			cnt_1times++;
			if(cnt_1times >= 400)
			{
				flag_1times = 1;
				cnt_1times = 0;
				Flag_LED_Start = 1;
			}
		}	

		if(BL_V_A_Detect_Over)
		{			
			////////////////////////////////////////////背光1
			if(Get_Detect_Power_Status(DETECT_POWER_LEDN1))		//背光1开启
			{
				Get_Option_Para(PWR7_LED1N_V_HIGH_INDEX,&option_temp1);
				Get_Option_Para(PWR7_LED1N_V_LOW_INDEX,&option_temp2);					
				if((LEDP_V_Alarm_Val - LEDN1_V_Alarm_Val) >= option_temp1 || (LEDP_V_Alarm_Val - LEDN1_V_Alarm_Val) <= option_temp2)
				{						
					Set_Sys_Err_Flag(24);
					flag1_ok = 1;
				}
				else
				{
					//flag1_ok = 0;
				}
				Get_Option_Para(PWR7_LED1N_A_HIGH_INDEX,&option_temp1);
				Get_Option_Para(PWR7_LED1N_A_LOW_INDEX,&option_temp2);			

				
				if(LEDN1_A_Alarm_Val >= (option_temp1 * 10) || LEDN1_A_Alarm_Val <= (option_temp2 * 10))
				{						
					Set_Sys_Err_Flag(25);
					flag1_ok = 1;
				}
				else
				{
					//flag_ok = 0;
				}				
			}
			else
			{
				
			}

			////////////////////////////////////////////背光2
			if(Get_Detect_Power_Status(DETECT_POWER_LEDN2))		//背光1开启
			{
				Get_Option_Para(PWR8_LED2N_V_HIGH_INDEX,&option_temp1);
				Get_Option_Para(PWR8_LED2N_V_LOW_INDEX,&option_temp2);	
				//printf("vsp = %d %d %d %d\r\n",LEDP_V_Alarm_Val,LEDN2_V_Alarm_Val,option_temp1,option_temp2);
				if((LEDP_V_Alarm_Val - LEDN2_V_Alarm_Val) >= option_temp1 || (LEDP_V_Alarm_Val - LEDN2_V_Alarm_Val) <= option_temp2)
				{				
					
					Set_Sys_Err_Flag(26);
					flag2_ok = 1;
				}
				else
				{
					//flag_ok = 0;
				}
				Get_Option_Para(PWR8_LED2N_A_HIGH_INDEX,&option_temp1);
				Get_Option_Para(PWR8_LED2N_A_LOW_INDEX,&option_temp2);

				//led_a_low += option_temp2;
				//led_a_high += option_temp1;

				//printf("led_a_low = %d led_a_high = %d sum = %d\r\n",led_a_low,led_a_high,LEDN2_A_Alarm_Val + LEDN1_A_Alarm_Val);

				
				if(LEDN2_A_Alarm_Val >= (option_temp1 * 10) || LEDN2_A_Alarm_Val <= (option_temp2 * 10))
				{						
					Set_Sys_Err_Flag(27);
					flag2_ok = 1;
				}
				else
				{
					//flag_ok = 0;
				}				

				if(flag2_ok == 0)
				{					
					Flag_Sys_Ready = 1;						
				}
				else
				{					
					
				}			
			}
			else
			{
				
			}			
			
			////////////////////////////////////////////背光3
			#if 1
			if(Get_Detect_Power_Status(DETECT_POWER_LEDN3))		//背光1开启
			{					
				Get_Option_Para(PWR9_LED3N_V_HIGH_INDEX,&option_temp1);
				Get_Option_Para(PWR9_LED3N_V_LOW_INDEX,&option_temp2);	
				//printf("vsp = %d %d %d %d\r\n",LEDP_V_Alarm_Val,LEDN2_V_Alarm_Val,option_temp1,option_temp2);
				if((LEDP_V_Alarm_Val - LEDN3_V_Alarm_Val) >= option_temp1 || (LEDP_V_Alarm_Val - LEDN3_V_Alarm_Val) <= option_temp2)
				{				
					
					Set_Sys_Err_Flag(28);
					flag3_ok = 1;
				}
				else
				{
					//flag_ok = 0;
				}
				Get_Option_Para(PWR9_LED3N_A_HIGH_INDEX,&option_temp1);
				Get_Option_Para(PWR9_LED3N_A_LOW_INDEX,&option_temp2);
				if(LEDN3_A_Alarm_Val >= (option_temp1 * 10) || LEDN3_A_Alarm_Val <= (option_temp2 * 10))
				{				
					
					Set_Sys_Err_Flag(29);
					flag3_ok = 1;
				}
				else
				{
					//flag_ok = 0;
				}

				if(flag3_ok == 0)
				{					
				
				}
				else
				{
					
				}			
			}
			else
			{
								
			}
			#else
						
			#endif

			BL_V_A_Detect_Over = 0;
		}
	}
	else
	{		
		if(flag_1times)
		{
			led_a_low = 0;
			led_a_high = 0;
			
			flag_1times = 0;
			cnt_1times = 0;
			flag1_ok = 0;
			flag2_ok = 0;
			flag3_ok = 0;
			BL_V_A_Detect_Over = 0;			
			Flag_LED_Start = 0;		
			LEDP_V_Alarm_Val = 0;
			LEDN1_V_Alarm_Val = 0;
			LEDN2_V_Alarm_Val = 0;
			LEDN3_V_Alarm_Val = 0;
			LEDP_A_Alarm_Val = 0;
			LEDN1_A_Alarm_Val = 0;
			LEDN2_A_Alarm_Val = 0;
			LEDN3_A_Alarm_Val = 0;
	
		}
	}	
}


u8 Get_Flag_Sys_Ready(void)
{
	return(Flag_Sys_Ready);
}



void Clear_Flag_Sys_Ready(void)
{
	Flag_Sys_Ready = 0;
}




u8 Get_UA_Err_Flag(void)
{
	return(Flag_UA_err);
}


void Set_UA_Err_Flag(u8 val)
{
	Flag_UA_err = val;
}


u8 Get_A_Err_Flag(void)
{
	return(Flag_A_err);
}


void Set_A_Err_Flag(u8 val)
{
	Flag_A_err = val;
}


u8 Get_V_Err_Flag(void)
{
	return(Flag_V_err);
}


void Set_V_Err_Flag(u8 val)
{
	Flag_V_err = val;
}




