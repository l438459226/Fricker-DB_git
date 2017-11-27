#include "AD715.h"
#include "i2c1_bitbang.h"
#include "stm32f10x.h"




#define aD715_I2C_7BIT_ADDRESS1 0x48
#define aD715_I2C_7BIT_ADDRESS2 0x4a





int ad715_i2c_write(uint8_t val)
{
	int retval;
	
	retval = i2c1_start(aD715_I2C_7BIT_ADDRESS1<<1);
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


int ad715_i2c_read(uint8_t* pBuffer)
{
	int retval;
	retval = i2c1_start(aD715_I2C_7BIT_ADDRESS1<<1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_start_repeat((aD715_I2C_7BIT_ADDRESS1<<1)|0x01);
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




int ad715_i2c_write2(uint8_t val)
{
	int retval;
	
	retval = i2c1_start(aD715_I2C_7BIT_ADDRESS2<<1);
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


int ad715_i2c_read2(uint8_t* pBuffer)
{
	int retval;
	retval = i2c1_start(aD715_I2C_7BIT_ADDRESS2<<1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_start_repeat((aD715_I2C_7BIT_ADDRESS2<<1)|0x01);
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








void Tps65651_test(void)
{
	OLED_AVDD_ON;			//¿ªÆôÏÔÊ¾ÆÁµçÑ¹
	OLED_ELVDD_ON;
}







void ad715_Init(void)
{
	u8 temp = 0;
	ad715_i2c_write(VOUT_NONE);				//ÇÐ»»µçÑ¹
	//ad715_i2c_read(&temp);
	//printf("ad715 temp1 = %d\r\n",temp);
	ad715_i2c_write2(DETE_PORTAB_NONE);		//ÇÐ»»portab ID TE PWM
	//ad715_i2c_read2(&temp);
	//printf("ad715 temp2 = %d\r\n",temp);
	VSP_DETECTED_OFF;
	LED_PWR_DETECTED_OFF;
	//Tps65651_test();
}





/******************* (C) COPYRIGHT 2012 *****END OF FILE************/


