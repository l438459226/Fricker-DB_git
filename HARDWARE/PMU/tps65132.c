/*
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
*/


#include "tps65132.h"
#include "myiic.h"
#include "stm32f10x.h"
#include "usart.h"
#include "sys.h"


#define tps65132_I2C_7BIT_ADDRESS 0x3E
#define ENABLE_ERROR_PRINT

//"Write" Data To DAC
int tps65132_i2c_write(uint8_t reg, uint8_t val)
{
	int retval;
//	uint8_t i;
	
	retval = i2c1_start(tps65132_I2C_7BIT_ADDRESS<<1);
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
	
	retval = i2c1_write_u8((uint8_t)(val));
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_write_u8(val) Failed\r\n");
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



//Read Data From DAC/EEPROM
// 1 - Read from EEPROM Register
// 0 - Read from DAC Register
int tps65132_i2c_read(uint8_t reg, uint8_t* pBuffer, uint8_t from_EEPROM)
{
  int retval;
	if(from_EEPROM){
		//0xFF is the control register
		retval = tps65132_i2c_write(0xFF,1);
	}else{
		retval = tps65132_i2c_write(0xFF,0);
	}
	
	retval = i2c1_start(tps65132_I2C_7BIT_ADDRESS<<1);
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
	
	retval = i2c1_start_repeat((tps65132_I2C_7BIT_ADDRESS<<1)|0x01);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c1_start_repeat Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_read_u8(pBuffer, 1);
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

//VPOS/VNEG Range: 4000mv - 6000mV
//VPOS Register 0x00, VNEG Register: 0x01
//VPOS/VNEG REG: 0->4000,1->4100,...,20->6000
int tps65132_set_voltage(uint16_t voltage_mV,uint8_t is_Vneg)//0 - VPOS, 1 - VNEG
{
	uint8_t val = 0;
	int ret = 0;
	u8 regdat;

	if (voltage_mV < 4000 ||
	    voltage_mV > 6000)
		return -2;

	val = (uint8_t)((float)(voltage_mV - 4000)/100);
	
	TESTER_MAIN_DEBUG("voltage_mV = %d, val = %d\r\n",voltage_mV,val);
	
	if(is_Vneg){
		ret = tps65132_i2c_write(0x01,val);
		tps65132_i2c_read(0x01,&regdat,0);
	}else{
		ret = tps65132_i2c_write(0x00,val);
		tps65132_i2c_read(0x00,&regdat,0);
	}
	printf("read reg:%d\r\n",regdat);
	return ret;
}

//apps: 0 - smart phone, 1 - tablet
//APPS - DISP - DISN Register 0x03
int tps65132_set_mode(uint8_t apps, uint8_t vpos_discharg, uint8_t vneg_discharge)
{
	uint8_t val = 0;
	int ret = 0;

	if(apps){
		val = val | 0x40;
		}
	
	if(vpos_discharg){
		val = val | 0x02;
	}
	
	if(vneg_discharge){
		val = val | 0x01;
	}
	
	TESTER_MAIN_DEBUG("tps65132_set_mode val = %d\r\n",val);
	
	ret = tps65132_i2c_write(0x03,val);
	return ret;
}


int Tps65132_Init(u16 vsp, u16 vsn)
{
	tps65132_set_voltage(vsn,1);			//…Ë÷√VSP VSNµÁ—π
	tps65132_set_voltage(vsp,0);	
	tps65132_set_mode(0,1,1);	
	return 0;
}

