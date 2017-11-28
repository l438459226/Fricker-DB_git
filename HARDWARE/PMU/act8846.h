/* include/linux/regulator/act8846.h
 *
 * Copyright (C) 2011 ROCKCHIP, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "myiic.h"
#include "stm32f10x.h"
 
#ifndef __LINUX_REGULATOR_act8846_H
#define __LINUX_REGULATOR_act8846_H

//#define ACT8846_START 30

#define ACT8846_DCDC1  0                     //(0+ACT8846_START) 

#define ACT8846_LDO1 4                //(4+ACT8846_START)


#define act8846_NUM_REGULATORS 12
int act8846_i2c_read(uint8_t reg, uint8_t* pBuffer);
int act8846_i2c_write(uint8_t reg, uint8_t val);
int act8846_set_bits(uint8_t reg, uint8_t mask, uint8_t val);
void act8846_device_shutdown(void);

//4 DCDC
int act8846_dcdc_is_enabled(uint8_t dcdc_id);
int act8846_dcdc_enable(uint8_t dcdc_id);
int act8846_dcdc_disable(uint8_t dcdc_id);
int act8846_dcdc_get_voltage(uint8_t dcdc_id);
int act8846_dcdc_set_voltage(uint8_t dcdc_id,int min_mV, int max_mV);
int act8846_dcdc_set_sleep_voltage(uint8_t dcdc_id, int mV);
unsigned int act8846_dcdc_get_mode(uint8_t dcdc_id);
int act8846_dcdc_set_mode(uint8_t dcdc_id, uint8_t mode);////1:ON, 0:OFF

//8 LDO
int act8846_ldo_is_enabled(uint8_t ldo_id);
int act8846_ldo_enable(uint8_t ldo_id);
int act8846_ldo_disable(uint8_t ldo_id);
int act8846_ldo_get_voltage(uint8_t ldo_id); //return mV
int act8846_ldo_set_voltage(uint8_t ldo_id,int min_mV, int max_mV);
unsigned int act8846_ldo_get_mode(uint8_t ldo_id);
int act8846_ldo_set_mode(uint8_t ldo_id, uint8_t mode);	//1:ON, 0:OFF

int act8846_init(void);
int act8846_on(void);
int act8846_off(void);

//latancy function, to compatable with older tester
void poweron(float vdd, float vbus, float vled, float vddtx);
void updateVled(float vled);
void poweroff(void);

#endif

