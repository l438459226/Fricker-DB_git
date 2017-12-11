#ifndef __PARM_H
#define __PARM_H	

#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define LED0 PBout(0)	// PB0
#define LED1 PBout(1)	// PB1	

#define RIIC_CLK    		PBin(6)		//GT9147????
#define RIIC_DAT    		PBin(7)		//GT9147????	
#define RIIC_DATT 			PBout(7)

typedef  u32 (*Interface)();


typedef struct
{
	u16 G_Select_Port;
	u16 G_disp_width;
	u16 G_disp_height;
	
	u16 IOVCC; 
	u16 VDDH; 
	u16 TP_VDD; 
	u16 TP_VIO;
	u16 VSP;
	u16 VSN;
	u16 VMTP;
	u16 VNEG;
	u16 EXT_PWRA;
	u16 EXT_PWRB;
	u16 TESTER_VBUS;
	u16 BL1N;
	u16 BL2N;
	u16 BL3N;
	u16 LED_5V;
	
	u16 PWR1_IOVCC_HIGH;	//电压上下线
	u16 PWR1_IOVCC_LOW;
	u16 PWR2_VDDH_HIGH; 
	u16 PWR2_VDDH_LOW;
	u16 PWR3_TP_VDD_HIGH; 
	u16 PWR3_TP_VDD_LOW; 
	u16 PWR4_TP_VIO_HIGH;
	u16 PWR4_TP_VIO_LOW;
	u16 PWR5_VSP_AVDD_HIGH;
	u16 PWR5_VSP_AVDD_LOW;
	u16 PWR6_VSN_AVEE_HIGH;
	u16 PWR6_VSN_AVEE_LOW;
	u16 PWR7_LED1N_HIGH;
	u16 PWR7_LED1N_LOW;
	u16 PWR8_LED2N_HIGH;
	u16 PWR8_LED2N_LOW;
	u16 PWR9_LED3N_HIGH;
	u16 PWR9_LED3N_LOW;
	u16 PWR10_VMTP_HIGH;
	u16 PWR10_VMTP_LOW;
	u16 PWR11_VNEG_HIGH;
	u16 PWR11_VNEG_LOW;
	u16 PWR12_LED_PWR_5V_HIGH;
	u16 PWR12_LED_PWR_5V_LOW;
	u16 PWR13_EXT_PWR_A_HIGH;
	u16 PWR13_EXT_PWR_A_LOW;
	u16 PWR14_EXT_PWR_B_HIGH;
	u16 PWR14_EXT_PWR_B_LOW;
	u16 PWR15_TESTER_VBUS_HIGH;
	u16 PWR15_TESTER_VBUS_LOW;
	
	u16 PWR1_IOVCC_A_HIGH;	//电流上下线
	u16 PWR1_IOVCC_A_LOW;
	u16 PWR2_VDDH_A_HIGH; 
	u16 PWR2_VDDH_A_LOW;
	u16 PWR3_TP_VDD_A_HIGH; 
	u16 PWR3_TP_VDD_A_LOW; 
	u16 PWR4_TP_VIO_A_HIGH;
	u16 PWR4_TP_VIO_A_LOW;
	u16 PWR5_VSP_AVDD_A_HIGH;
	u16 PWR5_VSP_AVDD_A_LOW;
	u16 PWR6_VSN_AVEE_A_HIGH;
	u16 PWR6_VSN_AVEE_A_LOW;
	u16 PWR7_LED1N_A_HIGH;
	u16 PWR7_LED1N_A_LOW;
	u16 PWR8_LED2N_A_HIGH;
	u16 PWR8_LED2N_A_LOW;
	u16 PWR9_LED3N_A_HIGH;
	u16 PWR9_LED3N_A_LOW;
	u16 PWR10_VMTP_A_HIGH;
	u16 PWR10_VMTP_A_LOW;
	u16 PWR11_VNEG_A_HIGH;
	u16 PWR11_VNEG_A_LOW;
	u16 PWR12_LED_PWR_5V_A_HIGH;
	u16 PWR12_LED_PWR_5V_A_LOW;
	u16 PWR13_EXT_PWR_A_A_HIGH;
	u16 PWR13_EXT_PWR_A_A_LOW;
	u16 PWR14_EXT_PWR_B_A_HIGH;
	u16 PWR14_EXT_PWR_B_A_LOW;
	u16 PWR15_TESTER_VBUS_A_HIGH;
	u16 PWR15_TESTER_VBUS_A_LOW;
	
	u16 PWR1_IOVCC_UA_HIGH; 	//UA电流上下线
	u16 PWR1_IOVCC_UA_LOW;
	u16 PWR2_VDDH_UA_HIGH; 
	u16 PWR2_VDDH_UA_LOW;
	u16 PWR3_TP_VDD_UA_HIGH; 
	u16 PWR3_TP_VDD_UA_LOW; 
	u16 PWR4_TP_VIO_UA_HIGH;
	u16 PWR4_TP_VIO_UA_LOW;
	u16 PWR5_VSP_AVDD_UA_HIGH;
	u16 PWR5_VSP_AVDD_UA_LOW;
	u16 PWR6_VSN_AVEE_UA_HIGH;
	u16 PWR6_VSN_AVEE_UA_LOW;
	u16 PWR7_LED1N_UA_HIGH;
	u16 PWR7_LED1N_UA_LOW;
	u16 PWR8_LED2N_UA_HIGH;
	u16 PWR8_LED2N_UA_LOW;
	u16 PWR9_LED3N_UA_HIGH;
	u16 PWR9_LED3N_UA_LOW;
	u16 PWR10_VMTP_UA_HIGH;
	u16 PWR10_VMTP_UA_LOW;
	u16 PWR11_VNEG_UA_HIGH;
	u16 PWR11_VNEG_UA_LOW;
	u16 PWR12_LED_PWR_5V_UA_HIGH;
	u16 PWR12_LED_PWR_5V_UA_LOW;
	u16 PWR13_EXT_PWR_A_UA_HIGH;
	u16 PWR13_EXT_PWR_A_UA_LOW;
	u16 PWR14_EXT_PWR_B_UA_HIGH;
	u16 PWR14_EXT_PWR_B_UA_LOW;
	u16 PWR15_TESTER_VBUS_UA_HIGH;
	u16 PWR15_TESTER_VBUS_UA_LOW;

	
	u16 Flag_Current_En;
	u16 Flag_Voltage_En;
	u16 Flag_Sleep_Current_En;
	u16 Flag_Hard_ID_En;
	u16 Hard_ID_En_Val;
	u16 Flag_Soft_ID_En;
	u16 Soft_ID12_Val;
	u16 Soft_ID34_Val;
	u16 Flag_OTP_Times_En;
	u16 OTP_Times_High;
	u16 OTP_Times_Low;
	u16 Flag_DuiWei_En;
	u16 Flag_PWM_En;
	u16 PWM_Seq_High;
	u16 PWM_Seq_Low;
	u16 PWM_Duty_High;
	u16 PWM_Duty_Low;
	u16 PWM_Volt_High;
	u16 PWM_Volt_Low;
	u16 Flag_TE_En;
	u16 TE_Seq_High;
	u16 TE_Seq_Low;
	u16 TE_Duty_High;
	u16 TE_Duty_Low;
	u16 TE_Volt_High;
	u16 TE_Volt_Low;
	u16 FLAG_BK_En;
	//key部分

	u16 Flag_Key_OnOff_Short;
	u16 Flag_Key_OnOff_Long;
	u16 Key_OnOff_NormalPress;
	u16 Key_OnOff_LongPress;

	u16 Flag_Key_Left_Short;
	u16 Flag_Key_Left_Long;
	u16 Key_Left_NormalPress;
	u16 Key_Left_LongPress;

	u16 Flag_Key_Auto_Short;
	u16 Flag_Key_Auto_Long;
	u16 Key_Auto_NormalPress;
	u16 Key_Auto_LongPress;

	u16 Flag_Key_Right_Short;
	u16 Flag_Key_Right_Long;
	u16 Key_Right_NormalPress;
	u16 Key_Right_LongPress;

	u16 Flag_Key_Test_Short;
	u16 Flag_Key_Test_Long;
	u16 Key_Test_NormalPress;
	u16 Key_Test_LongPress;
	
	u16 Key_Lock_Time;
	
	char *lcmname;

	u32 Lcm_CRC;
	//char name[64]; 					//索引最大值		占4字节
	
}lcm_parameter;

typedef enum 
{
	PWR1_IOVCC_INDEX = 3,
	PWR2_VDDH_INDEX,
	PWR3_TP_VDD_INDEX, 
	PWR4_TP_VIO_INDEX,
	PWR5_VSP_AVDD_INDEX,
	PWR6_VSN_AVEE_INDEX,
	PWR10_VMTP_INDEX,
	PWR11_VNEG_INDEX,
	PWR13_EXT_PWR_A_INDEX,
	PWR14_EXT_PWR_B_INDEX,
	PWR15_TESTER_VBUS_INDEX,
	PWR7_LED1N_INDEX,
	PWR8_LED2N_INDEX,
	PWR9_LED3N_INDEX,
	PWR12_LED_PWR_5V_INDEX,
}POWER_CHANNEL_INDEX_TypeDef;

extern const lcm_parameter lcm_para;

u16 Get_Volt_Val(POWER_CHANNEL_INDEX_TypeDef index);


#endif
