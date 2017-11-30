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

#include "AD715.h"
#include "myiic.h"
#include "stm32f10x.h"




#define aD715_I2C_7BIT_ADDRESS 0x48



int ad715_i2c_write(uint8_t val)
{
	int retval;
	
	retval = i2c1_start(aD715_I2C_7BIT_ADDRESS<<1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_write_u8((uint8_t)(val));
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_write_u8(val) Failed\r\n");
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


void Tps65651_test(void)
{
	OLED_AVDD_ON;			//¿ªÆôÏÔÊ¾ÆÁµçÑ¹
	OLED_ELVDD_ON;
}



void ad715_Init(void)
{
	ad715_i2c_write(VOUT_NONE);

}





/******************* (C) COPYRIGHT 2012 *****END OF FILE************/


