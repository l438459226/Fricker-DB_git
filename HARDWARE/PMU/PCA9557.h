/*********************************************************************************************************
* 文件名称: PCA9557.h
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: CMS
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/
#ifndef __PCA9557_H
#define __PCA9557_H



/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "stm32f10x.h"




/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define  PCA9557_DEVEICE0_ADD		0x18			//PCA9557设备1地址
#define  PCA9557_DEVEICE1_ADD		0x1C

#define  PCA9557_OUTPUT_REG			0x01
#define  PCA9557_MODE_REG			0x03


#define  IO_SWITCH_ON				0x01
#define  IO_SWITCH_OFF				0x00







/*********************************************************************************************************
*                                              结构体定义
*********************************************************************************************************/
typedef enum 
{
	IO_DEV1_NONE = 0,
	IO_DEV1_LCD_VMTP_ON,				//产生VMTP电压
	IO_DEV1_VSP_OUT_ON,
	IO_DEV1_BL_ON,
	IO_DEV1_DC9V_OUT_ON,
	IO_DEV1_RL1_ON,
	IO_DEV1_RL2_ON,
	IO_DEV1_RL3_ON,
	IO_DEV1_RL4_ON,
	IO_DEV1_ALL_ON,
	IO_DEV1_MAX,	
}IO_EXPANDER_DEV1_TypeDef;



typedef enum 
{
	IO_DEV0_NONE = 0,
	IO_DEV0_BL_LEDP_EN,
	IO_DEV0_BL_LED1N_EN,
	IO_DEV0_BL_LED2N_EN,
	IO_DEV0_BL_LED3N_EN,
	IO_DEV0_LCD_VMTP_EN,				//使能VMTP输出
	IO_DEV0_LCD_VMTP_ON,
	IO_DEV0_I2C_EXIO,
	IO_DEV0_ADC_LED_PWR,
	IO_DEV0_ALL_EN,
	IO_DEV0_MAX,	
}IO_EXPANDER_DEV0_TypeDef;





/*********************************************************************************************************
*                                              API函数定义
*********************************************************************************************************/
void PCA9557_Sel_Dev(u8 val);
u8 PCA9557_Dev0_Set(IO_EXPANDER_DEV0_TypeDef tpye,u8 val);
u8 PCA9557_Dev1_Set(IO_EXPANDER_DEV1_TypeDef tpye,u8 val);
void PCA9557_Init(void);
int PCA9557_I2c_write(u8 reg,u8 val);



















#endif

