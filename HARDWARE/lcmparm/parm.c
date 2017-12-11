#include "parm.h"
#include "usart.h"

#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

/*
#define	 NATIVE_INT		 int
#define  _AUPBND         (sizeof (NATIVE_INT) - 1)
#define  _ADNBND         (sizeof (NATIVE_INT) - 1)
*/
//typedef int *__va_list[1]; 
/*
typedef int *va_list;
#define va_start(ap, A)		(ap = (int *)&(A) + 1)
#define va_arg(ap, T)		(*(T *)ap++)
#define va_end(ap)		((void)0)

*/

/*
 * Variable argument list macro definitions
 */



//初始化PB5和PE5为输出口.并使能这两个口的时钟
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;				 //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOB,GPIO_Pin_0);						 //PA.8 输出高
}


//u32 rw[10] __attribute__ ((section ("codeinterge"))) ; 


const lcm_parameter lcm_para={
	
	.G_Select_Port = 1,
	.G_disp_width = 1080,
	.G_disp_height = 1920,
	
	.IOVCC = 1850,
	.VDDH = 3000,
	.TP_VDD = 2800,
	.TP_VIO = 1850,
	.VSP = 5850,
	.VSN = 5850,
	.VMTP = 7100,
	.VNEG = 4600,
	.EXT_PWRA = 2000,
	.EXT_PWRB = 2000,
	.TESTER_VBUS = 1800,
	.BL1N = 20,
	.BL2N = 20,
	.BL3N = 20,
	.LED_5V = 5000,

	
	.PWR1_IOVCC_HIGH = 1950,	//电压上下线
	.PWR1_IOVCC_LOW = 1650,
	.PWR2_VDDH_HIGH =  3200,
	.PWR2_VDDH_LOW = 2800,
	.PWR3_TP_VDD_HIGH = 2900, 
	.PWR3_TP_VDD_LOW =  2700,
	.PWR4_TP_VIO_HIGH = 1950,
	.PWR4_TP_VIO_LOW = 1650,
	.PWR5_VSP_AVDD_HIGH = 5500,
	.PWR5_VSP_AVDD_LOW = 5200,
	.PWR6_VSN_AVEE_HIGH = 5500,
	.PWR6_VSN_AVEE_LOW = 5200,
	.PWR7_LED1N_HIGH = 30000,
	.PWR7_LED1N_LOW = 15000,
	.PWR8_LED2N_HIGH = 30000,
	.PWR8_LED2N_LOW = 15000,
	.PWR9_LED3N_HIGH = 30000,
	.PWR9_LED3N_LOW = 15000,
	.PWR10_VMTP_HIGH = 7800,
	.PWR10_VMTP_LOW = 5600,
	.PWR11_VNEG_HIGH = 4500,
	.PWR11_VNEG_LOW = 4200,
	.PWR12_LED_PWR_5V_HIGH = 5200,
	.PWR12_LED_PWR_5V_LOW = 5100,
	.PWR13_EXT_PWR_A_HIGH = 5200,
	.PWR13_EXT_PWR_A_LOW = 5100,
	.PWR14_EXT_PWR_B_HIGH = 5200,
	.PWR14_EXT_PWR_B_LOW = 5100,
	.PWR15_TESTER_VBUS_HIGH = 1900,
	.PWR15_TESTER_VBUS_LOW = 1700,
	
	.PWR1_IOVCC_A_HIGH = 65,	//电流上下线
	.PWR1_IOVCC_A_LOW = 0,
	.PWR2_VDDH_A_HIGH = 50, 
	.PWR2_VDDH_A_LOW = 0,
	.PWR3_TP_VDD_A_HIGH = 50, 
	.PWR3_TP_VDD_A_LOW = 0, 
	.PWR4_TP_VIO_A_HIGH = 50,
	.PWR4_TP_VIO_A_LOW = 0,
	.PWR5_VSP_AVDD_A_HIGH = 50,
	.PWR5_VSP_AVDD_A_LOW = 0,
	.PWR6_VSN_AVEE_A_HIGH = 50,
	.PWR6_VSN_AVEE_A_LOW = 0,
	.PWR7_LED1N_A_HIGH = 22,
	.PWR7_LED1N_A_LOW = 18,
	.PWR8_LED2N_A_HIGH = 22,
	.PWR8_LED2N_A_LOW = 18,
	.PWR9_LED3N_A_HIGH = 22,
	.PWR9_LED3N_A_LOW = 18,
	.PWR10_VMTP_A_HIGH = 100,
	.PWR10_VMTP_A_LOW = 0,
	.PWR11_VNEG_A_HIGH = 50,
	.PWR11_VNEG_A_LOW = 0,
	.PWR12_LED_PWR_5V_A_HIGH = 70,
	.PWR12_LED_PWR_5V_A_LOW = 0,
	.PWR13_EXT_PWR_A_A_HIGH = 50,
	.PWR13_EXT_PWR_A_A_LOW = 0,
	.PWR14_EXT_PWR_B_A_HIGH = 50,
	.PWR14_EXT_PWR_B_A_LOW = 0,
	.PWR15_TESTER_VBUS_A_HIGH = 50,
	.PWR15_TESTER_VBUS_A_LOW = 0,
	
	.PWR1_IOVCC_UA_HIGH =  100,	//UA电流上下线
	.PWR1_IOVCC_UA_LOW = 0,
	.PWR2_VDDH_UA_HIGH = 100,
	.PWR2_VDDH_UA_LOW = 0,
	.PWR3_TP_VDD_UA_HIGH = 100, 
	.PWR3_TP_VDD_UA_LOW = 0, 
	.PWR4_TP_VIO_UA_HIGH = 100,
	.PWR4_TP_VIO_UA_LOW = 0,
	.PWR5_VSP_AVDD_UA_HIGH = 100,
	.PWR5_VSP_AVDD_UA_LOW = 0,
	.PWR6_VSN_AVEE_UA_HIGH = 100,
	.PWR6_VSN_AVEE_UA_LOW = 0,
	.PWR7_LED1N_UA_HIGH = 100,
	.PWR7_LED1N_UA_LOW = 0,
	.PWR8_LED2N_UA_HIGH = 100,
	.PWR8_LED2N_UA_LOW = 0,
	.PWR9_LED3N_UA_HIGH = 100,
	.PWR9_LED3N_UA_LOW = 0,
	.PWR10_VMTP_UA_HIGH = 100,
	.PWR10_VMTP_UA_LOW = 0,
	.PWR11_VNEG_UA_HIGH = 100,
	.PWR11_VNEG_UA_LOW = 0,
	.PWR12_LED_PWR_5V_UA_HIGH = 100,
	.PWR12_LED_PWR_5V_UA_LOW = 0,
	.PWR13_EXT_PWR_A_UA_HIGH = 100,
	.PWR13_EXT_PWR_A_UA_LOW = 0,
	.PWR14_EXT_PWR_B_UA_HIGH = 100,
	.PWR14_EXT_PWR_B_UA_LOW = 0,
	.PWR15_TESTER_VBUS_UA_HIGH = 100,
	.PWR15_TESTER_VBUS_UA_LOW = 0,

	
	.Flag_Current_En = 1,
	.Flag_Voltage_En = 1,
	.Flag_Sleep_Current_En = 1,
	.Flag_Hard_ID_En = 0, 
	.Hard_ID_En_Val = 10,
	.Flag_Soft_ID_En = 0,
	.Soft_ID12_Val = 0,
	.Soft_ID34_Val = 0,
	.Flag_OTP_Times_En = 0,
	.OTP_Times_High = 8,
	.OTP_Times_Low = 0,
	.Flag_DuiWei_En = 0, 
	.Flag_PWM_En = 0,
	.PWM_Seq_High = 30000,
	.PWM_Seq_Low = 20000,
	.PWM_Duty_High = 100,
	.PWM_Duty_Low = 0,
	.PWM_Volt_High = 4000,
	.PWM_Volt_Low = 0,
	.Flag_TE_En = 0,
	.TE_Seq_High = 65,
	.TE_Seq_Low = 55,
	.TE_Duty_High = 100,
	.TE_Duty_Low = 5,
	.TE_Volt_High = 2000,
	.TE_Volt_Low = 1000,
	.FLAG_BK_En = 0,
	//key部分

	.Flag_Key_OnOff_Short = 1,
	.Flag_Key_OnOff_Long = 1,
	.Key_OnOff_NormalPress = 0,
	.Key_OnOff_LongPress = 0,

	.Flag_Key_Left_Short = 1,
	.Flag_Key_Left_Long = 1,
	.Key_Left_NormalPress = 0,
	.Key_Left_LongPress = 0,

	.Flag_Key_Auto_Short = 1,
	.Flag_Key_Auto_Long = 1,
	.Key_Auto_NormalPress = 0,
	.Key_Auto_LongPress = 0,

	.Flag_Key_Right_Short = 1,
	.Flag_Key_Right_Long = 1,
	.Key_Right_NormalPress = 0,
	.Key_Right_LongPress = 0,

	.Flag_Key_Test_Short = 1,
	.Flag_Key_Test_Long = 1,
	.Key_Test_NormalPress = 0,
	.Key_Test_LongPress = 0,

	.Key_Lock_Time=100,

	.lcmname = "hello world",
	.Lcm_CRC = 50,
};



u16 Get_Volt_Val(POWER_CHANNEL_INDEX_TypeDef index)
{

	u16 *para = (u16 *)(&lcm_para);

	return para[index];
	
}


