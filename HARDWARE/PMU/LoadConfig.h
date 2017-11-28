/*********************************************************************************************************
* 文件名称: LoadConfig.h
* 文件作者: zq
* 当前版本: V1.01
* 所属模块: 
* 完成日期: 2016年09月06日
* 文件摘要: 
* 注意事项:	
* 更改说明: 
**********************************************************************************************************/
#ifndef __LOADCONFIG_H
#define	__LOADCONFIG_H

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "stm32f10x.h"




/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define	REG_GENERAL_INFO_ADD 			(u32)0x0010		//常规信息
#define	REG_PATTERN_SET_ADD 			(u32)0x0018		//图片设置
#define	REG_BOARD_SET_ADD 				(u32)0x0020		//FPGA设置
#define	REG_POWER_ON_ADD 				(u32)0x0028		//上电时序
#define	REG_INIT_CODE_ADD 				(u32)0x0030		//LCD设置
#define	REG_POWER_OFF_ADD 				(u32)0x0038		//关电时序
#define	REG_SLEEP_SEQ_ADD 				(u32)0x0040		//睡眠时序
#define	REG_INIT2_CODE_ADD 				(u32)0x0048		//睡眠时序
#define	REG_CMD_CODE1_ADD 				(u32)0x0050		//CMD1
#define	REG_CMD_CODE2_ADD 				(u32)0x0058		//CMD2
#define	REG_CMD_CODE3_ADD 				(u32)0x0060		//CMD3
#define	REG_CMD_CODE4_ADD 				(u32)0x0067		//CMD4




#define	REG_RESOLUTION				(u8)248
#define	REG_PATTERN_CTRL			(u8)247
#define	REG_PATTERN_RGB				(u8)246
#define	REG_DELAY					(u8)255
#define	REG_SET_VOLTAGE				(u8)254
#define	REG_VOLTAGE_HIGH			(u8)253
#define	REG_VOLTAGE_LOW				(u8)252
#define	REG_CURRENT_HIGH			(u8)251
#define	REG_CURRENT_LOW				(u8)250
#define	REG_POWER_SEQ				(u8)249
#define	REG_SHOW_PIC				(u8)244
#define	REG_OPTION					(u8)245
#define	REG_SLEEP_SCAN				(u8)243		//CONFIG 1.25版本后增加了自动睡眠电流测试和lcd扫描方式的控制




#define POWER_ON		0x01
#define POWER_OFF		0x00

//大电流检测
#define	PWR1_IOVCC_A_DECT_HIGH		1000			//表示最大电流为100.0ma
#define	PWR2_VDDH_A_DECT_HIGH		1000			//表示最大电流为100.0ma
#define	PWR3_TP_VDD_A_DECT_HIGH		1000
#define	PWR4_TP_VIO_A_DECT_HIGH		1000
#define	PWR5_VSP_AVDD_A_DECT_HIGH	1000
#define	PWR6_VSN_AVEE_A_DECT_HIGH	1000








#define DEBUG_BIN_CFG	0x01


/*********************************************************************************************************
*                                              结构体定义
*********************************************************************************************************/
typedef enum 
{

	READ_START_ADD_NONE = 0,
	READ_START_ADD_INFO,				
	READ_START_ADD_PATTEN,
	READ_START_ADD_BOARD,
	READ_START_ADD_POWER_ON,
	READ_START_ADD_INIT_CODE,
	READ_START_ADD_POWER_OFF,
	READ_START_ADD_SLEEP_SEQ,
	READ_START_ADD_INIT2_COD,
	READ_START_ADD_CMD_CODE1,
	READ_START_ADD_CMD_CODE2,
	READ_START_ADD_CMD_CODE3,
	READ_START_ADD_CMD_CODE4,
	READ_START_ADD_MAX,	
	
}READ_START_ADD_TypeDef;


typedef enum 
{
	PWR1_IOVCC_INDEX = 0,
	PWR2_VDDH_INDEX,
	PWR3_TP_VDD_INDEX, 
	PWR4_TP_VIO_INDEX,
	PWR5_VSP_AVDD_INDEX,
	PWR6_VSN_AVEE_INDEX,
	PWR7_LED1N_INDEX,
	PWR8_LED2N_INDEX,
	PWR9_LED3N_INDEX,
	PWR10_VMTP_INDEX,
	PWR11_VNEG_INDEX,
	PWR12_LED_PWR_5V_INDEX,
	PWR13_EXT_PWR_A_INDEX,
	PWR14_EXT_PWR_B_INDEX,
	PWR15_TESTER_VBUS_INDEX,
	
}POWER_CHANNEL_INDEX_TypeDef;




typedef enum 
{
	POWER_CTRL_OFF = 0,
	POWER_CTRL_ON,				
	POWER_CTRL_PROTECT_ON,
	POWER_CTRL_PROTECT_OFF,
	POWER_CTRL_MAX,		
}POWER_CTRL_TypeDef;



typedef enum 
{
	DETECT_POWER_IOVCC = 0,
	DETECT_POWER_LCDVDD,
	DETECT_POWER_TPVIO,
	DETECT_POWER_TPVDD,
	DETECT_POWER_VSP,
	DETECT_POWER_VSN,
	DETECT_POWER_LEDN1,
	DETECT_POWER_LEDN2,
	DETECT_POWER_LEDN3,
	DETECT_POWE_MAX,		
}DETECT_POWER_TypeDef;


//OPTION PART
typedef enum 
{
	CURRENT_EN_INDEX = 0,
	VOLTAGE_EN_INDEX,
	SLEEP_CURRENT_EN_INDEX,
	HARD_ID_EN_INDEX,
	HARD_ID_VAL_INDEX,
	SOFT_ID_EN_INDEX,
	SOFT_ID12_VAL_INDEX,
	SOFT_ID34_VAL_INDEX,
	OTP_EN_INDEX,
	OTP_HIGH_INDEX,
	OTP_LOW_INDEX,
	DUIWEI_EN_INDEX,
	PWM_EN_INDEX,
	PWM_SEQ_HIGH_INDEX,
	PWM_SEQ_LOW_INDEX,
	PWM_DUTY_HIGH_INDEX,
	PWM_DUTY_LOW_INDEX,
	PWM_VOLT_HIGH_INDEX,
	PWM_VOLT_LOW_INDEX,
	TE_EN_INDEX,
	TE_SEQ_HIGH_INDEX,
	TE_SEQ_LOW_INDEX,
	TE_DUTY_HIGH_INDEX,
	TE_DUTY_LOW_INDEX,
	TE_VOLT_HIGH_INDEX,
	TE_VOLT_LOW_INDEX,	
	BK_EN_INDEX,
	KEY_ONOFF_SHORT_EN,
	KEY_ONOFF_LONG_EN,
	KEY_ONOFF_SHORT_PRESS,
	KEY_ONOFF_LONG_PRESS,
	KEY_LEFT_SHORT_EN,
	KEY_LEFT_LONG_EN,
	KEY_LEFT_SHORT_PRESS,
	KEY_LEFT_LONG_PRESS,
	KEY_AUTO_SHORT_EN,
	KEY_AUTO_LONG_EN,
	KEY_AUTO_SHORT_PRESS,
	KEY_AUTO_LONG_PRESS,
	KEY_RIGHT_SHORT_EN,
	KEY_RIGHT_LONG_EN,
	KEY_RIGHT_SHORT_PRESS,
	KEY_RIGHT_LONG_PRESS,
	KEY_TEST_SHORT_EN,
	KEY_TEST_LONG_EN,
	KEY_TEST_SHORT_PRESS,
	KEY_TEST_LONG_PRESS,
	KEY_LOCK_PIC_TIME,
	PWR1_IOVCC_V_HIGH_INDEX,
	PWR2_VDDH_V_HIGH_INDEX,
	PWR3_TP_VDD_V_HIGH_INDEX, 
	PWR4_TP_VIO_V_HIGH_INDEX,
	PWR5_VSP_AVDD_V_HIGH_INDEX,
	PWR6_VSN_AVEE_V_HIGH_INDEX,
	PWR7_LED1N_V_HIGH_INDEX,
	PWR8_LED2N_V_HIGH_INDEX,
	PWR9_LED3N_V_HIGH_INDEX,
	PWR1_IOVCC_A_HIGH_INDEX,
	PWR2_VDDH_A_HIGH_INDEX,
	PWR3_TP_VDD_A_HIGH_INDEX, 
	PWR4_TP_VIO_A_HIGH_INDEX,
	PWR5_VSP_AVDD_A_HIGH_INDEX,
	PWR6_VSN_AVEE_A_HIGH_INDEX,
	PWR7_LED1N_A_HIGH_INDEX,
	PWR8_LED2N_A_HIGH_INDEX,
	PWR9_LED3N_A_HIGH_INDEX,
	PWR1_IOVCC_UA_HIGH_INDEX,
	PWR2_VDDH_UA_HIGH_INDEX,
	PWR3_TP_VDD_UA_HIGH_INDEX, 
	PWR4_TP_VIO_UA_HIGH_INDEX,
	PWR5_VSP_AVDD_UA_HIGH_INDEX,
	PWR6_VSN_AVEE_UA_HIGH_INDEX,
	PWR7_LED1N_UA_HIGH_INDEX,
	PWR8_LED2N_UA_HIGH_INDEX,
	PWR9_LED3N_UA_HIGH_INDEX,
	PWR1_IOVCC_V_LOW_INDEX,
	PWR2_VDDH_V_LOW_INDEX,
	PWR3_TP_VDD_V_LOW_INDEX, 
	PWR4_TP_VIO_V_LOW_INDEX,
	PWR5_VSP_AVDD_V_LOW_INDEX,
	PWR6_VSN_AVEE_V_LOW_INDEX,
	PWR7_LED1N_V_LOW_INDEX,
	PWR8_LED2N_V_LOW_INDEX,
	PWR9_LED3N_V_LOW_INDEX,
	PWR1_IOVCC_A_LOW_INDEX,
	PWR2_VDDH_A_LOW_INDEX,
	PWR3_TP_VDD_A_LOW_INDEX, 
	PWR4_TP_VIO_A_LOW_INDEX,
	PWR5_VSP_AVDD_A_LOW_INDEX,
	PWR6_VSN_AVEE_A_LOW_INDEX,
	PWR7_LED1N_A_LOW_INDEX,
	PWR8_LED2N_A_LOW_INDEX,
	PWR9_LED3N_A_LOW_INDEX,
	PWR1_IOVCC_UA_LOW_INDEX,
	PWR2_VDDH_UA_LOW_INDEX,
	PWR3_TP_VDD_UA_LOW_INDEX, 
	PWR4_TP_VIO_UA_LOW_INDEX,
	PWR5_VSP_AVDD_UA_LOW_INDEX,
	PWR6_VSN_AVEE_UA_LOW_INDEX,
	PWR7_LED1N_UA_LOW_INDEX,
	PWR8_LED2N_UA_LOW_INDEX,
	PWR9_LED3N_UA_LOW_INDEX,
}OPTION_INDEX_TypeDef;






typedef struct
{
	u16 Resolution_X; 
	u16 Resolution_Y;
	u16	Version;
	u8  Bin_Name[50];	
}PARA_STRUCT_TypeDef;




typedef struct
{
	u8  Pattern_Index_Buf[60];
	u16 Pic_Name_Buf[60]; 				//图片索引位置
	u32 Pure_Color_Index_Buf[60]; 
	
	u8 Pic_Index_Max; 					//索引最大值
	
}PATTERN_STRUCT_TypeDef;










extern PATTERN_STRUCT_TypeDef  PatternManage;





void Initial_Code_Handle(void);
void Load_Initial_Code(void);
u16 Get_Volt_Val(POWER_CHANNEL_INDEX_TypeDef index);
void  FPGA_Key_On(void);
void  FPGA_Key_Off(void);
u8 Get_Sleep_Status(void);
void Set_Sleep_Status(u8 val);
u8 Get_Power_Status(void);
void Set_Power_Status(u8 val);


void  FPGA_Sleep_Off(void);
void System_Power_On_All(void);
void System_Power_Off_All(void);



u8 Get_Big_Current_Err(void);
void Big_Current_Dect(void);
u8 Get_Detect_Power_Status(DETECT_POWER_TypeDef detect);
void  FPGA_Key_Long_On(void); 
void  Special_Init2_On(void);
void Get_Option_Para(OPTION_INDEX_TypeDef option,u16 *val);
void Option_Test(void);







#endif













