/*********************************************************************************************************
* 文件名称: GPIO_Config.h
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: Cth
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/
#ifndef __GPIO_Config_H
#define	__GPIO_Config_H





/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "stm32f10x.h"




/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define	 OK				0
#define	 ERR			-1



///////////////////////////////////////////////////////////////////////////////////
#define  VSP_ON()					GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define  VSP_OFF()	 				GPIO_ResetBits(GPIOC,GPIO_Pin_8)
#define  VSN_ON()					GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define  VSN_OFF()					GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define  VSP_PWR_ON()				GPIO_SetBits(GPIOG,GPIO_Pin_1)
#define  VSP_PWR_OFF()	 			GPIO_ResetBits(GPIOG,GPIO_Pin_1)


//////////////////////////////////////////////////////////////////////////////////
#define  BL_ADJ_OFF()   			GPIO_SetBits(GPIOE,GPIO_Pin_6)
#define  BL_ADJ_ON()    			GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define	 BL_POWER_ON()   		 	GPIO_SetBits(GPIOE, GPIO_Pin_4);		// 开启背光芯片电源	
#define	 BL_POWER_OFF()   			GPIO_ResetBits(GPIOE, GPIO_Pin_4);		// 关闭背光芯片电源	
#define	 OLEDLCD_MAIN_POWER_ON()   	GPIO_SetBits(GPIOG, GPIO_Pin_3);		//OLED ON
#define	 OLEDLCD_MAIN_POWER_OFF()   GPIO_ResetBits(GPIOG, GPIO_Pin_3);		//LCD ON

#define  BL_DETECT_ON()				GPIO_SetBits(GPIOG, GPIO_Pin_2);		//背光检测使能
#define  BL_DETECT_OFF()			GPIO_ResetBits(GPIOG, GPIO_Pin_2);		




//////////////////////////////////////////////////////////////////////////////////
#define	 TPS22993_TPVIO_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_0);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_0) 
									
#define	 TPS22993_IOVCC_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_1);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_1) 

#define	 TPS22993_VSP_VSN_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_2);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_2) 

#define	 TPS22993_VDDH_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_3);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_3) 										

#define	 TPS22993_EXT2_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_4);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_4)	

#define	 TPS22993_EXT1_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_5);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_5)

#define	 TPS22993_TPVDD_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_6);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_6) 

#define	 TPS22993_EXT3_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOD,GPIO_Pin_8);\
									else		GPIO_ResetBits(GPIOD,GPIO_Pin_8)	


//////////////////////////////////////////////////////////////////////////////////
#define	 RELAY_UA_IOVCC_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOG,GPIO_Pin_4);\
									else		GPIO_ResetBits(GPIOG,GPIO_Pin_4) 

#define	 RELAY_UA_VDDH_ON_OFF(x)	if(x != 0)	GPIO_SetBits(GPIOG,GPIO_Pin_5);\
									else		GPIO_ResetBits(GPIOG,GPIO_Pin_5) 

#define	 RELAY_UA_VSP_ON_OFF(x)		if(x != 0)	GPIO_SetBits(GPIOG,GPIO_Pin_6);\
									else		GPIO_ResetBits(GPIOG,GPIO_Pin_6) 

#define	 RELAY_UA_VSN_ON_OFF(x)		if(x != 0)	GPIO_SetBits(GPIOG,GPIO_Pin_7);\
									else		GPIO_ResetBits(GPIOG,GPIO_Pin_7)

									
#define  RELAY_UA_ALL_ON_OFF(x)		if(x != 0)	{RELAY_UA_IOVCC_ON_OFF(1);RELAY_UA_VDDH_ON_OFF(1);RELAY_UA_VSP_ON_OFF(1);RELAY_UA_VSN_ON_OFF(1);}\
									else		{RELAY_UA_IOVCC_ON_OFF(0);RELAY_UA_VDDH_ON_OFF(0);RELAY_UA_VSP_ON_OFF(0);RELAY_UA_VSN_ON_OFF(0);}

//////////////////////////////////////////////////////////////////////////////////
#define	 DETECT_OLED_AVDD_ON_OFF(x)	if(x != 0)	GPIO_ResetBits(GPIOE,GPIO_Pin_15);\
									else		GPIO_SetBits(GPIOE,GPIO_Pin_15) 

#define	 DETECT_VSP_ON_OFF(x)		if(x != 0)	GPIO_ResetBits(GPIOE,GPIO_Pin_3);\
									else		GPIO_SetBits(GPIOE,GPIO_Pin_3) 
										

#define	 DETECT_LEDP_ON_OFF(x)		if(x != 0)	GPIO_ResetBits(GPIOE,GPIO_Pin_8);\
									else		GPIO_SetBits(GPIOE,GPIO_Pin_8) 

#define	 DETECT_LED1N_ON_OFF(x)		if(x != 0)	GPIO_ResetBits(GPIOE,GPIO_Pin_9);\
									else		GPIO_SetBits(GPIOE,GPIO_Pin_9) 									

#define	 DETECT_LED2N_ON_OFF(x)		if(x != 0)	GPIO_ResetBits(GPIOE,GPIO_Pin_10);\
									else		GPIO_SetBits(GPIOE,GPIO_Pin_10)	

#define	 DETECT_LED3N_ON_OFF(x)		if(x != 0)	GPIO_ResetBits(GPIOE,GPIO_Pin_11);\
									else		GPIO_SetBits(GPIOE,GPIO_Pin_11) 


//////////////////////////////////////////////////////////////////////////////////
#define	 OTP_EN_ON_OFF(x)			if(x != 0)	GPIO_SetBits(GPIOE,GPIO_Pin_14);\
									else		GPIO_ResetBits(GPIOE,GPIO_Pin_14) 

#define	 OTP_P_ON_OFF(x)			if(x != 0)	GPIO_SetBits(GPIOE,GPIO_Pin_13);\
									else		GPIO_ResetBits(GPIOE,GPIO_Pin_13) 

#define	 DETECT_OTP_P_ON_OFF(x)		if(x != 0)	GPIO_ResetBits(GPIOE,GPIO_Pin_12);\
									else		GPIO_SetBits(GPIOE,GPIO_Pin_12) 



#define	 VA_ERR_ON_OFF(x)			if(x != 0)	GPIO_ResetBits(GPIOC,GPIO_Pin_5);\
									else		GPIO_SetBits(GPIOC,GPIO_Pin_5) 

//////////////////////////////////////////////////////////////////////////////////
#define	 USR_LED_ON_OFF(x)			if(x != 0)	GPIO_ResetBits(GPIOB,GPIO_Pin_3);\
									else		GPIO_SetBits(GPIOB,GPIO_Pin_3) 



/*********************************************************************************************************
*                                              结构体定义
*********************************************************************************************************/
typedef enum 
{
	DCDC_SW1 = 0,
	DCDC_SW2,
	DCDC_SW3,							
	DCDC_SW4,								
}ACT8846_DCDC_CHN_TypeDef;


typedef enum 
{
	LDO_SW1 = 0,			//对应OUTPUT5
	LDO_SW2,
	LDO_SW3,							
	LDO_SW4,	
	LDO_SW5,
	LDO_SW6,							
	LDO_SW7,
	LDO_SW8,
}ACT8846_LDO_CHN_TypeDef;


typedef enum 
{
	ACT8846_SWITCH_OFF = 0, 		//对应OUTPUT5
	ACT8846_SWITCH_ON,

}ACT8846_SWITCH_TypeDef;












/*********************************************************************************************************
*                                              API函数定义
*********************************************************************************************************/
void Set_PMU_RST_IO(u8 value);
void Set_PMU_PWREN_IO(u8 value);
void  System_GPIO_Config(void);
void TPS22993_PWR_ON_Cfg(void);
void LT3582_PWR_ON_Cfg(void);
void LM36923_PWR_ON_Cfg(u16 val);	
void AD715_PWR_ON_Cfg(void);		













#endif

