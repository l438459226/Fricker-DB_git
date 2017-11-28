/*
 * Regulator driver for Active-semi act8846 PMIC chip for rk29xx
 *
 * Copyright (C) 2010, 2011 ROCKCHIP, Inc.

 * Based on act8846.c that is work by zhangqing<zhangqing@rock-chips.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include "myiic.h"
#include "act8846.h"
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

#include "exti.h"
#include "usart.h"

#include "parm.h"
#include "GPIO_Config.h"

//static u8 act8846_reg_read(struct act8846 *act8846, u8 reg);
//static int act8846_set_bits(struct act8846 *act8846, u8 reg, u16 mask, u16 val);

#define act8846_I2C_7BIT_ADDRESS 0x5A  //change to ACT8846 Address


#define act8846_BUCK1_SET_VOL_BASE 0x10
#define act8846_BUCK2_SET_VOL_BASE 0x20
#define act8846_BUCK3_SET_VOL_BASE 0x30
#define act8846_BUCK4_SET_VOL_BASE 0x40

#define act8846_BUCK2_SLP_VOL_BASE 0x21
#define act8846_BUCK3_SLP_VOL_BASE 0x31
#define act8846_BUCK4_SLP_VOL_BASE 0x41

#define act8846_LDO1_SET_VOL_BASE 0x50
#define act8846_LDO2_SET_VOL_BASE 0x58
#define act8846_LDO3_SET_VOL_BASE 0x60
#define act8846_LDO4_SET_VOL_BASE 0x68
#define act8846_LDO5_SET_VOL_BASE 0x70
#define act8846_LDO6_SET_VOL_BASE 0x80
#define act8846_LDO7_SET_VOL_BASE 0x90
#define act8846_LDO8_SET_VOL_BASE 0xa0
//#define act8846_LDO9_SET_VOL_BASE 0xb1

#define act8846_BUCK1_CONTR_BASE 0x12
#define act8846_BUCK2_CONTR_BASE 0x22
#define act8846_BUCK3_CONTR_BASE 0x32
#define act8846_BUCK4_CONTR_BASE 0x42

#define act8846_LDO1_CONTR_BASE 0x51
#define act8846_LDO2_CONTR_BASE 0x59
#define act8846_LDO3_CONTR_BASE 0x61
#define act8846_LDO4_CONTR_BASE 0x69
#define act8846_LDO5_CONTR_BASE 0x71
#define act8846_LDO6_CONTR_BASE 0x81
#define act8846_LDO7_CONTR_BASE 0x91
#define act8846_LDO8_CONTR_BASE 0xa1
#define act8846_LDO9_CONTR_BASE 0xb1

#define BUCK_VOL_MASK 0x3f
#define LDO_VOL_MASK 0x3f

#define VOL_MIN_IDX 0x00
#define VOL_MAX_IDX 0x3f

//PE5 - 1: ALL ON, 0: SHUTDOWN
int act8846_init(void)
{	
	int ret;
	uint8_t i;
	u16 temp = 0;
	static u8 flag = 0;


	if(flag == 0)
	{
		flag = 1;
			
		//i2c1_init();

		Set_PMU_RST_IO(0);	
		Delay_ms(100); 
		Set_PMU_RST_IO(1);	
		Delay_ms(200);
		Set_PMU_PWREN_IO(1);
		Delay_ms(100);
			

			for(i=0;i<4;i++){
				ret = act8846_dcdc_disable(i);
				if(ret < 0)
				{
					TESTER_MAIN_DEBUG("act8846_dcdc_disable error.%d\r\n",i);
					return ret;
				}
			}
			
			for(i=0;i<8;i++){
				ret = act8846_ldo_disable(i);
				if(ret < 0){
					TESTER_MAIN_DEBUG("act8846_ldo_disable error.");
					return ret;
				}
			}
			
			ret = act8846_dcdc_set_voltage(1,3300, 3300);
			if(ret < 0){
					TESTER_MAIN_DEBUG("act8846_dcdc_set_voltage(1,3300, 3300) error.");
					return ret;
				}
			ret = act8846_dcdc_set_voltage(2,1200, 1200);
			if(ret < 0){
					TESTER_MAIN_DEBUG("act8846_dcdc_set_voltage(2,1200, 1200); error.");
					return ret;
				}
			ret = act8846_dcdc_set_voltage(3,2500, 2500);
			if(ret < 0){
					TESTER_MAIN_DEBUG("act8846_dcdc_set_voltage(3,2500, 2500); error.");
					return ret;
				}
		}
		
		
		temp = Get_Volt_Val(PWR13_EXT_PWR_A_INDEX);
		//printf("PWR13_EXT_PWR_A_INDEX:%d    temp:%d\r\n",PWR13_EXT_PWR_A_INDEX,temp);
		if(temp != 0)
		{
			ret = act8846_ldo_set_voltage(6,temp,temp);//EXT_POWER_1
		}
		else
		{
			ret = act8846_ldo_set_voltage(6,600,600);
		}
		if(ret < 0){
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(0,1800,1800); error.");
				return ret;
			}

		///////////////////////////////////////////////////////////////////////
		temp = Get_Volt_Val(PWR15_TESTER_VBUS_INDEX);
		if(temp != 0)
		{
			ret = act8846_ldo_set_voltage(1,temp,temp);//TESTER VBUS
		}
		else
		{
			ret = act8846_ldo_set_voltage(1,600,600);
		}		
		if(ret < 0){
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(1,1800,1800); error.");
				return ret;
			}	

		///////////////////////////////////////////////////////////////////////
		temp = Get_Volt_Val(PWR3_TP_VDD_INDEX);
		if(temp != 0)
		{
			ret = act8846_ldo_set_voltage(7,temp,temp);//TP_AVDD
		}
		else
		{
			ret = act8846_ldo_set_voltage(7,600,600);
		}			
		if(ret < 0){
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(2,3000,3000); error.");
				return ret;
			}	
		
		///////////////////////////////////////////////////////////////////////
		temp = Get_Volt_Val(PWR4_TP_VIO_INDEX);
		if(temp != 0)
		{
			ret = act8846_ldo_set_voltage(0,temp,temp);//TP_VBUS
		}
		else
		{
			ret = act8846_ldo_set_voltage(0,600,600);
		}			
		if(ret < 0){
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(3,1800,1800); error.");
				return ret;
			}

		///////////////////////////////////////////////////////////////////////
		temp = Get_Volt_Val(PWR1_IOVCC_INDEX);
		if(temp != 0)
		{
			ret = act8846_ldo_set_voltage(2,temp,temp);//LCD_DVDD
			
		}
		else
		{
			ret = act8846_ldo_set_voltage(2,600,600);		
		}
		
		if(ret < 0){
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(2,1800,1800); error.");
				return ret;
			}	


		//////////////////////////////////////////////////////////////////////
		temp = Get_Volt_Val(PWR2_VDDH_INDEX);
		if(temp != 0)
		{
			ret = act8846_ldo_set_voltage(4,temp,temp);//LCD_AVDD
		}
		else
		{
			ret = act8846_ldo_set_voltage(4,600,600);		
		}		
		if(ret < 0){
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(4,3000,3000); error.");
				return ret;
			}

		
		ret = act8846_ldo_set_voltage(3,3300,3300);//VSP/VSN_POWER
		if(ret < 0){
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(6,3300,3300); error.");
				return ret;
			}
		ret = act8846_ldo_set_voltage(8,1800,1800);//EXT_POWER3
		if(ret < 0)
		{
				TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(7,1800,1800); error.");
				return ret;
		}

		temp = Get_Volt_Val(PWR14_EXT_PWR_B_INDEX);			//ext2
		if(temp == 0)
		{
			ret = act8846_ldo_set_voltage(5,600,600);//EXT_POWER2
		}
		else
		{
			ret = act8846_ldo_set_voltage(5,temp,temp);//EXT_POWER2
		}
		if(ret < 0)
		{
			TESTER_MAIN_DEBUG("act8846_ldo_set_voltage(5,2500,2500); error.");
			return ret;
		}
		Delay_ms(5); 
	return 0;
}





int act8846_on(void){
	int ret;
	uint8_t i;

	for(i=0;i<4;i++)
	{
		ret = act8846_dcdc_enable(i);
		if(ret < 0){
			TESTER_MAIN_DEBUG("act8846_dcdc_disable error.");
			return ret;
		}
	}
	
	for(i=0;i<8;i++)
	{
		ret = act8846_ldo_enable(i);
		if(ret < 0){
			TESTER_MAIN_DEBUG("act8846_ldo_disable error.");
			return ret;
		}
	}	
	
	//TESTER_MAIN_DEBUG("status = %d\r\n",ret);			
	return 0;	
}


int act8846_off(void){
	int ret;
	uint8_t i;
	
	for(i=0;i<4;i++)
	{
		ret = act8846_dcdc_disable(i);
		if(ret < 0){
			TESTER_MAIN_DEBUG("act8846_dcdc_disable error.");
			return ret;
		}
	}
	
	for(i=0;i<9;i++)
	{
		ret = act8846_ldo_disable(i);
		if(ret < 0){
			TESTER_MAIN_DEBUG("act8846_ldo_disable error.");
			return ret;
		}
	}
	
	return 0;	
}


const static int buck_set_vol_base_addr[] = {
	act8846_BUCK1_SET_VOL_BASE,
	act8846_BUCK2_SET_VOL_BASE,
	act8846_BUCK3_SET_VOL_BASE,
	act8846_BUCK4_SET_VOL_BASE,
};

const static int buck_contr_base_addr[] = {
	act8846_BUCK1_CONTR_BASE,
 	act8846_BUCK2_CONTR_BASE,
 	act8846_BUCK3_CONTR_BASE,
 	act8846_BUCK4_CONTR_BASE,
};

#define act8846_BUCK_SET_VOL_REG(x) (buck_set_vol_base_addr[x])
#define act8846_BUCK_CONTR_REG(x) (buck_contr_base_addr[x])


const static int ldo_set_vol_base_addr[] = {
	act8846_LDO1_SET_VOL_BASE,
	act8846_LDO2_SET_VOL_BASE,
	act8846_LDO3_SET_VOL_BASE,
	act8846_LDO4_SET_VOL_BASE, 
	act8846_LDO5_SET_VOL_BASE, 
	act8846_LDO6_SET_VOL_BASE, 
	act8846_LDO7_SET_VOL_BASE, 
	act8846_LDO8_SET_VOL_BASE, 
//	act8846_LDO9_SET_VOL_BASE, 
};



const static int ldo_contr_base_addr[] = {
	act8846_LDO1_CONTR_BASE,
	act8846_LDO2_CONTR_BASE,
	act8846_LDO3_CONTR_BASE,
	act8846_LDO4_CONTR_BASE,
	act8846_LDO5_CONTR_BASE,
	act8846_LDO6_CONTR_BASE,
	act8846_LDO7_CONTR_BASE,
	act8846_LDO8_CONTR_BASE,
  	act8846_LDO9_CONTR_BASE,
};




#define act8846_LDO_SET_VOL_REG(x) (ldo_set_vol_base_addr[x])
#define act8846_LDO_CONTR_REG(x) (ldo_contr_base_addr[x])



const static int buck_voltage_map[] = 
{
	 600, 625, 650, 675, 700, 725, 750, 775,
	 800, 825, 850, 875, 900, 925, 950, 975,
	 1000, 1025, 1050, 1075, 1100, 1125, 1150,
	 1175, 1200, 1250, 1300, 1350, 1400, 1450,
	 1500, 1550, 1600, 1650, 1700, 1750, 1800, 
	 1850, 1900, 1950, 2000, 2050, 2100, 2150, 
	 2200, 2250, 2300, 2350, 2400, 2500, 2600, 
	 2700, 2800, 2900, 3000, 3100, 3200,
	 3300, 3400, 3500, 3600, 3700, 3800, 3900,
};

const static int ldo_voltage_map[] = 
{
	 600, 625, 650, 675, 700, 725, 750, 775,
	 800, 825, 850, 875, 900, 925, 950, 975,
	 1000, 1025, 1050, 1075, 1100, 1125, 1150,
	 1175, 1200, 1250, 1300, 1350, 1400, 1450,
	 1500, 1550, 1600, 1650, 1700, 1750, 1800, 
	 1850, 1900, 1950, 2000, 2050, 2100, 2150, 
	 2200, 2250, 2300, 2350, 2400, 2500, 2600, 
	 2700, 2800, 2900, 3000, 3100, 3200,
	 3300, 3400, 3500, 3600, 3700, 3800, 3900,
};




/*
 *
 */
 
u8 act8846_Read(u8 reg)
{
	u8 date = 0x5a;
 	IIC_Start();	
 	IIC_Send_Byte(act8846_I2C_7BIT_ADDRESS<<1);  
	IIC_Wait_Ack();
 	IIC_Send_Byte(reg);   	
	IIC_Wait_Ack();  
 	IIC_Start();  	 	   
	IIC_Send_Byte((act8846_I2C_7BIT_ADDRESS<<1)|0x01);   //读 - 地址最低位为1    写- 地址最低位为0
	IIC_Wait_Ack();	

	date = IIC_Read_Byte(); 

	IIC_Stop();
	return date;
}
/*
int act8846_i2c_read(uint8_t reg, uint8_t* pBuffer)
{
	*pBuffer = act8846_Read(reg);
	return 0;
}
*/


int act8846_i2c_read(uint8_t reg, uint8_t* pBuffer)
{
    int retval;
	retval = i2c1_start(act8846_I2C_7BIT_ADDRESS<<1);
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
	
	retval = i2c1_start_repeat((act8846_I2C_7BIT_ADDRESS<<1)|0x01);
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
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c1_stop Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	return I2C_OK;
}


u8 act8846_Write(u8 reg,u8 date)
{
	u8 ret=0;
	IIC_Start();	
 	IIC_Send_Byte(act8846_I2C_7BIT_ADDRESS<<1);   // 写- 地址最低位为0
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   	
	IIC_Wait_Ack();  
	IIC_Send_Byte(date);  	
	ret = IIC_Wait_Ack();
	if(ret)	return 255;  
	IIC_Stop();					
	return ret; 
}
/*
int act8846_i2c_write(uint8_t reg, uint8_t val)
{
	return act8846_Write(reg,val);
}
*/

int act8846_i2c_write(uint8_t reg, uint8_t val)
{
	int retval;
	
	retval = i2c1_start(act8846_I2C_7BIT_ADDRESS<<1);
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


int act8846_set_bits(uint8_t reg, uint8_t mask, uint8_t val)
{
	uint8_t tmp;
	int ret;

	ret = act8846_i2c_read(reg, &tmp);
	if(ret < 0)
	{
		return ret;
	}
	tmp = (tmp & ~mask) | val;
	ret = act8846_i2c_write(reg, tmp);
	if(ret < 0)
	{
		return ret;
	}
//	ret = act8846_i2c_read(reg, &tmp);
//	if(ret < 0){
//		return ret;
//	}
	return I2C_OK;//ret;	
}


int act8846_ldo_is_enabled(uint8_t ldo_id)
{
	int retval;
	uint8_t val;
	uint8_t mask=0x80;
	retval = act8846_i2c_read(act8846_LDO_CONTR_REG(ldo_id), &val);
	if (retval < 0)
		return retval;
	val=val&~0x7f;
	if (val & mask)
		return 1;
	else
		return 0; 	
}

int act8846_ldo_enable(uint8_t ldo_id)
{
	uint8_t mask=0x80;	
	//static int act8846_set_bits(uint8_t reg, uint8_t mask, uint8_t val)
	return act8846_set_bits(act8846_LDO_CONTR_REG(ldo_id), mask, 0x80);
}

int act8846_ldo_disable(uint8_t ldo_id)
{
	uint8_t mask=0x80;
	
	return act8846_set_bits(act8846_LDO_CONTR_REG(ldo_id), mask, 0);

}


int act8846_ldo_get_voltage(uint8_t ldo_id) //return mV
{
	uint8_t reg_val = 0;
	int val;
	int ret;
	//static int act8846_i2c_read(uint8_t reg, uint8_t* pBuffer)
	ret = act8846_i2c_read(act8846_LDO_SET_VOL_REG(ldo_id),&reg_val);
	if(ret < 0){
		TESTER_MAIN_DEBUG("act8846_i2c_read error.");
		return ret;
	}
	reg_val &= LDO_VOL_MASK;
	val = ldo_voltage_map[reg_val];	//mV
	return val;
}

int act8846_ldo_set_voltage(uint8_t ldo_id,int min_mV, int max_mV)
{
	int min_vol = min_mV, max_vol = max_mV;
	const int *vol_map =ldo_voltage_map;
	uint8_t val;
	int ret = 0;

	if (min_vol < vol_map[VOL_MIN_IDX] ||
	    min_vol > vol_map[VOL_MAX_IDX])
		return -2;

	for (val = VOL_MIN_IDX; val <= VOL_MAX_IDX; val++){
		if (vol_map[val] >= min_vol)
			break;	
        }
		
	if (vol_map[val] > max_vol)
		return -2;

	ret = act8846_set_bits(act8846_LDO_SET_VOL_REG(ldo_id),
	       	LDO_VOL_MASK, val);
	return ret;

}

unsigned int act8846_ldo_get_mode(uint8_t ldo_id)
{
	uint8_t mask = 0x80;
	uint8_t val;
	int ret;
	ret = act8846_i2c_read(act8846_LDO_CONTR_REG(ldo_id), &val);
	if(ret < 0){
		TESTER_MAIN_DEBUG("act8846_i2c_read error.");
		return ret;
	}
	val=val & mask;
	if (val== mask)
		return 1;	//ON
	else
		return 0;	//OFF
}

int act8846_ldo_set_mode(uint8_t ldo_id, uint8_t mode)	//1:ON, 0:OFF
{
	uint8_t mask = 0x80;
	switch(mode)
	{
	case 1:
		return act8846_set_bits(act8846_LDO_CONTR_REG(ldo_id), mask, mask);		
	case 0:
		return act8846_set_bits(act8846_LDO_CONTR_REG(ldo_id), mask, 0);
	default:
		TESTER_MAIN_DEBUG("error:pmu_act8846 only lowpower and nomal mode\n");
		return I2C_ERR;
	}
}

//static int act8846_dcdc_list_voltage(unsigned index)
//{
//	if (index >= ARRAY_SIZE(buck_voltage_map))
//		return -EINVAL;
//	return buck_voltage_map[index];
//}
int act8846_dcdc_is_enabled(uint8_t dcdc_id)
{
	uint8_t val;
	u16 mask=0x80;	
	int ret;

	ret = act8846_i2c_read(act8846_BUCK_CONTR_REG(dcdc_id), &val);
	if(ret < 0){
		TESTER_MAIN_DEBUG("act8846_i2c_read error.");
		return ret;
	}
  val=val&~0x7f;
	if (val & mask)
		return 1;
	else
		return 0; 	
}


int act8846_dcdc_enable(uint8_t dcdc_id)
{
	uint8_t mask=0x80;	
	//act8846_set_bits(uint8_t reg, uint8_t mask, uint8_t val)
	return act8846_set_bits(act8846_BUCK_CONTR_REG(dcdc_id), mask, 0x80);
}



int act8846_dcdc_disable(uint8_t dcdc_id)
{
	uint8_t mask=0x80;
	return act8846_set_bits(act8846_BUCK_CONTR_REG(dcdc_id), mask, 0);
}



int act8846_dcdc_get_voltage(uint8_t dcdc_id)
{
	uint8_t reg_val = 0;
	int val;
	int ret;
	ret = act8846_i2c_read(act8846_BUCK_SET_VOL_REG(dcdc_id),&reg_val);
	if(ret < 0){
		TESTER_MAIN_DEBUG("act8846_i2c_read error.");
		return ret;
	}
	reg_val &= BUCK_VOL_MASK;
	val = buck_voltage_map[reg_val];	
	return val;
}



int act8846_dcdc_set_voltage(uint8_t dcdc_id,int min_mV, int max_mV)
{
	int min_vol = min_mV, max_vol = max_mV;
	const int *vol_map = buck_voltage_map;
	uint8_t val;
	int ret = 0;

	if (min_vol < vol_map[VOL_MIN_IDX] ||
	    min_vol > vol_map[VOL_MAX_IDX])
		return -2;

	for (val = VOL_MIN_IDX; val <= VOL_MAX_IDX; val++){
		if (vol_map[val] >= min_vol)
			break;
        }

	if (vol_map[val] > max_vol)
		TESTER_MAIN_DEBUG("WARNING:this voltage is not support!voltage set is %d mv\n",vol_map[val]);

	ret = act8846_set_bits(act8846_BUCK_SET_VOL_REG(dcdc_id),BUCK_VOL_MASK, val);
	if(ret < 0)
		TESTER_MAIN_DEBUG("##################:set voltage error!voltage set is %d mv\n",vol_map[val]);
	
	return ret;
}

int act8846_dcdc_set_sleep_voltage(uint8_t dcdc_id, int mV)
{
	int min_vol = mV,max_vol = mV;
	const int *vol_map = buck_voltage_map;
	uint8_t val;
	int ret = 0;

	if (min_vol < vol_map[VOL_MIN_IDX] ||
	    min_vol > vol_map[VOL_MAX_IDX])
		return -2;

	for (val = VOL_MIN_IDX; val <= VOL_MAX_IDX; val++){
		if (vol_map[val] >= min_vol)
			break;
        }

	if (vol_map[val] > max_vol)
		TESTER_MAIN_DEBUG("WARNING:this voltage is not support!voltage set is %d mv\n",vol_map[val]);

	ret = act8846_set_bits((act8846_BUCK_SET_VOL_REG(dcdc_id) +0x01), BUCK_VOL_MASK, val);
	return ret;
}

unsigned int act8846_dcdc_get_mode(uint8_t dcdc_id)
{
	uint8_t mask = 0x80;
	uint8_t val;
	int ret;
	
	ret = act8846_i2c_read(act8846_BUCK_CONTR_REG(dcdc_id),&val);
	if(ret < 0){
		TESTER_MAIN_DEBUG("act8846_i2c_read error.");
		return ret;
	}
	val=val & mask;
	if (val== mask)
		return 1; //ON
	else
		return 0; //OFF

}


int act8846_dcdc_set_mode(uint8_t dcdc_id, uint8_t mode) ////1:ON, 0:OFF
{
	uint8_t mask = 0x80;

	switch(mode)
	{
	case 0:
		return act8846_set_bits(act8846_BUCK_CONTR_REG(dcdc_id), mask, 0);
	case 1:
		return act8846_set_bits(act8846_BUCK_CONTR_REG(dcdc_id), mask, mask);
	default:
		TESTER_MAIN_DEBUG("error:pmu_act8846 only powersave and pwm mode\n");
		return -1;
	}
}

void act8846_device_shutdown(void)
{
	TESTER_MAIN_DEBUG("%s\n",__func__);
	
#if 1
	act8846_off();
	Delay_ms(100);
	
#else
	ret = act8846_reg_read(act8846,0xc3);
	if (ret < 0) {
		TESTER_MAIN_DEBUG("act8846 set 0xc3 error!\n");
		return I2C_ERR;
	}
	ret = act8846_set_bits(act8846, 0xc3,(0x1<<3),(0x1<<3));
	if (ret < 0) {
		TESTER_MAIN_DEBUG("act8846 set 0xc3 error!\n");
		return I2C_ERR;
	}
	ret = act8846_set_bits(act8846, 0xc3,(0x1<<4),(0x1<<4));
	if (ret < 0) {
		TESTER_MAIN_DEBUG("act8846 set 0xc3 error!\n");
		return I2C_ERR;
	}
#endif
}


//latancy function, to compatable with older tester
void poweron(float vdd, float vbus, float vled, float vddtx)
{

}

void updateVled(float vled)
{

}

void poweroff(void)
{

}




