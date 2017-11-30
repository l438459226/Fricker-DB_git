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

#include "delay.h"







#define	PRINT_LED	0x01	//打印背光部分信息

void INA226_Volt(void)
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
		
}



//电流电压刷新
void  Current_Volt(void)
{
	u16  V_val_temp = 0,A_val_temp = 0;

	Delay_ms(10);

	INA226_selectDevice(0);
	ad715_i2c_write(VOUT_NONE);				
	

	Delay_ms(100);			
	ad715_i2c_write(LCD_ABUS);		//lcd_dvdd			

		
	Delay_ms(200);

	V_val_temp = (u16)(INA226_readBusVoltage() * 1000);		//精确到mv
	A_val_temp = (u16)(INA226_readShuntCurrent() *10000);	//精确到ma				

	ad715_i2c_write(LCD_AVDD);		//lcd_avdd	
	TESTER_MAIN_DEBUG("iovcc v = %d\r\n",V_val_temp);
	TESTER_MAIN_DEBUG("iovcc a= %d\r\n",A_val_temp);
	Delay_ms(200);	

}




