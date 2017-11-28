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
#include "sys.h"



/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define	 OK				0
#define	 ERR			-1

///////////////////////////////////////////////////////////////////////////////////
#define  VSP_ON()							PCout(8) = 1	
#define  VSP_OFF()	 					PCout(8) = 0	
#define  VSN_ON()							PCout(9) = 1	
#define  VSN_OFF()						PCout(9) = 0	
#define  VSP_PWR_ON()					PGout(1) = 1	
#define  VSP_PWR_OFF()	 			PGout(1) = 0	

//////////////////////////////////////////////////////////////////////////////////
#define  BL_ADJ_OFF()   						PEout(6) = 1	
#define  BL_ADJ_ON()    						PEout(6) = 0	
#define	 BL_POWER_ON()   		 				PEout(4) = 1		// 开启背光芯片电源	
#define	 BL_POWER_OFF()   					PEout(4) = 0			// 关闭背光芯片电源	
#define	 OLEDLCD_MAIN_POWER_ON()   	PGout(3) = 1		//OLED ON
#define	 OLEDLCD_MAIN_POWER_OFF()   PGout(3) = 0			//LCD ON
#define  BL_DETECT_ON()							PGout(2) = 1		//背光检测使能
#define  BL_DETECT_OFF()						PGout(2) = 0			


//////////////////////////////////////////////////////////////////////////////////
#define	 TPS22993_TPVIO_ON_OFF(x)			PDout(0) = x	
#define	 TPS22993_IOVCC_ON_OFF(x)			PDout(1) = x	
#define	 TPS22993_VSP_VSN_ON_OFF(x)		PDout(2) = x
#define	 TPS22993_VDDH_ON_OFF(x)			PDout(3) = x										
#define	 TPS22993_EXT2_ON_OFF(x)			PDout(4) = x	
#define	 TPS22993_EXT1_ON_OFF(x)			PDout(5) = x	
#define	 TPS22993_TPVDD_ON_OFF(x)			PDout(6) = x	
#define	 TPS22993_EXT3_ON_OFF(x)			PDout(8) = x	
//////////////////////////////////////////////////////////////////////////////////
#define	 RELAY_UA_IOVCC_ON_OFF(x)			PGout(4) = x	
#define	 RELAY_UA_VDDH_ON_OFF(x)			PGout(5) = x	
#define	 RELAY_UA_VSP_ON_OFF(x)				PGout(6) = x	
#define	 RELAY_UA_VSN_ON_OFF(x)				PGout(7) = x	

#define  RELAY_UA_ALL_ON_OFF(x)		if(x != 0)	{RELAY_UA_IOVCC_ON_OFF(1);RELAY_UA_VDDH_ON_OFF(1);RELAY_UA_VSP_ON_OFF(1);RELAY_UA_VSN_ON_OFF(1);}\
									else		{RELAY_UA_IOVCC_ON_OFF(0);RELAY_UA_VDDH_ON_OFF(0);RELAY_UA_VSP_ON_OFF(0);RELAY_UA_VSN_ON_OFF(0);}

//////////////////////////////////////////////////////////////////////////////////
#define	 DETECT_OLED_AVDD_ON_OFF(x)			PEout(15) = x	
#define	 DETECT_VSP_ON_OFF(x)						PEout(3) = x
#define	 DETECT_LEDP_ON_OFF(x)					PEout(8) = x	
#define	 DETECT_LED1N_ON_OFF(x)					PEout(9) = x									
#define	 DETECT_LED2N_ON_OFF(x)					PEout(10) = x	
#define	 DETECT_LED3N_ON_OFF(x)					PEout(11) = x	

//////////////////////////////////////////////////////////////////////////////////
#define	 OTP_EN_ON_OFF(x)							PEout(14) = x	
#define	 OTP_P_ON_OFF(x)							PEout(13) = x	
#define	 DETECT_OTP_P_ON_OFF(x)				PEout(12) = x	


#define	 VA_ERR_ON_OFF(x)							PCout(5) = x	
//////////////////////////////////////////////////////////////////////////////////
#define	 USR_LED_ON_OFF(x)						PBout(3) = x	


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




void ACT8846_DCDC_Switch(ACT8846_DCDC_CHN_TypeDef chn,ACT8846_SWITCH_TypeDef onoff);
void ACT8846_LDO_Switch(ACT8846_LDO_CHN_TypeDef chn,ACT8846_SWITCH_TypeDef onoff);







#endif

