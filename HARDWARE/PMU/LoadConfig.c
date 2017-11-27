/*********************************************************************************************************
* 文件名称: LoadConfig.c
* 文件作者: zq
* 当前版本: V1.01
* 所属模块: 
* 完成日期: 2016年11月21日
* 文件摘要: 
* 注意事项:	
* 更改说明: 
**********************************************************************************************************/
//***************************//
/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "LoadConfig.h"
#include "SysTick.h"
#include <string.h>
#include "tp1.h"
#include "VoltCurrentProc.h"
#include "TPS22993.h"
#include "LM36923.h"
#include "tps65132.h"
#include "fsmc_fpga.h"
#include "Key.h"
#include "UCD9081.h"
#include "lcd.h"
#include "tester_debug.h"
#include "GUI.h"
#include "adc.h"
#include "lcd_ui.h"
#include "..\LIB\inc\lib_System.h"
#include "Protocol.h"
#include "user.h"

extern uint8_t G_Select_Port;
extern uint8_t G_No_Of_Test_Case;
extern void SysCtlDelay(unsigned long ulCount);
#define SYSTEM_CORE_CLOCK 72000000UL
#define SYS_100NS (SYSTEM_CORE_CLOCK/1000)/(3*1000*10)+1
#define SYS_1US   (SYSTEM_CORE_CLOCK/1000)/(3*1000)
#define SYS_1MS   (SYSTEM_CORE_CLOCK/1000)/3

#define	READ_MAX_NUM	4000
#define	DEBUG_BIN_CFG	0x01
//#define	DEBUG_BIN_CFG1	


#define	BL_LM36923_POWER_ON   		GPIO_SetBits(GPIOE, GPIO_Pin_4);// 开启背光芯片电源	
#define	BL_LM36923_POWER_OFF   		GPIO_ResetBits(GPIOE, GPIO_Pin_4);// 关闭背光芯片电源	




//static CTL_ADD_VAL_STRUCT_TypeDef  CtrlAddValManage;
static PARA_STRUCT_TypeDef			ParaManage;
//static SET_VOLTAGE_TypeDef			SetVoltageManage;
static PATTERN_STRUCT_TypeDef      PatternManage;
//static POWER_STATUS_TypeDef		PowerStatusManage;


static	u8	PowerOnOffSeq = 0;			//0--默认关闭，1--打开
static  u8  Pic_Num_Max = 0;

static  u32 Info_Add = 0;
static  u16 Info_Add_Size = 0;
static  u32 Pattern_Add = 0;
static  u16 Pattern_Add_Size = 0;
static  u32 Board_Init_Add = 0;
static  u16 Board_Init_Add_Size = 0;
static  u32 PowerOn_Add = 0;
static  u16 PowerOn_Add_Size = 0;
static  u32 Code_Init_Add = 0;
static  u16 Code_Init_Add_Size = 0;
static  u32 PowerOff_Add = 0;
static  u16 PowerOff_Add_Size = 0;
static  u32 Sleep_Seq_Add = 0;
static  u16 Sleep_Seq_Add_Size = 0;
static  u32 Code_Init2_Add = 0;
static  u16 Code_Init2_Add_Size = 0;
static  u32 Code_Cmd1_Add = 0;
static  u16 Code_Cmd1_Add_Size = 0;
static  u32 Code_Cmd2_Add = 0;
static  u16 Code_Cmd2_Add_Size = 0;
static  u32 Code_Cmd3_Add = 0;
static  u16 Code_Cmd3_Add_Size = 0;
static  u32 Code_Cmd4_Add = 0;
static  u16 Code_Cmd4_Add_Size = 0;




 u8 Sleep_Status = 0;	//0---正常模式，1---休眠模式
 u8 Vsn_Power_Status = 0;	//VSN 0---关掉，1---打开
 u8 Iovcc_Power_Status = 0;	//IOVCC 0---关掉，1---打开
 u8 Vsp_Power_Status = 0;
 u8 Tpvio_Power_Status = 0;
 u8 Tpvdd_Power_Status = 0;
 u8 Lcdvdd_Power_Status = 0;


static u8 Flag_Big_Current_Err = 0;

static u16 Lcd_Iovcc_A_High = 0;
static u16 Lcd_Vddh_A_High = 0;
static u16 Tp_Vio_A_High = 0;
static u16 Tp_Vdd_A_High = 0;
static u16 Vsp_A_High = 0;
static u16 Vsn_A_High = 0;



/////////////////////////////////////////////////
//结构体部分定义   PowerStatusManage
u16 PWR1_IOVCC_Off_Status;
u16 PWR2_VDDH_Off_Status;
u16 PWR3_TP_VDD_Off_Status;
u16 PWR4_TP_VIO_Off_Status;
u16 PWR5_VSP_AVDD_Off_Status;
u16 PWR6_VSN_AVEE_Off_Status;
u16 PWR7_LED1N_Off_Status;
u16 PWR8_LED2N_Off_Status;
u16 PWR9_LED3N_Off_Status;
u16 PWR10_VMTP_Off_Status;
u16 PWR11_VNEG_Off_Status;
u16 PWR12_LED_PWR_5V_Off_Status;
u16 PWR13_EXT_PWR_A_Off_Status;
u16 PWR14_EXT_PWR_B_Off_Status;
u16 PWR15_TESTER_VBUS_Off_Status;

u16 PWR1_IOVCC_On_Status; 
u16 PWR2_VDDH_On_Status; 
u16 PWR3_TP_VDD_On_Status; 
u16 PWR4_TP_VIO_On_Status;
u16 PWR5_VSP_AVDD_On_Status;
u16 PWR6_VSN_AVEE_On_Status;
u16 PWR7_LED1N_On_Status;
u16 PWR8_LED2N_On_Status;
u16 PWR9_LED3N_On_Status;
u16 PWR10_VMTP_On_Status;
u16 PWR11_VNEG_On_Status;
u16 PWR12_LED_PWR_5V_On_Status;
u16 PWR13_EXT_PWR_A_On_Status;
u16 PWR14_EXT_PWR_B_On_Status;
u16 PWR15_TESTER_VBUS_On_Status;


///////////////////////////////////////////////////////
//结构体部分定义   SetVoltageManage
u16 PWR1_IOVCC; 
u16 PWR2_VDDH; 
u16 PWR3_TP_VDD; 
u16 PWR4_TP_VIO;
u16 PWR5_VSP_AVDD;
u16 PWR6_VSN_AVEE;
u16 PWR7_LED1N;
u16 PWR8_LED2N;
u16 PWR9_LED3N;
u16 PWR10_VMTP;
u16 PWR11_VNEG;
u16 PWR12_LED_PWR_5V;
u16 PWR13_EXT_PWR_A;
u16 PWR14_EXT_PWR_B;
u16 PWR15_TESTER_VBUS;

u16 PWR1_IOVCC_HIGH; 	//电压上下线
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

u16 PWR1_IOVCC_A_HIGH; 	//电流上下线
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




//////////////////////////////////////////

//结构体部分定义 CtrlAddValManage
u32 General_Info_Add; 
u32 Pattern_Set_Add; 
u32 Board_Add; 
u32 Power_On_Set_Add; 
u32 Init_Code_Add; 
u32 Power_Off_Set_Add; 
u32 Sleep_Seq_Set_Add; 
u32 Init2_Code_Add; 
u32 Cmd_Code1_Add; 
u32 Cmd_Code2_Add;
u32 Cmd_Code3_Add;
u32 Cmd_Code4_Add;




u16 General_Info_Size; 
u16 Pattern_Set_Size;
u32 Board_Size; 
u16 Power_On_Set_Size;
u16 Init_Code_Size; 
u16 Power_Off_Set_Size;
u16 Sleep_Seq_Set_Size; 
u16 Init2_Code_Size; 
u16 Cmd_Code1_Size; 
u16 Cmd_Code2_Size;
u16 Cmd_Code3_Size;
u16 Cmd_Code4_Size;


///////////////////////////////////////////////////
//选项配置区域

u16 Flag_Current_En = 0;
u16 Flag_Voltage_En = 0;
u16 Flag_Sleep_Current_En = 0;
u16 Flag_Hard_ID_En = 0;
u16 Hard_ID_En_Val = 0;				//使能和限制值
u16 Flag_Soft_ID_En = 0;
u16 Soft_ID12_Val = 0;
u16 Soft_ID34_Val = 0;
u16 Flag_OTP_Times_En = 0;
u16 OTP_Times_High = 0;
u16 OTP_Times_Low = 0;
u16 Flag_DuiWei_En = 0;
u16 Flag_PWM_En = 0;
u16 PWM_Seq_High = 0;
u16 PWM_Seq_Low = 0;
u16 PWM_Duty_High = 0;
u16 PWM_Duty_Low = 0;
u16 PWM_Volt_High = 0;
u16 PWM_Volt_Low = 0;
u16 Flag_TE_En = 0;
u16 TE_Seq_High = 0;
u16 TE_Seq_Low = 0;
u16 TE_Duty_High = 0;
u16 TE_Duty_Low = 0;
u16 TE_Volt_High = 0;
u16 TE_Volt_Low = 0;
u16 FLAG_BK_En = 0;
//key 部分

u16 Flag_Key_OnOff_Short = 0;
u16 Flag_Key_OnOff_Long = 0;
u16 Key_OnOff_NormalPress = 0x0f;		//0 - 正常LCD PWR ON/OFF  
u16 Key_OnOff_LongPress = 0x0f;			//0 - 进入休眠模式

u16 Flag_Key_Left_Short = 0;
u16 Flag_Key_Left_Long = 0;
u16 Key_Left_NormalPress = 0x0f;			//0 - 向左循环切图模式    1 - 向左单向切图模式
u16 Key_Left_LongPress = 0x0f;			//预留


u16 Flag_Key_Auto_Short = 0;
u16 Flag_Key_Auto_Long = 0;
u16 Key_Auto_NormalPress = 0x0f;			//0 - 向左循环切图模式    1 - 向左单向切图模式
u16 Key_Auto_LongPress = 0x0f;			//预留


u16 Flag_Key_Right_Short = 0;
u16 Flag_Key_Right_Long = 0;
u16 Key_Right_NormalPress = 0x0f;		//0 - 自动循环切图模式
u16 Key_Right_LongPress = 0x0f;			//预留


u16 Flag_Key_Test_Short = 0;
u16 Flag_Key_Test_Long = 0;
u16 Key_Test_NormalPress = 0x0f;			//0 - 触发TP测试
u16 Key_Test_LongPress = 0x0f;			//预留

u16 Key_Lock_Time = 0;

u8 Flag_Load_CFG_Done = 0;
u16 Pic_Lock_Time[40];			//图片索引时间
u8 Pic_Cmd_Index[40];			//图片索引时间
u8 PowerOn_Pic_Index = 0;
u8 Auto_Sleep_Index = 0xff;		//无效值
u8 Auto_Sleep_Mode = 0x00;		//0x00  手动模式   0x01  自动模式





lcm_parameter lcm_para={
	
	.G_Select_Port = 1,
	.G_disp_width = 1080,
	.G_disp_height = 1920,
	
	.IOVCC = 1850,
	.VDDH = 3000,
	.TP_VDD = 2800,
	.TP_VIO = 1850,
	.VSP = 5400,
	.VSN = 5400,
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

//////////////////////////////////////////////////////////////////
//上电时序
u8 Power_On_Seq_Index[10];
u16 Power_On_Seq_Delay[10];
u8 Power_On_Seq_Index_Cnt = 0;
u8 Flag_Power_On_Seq_Start = 0;

u8 Flag_Vsp_Vsn = 0;
u8 Flag_Port = 0;


extern FIL fdst_bin;
extern u8 G_fbuffer[4350]; 
extern FILINFO binfileInfo;
extern u16 G_disp_width;
extern u16 G_disp_height;
extern u16 G_Reg_X,G_Reg_Y;
extern FPGA_DISP_STRUCT_TypeDef Fpga_Disp_Manage;
extern FILINFO binfileInfo;
extern u8 G_DISP_FILE_BUFFER[20480];		//sram


///////////////////////////////////////////////////////////////////////
u8 AutoTestTp = 0;
u16 AutoTestTpDelay = 0;


/*******************************************************************************
* 文件名	  	 : Read_Start_Address
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
u8  Read_Start_Address(READ_START_ADD_TypeDef add)
{
	u32 temp = 0;
	u8 temparr[8],btr = 8;
	u32 br = 0;
	
	if((add != READ_START_ADD_NONE) && (add != READ_START_ADD_MAX))
	{
		;
	}
	else
	{
		return(1);
	}	
	
	switch(add)
	{
		case READ_START_ADD_INFO:			
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_GENERAL_INFO_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			 General_Info_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];				
			 General_Info_Size = temp;
			break;
			
		case READ_START_ADD_PATTEN:		
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_PATTERN_SET_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];	
			 Pattern_Set_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];			
			 Pattern_Set_Size = temp;
			break;
			
		case READ_START_ADD_BOARD:
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_BOARD_SET_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			 Board_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];			
			 Board_Size = temp;
			break;
			
		case READ_START_ADD_POWER_ON:
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_POWER_ON_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			 Power_On_Set_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];			
			 Power_On_Set_Size = temp;
			break;
			
		case READ_START_ADD_INIT_CODE:	
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_INIT_CODE_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];	
			 Init_Code_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			 Init_Code_Size = temp;
			break;
			
		case READ_START_ADD_POWER_OFF:			
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_POWER_OFF_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			 Power_Off_Set_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			 Power_Off_Set_Size = temp;
			break;
			
		case READ_START_ADD_SLEEP_SEQ:			
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_SLEEP_SEQ_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			 Sleep_Seq_Set_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			 Sleep_Seq_Set_Size = temp;
			break;

		case READ_START_ADD_INIT2_COD:			
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_INIT2_CODE_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			Init2_Code_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			Init2_Code_Size = temp;
			break;
			
		case READ_START_ADD_CMD_CODE1:
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_CMD_CODE1_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			Cmd_Code1_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			Cmd_Code1_Size = temp;
			break;

		case READ_START_ADD_CMD_CODE2:
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_CMD_CODE2_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			Cmd_Code2_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			Cmd_Code2_Size = temp;
			break;

		case READ_START_ADD_CMD_CODE3:
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_CMD_CODE3_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			Cmd_Code3_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			Cmd_Code3_Size = temp;
			break;

		case READ_START_ADD_CMD_CODE4:
			memcpy(temparr,&G_DISP_FILE_BUFFER[REG_CMD_CODE4_ADD],8);
			
			temp = ((u32)temparr[3])<<24 & 0xff000000;
			temp |= ((u32)temparr[2])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[1])<<8 & 0x0000ff00;
			temp |= (u32)temparr[0];
			Cmd_Code4_Add = temp;
			temp = ((u32)temparr[7])<<24 & 0xff000000;
			temp |= ((u32)temparr[6])<<16 & 0x00ff0000;
			temp |= ((u32)temparr[5])<<8 & 0x0000ff00;
			temp |= (u32)temparr[4];		
			Cmd_Code4_Size = temp;
			break;
			
	}

	return(0);
	
}




/*******************************************************************************
* 文件名	  	 : BinSetVoltVal_Handle
* 描述	         : 设置电压值
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void BinSetVoltVal_Handle(u8 chn,u16 val)
{
	//设置电压值
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC = val;
		break;
		case 2:
			 PWR2_VDDH = val;
		break;
		case 3:
			 PWR3_TP_VDD = val;
		break;
		case 4:
			 PWR4_TP_VIO = val;
		break;
		case 5:
			 PWR5_VSP_AVDD = val;
		break;
		case 6:
			 PWR6_VSN_AVEE = val;
		break;
		case 7:
			 PWR7_LED1N = val;
		break;
		case 8:
			 PWR8_LED2N = val;
		break;
		case 9:
			 PWR9_LED3N = val;
		break;
		case 10:
			 PWR10_VMTP = val;
		break;
		case 11:
			 PWR11_VNEG = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS = val;
		break;		
	}
}

/*******************************************************************************
* 文件名	  	 : BinSetVoltLow_Handle
* 描述	         : 设置电压下限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void BinSetVoltLow_Handle(u8 chn,u16 val)
{
	//设置电压下限	
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_LOW = val;
			UCD9081_ThersholdManage.UCD9081_RAIL1_LOW = val;
		break;
		case 2:
			 PWR2_VDDH_LOW = val;
			UCD9081_ThersholdManage.UCD9081_RAIL4_LOW = val;
		break;
		case 3:
			 PWR3_TP_VDD_LOW = val;
			UCD9081_ThersholdManage.UCD9081_RAIL6_LOW = val;
		break;
		case 4:
			 PWR4_TP_VIO_LOW = val;
			UCD9081_ThersholdManage.UCD9081_RAIL2_LOW = val;
		break;
		case 5:
			 PWR5_VSP_AVDD_LOW = val;
			UCD9081_ThersholdManage.UCD9081_RAIL3_LOW = val;
		break;
		case 6:
			 PWR6_VSN_AVEE_LOW = val;
		break;
		case 7:
			 PWR7_LED1N_LOW = val;
		break;
		case 8:
			 PWR8_LED2N_LOW = val;
		break;
		case 9:
			 PWR9_LED3N_LOW = val;
		break;
		case 10:
			 PWR10_VMTP_LOW = val;
			UCD9081_ThersholdManage.UCD9081_RAIL7_LOW = val;
		break;
		case 11:
			 PWR11_VNEG_LOW = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_LOW = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_LOW = val;
			UCD9081_ThersholdManage.UCD9081_RAIL5_LOW = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B_LOW = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_LOW = val;
		break;
	}		
}
	


/*******************************************************************************
* 文件名	  	 : BinSetVoltHigh_Handle
* 描述	         : 设置电压上限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/			
void BinSetVoltHigh_Handle(u8 chn,u16 val)
{
	//设置电压上限
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_HIGH = val;
			UCD9081_ThersholdManage.UCD9081_RAIL1_HIGH = val;
		break;
		case 2:
			 PWR2_VDDH_HIGH = val;
			UCD9081_ThersholdManage.UCD9081_RAIL4_HIGH = val;
		break;
		case 3:
			 PWR3_TP_VDD_HIGH = val;
			UCD9081_ThersholdManage.UCD9081_RAIL6_HIGH = val;
		break;
		case 4:
			 PWR4_TP_VIO_HIGH = val;
			UCD9081_ThersholdManage.UCD9081_RAIL2_HIGH = val;
		break;
		case 5:
			 PWR5_VSP_AVDD_HIGH = val;
			UCD9081_ThersholdManage.UCD9081_RAIL3_HIGH = val;
		break;
		case 6:
			 PWR6_VSN_AVEE_HIGH = val;
		break;
		case 7:
			 PWR7_LED1N_HIGH = val;
		break;
		case 8:
			 PWR8_LED2N_HIGH = val;
		break;
		case 9:
			 PWR9_LED3N_HIGH = val;
		break;
		case 10:
			 PWR10_VMTP_HIGH = val;
			UCD9081_ThersholdManage.UCD9081_RAIL7_HIGH = val;
		break;
		case 11:
			 PWR11_VNEG_HIGH = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_HIGH = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_HIGH = val;
			UCD9081_ThersholdManage.UCD9081_RAIL5_HIGH = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B_HIGH = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_HIGH = val;
		break;
	}
}



/*******************************************************************************
* 文件名	  	 : BinSetCurrLow_Handle
* 描述	         : 设置电流下限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/	
void BinSetCurrLow_Handle(u8 chn,u16 val)
{
	//设置电流下限	
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_A_LOW = val;
		break;
		case 2:
			 PWR2_VDDH_A_LOW = val;
		break;
		case 3:
			 PWR3_TP_VDD_A_LOW = val;
		break;
		case 4:
			 PWR4_TP_VIO_A_LOW = val;
		break;
		case 5:
			 PWR5_VSP_AVDD_A_LOW = val;
		break;
		case 6:
			 PWR6_VSN_AVEE_A_LOW = val;
		break;
		case 7:
			 PWR7_LED1N_A_LOW = val;
		break;
		case 8:
			 PWR8_LED2N_A_LOW = val;
		break;
		case 9:
			 PWR9_LED3N_A_LOW = val;
		break;
		case 10:
			 PWR10_VMTP_A_LOW = val;
		break;
		case 11:
			 PWR11_VNEG_A_LOW = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_A_LOW = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_A_LOW = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B_A_LOW = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_A_LOW = val;
		break;
		////////////////////////////////////////////////////////
		//ua part
		case 101:
			 PWR1_IOVCC_UA_LOW = val;
		break;
		case 102:
			 PWR2_VDDH_UA_LOW = val;
		break;
		case 103:
			 PWR3_TP_VDD_UA_LOW = val;
		break;
		case 104:
			 PWR4_TP_VIO_UA_LOW = val;
		break;
		case 105:
			 PWR5_VSP_AVDD_UA_LOW = val;
		break;
		case 106:
			 PWR6_VSN_AVEE_UA_LOW = val;
		break;
		case 107:
			 PWR7_LED1N_UA_LOW = val;
		break;
		case 108:
			 PWR8_LED2N_UA_LOW = val;
		break;
		case 109:
			 PWR9_LED3N_UA_LOW = val;
		break;
		case 110:
			 PWR10_VMTP_UA_LOW = val;
		break;
		case 111:
			 PWR11_VNEG_UA_LOW = val;
		break;
		case 112:
			 PWR12_LED_PWR_5V_UA_LOW = val;
		break;
		case 113:
			 PWR13_EXT_PWR_A_UA_LOW = val;
		break;
		case 114:
			 PWR14_EXT_PWR_B_UA_LOW = val;
		break;
		case 115:
			 PWR15_TESTER_VBUS_UA_LOW = val;
		break;
	}
}



/*******************************************************************************
* 文件名	  	 : BinSetCurrHigh_Handle
* 描述	         : 设置电流上限
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void BinSetCurrHigh_Handle(u8 chn,u16 val)
{
	//设置电流上限	
	switch(chn)		//电压通道
	{
		case 1:
			 PWR1_IOVCC_A_HIGH = val;
			Lcd_Iovcc_A_High = val;
		break;
		case 2:
			 PWR2_VDDH_A_HIGH = val;
			Lcd_Vddh_A_High = val;
		break;
		case 3:
			 PWR3_TP_VDD_A_HIGH = val;
			Tp_Vdd_A_High = val;
		break;
		case 4:
			 PWR4_TP_VIO_A_HIGH = val;
			Tp_Vio_A_High = val;
		break;
		case 5:
			 PWR5_VSP_AVDD_A_HIGH = val;
			Vsp_A_High = val;
		break;
		case 6:
			 PWR6_VSN_AVEE_A_HIGH = val;
			Vsn_A_High = val;
		break;
		case 7:
			 PWR7_LED1N_A_HIGH = val;
		break;
		case 8:
			 PWR8_LED2N_A_HIGH = val;
		break;
		case 9:
			 PWR9_LED3N_A_HIGH = val;
		break;
		case 10:
			 PWR10_VMTP_A_HIGH = val;
		break;
		case 11:
			 PWR11_VNEG_A_HIGH = val;
		break;
		case 12:
			 PWR12_LED_PWR_5V_A_HIGH = val;
		break;
		case 13:
			 PWR13_EXT_PWR_A_A_HIGH = val;
		break;
		case 14:
			 PWR14_EXT_PWR_B_A_HIGH = val;
		break;
		case 15:
			 PWR15_TESTER_VBUS_A_HIGH = val;
		break;
		////////////////////////////////////////////////////////
		//ua part
		case 101:
			 PWR1_IOVCC_UA_HIGH = val;
		break;
		case 102:
			 PWR2_VDDH_UA_HIGH = val;
		break;
		case 103:
			 PWR3_TP_VDD_UA_HIGH = val;
		break;
		case 104:
			 PWR4_TP_VIO_UA_HIGH = val;
		break;
		case 105:
			 PWR5_VSP_AVDD_UA_HIGH = val;
		break;
		case 106:
			 PWR6_VSN_AVEE_UA_HIGH = val;
		break;
		case 107:
			 PWR7_LED1N_UA_HIGH = val;
		break;
		case 108:
			 PWR8_LED2N_UA_HIGH = val;
		break;
		case 109:
			 PWR9_LED3N_UA_HIGH = val;
		break;
		case 110:
			 PWR10_VMTP_UA_HIGH = val;
		break;
		case 111:
			 PWR11_VNEG_UA_HIGH = val;
		break;
		case 112:
			 PWR12_LED_PWR_5V_UA_HIGH = val;
		break;
		case 113:
			 PWR13_EXT_PWR_A_UA_HIGH = val;
		break;
		case 114:
			 PWR14_EXT_PWR_B_UA_HIGH = val;
		break;
		case 115:
			 PWR15_TESTER_VBUS_UA_HIGH = val;
		break;
	}
}



/*******************************************************************************
* 文件名	  	 : Load_Soft_Init
* 描述	         : 主板上电初始化后需要初始化的变量
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void Load_Soft_Init(void)
{
	memset(PatternManage.Pattern_Index_Buf,0,60);
	memset(PatternManage.Pic_Name_Buf,0,120);				//图片要用到，先清零
	memset(PatternManage.Pure_Color_Index_Buf,0,240);
}
				



/*******************************************************************************
* 文件名	  	 : PowerUp_Init_Handle
* 描述	         : 主板上电初始化，不是点屏上电流程
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static void  PowerUp_Init_Handle(void)
{
	u16 temp = 0;
	
#if 1
	Delay_ms(200);
	
	i2c1_init();

	Delay_ms(20);

	I2C_Detect();

	
	//UCD9081_Init();

	Delay_ms(200);
#endif
	
	TESTER_MAIN_DEBUG("\r\nEnable ACT8846....\r\n\r\n");
	act8846_init(); 
	Delay_ms(200);
	TESTER_MAIN_DEBUG("int ACT8846_ON();\r\n");

	//act8846_on();


	//ACT8846_PWR_ON_Cfg();	
	
	
	ACT8846_DCDC_Switch(DCDC_SW1,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW2,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW3,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW4,ACT8846_SWITCH_ON);

	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON);
	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_OFF);
	//ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_OFF);

	//PCA9557_PWR_ON_Cfg();

	//BL_LM36923_POWER_ON; 
	Delay_ms(200);

	//I2C_Detect();
	
	//Delay_ms(50);	
	
	PCA9557_PWR_ON_Cfg();

	Delay_ms(50);	

	LT3582_PWR_ON_Cfg();				//OTP_EN 拉高,里面设置了电压

	TPS22993_PWR_ON_Cfg();
	TPS22993_Sel_Dev(0);				//VSP_VSN POWER ON
	TPS22993_On(TPS22993_CHN1);
	

	Load_Soft_Init();
	
	Delay_ms(800);						//延时时间不能太小
	
	
	//////////////////////////////////////////////////////////VSP VSN设置	
#if 1
	FPGA_Write_Register(0x1C,0x0032);		//BL ADJ 拉高，使能 vsp  vsn	

	BL_LM36923_POWER_ON; 									//背光主电源开启
	LM36923_PWR_ON_Cfg( PWR7_LED1N);		//设置背光电流
	
	Delay_ms(10);		
	Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE); //设置VSP VSN电压
	//Tps65132_Init(5000,5000); 

	Delay_ms(10);							

	FPGA_Write_Register(0x1C,0x0020);		//BL ADJ 拉高，断开vsp  vsn

	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp | 0x80);	
#endif

	Flag_Load_CFG_Done = 1;				//初始化配置load完
	

#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("x=%d y=%d\t\r\n",ParaManage.Resolution_X,ParaManage.Resolution_Y);
	TESTER_MAIN_DEBUG("pwr1=%d %d %d %d %d\t\r\n", PWR1_IOVCC, PWR2_VDDH, PWR3_TP_VDD, PWR4_TP_VIO, PWR5_VSP_AVDD);
	TESTER_MAIN_DEBUG("pwr6=%d %d %d %d %d\t\r\n", PWR6_VSN_AVEE, PWR7_LED1N, PWR8_LED2N, PWR9_LED3N, PWR10_VMTP);
	TESTER_MAIN_DEBUG("pwr11=%d %d %d %d \t\r\n", PWR11_VNEG, PWR12_LED_PWR_5V, PWR13_EXT_PWR_A, PWR14_EXT_PWR_B);
	TESTER_MAIN_DEBUG("pwr threshold low=%d %d %d %d %d\t\r\n",UCD9081_ThersholdManage.UCD9081_RAIL1_LOW,UCD9081_ThersholdManage.UCD9081_RAIL2_LOW,UCD9081_ThersholdManage.UCD9081_RAIL3_LOW,UCD9081_ThersholdManage.UCD9081_RAIL4_LOW,UCD9081_ThersholdManage.UCD9081_RAIL5_LOW,UCD9081_ThersholdManage.UCD9081_RAIL6_LOW);
	TESTER_MAIN_DEBUG("pwr threshold high=%d %d %d %d %d\t\r\n",UCD9081_ThersholdManage.UCD9081_RAIL1_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL2_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL3_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL4_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL5_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL6_HIGH);

#endif

#if 0		//电压电流上下限
	printf("IOVCC LIMIT = %d %d %d %d %d %d\r\n",PWR1_IOVCC_HIGH,PWR1_IOVCC_LOW,PWR1_IOVCC_A_HIGH,PWR1_IOVCC_A_LOW,PWR1_IOVCC_UA_HIGH,PWR1_IOVCC_UA_LOW);
	printf("VDDH LIMIT = %d %d %d %d %d %d\r\n",PWR2_VDDH_HIGH,PWR2_VDDH_LOW,PWR2_VDDH_A_HIGH,PWR2_VDDH_A_LOW,PWR2_VDDH_UA_HIGH,PWR2_VDDH_UA_LOW);
	printf("TPVDD LIMIT = %d %d %d %d %d %d\r\n",PWR3_TP_VDD_HIGH,PWR3_TP_VDD_LOW,PWR3_TP_VDD_A_HIGH,PWR3_TP_VDD_A_LOW,PWR3_TP_VDD_UA_HIGH,PWR3_TP_VDD_UA_LOW);
	printf("TPVIO LIMIT = %d %d %d %d %d %d\r\n",PWR4_TP_VIO_HIGH,PWR4_TP_VIO_LOW,PWR4_TP_VIO_A_HIGH,PWR4_TP_VIO_A_LOW,PWR4_TP_VIO_UA_HIGH,PWR4_TP_VIO_UA_LOW);
	printf("TPVDD LIMIT = %d %d %d %d %d %d\r\n",PWR3_TP_VDD_HIGH,PWR3_TP_VDD_LOW,PWR3_TP_VDD_A_HIGH,PWR3_TP_VDD_A_LOW,PWR3_TP_VDD_UA_HIGH,PWR3_TP_VDD_UA_LOW);
	printf("VSP LIMIT = %d %d %d %d %d %d\r\n",PWR5_VSP_AVDD_HIGH,PWR5_VSP_AVDD_LOW,PWR5_VSP_AVDD_A_HIGH,PWR5_VSP_AVDD_A_LOW,PWR5_VSP_AVDD_UA_HIGH,PWR5_VSP_AVDD_UA_LOW);	
	printf("VSN LIMIT = %d %d %d %d %d %d\r\n",PWR6_VSN_AVEE_HIGH,PWR6_VSN_AVEE_LOW,PWR6_VSN_AVEE_A_HIGH,PWR6_VSN_AVEE_A_LOW,PWR6_VSN_AVEE_UA_HIGH,PWR6_VSN_AVEE_UA_LOW);
	printf("LED1 = %d %d %d %d %d %d\r\n",PWR7_LED1N_HIGH,PWR7_LED1N_LOW,PWR7_LED1N_A_HIGH,PWR7_LED1N_A_LOW,PWR7_LED1N_UA_HIGH,PWR7_LED1N_UA_LOW);
	printf("LED2 = %d %d %d %d %d %d\r\n",PWR8_LED2N_HIGH,PWR8_LED2N_LOW,PWR8_LED2N_A_HIGH,PWR8_LED2N_A_LOW,PWR8_LED2N_UA_HIGH,PWR8_LED2N_UA_LOW);
	printf("LED3 = %d %d %d %d %d %d\r\n",PWR9_LED3N_HIGH,PWR9_LED3N_LOW,PWR9_LED3N_A_HIGH,PWR9_LED3N_A_LOW,PWR9_LED3N_UA_HIGH,PWR9_LED3N_UA_LOW);
#endif
	
}



/*******************************************************************************
* 文件名	  	 : Load_Pic
* 描述	         : 加载图片
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void Load_Pic(void)
{
	u8 i = 0;
	char str_tmp[22]="";
	char str_tmp1[5]="";
	char str_tmp2[6]="";

	#if DEBUG_BIN_CFG	
	TESTER_MAIN_DEBUG("pic index arr[0] = 0x%x 0x%x 0x%x 0x%x\t\r\n",PatternManage.Pattern_Index_Buf[0],PatternManage.Pattern_Index_Buf[1],PatternManage.Pattern_Index_Buf[2],PatternManage.Pattern_Index_Buf[3]);
	TESTER_MAIN_DEBUG("pic name arr[0] = 0x%x 0x%x 0x%x 0x%x\t\r\n",PatternManage.Pic_Name_Buf[0],PatternManage.Pic_Name_Buf[1],PatternManage.Pic_Name_Buf[2],PatternManage.Pic_Name_Buf[3]);
	TESTER_MAIN_DEBUG("Pic_Index_Max = %d\t\r\n",PatternManage.Pic_Index_Max);
	TESTER_MAIN_DEBUG("Pure_Index_Max = %d\t\r\n",PatternManage.Pure_Color_Index_Buf[0]);
	#endif

	#if 0		//获取pic锁定时间
	//printf("index max = %d\r\n",Get_Max_Index());
	for(i = 0; i < PatternManage.Pic_Index_Max; i++)
	{
		printf("lock time[%d] = %d\r\n",i,Pic_Lock_Time[i]);
	}
	#endif

	G_disp_width = ParaManage.Resolution_X;
	G_disp_height = ParaManage.Resolution_Y;

	TESTER_MAIN_DEBUG("G_disp_height=%d,G_disp_width=%d\r\n",G_disp_height,G_disp_width);
	Init_Mem_BY_Resolution(G_disp_height,G_disp_width);

#if 1		//显示图片
	for(i=0;i<PatternManage.Pic_Index_Max;i++)					//根据上位机配置选择
	{		
		if(G_disp_height >1920)
		{//WQHD
			snprintf(str_tmp, 20, "0:/PIC/WQHD/%d.bmp", i);
			
		}else if(G_disp_height >1280)
		{//FHD
			if(PatternManage.Pic_Name_Buf[i] < 0xFFFE)
			{
				snprintf(str_tmp, 19, "0:/PIC/FHD/%d.bmp", PatternManage.Pic_Name_Buf[i]);
				Load_BMP_Image(str_tmp,PatternManage.Pattern_Index_Buf[i]);//加载图片
				TESTER_MAIN_DEBUG("Load_BMP_Image(%s,%d)\r\n",str_tmp,PatternManage.Pattern_Index_Buf[i]);	
			}
			else if(PatternManage.Pic_Name_Buf[i] == 0xFFFF)
			{
				FPGA_Lcd_Clear(PatternManage.Pure_Color_Index_Buf[i],PatternManage.Pattern_Index_Buf[i]);				
			}
			else if(PatternManage.Pic_Name_Buf[i] == 0xFFFE)		//此命令在config 1.15版本后被取消
			{
				//FPGA_Lcd_Clear(FPGA_COLOUR_USR1,PatternManage.Pattern_Index_Buf[i]);		//刷黑色图片
			}
		}
		else
		{//qHD or lower
			//snprintf(str_tmp, 18, "0:/PIC/HD/%d.bmp", i);
			if(PatternManage.Pic_Name_Buf[i] < 0xFFFE)
			{
				snprintf(str_tmp, 19, "0:/PIC/HD/%d.bmp", PatternManage.Pic_Name_Buf[i]);
				Load_BMP_Image(str_tmp,PatternManage.Pattern_Index_Buf[i]);
				TESTER_MAIN_DEBUG("Load_BMP_Image(%s,%d)\r\n",str_tmp,PatternManage.Pattern_Index_Buf[i]);	
			}
			else if(PatternManage.Pic_Name_Buf[i] == 0xFFFF)
			{
				FPGA_Lcd_Clear(PatternManage.Pure_Color_Index_Buf[i],PatternManage.Pattern_Index_Buf[i]);				
			}
			else if(PatternManage.Pic_Name_Buf[i] == 0xFFFE)		//此命令在config 1.15版本后被取消
			{
				//FPGA_Lcd_Clear(FPGA_COLOUR_USR1,PatternManage.Pattern_Index_Buf[i]);	
			}			
		}		

		if(i == 0)
		{
			snprintf(str_tmp2, 6, "(%2d):", PatternManage.Pic_Index_Max);
			LCD_Rectangle(8+8*3,319-8-16*2+4,136,16,WHITE);
			LCD_Str_O(8+8*3,319-8-16*2+4,"Loading Picture",RED);	
			LCD_Str_O(8+8*3+8*15,319-8-16*2+4,str_tmp2,BLACK);

			Gui_DrawFont_GBK16(2,16*6,RED,BK_COLOR,"Load Pic");
			Gui_DrawFont_GBK16(2+8*8,16*6,BLACK,BK_COLOR,str_tmp2);
			
		}
		snprintf(str_tmp1, 3, "%2d", (i+1));
		LCD_Rectangle(8+8*3+8*20,319-8-16*2+4,16,16,WHITE);
		LCD_Str_O(8+8*3+8*20,319-8-16*2+4,str_tmp1,GREEN);	

		strcat(str_tmp1,"  ");
		Gui_DrawFont_GBK16(2+8*13,16*6,GREEN,BK_COLOR,str_tmp1);
		
		if(i==0)
		{
			Gui_DrawFont_GBK16(2,16*7,BLACK,BK_COLOR,"PORT:");
			if(G_Select_Port == 3)
			{
				Gui_DrawFont_GBK16(2+8*5,16*7,RED,BK_COLOR,"A+B");
			}
			else
			{
				Gui_DrawFont_GBK16(2+8*5,16*7,RED,BK_COLOR,"A  ");
			}
		}
		

		if(Get_Key_Right_Long_Val() == 1)			//right long key
		{
			Clear_Key_Right_Long_Val();
			if(G_Select_Port == 3)
			{
				G_Select_Port = 1;
				Gui_DrawFont_GBK16(2+8*5,16*7,RED,BK_COLOR,"A  ");
			}
			else
			{
				G_Select_Port = 3;
				Gui_DrawFont_GBK16(2+8*5,16*7,RED,BK_COLOR,"A+B");
			}
		}
	}
	
#else		//显示内容
		
	
#endif 

}



void Get_Power_On_Delay(u16 val)
{
	if(Flag_Power_On_Seq_Start)
	{
		if(Power_On_Seq_Index_Cnt <= 6)
		{
			Power_On_Seq_Delay[Power_On_Seq_Index_Cnt] = val;
		}
	}
}



/*******************************************************************************
* 文件名	  	 : Info_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
void Power_On_Handle(u8 chn,u8 val)//上电时序
{
	u16 temp = 0;

	if(chn <= 6)
	{
		if(Flag_Power_On_Seq_Start)
		{
			Power_On_Seq_Index_Cnt++;
			Power_On_Seq_Index[Power_On_Seq_Index_Cnt] = chn;	
			if(Power_On_Seq_Index_Cnt == 6)
			{
				Power_On_Seq_Index_Cnt = 7;
			}
		}
	}
	
	switch(chn)		//电压通道
	{
		case 1:
			switch(val)
			{
				case POWER_CTRL_OFF:
					PWR1_IOVCC_On_Status = POWER_OFF;
					ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_OFF);
					TPS22993_Sel_Dev(0);	
					TPS22993_Off(TPS22993_CHN2);

					PWR14_EXT_PWR_B_On_Status = POWER_OFF;
					ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF); 
					Iovcc_Power_Status = 0;					
					break;
				case POWER_CTRL_ON:	
					#if 0
					if( PWR1_IOVCC_On_Status != POWER_ON)
					{
						ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
						TPS22993_Sel_Dev(0);	
						TPS22993_On(TPS22993_CHN2);
						 PWR1_IOVCC_On_Status = POWER_ON;
					}
					#else
						ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
						TPS22993_Sel_Dev(0);	
						TPS22993_On(TPS22993_CHN2);
						 PWR1_IOVCC_On_Status = POWER_ON;

						 PWR14_EXT_PWR_B_On_Status = POWER_ON;
						ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON); 
						Iovcc_Power_Status = 1;						
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:					
					break;
				case POWER_CTRL_PROTECT_OFF:					
					break;
			}
		break;
		
		case 2:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR2_VDDH_On_Status = POWER_OFF;
					ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_OFF); 
					TPS22993_Sel_Dev(1);	
					TPS22993_Off(TPS22993_CHN3);
					Lcdvdd_Power_Status = 0;					
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR2_VDDH_On_Status != POWER_ON)
					{
						ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(1);	
						TPS22993_On(TPS22993_CHN3);
						 PWR2_VDDH_On_Status = POWER_ON;
					}
					#else
						ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(1);	
						TPS22993_On(TPS22993_CHN3);
						PWR2_VDDH_On_Status = POWER_ON;
						Lcdvdd_Power_Status = 1;					
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					
					break;
				case POWER_CTRL_PROTECT_OFF:
					
					break;
			}
		break;
		
		case 3:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR3_TP_VDD_On_Status = POWER_OFF;
					ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_OFF); 
					TPS22993_Sel_Dev(1);	
					TPS22993_Off(TPS22993_CHN1);
					Tpvdd_Power_Status = 0;
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR3_TP_VDD_On_Status != POWER_ON)
					{						
						ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON); 	
						TPS22993_Sel_Dev(1);	
						TPS22993_On(TPS22993_CHN1);
						 PWR3_TP_VDD_On_Status = POWER_ON;
					}
					#else
						ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON); 	
						TPS22993_Sel_Dev(1);	
						TPS22993_On(TPS22993_CHN1);
						 PWR3_TP_VDD_On_Status = POWER_ON;
						Tpvdd_Power_Status = 1;
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:					
					break;
				case POWER_CTRL_PROTECT_OFF:					
					break;
			}
		break;
		
		case 4:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR4_TP_VIO_On_Status = POWER_OFF;
					ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF); 
					TPS22993_Sel_Dev(1);	
					TPS22993_Off(TPS22993_CHN2);
					Tpvio_Power_Status = 0;
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR4_TP_VIO_On_Status != POWER_ON)
					{
						ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);
						TPS22993_Sel_Dev(1);	
						TPS22993_On(TPS22993_CHN2);
						 PWR4_TP_VIO_On_Status = POWER_ON;
					}
					#else
						ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);
						TPS22993_Sel_Dev(1);	
						TPS22993_On(TPS22993_CHN2);
						 PWR4_TP_VIO_On_Status = POWER_ON;
						Tpvio_Power_Status = 1;
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 5:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR5_VSP_AVDD_On_Status = POWER_OFF;

					temp = FPGA_Read_Register(0x1c);
					FPGA_Write_Register(0x1C,temp & 0xFD);	
					
					
					Vsp_Power_Status = 0;

					if(Flag_Vsp_Vsn == 1)
					{
						ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_OFF); 
						TPS22993_Sel_Dev(0);	
						TPS22993_Off(TPS22993_CHN1);
					}
					Flag_Vsp_Vsn = 1;
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR5_VSP_AVDD_On_Status != POWER_ON)
					{
						 PWR5_VSP_AVDD_On_Status = POWER_ON;

						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp | 0x02);	

						//Tps65132_Init(5400,5400); 
						//Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);
						
						ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(0);	
						TPS22993_On(TPS22993_CHN1);
					}
					#else
						PWR5_VSP_AVDD_On_Status = POWER_ON;

						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp | 0x02);	

						//Tps65132_Init(5400,5400); 
						Delay_us(50);
						Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);
						
						ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(0);	
						TPS22993_On(TPS22993_CHN1);
						Vsp_Power_Status = 1;
						Flag_Vsp_Vsn = 0;
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 6:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR6_VSN_AVEE_On_Status = POWER_OFF;

					temp = FPGA_Read_Register(0x1c);
					FPGA_Write_Register(0x1C,temp & 0xEF);

					Vsn_Power_Status = 0;

					if(Flag_Vsp_Vsn == 1)
					{
						ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_OFF); 
						TPS22993_Sel_Dev(0);	
						TPS22993_Off(TPS22993_CHN1);
					}
					Flag_Vsp_Vsn = 1;
					
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR6_VSN_AVEE_On_Status != POWER_ON)
					{
						 PWR6_VSN_AVEE_On_Status = POWER_ON;

						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp | 0x10);

						//Tps65132_Init(5600,5600);  
						Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);
						Vsn_Power_Status = 1;
						
						ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(0);	
						TPS22993_On(TPS22993_CHN1);
					}
					#else
						 PWR6_VSN_AVEE_On_Status = POWER_ON;

						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp | 0x10);

						//Tps65132_Init(5600,5600);  
						Delay_us(50);
						Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);
						Vsn_Power_Status = 1;
						Flag_Vsp_Vsn = 0;
						
						ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(0);	
						TPS22993_On(TPS22993_CHN1);
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 7:
			switch(val)
			{
				case POWER_CTRL_OFF:
					//BL_LM36923_POWER_OFF;	//1?±31a?÷μ??′	
					LM36923_Off(BL_CHN1);		
					PWR7_LED1N_On_Status = POWER_OFF;	
					temp = FPGA_Read_Register(0x1c);
					FPGA_Write_Register(0x1C,temp | 0x20);	
					//LM36923_AllOff();
					
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR7_LED1N_On_Status != POWER_ON)
					{
						 PWR7_LED1N_On_Status = POWER_ON;
						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp & 0x00DF);
						LM36923_AllOn();
						//LM36923_PWR_ON_Cfg( PWR7_LED1N/5);	
					}
					#else
						//BL_LM36923_POWER_ON;		//?a±31a?÷μ??′	
						//LM36923_PWR_ON_Cfg( PWR7_LED1N);		//??D?éè??±31aμ?á÷
						
						 PWR7_LED1N_On_Status = POWER_ON;
						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp & 0x00DF);
						//LM36923_AllOn();
						LM36923_On(BL_CHN1);
						
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 8:
			switch(val)
			{
				case POWER_CTRL_OFF:
					//BL_LM36923_POWER_OFF;	//关背光主电源	
					LM36923_Off(BL_CHN2);
					PWR8_LED2N_On_Status = POWER_OFF;	
					
					//temp = FPGA_Read_Register(0x1c);
					//FPGA_Write_Register(0x1C,temp | 0x20);
					
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR8_LED2N_On_Status != POWER_ON)
					{
						 PWR8_LED2N_On_Status = POWER_ON;
						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp & 0x00DF);
						//LM36923_PWR_ON_Cfg( PWR7_LED1N *1);	
					}
					#else
						//BL_LM36923_POWER_ON;		//?a±31a?÷μ??′
						//LM36923_PWR_ON_Cfg( PWR7_LED1N);		//??D?éè??±31aμ?á÷
						
						PWR8_LED2N_On_Status = POWER_ON;
						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp & 0x00DF);
						LM36923_On(BL_CHN2);
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 9:
			switch(val)
			{
				case POWER_CTRL_OFF:
					//BL_LM36923_POWER_OFF;	//1?±31a?÷μ??′	
					
					LM36923_Off(BL_CHN3);
					PWR9_LED3N_On_Status = POWER_OFF;
					
					//temp = FPGA_Read_Register(0x1c);
					//FPGA_Write_Register(0x1C,temp | 0x20);
					
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR9_LED3N_On_Status != POWER_ON)
					{
						 PWR9_LED3N_On_Status = POWER_ON;
						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp & 0x00DF);
						//LM36923_PWR_ON_Cfg( PWR7_LED1N *1);	
					}
					#else
						//BL_LM36923_POWER_ON;		//?a±31a?÷μ??′	
						//LM36923_PWR_ON_Cfg( PWR7_LED1N);		//??D?éè??±31aμ?á÷
						
						 PWR9_LED3N_On_Status = POWER_ON;
						temp = FPGA_Read_Register(0x1c);
						FPGA_Write_Register(0x1C,temp & 0x00DF);
						LM36923_On(BL_CHN3);
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 10:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR10_VMTP_On_Status = POWER_OFF;
					 LT3582_Off();
					break;
				case POWER_CTRL_ON:
					 PWR10_VMTP_On_Status = POWER_ON;
					 LT3582_On();
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 11:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR11_VNEG_On_Status = POWER_OFF;
					 LT3582_Off();
					break;
				case POWER_CTRL_ON:
					 PWR11_VNEG_On_Status = POWER_ON;
					 LT3582_On();
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 12:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR12_LED_PWR_5V_On_Status = POWER_OFF;
					break;
				case POWER_CTRL_ON:
					 PWR12_LED_PWR_5V_On_Status = POWER_ON;
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 13:
			switch(val)
			{
				case POWER_CTRL_OFF:
					 PWR13_EXT_PWR_A_On_Status = POWER_OFF;
					 ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF); 
					ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_OFF); 
					TPS22993_Sel_Dev(1);	
					TPS22993_Off(TPS22993_CHN4);
					break;
				case POWER_CTRL_ON:
					#if 0
					if( PWR13_EXT_PWR_A_On_Status != POWER_ON)
					{
						 PWR13_EXT_PWR_A_On_Status = POWER_ON;
						ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(0);	
						TPS22993_On(TPS22993_CHN4);
					}
					#else
						 PWR13_EXT_PWR_A_On_Status = POWER_ON;
						ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);
						ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON); 
						TPS22993_Sel_Dev(1);	
						TPS22993_On(TPS22993_CHN4);
					#endif
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
		
		case 14:
			
		break;

		case 15:
			switch(val)
			{
				case POWER_CTRL_OFF:				
						 PWR15_TESTER_VBUS_On_Status = POWER_OFF;
						ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_OFF);								 
					break;
				case POWER_CTRL_ON:					
						 PWR15_TESTER_VBUS_On_Status = POWER_ON;
						ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON); 					
					break;
				case POWER_CTRL_PROTECT_ON:
					break;
				case POWER_CTRL_PROTECT_OFF:
					break;
			}
		break;
	}
}



/*******************************************************************************
* 文件名	  	 : MutiBuff_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static u8 MutiBuff_Handle(u32 add,u16 size)
{
	u16 cnt = 0,cnt1 = 0;
	u32 br = 0,btr = 0,j = 0;
	u8 *pbuf;

	
	j = 0;
	pbuf = G_DISP_FILE_BUFFER;
	cnt = size;
		
	BinProctcoal_Handle(&pbuf[add],size);
		
	TESTER_MAIN_DEBUG("Read %d bytes of data.\r\n", size);

	return(0);
}


 u8 MutiBuff_HandleE(u32 add,u16 size)
{
	u16 cnt = 0,cnt1 = 0;
	u32 br = 0,btr = 0,j = 0;
	u8 *pbuf;

	
	j = 0;
	pbuf = G_DISP_FILE_BUFFER;
	cnt = size;
		
	BinProctcoal_HandleE(&pbuf[add],size);
		
	TESTER_MAIN_DEBUG("Read %d bytes of data.\r\n", size);

	return(0);
}


/*******************************************************************************
* 文件名	  	 : Initial_Code_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static void Initial_Code_Handle(void)
{
	u32 br = 0,btr = 0,j = 0;
	u8 *pbuf;
	
	uint16_t fpga_version;
	char fpga_version_char[11];

	Read_Start_Address(READ_START_ADD_INFO);		//读取常规设置的地址和size	
	Info_Add =  General_Info_Add;
	Info_Add_Size =  General_Info_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("info_add = 0x%x 0x%x\r\n", General_Info_Add, General_Info_Size);	
	#endif	
	MutiBuff_Handle( General_Info_Add, General_Info_Size);	
	PowerUp_Init_Handle();		//解析完了参数，然后初始化电源芯片
	
	ASH204A_Test(1,1);		// LCM2000 + FPGA, Need run ASH204A_Test(1,1), otherwise FPGA will not working

	fpga_version = FPGA_Read_Register(0x27);
	LCD_Str_O(8,10+16,"FPGA: ",BLACK);
	snprintf(fpga_version_char, 5, "%X", fpga_version);
	LCD_Str_O(8+8*6,10+16,fpga_version_char,RED);

	Gui_DrawFont_GBK16(2,16*2,BLACK,BK_COLOR,"FPGA: ");
	Gui_DrawFont_GBK16(2+8*6,16*2,RED,BK_COLOR,fpga_version_char);

	Delay_ms(100);
	Get_INIT_STATUS(INIT_STATUS_VER);		//获取电源板ver
	Delay_ms(100);
	fpga_version = Return_Power_Version();
	//printf("version = %d\r\n",fpga_version);
	LCD_Str_O(8,10+16*2,"POWER: ",BLACK);
	snprintf(fpga_version_char, 5, "%.2f", ((float)fpga_version)/100);
	LCD_Str_O(8+8*7,10+16*2,fpga_version_char,RED);

	Gui_DrawFont_GBK16(2,16*3,BLACK,BK_COLOR,"POWER: ");
	Gui_DrawFont_GBK16(2+8*7,16*3,RED,BK_COLOR,fpga_version_char);

	////////////////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_PATTEN);		//读取图片
	Pattern_Add =  Pattern_Set_Add;
	Pattern_Add_Size =  Pattern_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("patten_add = 0x%x 0x%x\r\n", Pattern_Set_Add, Pattern_Set_Size);
	#endif
	MutiBuff_Handle( Pattern_Set_Add, Pattern_Set_Size);
	if(Get_Usb_Cmd() != 3)
	{
		Load_Pic();
	}
	Pic_Num_Max = PatternManage.Pic_Index_Max;
	

	///////////////////////////////////////////////////////////////////////////////	
	Read_Start_Address(READ_START_ADD_BOARD);		//读取board发送数据	
	Board_Init_Add =  Board_Add;
	Board_Init_Add_Size =  Board_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("board_add = 0x%x 0x%x\r\n", Board_Add, Board_Size);
	#endif
	Lcm_Fpga_init(); //on

	//MutiBuff_Handle( Board_Add, Board_Size);//--
	Lcm_SSD2828_init();

	


	///////////////////////////////////////////////////////////////////////////////	
	Read_Start_Address(READ_START_ADD_POWER_ON);		//读取power on数据	
	PowerOn_Add =  Power_On_Set_Add;
	PowerOn_Add_Size =  Power_On_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("on_add = 0x%x 0x%x\r\n", Power_On_Set_Add, Power_On_Set_Size);
	#endif


	///////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_INIT_CODE);		//读取lcd init数据
	Code_Init_Add =  Init_Code_Add;
	Code_Init_Add_Size =  Init_Code_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("init_add = 0x%x 0x%x\r\n", Init_Code_Add, Init_Code_Size);
	#endif
	//MutiBuff_Handle( Init_Code_Add, Init_Code_Size);			//记录下地址和size
	
	///////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_POWER_OFF);		//读取power off数据	
	PowerOff_Add =  Power_Off_Set_Add;
	PowerOff_Add_Size =  Power_Off_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("off_add = 0x%x 0x%x\r\n", Power_Off_Set_Add, Power_Off_Set_Size);
	#endif


	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_SLEEP_SEQ);		//读取休眠SLEEP数据	
	Sleep_Seq_Add =  Sleep_Seq_Set_Add;
	Sleep_Seq_Add_Size =  Sleep_Seq_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("off_add = 0x%x 0x%x\r\n", Sleep_Seq_Set_Add, Sleep_Seq_Set_Size);
	#endif


	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_INIT2_COD);		//读取用户定义数据	
	Code_Init2_Add =  Init2_Code_Add;
	Code_Init2_Add_Size =  Init2_Code_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("off_add = 0x%x 0x%x\r\n", Init2_Code_Add, Init2_Code_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE1);		//读取cmd1数据	
	Code_Cmd1_Add =  Cmd_Code1_Add;
	Code_Cmd1_Add_Size =  Cmd_Code1_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code1 = 0x%x 0x%x\r\n", Cmd_Code1_Add, Cmd_Code1_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE2);		//读取cmd2数据	
	Code_Cmd2_Add =  Cmd_Code2_Add;
	Code_Cmd2_Add_Size =  Cmd_Code2_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code2 = 0x%x 0x%x\r\n", Cmd_Code2_Add, Cmd_Code2_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE3);		//读取cmd3数据	
	Code_Cmd3_Add =  Cmd_Code3_Add;
	Code_Cmd3_Add_Size =  Cmd_Code3_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code3 = 0x%x 0x%x\r\n", Cmd_Code3_Add, Cmd_Code3_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE4);		//读取cmd4数据	
	Code_Cmd4_Add =  Cmd_Code4_Add;
	Code_Cmd4_Add_Size =  Cmd_Code4_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code4 = 0x%x 0x%x\r\n", Cmd_Code4_Add, Cmd_Code4_Size);
	#endif
	
	#if 0
	if(Get_PowerSeq_Index())		//上电状态
	{
		MutiBuff_Handle( Power_On_Set_Add, Power_On_Set_Size);		//执行上电程序
	}
	else
	{
		MutiBuff_Handle( Power_Off_Set_Add, Power_Off_Set_Size);	//执行掉电程序
	}
	#endif
	
}


/*******************************************************************************
* 文件名	  	 : Bin_Option_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
static void Bin_Option_Handle(u8 cmd,u16 val)
{
	switch(cmd)
	{	
		case 1:
			Flag_Current_En = val;		
			break;
		case 2:
			Flag_Voltage_En = val;		
			break;
		case 3:
			Flag_Sleep_Current_En = val;
			break;
		case 4:
			Flag_Hard_ID_En = (val&0xff00)>>8;
			Hard_ID_En_Val = (val&0x00ff);
			break;
		case 5:
			Flag_Soft_ID_En = val;
			break;
		case 6:
			Soft_ID12_Val = val;
			break;
		case 7:
			Soft_ID34_Val = val;
			break;
		case 8:
			Flag_OTP_Times_En = val;
			break;
		case 9:
			OTP_Times_High = (val&0xff00)>>8;
			OTP_Times_Low = (val&0x00ff);
			break;
		case 10:
			Flag_DuiWei_En = val;		
			break;
		case 11:
			Flag_PWM_En = val;
			break;
		case 12:
			PWM_Seq_Low = val;
			break;
		case 13:
			PWM_Seq_High= val;
			break;
		case 14:
			PWM_Duty_Low = val;
			break;
		case 15:
			PWM_Duty_High = val;
			break;
		case 16:
			PWM_Volt_Low = val;
			break;
		case 17:
			PWM_Volt_High = val;
			break;
		case 18:
			Flag_TE_En = val;
			break;
		case 19:
			TE_Seq_Low = val;
			break;
		case 20:
			TE_Seq_High= val;
			break;
		case 21:
			TE_Duty_Low = val;
			break;
		case 22:
			TE_Duty_High = val;
			break;
		case 23:
			TE_Volt_Low = val;
			break;
		case 24:
			TE_Volt_High = val;
			break;
		case 25:
			FLAG_BK_En = val;
			break;
		case 26:			
			if(val & 0x8000)
			{
				AutoTestTp = 1;
			}
			else
			{
				AutoTestTp = 0;	
			}
			AutoTestTpDelay = val & 0x7fff;
			break;
		case 100:
			Flag_Key_OnOff_Short = (val&0xF000)>>12;
			Flag_Key_OnOff_Long = (val&0x0F00)>>8;
			Key_OnOff_NormalPress = (val&0x00F0)>>4;
			Key_OnOff_LongPress = val&0x000F;
			break;
		case 101:
			Flag_Key_Left_Short = (val&0xF000)>>12;
			Flag_Key_Left_Long = (val&0x0F00)>>8;
			Key_Left_NormalPress = (val&0x00F0)>>4;
			Key_Left_LongPress = val&0x000F;
			break;
		case 102:
			Flag_Key_Auto_Short = (val&0xF000)>>12;
			Flag_Key_Auto_Long = (val&0x0F00)>>8;
			Key_Auto_NormalPress = (val&0x00F0)>>4;
			Key_Auto_LongPress = val&0x000F;
			break;
		case 103:
			Flag_Key_Right_Short = (val&0xF000)>>12;
			Flag_Key_Right_Long = (val&0x0F00)>>8;
			Key_Right_NormalPress = (val&0x00F0)>>4;
			Key_Right_LongPress = val&0x000F;
			break;
		case 104:
			Flag_Key_Test_Short = (val&0xF000)>>12;
			Flag_Key_Test_Long = (val&0x0F00)>>8;
			Key_Test_NormalPress = (val&0x00F0)>>4;
			Key_Test_LongPress = val&0x000F;
			break;			
		case 105:
			Key_Lock_Time = val;
			break;
	}
	
}


#define DEBUG_BIN_CFG1
/*******************************************************************************
* 文件名	  	 : MutiBuff_Handle
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/

int BinProctcoal_Handle(u8 *buf, u32 size)//Size 为4的整数倍
{
	u32 i = 0;
	u16 pDATA = 0;
	u8  pAdd = 0;
	u8  pic_index = 0;
	u8  name_arr[2];

	
	if(size%4)
	{
		TESTER_MAIN_DEBUG("File Size is incorrect.\r\n");	
		return TP_ERR;
	}
	
	for(i=0;i<size;i= i+4)
	{	
		 //Write data to FPGA FSMC
		 pDATA = buf[i+1]; 
		 pDATA = pDATA << 8;
		 pDATA |= buf[i];
		 //FPGA FSMC ADDRESS
		 pAdd = buf[i+2];
		TESTER_MAIN_DEBUG("\r\n %x %x %x %x",buf[i],buf[i+1],buf[i+2],buf[i+3]);
		#if 0
		if(size == 0x40)
		{
			TESTER_MAIN_DEBUG("i = %d\r\n",i);
		}
		#endif
		
		
		 switch (buf[i+3])
		 {
			case FSMC_WRITE_DATA:
			  #ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_WRITE_DATA:   ");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x    ",i+3,buf[i+3]);	
			  #endif
			  	
			  FPGA_Write_Register(pAdd,pDATA);

				G_Reg_Y = pDATA;
				
				if(pAdd == 0x03){//Display width
					G_disp_width = pDATA;

				}else if(pAdd == 0x07){//Display height
					G_disp_height = pDATA;
				}
			
				break;	
			
			case FSMC_READ_DATA:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_READ_DATA:   ");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x ",i+3,buf[i+3]);
			  #endif
				G_Reg_X = FPGA_Read_Register(pAdd);
				break;
			
			case FSMC_READ_OPT:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_READ_OPT:   ");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x \r\n",i+3,buf[i+3]);
			  	#endif
				
	 			if(buf[i+2] == 0)       //AND
				{
					G_Reg_Y = pDATA;
					G_Reg_X = G_Reg_X & G_Reg_Y;		
				}
	  			else if(buf[i+2] == 1)  //OR
				{
					G_Reg_Y = pDATA;
					G_Reg_X = G_Reg_X | G_Reg_Y;
				}	
	 			 else if(buf[i+2] == 2) //NOT
				{
					G_Reg_X = ~G_Reg_X;
				}	
	  			else if(buf[i+2] == 3) //XOR
				{
					G_Reg_Y = pDATA;
					G_Reg_X = G_Reg_X ^ G_Reg_Y;
				}	
	  			else
				{
					return TP_ERR;
				}	
				TESTER_MAIN_DEBUG("\r\nG_Reg_X:%d, G_Reg_Y:%d   \r\n",G_Reg_X,G_Reg_Y);
				
			break;	
			
			case FSMC_RESULT_WRITE:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_RESULT_WRITE:");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x   ",i+3,buf[i+3]);
					TESTER_MAIN_DEBUG("G_Reg_X=%x   FPGA_Write_Register(pAdd,G_Reg_X)",G_Reg_X);
			 	 #endif
			
				FPGA_Write_Register(pAdd,G_Reg_X);
				if(FPGA_FSMC_GetStatus(FPGA_TIMEOUT) != 0)
				{
					TESTER_MAIN_DEBUG("FPGA_FSMC_GetStatus timeout.");
					return TP_ERR;
				}
			  
				break;	
			
			case FSMC_MIPI_CTRL:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_MIPI_CTRL:");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x    ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x   ",i+3,buf[i+3]);
			  	#endif
				break;	
			
			case FSMC_DELAY_OPT:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_DELAY_OPT:");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x   ",i+3,buf[i+3]);
			    #endif

				Get_Power_On_Delay(pDATA);
				
				if(buf[i+2] == 0) //ms
				{
					//Delay_ms((u32)(pDATA));
					SysCtlDelay( ((u32)(pDATA)) * SYS_1MS);
				}
				else if(buf[i+2] == 1) //10us
				{
					//Delay_us((u32)(pDATA / 10)); //(10us*pDATA)/10us
					SysCtlDelay( ((u32)(pDATA)) * SYS_1US);
				}
				else
				{
					return TP_ERR;
				}

				break;
			//////////////////////////////////////////////////////////161123 新添加部分
			case REG_RESOLUTION:		//分辨率获取					
				if(pAdd == 0)			//x坐标
				{
					ParaManage.Resolution_X = pDATA;
				}
				else if(pAdd == 1)		//Y坐标
				{
					ParaManage.Resolution_Y = pDATA;
				}	
				else if(pAdd == 2)		//版本号
				{
					ParaManage.Version = pDATA;//
				}
				else if(pAdd == 3)		//port 
				{					
					//printf("port = %d\r\n",pDATA);
					if(Flag_Port == 0)
					{
						Flag_Port = 1;
						G_Select_Port = pDATA;
					}
				}
				else if(pAdd == 4)		//lcd or oled
				{
					//printf("lcd = %d\r\n",pDATA);					
				}
				else if(pAdd >= 101 && pAdd <= 125)		//bin name
				{
					name_arr[0] = pDATA >> 8;
					name_arr[1] = pDATA;					
					strcat(ParaManage.Bin_Name,name_arr);
				}
				break;
				

			case REG_SET_VOLTAGE:	     //电压参数设置
				BinSetVoltVal_Handle(pAdd,pDATA);//
				break;		

			case REG_VOLTAGE_HIGH:	     //电压参数设置
				BinSetVoltHigh_Handle(pAdd,pDATA);
				break;	

			case REG_VOLTAGE_LOW:	     //电压参数设置
				BinSetVoltLow_Handle(pAdd,pDATA);
				break;

			case REG_CURRENT_HIGH:	     //电压参数设置
				BinSetCurrHigh_Handle(pAdd,pDATA);
				break;

			case REG_CURRENT_LOW:	     //电压参数设置
				BinSetCurrLow_Handle(pAdd,pDATA);
				break;

			case REG_PATTERN_CTRL:
				if(pAdd >= 0 && pAdd <= 99)
				{
					if(pDATA == 0xffff)		//读取pattern 信息 
					{
						;//是纯色信息
					}
					else
					{
						
					}	
					PatternManage.Pattern_Index_Buf[pic_index] = pAdd;
					PatternManage.Pic_Name_Buf[pic_index] = pDATA;
					
					pic_index++;

					PatternManage.Pic_Index_Max = pic_index;			//记录下索引最大值
					
				}
				else if(pAdd >= 100 && pAdd <= 199)
				{
					Pic_Lock_Time[pAdd - 100] = pDATA;
					//printf(" lock time = %d\r\n",Pic_Lock_Time[pAdd - 100]);
				}
				break;		

			case REG_SLEEP_SCAN:
				if(pAdd == 0)		//IMG
				{
					Pic_Cmd_Index[pic_index-1] = pDATA;
				}
				else if(pAdd == 1)	//CMD
				{
					if(pDATA == 0)
					{
						Auto_Sleep_Index = pic_index;					
					}
					else if(pDATA == 1)		//支持自动启动
					{
						Auto_Sleep_Index = pic_index;
						Auto_Sleep_Mode = 1;						
					}
				}				
				break;
			
			case REG_PATTERN_RGB:

				PatternManage.Pure_Color_Index_Buf[pic_index-1] = ((u32)buf[i+2])<<16 |((u32)buf[i+1])<<8  | buf[i+0];
				break;	

			case REG_POWER_SEQ:	
				
				Power_On_Handle(pAdd,pDATA);
				break;	

			case REG_SHOW_PIC:
				
				Fpga_Disp_Manage.Fpga_Disp_Index = pDATA;
				PowerOn_Pic_Index = Fpga_Disp_Manage.Fpga_Disp_Index;			//记录上电运行的图片
				//setAndShowPic(Fpga_Disp_Manage.Fpga_Disp_Index,1);
				if((Get_Sleep_Status() == 0) && (Get_Sys_Err_Flag() == 0)) 	//非睡眠模式下
				{				
					clearRGBShowPic(3,Fpga_Disp_Manage.Fpga_Disp_Index,1);
				}
				//Fpga_Disp_Manage.Fpga_Disp_Flag_Fresh = 1;				
				break;		

			case REG_OPTION:			
				//选项功能配置
				Bin_Option_Handle(pAdd,pDATA);
				break;	
			
			default:
				break;
			  //return TP_ERR;	
		}				
	}	
	return TP_OK;
}



int BinProctcoal_HandleE(u8 *buf, u32 size)//Size 为4的整数倍
{
	u32 i = 0;
	u16 pDATA = 0;
	u8  pAdd = 0;
	u8  pic_index = 0;
	u8  name_arr[2];

	
	if(size%4)
	{
		TESTER_MAIN_DEBUG("File Size is incorrect.\r\n");	
		return TP_ERR;
	}
	
	for(i=0;i<size;i= i+4)
	{	
		 //Write data to FPGA FSMC
		 pDATA = buf[i+1]; 
		 pDATA = pDATA << 8;
		 pDATA |= buf[i];
		 //FPGA FSMC ADDRESS
		 pAdd = buf[i+2];
		TESTER_MAIN_DEBUG("\r\n %x %x %x %x",buf[i],buf[i+1],buf[i+2],buf[i+3]);
		#if 0
		if(size == 0x40)
		{
			TESTER_MAIN_DEBUG("i = %d\r\n",i);
		}
		#endif
		
		
		 switch (buf[i+3])
		 {
			case FSMC_WRITE_DATA:
			  #ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_WRITE_DATA1:   ");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x    ",i+3,buf[i+3]);	
			  #endif
			  	
			  //FPGA_Write_Register(pAdd,pDATA);

				G_Reg_Y = pDATA;
				
				if(pAdd == 0x03){//Display width
					G_disp_width = pDATA;

				}else if(pAdd == 0x07){//Display height
					G_disp_height = pDATA;
				}
			
				break;	
			
			case FSMC_READ_DATA:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_READ_DATA:   ");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x ",i+3,buf[i+3]);
			  #endif
				G_Reg_X = FPGA_Read_Register(pAdd);
				break;
			
			case FSMC_READ_OPT:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_READ_OPT:   ");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x \r\n",i+3,buf[i+3]);
			  	#endif
				
	 			if(buf[i+2] == 0)       //AND
				{
					G_Reg_Y = pDATA;
					G_Reg_X = G_Reg_X & G_Reg_Y;		
				}
	  			else if(buf[i+2] == 1)  //OR
				{
					G_Reg_Y = pDATA;
					G_Reg_X = G_Reg_X | G_Reg_Y;
				}	
	 			 else if(buf[i+2] == 2) //NOT
				{
					G_Reg_X = ~G_Reg_X;
				}	
	  			else if(buf[i+2] == 3) //XOR
				{
					G_Reg_Y = pDATA;
					G_Reg_X = G_Reg_X ^ G_Reg_Y;
				}	
	  			else
				{
					return TP_ERR;
				}	
				TESTER_MAIN_DEBUG("\r\nG_Reg_X:%d, G_Reg_Y:%d   \r\n",G_Reg_X,G_Reg_Y);
				
			break;	
			
			case FSMC_RESULT_WRITE:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_RESULT_WRITE:");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x   ",i+3,buf[i+3]);
			 	 #endif
			
				FPGA_Write_Register(pAdd,G_Reg_X);
				if(FPGA_FSMC_GetStatus(FPGA_TIMEOUT) != 0)
				{
					TESTER_MAIN_DEBUG("FPGA_FSMC_GetStatus timeout.");
					return TP_ERR;
				}
			  
				break;	
			
			case FSMC_MIPI_CTRL:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_MIPI_CTRL:");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x    ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x   ",i+3,buf[i+3]);
			  	#endif
				break;	
			
			case FSMC_DELAY_OPT:
				#ifdef  DEBUG_BIN_CFG1
					TESTER_MAIN_DEBUG("\r\nFSMC_DELAY_OPT:");
					TESTER_MAIN_DEBUG("pDATA[%d]=%x   ",i,pDATA);
					TESTER_MAIN_DEBUG("pAdd[%d]=%x   ",i,pAdd);
					TESTER_MAIN_DEBUG("buf[%d]=%x   ",i+3,buf[i+3]);
			    #endif

				Get_Power_On_Delay(pDATA);
				
				if(buf[i+2] == 0) //ms
				{
					//Delay_ms((u32)(pDATA));
					SysCtlDelay( ((u32)(pDATA)) * SYS_1MS);
				}
				else if(buf[i+2] == 1) //10us
				{
					//Delay_us((u32)(pDATA / 10)); //(10us*pDATA)/10us
					SysCtlDelay( ((u32)(pDATA)) * SYS_1US);
				}
				else
				{
					return TP_ERR;
				}

				break;
			//////////////////////////////////////////////////////////161123 新添加部分
			case REG_RESOLUTION:		//分辨率获取					
				if(pAdd == 0)			//x坐标
				{
					ParaManage.Resolution_X = pDATA;
				}
				else if(pAdd == 1)		//Y坐标
				{
					ParaManage.Resolution_Y = pDATA;
				}	
				else if(pAdd == 2)		//版本号
				{
					ParaManage.Version = pDATA;//
				}
				else if(pAdd == 3)		//port 
				{					
					//printf("port = %d\r\n",pDATA);
					if(Flag_Port == 0)
					{
						Flag_Port = 1;
						G_Select_Port = pDATA;
					}
				}
				else if(pAdd == 4)		//lcd or oled
				{
					//printf("lcd = %d\r\n",pDATA);					
				}
				else if(pAdd >= 101 && pAdd <= 125)		//bin name
				{
					name_arr[0] = pDATA >> 8;
					name_arr[1] = pDATA;					
					strcat(ParaManage.Bin_Name,name_arr);
				}
				break;
				

			case REG_SET_VOLTAGE:	     //电压参数设置
				BinSetVoltVal_Handle(pAdd,pDATA);
				break;		

			case REG_VOLTAGE_HIGH:	     //电压参数设置
				BinSetVoltHigh_Handle(pAdd,pDATA);
				break;	

			case REG_VOLTAGE_LOW:	     //电压参数设置
				BinSetVoltLow_Handle(pAdd,pDATA);
				break;

			case REG_CURRENT_HIGH:	     //电压参数设置
				BinSetCurrHigh_Handle(pAdd,pDATA);
				break;

			case REG_CURRENT_LOW:	     //电压参数设置
				BinSetCurrLow_Handle(pAdd,pDATA);
				break;

			case REG_PATTERN_CTRL:
				if(pAdd >= 0 && pAdd <= 99)
				{
					if(pDATA == 0xffff)		//读取pattern 信息 
					{
						;//是纯色信息
					}
					else
					{
						
					}	
					PatternManage.Pattern_Index_Buf[pic_index] = pAdd;
					PatternManage.Pic_Name_Buf[pic_index] = pDATA;
					
					pic_index++;

					PatternManage.Pic_Index_Max = pic_index;			//记录下索引最大值
					
				}
				else if(pAdd >= 100 && pAdd <= 199)
				{
					Pic_Lock_Time[pAdd - 100] = pDATA;
					//printf(" lock time = %d\r\n",Pic_Lock_Time[pAdd - 100]);
				}
				break;		

			case REG_SLEEP_SCAN:
				if(pAdd == 0)		//IMG
				{
					Pic_Cmd_Index[pic_index-1] = pDATA;
				}
				else if(pAdd == 1)	//CMD
				{
					if(pDATA == 0)
					{
						Auto_Sleep_Index = pic_index;					
					}
					else if(pDATA == 1)		//支持自动启动
					{
						Auto_Sleep_Index = pic_index;
						Auto_Sleep_Mode = 1;						
					}
				}				
				break;
			
			case REG_PATTERN_RGB:
				//保存图片名字序号或颜色值
				PatternManage.Pure_Color_Index_Buf[pic_index-1] = ((u32)buf[i+2])<<16 |((u32)buf[i+1])<<8  | buf[i+0];
				break;	

			case REG_POWER_SEQ:	
				
				Power_On_Handle(pAdd,pDATA);
				break;	

			case REG_SHOW_PIC:
				
				Fpga_Disp_Manage.Fpga_Disp_Index = pDATA;
				PowerOn_Pic_Index = Fpga_Disp_Manage.Fpga_Disp_Index;			//记录上电运行的图片
				//setAndShowPic(Fpga_Disp_Manage.Fpga_Disp_Index,1);
				if((Get_Sleep_Status() == 0) && (Get_Sys_Err_Flag() == 0)) 	//非睡眠模式下
				{				
					clearRGBShowPic(3,Fpga_Disp_Manage.Fpga_Disp_Index,1);
				}
				//Fpga_Disp_Manage.Fpga_Disp_Flag_Fresh = 1;				
				break;		

			case REG_OPTION:			
				//选项功能配置
				Bin_Option_Handle(pAdd,pDATA);
				break;	
			
			default:
				break;
			  //return TP_ERR;	
		}				
	}	
	return TP_OK;
}

/*******************************************************************************
* 文件名	  	 : Load_Initial_Code1
* 描述	         : 
* 输入           : 
* 输出           : 
* 返回           : 无
*******************************************************************************/
int Load_Initial_Code1(const char *path)
{
	u32 i,br,btr;
	s32 res;
	u32 initCodeFileSzie = 0;
	u8 * ptr;
	
	//1. Read File information
	res = f_stat(path,&binfileInfo);//读取文件信息
	if ( res == FR_OK )
	{
		initCodeFileSzie = binfileInfo.fsize;
		TESTER_MAIN_DEBUG("\r\n initCodeFileSzie Size: %d bytes\n", initCodeFileSzie);
	}

	//2. Open bin File
	res = f_open(&fdst_bin,path,FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK )
	{ 
		#ifdef DEBUG_BIN_CFG
			TESTER_MAIN_DEBUG("\r\nFAILED to Open .bin file.\r\n" ); 
		#endif
		return -1;
	}
	#ifdef DEBUG_BIN_CFG
		TESTER_MAIN_DEBUG("\r\n File successfully Open .bin file.\r\n" ); 
	#endif


	i=0;
	ptr = G_DISP_FILE_BUFFER; // 20k
	while(i<initCodeFileSzie)
	{
		//set read/write bytes number

		#if 0
		f_lseek(&fdst,10);
		f_read(&fdst, ptr, 5, &br );			
		TESTER_MAIN_DEBUG("ptr = 0x%x 0x%x 0x%x 0x%x 0x%x\r\n",*ptr,*(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4));
		#else		
		btr = binfileInfo.fsize;	//文件大小
		res = f_read(&fdst_bin, ptr, btr, &br ); /* 将文件里面的内容读到缓冲区 */
		#endif

		if ( (res != FR_OK) | (br != btr)  ){//读取到的大小不等于文件大小
			#ifdef DEBUG_LCM_COMM
				TESTER_MAIN_DEBUG("Read .bin FILE Failed\r\n");
			#endif
			return -1;
		}
		
		TESTER_MAIN_DEBUG("Read %d bytes of data.\r\n", br);

		i += br; 
	}

	USART2_Send(G_DISP_FILE_BUFFER,i);
	Send_Bin_Len(i);	

	TESTER_MAIN_DEBUG("Total Write %d bytes of data.\r\n", i);

	#if 0		//查看数据
	for(i=0;i<binfileInfo.fsize;i=i+4)
	{
		TESTER_MAIN_DEBUG("buf[%x]=%02x %02x %02x %02x\t\r\n",i,ptr[i+0],ptr[i+1],ptr[i+2],ptr[i+3]);
	}
	#endif

	f_close(&fdst_bin);
	
	Initial_Code_Handle();
	
	return TP_OK;
		
}




/*******************************************************************************
* 文件名	     : Get_Volt_Val
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u16 Get_Volt_Val(POWER_CHANNEL_INDEX_TypeDef index)
{
	u16 temp = 0;
	switch(index)
	{
		case PWR1_IOVCC_INDEX:
			temp =  PWR1_IOVCC;
			break;
		case PWR2_VDDH_INDEX:
			temp =  PWR2_VDDH;
			break;
		case PWR3_TP_VDD_INDEX:
			temp =  PWR3_TP_VDD;
			break;
		case PWR4_TP_VIO_INDEX:
			temp =  PWR4_TP_VIO;
			break;
		case PWR5_VSP_AVDD_INDEX:
			temp =  PWR5_VSP_AVDD;
			break;
		case PWR6_VSN_AVEE_INDEX:
			temp =  PWR6_VSN_AVEE;
			break;
		case PWR7_LED1N_INDEX:
			temp =  PWR7_LED1N;
			break;
		case PWR8_LED2N_INDEX:
			temp =  PWR8_LED2N;
			break;
		case PWR9_LED3N_INDEX:
			temp =  PWR9_LED3N;
			break;
		case PWR10_VMTP_INDEX:
			temp =  PWR10_VMTP;
			break;
		case PWR11_VNEG_INDEX:
			temp =  PWR11_VNEG;
			break;
		case PWR12_LED_PWR_5V_INDEX:
			temp =  PWR12_LED_PWR_5V;
			break;
		case PWR13_EXT_PWR_A_INDEX:
			temp =  PWR13_EXT_PWR_A;
			break;
		case PWR14_EXT_PWR_B_INDEX:
			//temp =  PWR14_EXT_PWR_B;
			temp =  PWR1_IOVCC;
			break;
		case PWR15_TESTER_VBUS_INDEX:
			temp =  PWR15_TESTER_VBUS;
			break;
	}

	return(temp);
}



/*******************************************************************************
* 文件名	     : FPGA_Key_On
* 描述	         : 点屏键控制流程
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void  FPGA_Key_Ony(void)//yuan
{
	u16 temp = 0;
	u16 j;

	TESTER_MAIN_DEBUG("\r\n fpga key on\r\n");
	
	#if 0
	MutiBuff_Handle( Power_On_Set_Add, Power_On_Set_Size);		//?′DDé?μ?3ìDò	
	MutiBuff_Handle( Init_Code_Add, Init_Code_Size);		//·￠?ílcd_init′ú??
	#else
	MutiBuff_Handle( Info_Add, Info_Add_Size);	
	MutiBuff_Handle( Board_Init_Add, Board_Init_Add_Size);
	Flag_Power_On_Seq_Start = 1;	
	Send_Power_Seq(POWER_SEQ_ON);						
	MutiBuff_Handle(PowerOn_Add,PowerOn_Add_Size);			
	Flag_Power_On_Seq_Start = 0;

	MutiBuff_Handle(Code_Init_Add,Code_Init_Add_Size);		
	#endif

	//temp = FPGA_Read_Register(0x1c);
	//FPGA_Write_Register(0x1C,temp & 0x00DF);	

	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("pwr1sta=%d %d %d %d %d\t\r\n", PWR1_IOVCC_On_Status, PWR2_VDDH_On_Status, PWR3_TP_VDD_On_Status, PWR4_TP_VIO_On_Status, PWR5_VSP_AVDD_On_Status);
	TESTER_MAIN_DEBUG("pwr6sta=%d %d %d %d %d\t\r\n", PWR6_VSN_AVEE_On_Status, PWR7_LED1N_On_Status, PWR8_LED2N_On_Status, PWR9_LED3N_On_Status, PWR10_VMTP_On_Status);
	TESTER_MAIN_DEBUG("pwr11sta=%d %d %d %d %d\t\r\n", PWR11_VNEG_On_Status, PWR12_LED_PWR_5V_On_Status, PWR13_EXT_PWR_A_On_Status, PWR14_EXT_PWR_B_On_Status, PWR15_TESTER_VBUS_On_Status);
	#endif


	#if 0
	printf("Power_On_Seq_Delay = %d %d %d %d %d %d \r\n",Power_On_Seq_Delay[1],Power_On_Seq_Delay[2],Power_On_Seq_Delay[3],Power_On_Seq_Delay[4],Power_On_Seq_Delay[5],Power_On_Seq_Delay[6]);	
	printf("Power_On_Seq_Index = %d %d %d %d %d %d \r\n",Power_On_Seq_Index[1],Power_On_Seq_Index[2],Power_On_Seq_Index[3],Power_On_Seq_Index[4],Power_On_Seq_Index[5],Power_On_Seq_Index[6]);
	memset(Power_On_Seq_Delay,0,10);
	memset(Power_On_Seq_Index,0,10);
	Power_On_Seq_Index_Cnt = 0;
	#endif

	Option_Test();							//测试读出来的配置是否正确

	//FPGA_Write_Register(0x1C,0x01F);		//开启背光，然后复位	

	
	#if 1
	if((Get_Flag_Sleep_PowerOn() == 0) && (Get_Flag_Fog_PowerOn() == 0))
	{
		if(G_Select_Port == 3){
			
			for(j = 1;j <= G_No_Of_Test_Case; j++)
			{
				setCurrentI2cPort(2);
				UpdateItemUIResult(j,TP_RECOVER);
				setCurrentI2cPort(1);
				UpdateItemUIResult(j,TP_RECOVER);
			}
		}else{
			for(j = 1;j <= G_No_Of_Test_Case; j++)
			{
				UpdateItemUIResult(j,TP_RECOVER);
			}
		}
			
		LCD_Rectangle(4,216,240-4,254-216,WHITE);//Clear progress_bar
		LCD_Rectangle(80,245,240-80,320-245,LightGrey);		
		if(G_Select_Port == 3){
			LCD_64_60(80+16,235+12,6,RED);	//Show TP-WAIT
			LCD_64_60(80+80+16,235+12,6,RED);	//Show TP-WAIT
		}else{
			LCD_128_60(80+16,235+12,6,RED);	//Show TP-WAIT
		}
	}
	
	#endif

	Set_Sleep_Status(0);			//取消休眠
	Set_Power_Status(1);			//0---关机状态，1---开机状态
}

u8 mipibufer[16][16]={
	{0x29,5,0xDE,0x00,0x3F,0xFF,0x90},
	{0x29,2,0xB0,0x00},	
	{0x29,2,0xD6,0x01},
	{0x29,2,0xB3,0x3D},
	{0x29,2,0xB0,0x00},
	{0xff,20},
	{0x05,1,0x29},
	{0xff,120},
	{0x05,1,0x11},
	{0xff,20},
	{0xff,20},
	{0xff,20},
};

u8 bb[16];

void  FPGA_Key_On(void)//
{
	u16 temp = 0;
	u16 j;
	uint16_t mode;

	
	TESTER_MAIN_DEBUG("\r\n $$$$$$$$$$$$$$$$$fpga key on$$$$$$$$$$$$$$$$$\r\n\r\n");
	
	Flag_Power_On_Seq_Start = 1;	
	
	Power_On_Handle(PWR_IOVCC,ON);
	Delay_ms(5);
	Power_On_Handle(PWR_VSP,ON);
	Delay_ms(5);
	Power_On_Handle(PWR_VSN,ON);
	Delay_ms(20);

	FPGA_Write_Register(0x1c,FPGA_Read_Register(0x1c)&0xfffb);	// lcm  reset = 0 
	Delay_ms(20);
	FPGA_Write_Register(0x1c,FPGA_Read_Register(0x1c)|0x04);	//lcm  reset = 1
	
	Flag_Power_On_Seq_Start = 0;
	
	mode = FPGA_Read_Register(0x10) >> 15;
	TESTER_MAIN_DEBUG("\r\n  MODE1 =%d \r\n\r\n",mode);

	//MutiBuff_Handle(Code_Init_Add,Code_Init_Add_Size);		

	mode = FPGA_Read_Register(0x10) >> 15;
	TESTER_MAIN_DEBUG("\r\n  MODE2 =%d \r\n\r\n",mode);
	
	for(j=0;j<10;j++)
	{
		if(mipibufer[j][0] == 0xFF)
		{
			Delay_ms(mipibufer[j][1]);
			TESTER_MAIN_DEBUG("\r\n  Delay_ms(%d);\r\n",mipibufer[j][1]);
		}
		else
		{
			mipiWrite(1,0,mipibufer[j][0],mipibufer[j][1],&mipibufer[j][2]);
			TESTER_MAIN_DEBUG("\r\n  mipiWrite(1,0,0x%x,0x%x,0x%x);\r\n",mipibufer[j][0],mipibufer[j][1],mipibufer[j][2]);
			Delay_us(500);
		}
	}

/*
	bb[0]=0xB0;bb[1]=0x00;
	mipiWrite(1,0,0x29,2,bb);

	bb[0]=0xd6;bb[1]=0x01;
	mipiWrite(1,0,0x29,2,bb);

	bb[0]=0xb3;bb[1]=0x3d;
	mipiWrite(1,0,0x29,2,bb);

	bb[0]=0x29;bb[1]=0x00;
	mipiWrite(1,0,0x05,1,bb);
	
	Delay_ms(150);
	
	bb[0]=0x11;bb[1]=0x00;
	mipiWrite(1,0,0x05,1,bb);
	Delay_ms(150);
*/
	/*
	WriteSSD2828Reg(0xBC,0x0002);
	WriteSSD2828Reg(0xB6,0x004B);
	WriteSSD2828Reg(0xB7,0x0700);
	WriteSSD2828Reg(0xBF,0x00B0);
	
	WriteSSD2828Reg(0xBC,0x0002);
	WriteSSD2828Reg(0xB6,0x004B);
	WriteSSD2828Reg(0xB7,0x0700);
	WriteSSD2828Reg(0xBF,0x01D6);

	WriteSSD2828Reg(0xBC,0x0002);
	WriteSSD2828Reg(0xB6,0x004B);
	WriteSSD2828Reg(0xB7,0x0700);
	WriteSSD2828Reg(0xBF,0x3db3);

	WriteSSD2828Reg(0xBC,0x0001);
	WriteSSD2828Reg(0xB6,0x004B);
	WriteSSD2828Reg(0xB7,0x0740);
	WriteSSD2828Reg(0xBF,0x0029);

	Delay_ms(150);

	WriteSSD2828Reg(0xBC,0x0001);
	WriteSSD2828Reg(0xB6,0x004B);
	WriteSSD2828Reg(0xB7,0x0740);
	WriteSSD2828Reg(0xBF,0x0011);
*/
	
	Delay_ms(200);
	Power_On_Handle(PWR_BL1,ON);
	Power_On_Handle(PWR_BL2,ON);


	Fpga_Disp_Manage.Fpga_Disp_Index = 0x00;
	PowerOn_Pic_Index = Fpga_Disp_Manage.Fpga_Disp_Index;			//记录上电运行的图片

	WriteSSD2828Reg(0xB7,0x0309);
	clearRGBShowPic(3,Fpga_Disp_Manage.Fpga_Disp_Index,1);


	Set_Sleep_Status(0);			//取消休眠
	Set_Power_Status(1);			//0---关机状态，1---开机状态
}




/*******************************************************************************
* 文件名	     : FPGA_Key_On
* 描述	         : 点屏键控制流程
* 输入           : 无
* 输出           : 
* 返回           : 无
*******************************************************************************/
void  FPGA_Key_On1(void)
{
	u16 temp = 0;
	u16 j;

	TESTER_MAIN_DEBUG("\r\nFPGA_Key_On\r\n");

	Flag_Power_On_Seq_Start = 1;		
	
	//MutiBuff_Handle(PowerOn_Add,PowerOn_Add_Size);	
	
	Power_On_Handle(PWR_IOVCC,ON);
	Delay_ms(5);
	Power_On_Handle(PWR_VSP,ON);
	Delay_ms(5);
	Power_On_Handle(PWR_VSN,ON);
	Delay_ms(20);

	FPGA_Write_Register(0x1c,FPGA_Read_Register(0x1c)&0xfffb);	// lcm  reset = 0 
	Delay_ms(20);
	FPGA_Write_Register(0x1c,FPGA_Read_Register(0x1c)|0x04);	//lcm  reset = 1
	
	for(j=0;j<9;j++)
	{
		if(mipibufer[j][0] == 0xFF)
		{
			Delay_ms(mipibufer[j][1]);
		}
		else
		{
			mipiWrite(1,0,mipibufer[j][0],mipibufer[j][1],&mipibufer[j][2]);
			Delay_us(200);
		}
	}

	//MutiBuff_Handle(Code_Init_Add,Code_Init_Add_Size);	

	Delay_ms(100);
	Power_On_Handle(PWR_BL1,ON);
	Power_On_Handle(PWR_BL2,ON);

	Fpga_Disp_Manage.Fpga_Disp_Index = 0x00;
	PowerOn_Pic_Index = Fpga_Disp_Manage.Fpga_Disp_Index;			//记录上电运行的图片
					
	clearRGBShowPic(3,Fpga_Disp_Manage.Fpga_Disp_Index,1);

	Set_Sleep_Status(0);			//取消休眠
	Set_Power_Status(1);			//0---关机状态，1---开机状态

}

void  Special_Init2_On(void)
{
	Send_Power_Seq(POWER_SEQ_CUSTOM);

	MutiBuff_Handle(PowerOn_Add,PowerOn_Add_Size);		//?′DDé?μ?3ìDò	
	MutiBuff_Handle(Code_Init2_Add,Code_Init2_Add_Size);		//·￠?ílcd_init′ú??

	Set_Sleep_Status(0);			//è???DY??
	Set_Power_Status(1);			//0---1??ú×′ì?￡?1---?a?ú×′ì?
}


void FPGA_Key_Long_On(void) 
{
	Send_Power_Seq(POWER_SEQ_SLEEP);
	//Delay_ms(200);
	MutiBuff_Handle(Sleep_Seq_Add,Sleep_Seq_Add_Size);
	Set_Sleep_Status(1);   		//??è?DY???￡ê?
	Set_Power_Status(0);		//éè???a1??ú×′ì?
}



/*******************************************************************************
* 文件名	     : FPGA_Key_Off
* 描述	         : 灭亮键控制流程
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void  FPGA_Key_Off(void)
{	
	uint16_t fpga_r11;
	
	
	
	TIM_Cmd(TIM2, DISABLE);
	
	fpga_r11 = FPGA_Read_Register(0x11);
	FPGA_Write_Register(0x11,(fpga_r11 & 0xFFFC) | 3);	//Enable both Port
	
	
	Send_Power_Seq(POWER_SEQ_OFF);
	//Set_PowerSeq_Index(0);
	//MutiBuff_Handle( Power_Off_Set_Add, Power_Off_Set_Size);	
	TESTER_MAIN_DEBUG("\r\npower off ing,add = %x size = %x\r\n",PowerOff_Add,PowerOff_Add_Size);	
	MutiBuff_Handle(PowerOff_Add,PowerOff_Add_Size);	
	

	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("pwr1sta=%d %d %d %d %d\t\r\n", PWR1_IOVCC_On_Status, PWR2_VDDH_On_Status, PWR3_TP_VDD_On_Status, PWR4_TP_VIO_On_Status, PWR5_VSP_AVDD_On_Status);
	TESTER_MAIN_DEBUG("pwr6sta=%d %d %d %d %d\t\r\n", PWR6_VSN_AVEE_On_Status, PWR7_LED1N_On_Status, PWR8_LED2N_On_Status, PWR9_LED3N_On_Status, PWR10_VMTP_On_Status);
	TESTER_MAIN_DEBUG("pwr11sta=%d %d %d %d %d\t\r\n", PWR11_VNEG_On_Status, PWR12_LED_PWR_5V_On_Status, PWR13_EXT_PWR_A_On_Status, PWR14_EXT_PWR_B_On_Status, PWR15_TESTER_VBUS_On_Status);
	#endif
	
	//FPGA_Write_Register(0x1C,0x03F);
	fpga_r11 = FPGA_Read_Register(0x11);
	FPGA_Write_Register(0x11,(fpga_r11 & 0xFFFC) | 3);	//Enable both Port
	WriteSSD2828Reg(0xC0,0x0101);	//Clear All 2828 transfer Errors
	Delay_ms(50);

	//Set_Sleep_Status(0);			//取消休眠
	Set_Power_Status(0);			//0---关机状态，1---开机状态
}


/*******************************************************************************
* 文件名	     : FPGA_Sleep_Off
* 描述	         : 睡眠时候的数据发送
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void  FPGA_Sleep_Off(void)
{
	MutiBuff_Handle(PowerOn_Add,PowerOn_Add_Size);	
	//MutiBuff_Handle(PowerOff_Add,PowerOff_Add_Size);	
}



/*******************************************************************************
* ???t??	     : FPGA_Sleep_Off
* ?èê?	         : ?ˉ??ê±oòμ?êy?Y·￠?í
* ê?è?           : ?T
* ê?3?           : ?T
* ·μ??           : ?T
*******************************************************************************/
void  SCAN_Cmd(u8 cmd_index)
{
	switch(cmd_index)
	{
		case 1: 
		MutiBuff_Handle(Code_Cmd1_Add,Code_Cmd1_Add_Size);
		break;

		case 2: 
		MutiBuff_Handle(Code_Cmd2_Add,Code_Cmd2_Add_Size);
		break;

		case 3: 
		MutiBuff_Handle(Code_Cmd3_Add,Code_Cmd3_Add_Size);
		break;

		case 4: 
		MutiBuff_Handle(Code_Cmd4_Add,Code_Cmd4_Add_Size);
		break;
	}
}


/*******************************************************************************
* 文件名	     : System_Power_On_All
* 描述	         : 电源全部打开
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void System_Power_On_All(void)
{
	u16 temp;
	
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
	TPS22993_Sel_Dev(0);	
	TPS22993_On(TPS22993_CHN2);
	Iovcc_Power_Status = 1;

	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON); 
	TPS22993_Sel_Dev(1);	
	TPS22993_On(TPS22993_CHN3);
	Lcdvdd_Power_Status = 1;

	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON); 	
	TPS22993_Sel_Dev(1);	
	TPS22993_On(TPS22993_CHN1);
	Tpvdd_Power_Status = 1;


	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);
	TPS22993_Sel_Dev(1);	
	TPS22993_On(TPS22993_CHN2);
	Tpvio_Power_Status = 1;
	

	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp | 0x02);							
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 
	TPS22993_Sel_Dev(0);	
	TPS22993_On(TPS22993_CHN1);
	Vsp_Power_Status = 1;


	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp | 0x10);	 
	Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE);		
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON); 
	TPS22993_Sel_Dev(0);	
	TPS22993_On(TPS22993_CHN1);
	Vsn_Power_Status = 1;	

	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp & 0x00DF);
	LM36923_AllOn();

	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON); 
	TPS22993_Sel_Dev(0);	
	TPS22993_On(TPS22993_CHN4);


	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON); 
	
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON); 						
}




/*******************************************************************************
* 文件名	     : System_Power_On_All
* 描述	         : 电源全部关闭
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void System_Power_Off_All(void)
{
	u16 temp;
	
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_OFF);	
	TPS22993_Sel_Dev(0);	
	TPS22993_Off(TPS22993_CHN2);
	Iovcc_Power_Status = 0;

	
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_OFF); 
	TPS22993_Sel_Dev(1);	
	TPS22993_Off(TPS22993_CHN3);
	Lcdvdd_Power_Status = 0;

	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_OFF); 
	TPS22993_Sel_Dev(1);	
	TPS22993_Off(TPS22993_CHN1);
	Tpvdd_Power_Status = 0;
					

	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF); 
	TPS22993_Sel_Dev(1);	
	TPS22993_Off(TPS22993_CHN2);
	Tpvio_Power_Status = 0;
						

	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp & 0xFD);						
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_OFF); 
	TPS22993_Sel_Dev(0);	
	TPS22993_Off(TPS22993_CHN1);
	Vsp_Power_Status = 0;
					

	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp & 0xEF);					
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_OFF); 
	TPS22993_Sel_Dev(0);	
	TPS22993_Off(TPS22993_CHN1);
	Vsn_Power_Status = 0;	


	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp | 0x20);	
	LM36923_AllOff();


	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_OFF); 
	TPS22993_Sel_Dev(0);	
	TPS22993_Off(TPS22993_CHN4);

	LT3582_Off();


	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF); 
	
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_OFF); 	

	 PWR1_IOVCC_On_Status = POWER_OFF;
	 PWR2_VDDH_On_Status = POWER_OFF;
	 PWR3_TP_VDD_On_Status = POWER_OFF;
	 PWR4_TP_VIO_On_Status = POWER_OFF;
	 PWR5_VSP_AVDD_On_Status = POWER_OFF;
	 PWR6_VSN_AVEE_On_Status = POWER_OFF;
	 PWR7_LED1N_On_Status = POWER_OFF;
	 PWR8_LED2N_On_Status = POWER_OFF;
	 PWR9_LED3N_On_Status = POWER_OFF;
	 PWR10_VMTP_On_Status = POWER_OFF;
	 PWR11_VNEG_On_Status = POWER_OFF;
	 PWR12_LED_PWR_5V_On_Status = POWER_OFF;
	 PWR13_EXT_PWR_A_On_Status = POWER_OFF;
	 PWR14_EXT_PWR_B_On_Status = POWER_OFF;
	 PWR15_TESTER_VBUS_On_Status = POWER_OFF;
}



void Mipi_Led_Fresh(void)
{
	static u8 mipi_led_old = 0;

	if(mipi_led_old !=  PWR7_LED1N_On_Status)
	{
		if( PWR7_LED1N_On_Status)
		{
			MIPI_LED_ON();			
		}
		else
		{			
			MIPI_LED_OFF();
		}
	}


	if(Get_Sleep_Status())			//睡眠模式下，灯开
	{
		MIPI_LED_ON();
	}

	mipi_led_old =  PWR7_LED1N_On_Status;
}

/*******************************************************************************
* 文件名	     : Big_Current_Dect
* 描述	         : 大电流检测
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Big_Current_Dect(void)
{
	u8 err = 0;

	// PWR1_IOVCC_A_HIGH = 1001;
	
	if(Lcd_Iovcc_A_High > PWR1_IOVCC_A_DECT_HIGH)
	{
		err = 1;
	}

	if(Lcd_Vddh_A_High > PWR2_VDDH_A_DECT_HIGH)
	{
		err = 1;		
	}

	if(Tp_Vio_A_High > PWR3_TP_VDD_A_DECT_HIGH)
	{
		err = 1;	
	}

	if(Tp_Vdd_A_High > PWR4_TP_VIO_A_DECT_HIGH)
	{
		err = 1;	
	}

	if(Vsp_A_High> PWR5_VSP_AVDD_A_DECT_HIGH)
	{
		err = 1;	
	}

	if(Vsn_A_High > PWR6_VSN_AVEE_A_DECT_HIGH)
	{
		err = 1;	
	}


	if(err)		//大电流被检测到了
	{
		Flag_Big_Current_Err = 1;
	}
	else
	{
		Flag_Big_Current_Err = 0;
	}
}

/*******************************************************************************
* 文件名	     : Get_Max_Index
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 Get_Max_Index(void)
{
	//TESTER_MAIN_DEBUG("Pic_Index_Max = %d\r\n",Pic_Num_Max);
	return(Pic_Num_Max);
}


/*******************************************************************************
* 文件名	     : Get_PowerSeq_Index
* 描述	         : 获取上电时序状态
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 Get_PowerSeq_Index(void)
{
	return(PowerOnOffSeq);
}




/*******************************************************************************
* 文件名	     : Set_PowerSeq_Index
* 描述	         : 设置上电时序状态
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void Set_PowerSeq_Index(u8 val)
{
	PowerOnOffSeq = val;
}








u8 Get_Sleep_Status(void)
{
	return(Sleep_Status);
}


void Set_Sleep_Status(u8 val)
{
	Sleep_Status = val;
}




u8 Get_Detect_Power_Status(DETECT_POWER_TypeDef detect)//检测电源是否开启
{
	switch(detect)
	{
		case DETECT_POWER_IOVCC:
			return(Iovcc_Power_Status);
			break;
		case DETECT_POWER_LCDVDD:
			return(Lcdvdd_Power_Status);
			break;
		case DETECT_POWER_TPVIO:
			return(Tpvio_Power_Status);
			break;
		case DETECT_POWER_TPVDD:
			return(Tpvdd_Power_Status);
			break;
		case DETECT_POWER_VSP:
			return(Vsp_Power_Status);
			break;
		case DETECT_POWER_VSN:
			return(Vsn_Power_Status);
			break;
		case DETECT_POWER_LEDN1:
			return(PWR7_LED1N_On_Status);
			break;
		case DETECT_POWER_LEDN2:
			return(PWR8_LED2N_On_Status);
			break;
		case DETECT_POWER_LEDN3:
			return(PWR9_LED3N_On_Status);
			break;
	}
	
}



u8 Get_Big_Current_Err(void)
{
	return(Flag_Big_Current_Err);
}



void Get_Option_Para(OPTION_INDEX_TypeDef option,u16 *val)
{
	switch(option)
	{
		case CURRENT_EN_INDEX:
			*val = Flag_Current_En;
			break;
		case VOLTAGE_EN_INDEX:
			*val = Flag_Voltage_En;
			break;
		case SLEEP_CURRENT_EN_INDEX:
			*val = Flag_Sleep_Current_En;
			break;
		case HARD_ID_EN_INDEX:
			*val = Flag_Hard_ID_En;
			break;
		case HARD_ID_VAL_INDEX:
			*val = Hard_ID_En_Val;
			break;
		case SOFT_ID_EN_INDEX:
			*val = Flag_Soft_ID_En;
			break;
		case SOFT_ID12_VAL_INDEX:
			*val = Soft_ID12_Val;
			break;
		case SOFT_ID34_VAL_INDEX:
			*val = Soft_ID34_Val;
			break;
		case OTP_EN_INDEX:
			*val = Flag_OTP_Times_En;
			break;
		case OTP_HIGH_INDEX:
			*val = OTP_Times_High;
			break;
		case OTP_LOW_INDEX:
			*val = OTP_Times_Low;
			break;
		case DUIWEI_EN_INDEX:
			*val = Flag_DuiWei_En;
			break;
		case PWM_EN_INDEX:
			*val = Flag_PWM_En;
			break;
		case PWM_SEQ_HIGH_INDEX:
			*val = PWM_Seq_High;
			break;
		case PWM_SEQ_LOW_INDEX:
			*val = PWM_Seq_Low;
			break;
		case PWM_DUTY_HIGH_INDEX:
			*val = PWM_Duty_High;
			break;
		case PWM_DUTY_LOW_INDEX:
			*val = PWM_Duty_Low;
			break;
		case PWM_VOLT_HIGH_INDEX:
			*val = PWM_Volt_High;
			break;
		case PWM_VOLT_LOW_INDEX:
			*val = PWM_Volt_Low;
			break;
		case TE_EN_INDEX:
			*val = Flag_TE_En;
			break;
		case TE_SEQ_HIGH_INDEX:
			*val = TE_Seq_High;
			break;
		case TE_SEQ_LOW_INDEX:
			*val = TE_Seq_Low;
			break;
		case TE_DUTY_HIGH_INDEX:
			*val = TE_Duty_High;
			break;
		case TE_DUTY_LOW_INDEX:
			*val = TE_Duty_Low;
			break;
		case TE_VOLT_HIGH_INDEX:
			*val = TE_Volt_High;
			break;
		case TE_VOLT_LOW_INDEX:
			*val = TE_Volt_Low;
			break;
		case BK_EN_INDEX:
			*val = FLAG_BK_En;
			break;
		case KEY_ONOFF_SHORT_EN:
			*val = Flag_Key_OnOff_Short;
			break;
		case KEY_ONOFF_LONG_EN:
			*val = Flag_Key_OnOff_Long;
			break;
		case KEY_ONOFF_SHORT_PRESS:
			*val = Key_OnOff_NormalPress;
			break;
		case KEY_ONOFF_LONG_PRESS:
			*val = Key_OnOff_LongPress;
			break;
		case KEY_LEFT_SHORT_EN:
			*val = Flag_Key_Left_Short;
			break;
		case KEY_LEFT_LONG_EN:
			*val = Flag_Key_Left_Long;
			break;
		case KEY_LEFT_SHORT_PRESS:
			*val = Key_Left_NormalPress;
			break;
		case KEY_LEFT_LONG_PRESS:
			*val = Key_Left_LongPress;
			break;
		case KEY_AUTO_SHORT_EN:
			*val = Flag_Key_Auto_Short;
			break;
		case KEY_AUTO_LONG_EN:
			*val = Flag_Key_Auto_Long;
			break;
		case KEY_AUTO_SHORT_PRESS:
			*val = Key_Auto_NormalPress;
			break;
		case KEY_AUTO_LONG_PRESS:
			*val = Key_Auto_LongPress;
			break;
		case KEY_RIGHT_SHORT_EN:
			*val = Flag_Key_Right_Short;
			break;
		case KEY_RIGHT_LONG_EN:
			*val = Flag_Key_Right_Long;
			break;
		case KEY_RIGHT_SHORT_PRESS:
			*val = Key_Right_NormalPress;
			break;
		case KEY_RIGHT_LONG_PRESS:
			*val = Key_Right_LongPress;
			break;	
		case KEY_TEST_SHORT_EN:
			*val = Flag_Key_Test_Short;
			break;
		case KEY_TEST_LONG_EN:
			*val = Flag_Key_Test_Long;
			break;
		case KEY_TEST_SHORT_PRESS:
			*val = Key_Test_NormalPress;
			break;
		case KEY_TEST_LONG_PRESS:
			*val = Key_Test_LongPress;
			break;
		case KEY_LOCK_PIC_TIME:
			*val = Key_Lock_Time;
			break;
		case PWR1_IOVCC_V_HIGH_INDEX:
			*val = PWR1_IOVCC_HIGH;
			break;
		case PWR1_IOVCC_V_LOW_INDEX:
			*val = PWR1_IOVCC_LOW;
			break;
		case PWR1_IOVCC_A_HIGH_INDEX:
			*val = PWR1_IOVCC_A_HIGH;
			break;
		case PWR1_IOVCC_A_LOW_INDEX:
			*val = PWR1_IOVCC_A_LOW;
			break;
		case PWR1_IOVCC_UA_HIGH_INDEX:
			*val = PWR1_IOVCC_UA_HIGH;
			break;
		case PWR1_IOVCC_UA_LOW_INDEX:
			*val = PWR1_IOVCC_UA_LOW;
			break;
		case PWR2_VDDH_V_HIGH_INDEX:
			*val = PWR2_VDDH_HIGH;
			break;
		case PWR2_VDDH_V_LOW_INDEX:
			*val = PWR2_VDDH_LOW;
			break;
		case PWR2_VDDH_A_HIGH_INDEX:
			*val = PWR2_VDDH_A_HIGH;
			break;
		case PWR2_VDDH_A_LOW_INDEX:
			*val = PWR2_VDDH_A_LOW;
			break;
		case PWR2_VDDH_UA_HIGH_INDEX:
			*val = PWR2_VDDH_UA_HIGH;
			break;
		case PWR2_VDDH_UA_LOW_INDEX:
			*val = PWR2_VDDH_UA_LOW;
			break;
		case PWR3_TP_VDD_V_HIGH_INDEX:
			*val = PWR3_TP_VDD_HIGH;
			break;
		case PWR3_TP_VDD_V_LOW_INDEX:
			*val = PWR3_TP_VDD_LOW;
			break;
		case PWR3_TP_VDD_A_HIGH_INDEX:
			*val = PWR3_TP_VDD_A_HIGH;
			break;
		case PWR3_TP_VDD_A_LOW_INDEX:
			*val = PWR3_TP_VDD_A_LOW;
			break;
		case PWR3_TP_VDD_UA_HIGH_INDEX:
			*val = PWR3_TP_VDD_UA_HIGH;
			break;
		case PWR3_TP_VDD_UA_LOW_INDEX:
			*val = PWR3_TP_VDD_UA_LOW;
			break;
		case PWR4_TP_VIO_V_HIGH_INDEX:
			*val = PWR4_TP_VIO_HIGH;
			break;
		case PWR4_TP_VIO_V_LOW_INDEX:
			*val = PWR4_TP_VIO_LOW;
			break;
		case PWR4_TP_VIO_A_HIGH_INDEX:
			*val = PWR4_TP_VIO_A_HIGH;
			break;
		case PWR4_TP_VIO_A_LOW_INDEX:
			*val = PWR4_TP_VIO_A_LOW;
			break;
		case PWR4_TP_VIO_UA_HIGH_INDEX:
			*val = PWR4_TP_VIO_UA_HIGH;
			break;
		case PWR4_TP_VIO_UA_LOW_INDEX:
			*val = PWR4_TP_VIO_UA_LOW;
			break;
		case PWR5_VSP_AVDD_V_HIGH_INDEX:
			*val = PWR5_VSP_AVDD_HIGH;
			break;
		case PWR5_VSP_AVDD_V_LOW_INDEX:
			*val = PWR5_VSP_AVDD_LOW;
			break;
		case PWR5_VSP_AVDD_A_HIGH_INDEX:
			*val = PWR5_VSP_AVDD_A_HIGH;
			break;
		case PWR5_VSP_AVDD_A_LOW_INDEX:
			*val = PWR5_VSP_AVDD_A_LOW;
			break;
		case PWR5_VSP_AVDD_UA_HIGH_INDEX:
			*val = PWR5_VSP_AVDD_UA_HIGH;
			break;
		case PWR5_VSP_AVDD_UA_LOW_INDEX:
			*val = PWR5_VSP_AVDD_UA_LOW;
			break;
		case PWR6_VSN_AVEE_V_HIGH_INDEX:
			*val = PWR6_VSN_AVEE_HIGH;
			break;
		case PWR6_VSN_AVEE_V_LOW_INDEX:
			*val = PWR6_VSN_AVEE_LOW;
			break;
		case PWR6_VSN_AVEE_A_HIGH_INDEX:
			*val = PWR6_VSN_AVEE_A_HIGH;
			break;
		case PWR6_VSN_AVEE_A_LOW_INDEX:
			*val = PWR6_VSN_AVEE_A_LOW;
			break;
		case PWR6_VSN_AVEE_UA_HIGH_INDEX:
			*val = PWR6_VSN_AVEE_UA_HIGH;
			break;
		case PWR6_VSN_AVEE_UA_LOW_INDEX:
			*val = PWR6_VSN_AVEE_UA_LOW;
			break;
		case PWR7_LED1N_V_HIGH_INDEX:
			*val = PWR7_LED1N_HIGH;
			break;
		case PWR7_LED1N_V_LOW_INDEX:
			*val = PWR7_LED1N_LOW;
			break;
		case PWR7_LED1N_A_HIGH_INDEX:
			*val = PWR7_LED1N_A_HIGH;
			break;
		case PWR7_LED1N_A_LOW_INDEX:
			*val = PWR7_LED1N_A_LOW;
			break;
		case PWR7_LED1N_UA_HIGH_INDEX:
			*val = PWR7_LED1N_UA_HIGH;
			break;
		case PWR7_LED1N_UA_LOW_INDEX:
			*val = PWR7_LED1N_UA_LOW;
			break;
		case PWR8_LED2N_V_HIGH_INDEX:
			*val = PWR8_LED2N_HIGH;
			break;
		case PWR8_LED2N_V_LOW_INDEX:
			*val = PWR8_LED2N_LOW;
			break;
		case PWR8_LED2N_A_HIGH_INDEX:
			*val = PWR8_LED2N_A_HIGH;
			break;
		case PWR8_LED2N_A_LOW_INDEX:
			*val = PWR8_LED2N_A_LOW;
			break;
		case PWR8_LED2N_UA_HIGH_INDEX:
			*val = PWR8_LED2N_UA_HIGH;
			break;
		case PWR8_LED2N_UA_LOW_INDEX:
			*val = PWR8_LED2N_UA_LOW;
			break;
		case PWR9_LED3N_V_HIGH_INDEX:
			*val = PWR9_LED3N_HIGH;
			break;
		case PWR9_LED3N_V_LOW_INDEX:
			*val = PWR9_LED3N_LOW;
			break;
		case PWR9_LED3N_A_HIGH_INDEX:
			*val = PWR9_LED3N_A_HIGH;
			break;
		case PWR9_LED3N_A_LOW_INDEX:
			*val = PWR9_LED3N_A_LOW;
			break;
		case PWR9_LED3N_UA_HIGH_INDEX:
			*val = PWR9_LED3N_UA_HIGH;
			break;
		case PWR9_LED3N_UA_LOW_INDEX:
			*val = PWR9_LED3N_UA_LOW;
			break;
	}
}



void Get_Bin_Version(char *s)
{
	u8 temp[2] = 0;
	
#if 0		//是否和版本号一起显示
	temp[0] = ParaManage.Version + 0x30;
	strcat(ParaManage.Bin_Name,temp);
#endif
	s = ParaManage.Bin_Name;
}

void Get_Bin_VersionY(char **s)
{
	u8 temp[2] = 0;
	
	//printf("ver = %d\r\n",ParaManage.Version);
#if 0		//是否和版本号一起显示
	temp[0] = ParaManage.Version + 0x30;
	strcat(ParaManage.Bin_Name,temp);
#endif
	*s = ParaManage.Bin_Name;
}


void Option_Test(void)
{
	u16 temp = 0;
	//char temp_arr[50];
	char * temp_arr;
	
#if 0	
	Get_Bin_Version(&temp_arr);
	printf("Bin version = %s\r\n",temp_arr);
	Get_Option_Para(CURRENT_EN_INDEX,&temp);
	printf("current en = %d\r\n",Flag_Current_En);
	Get_Option_Para(VOLTAGE_EN_INDEX,&temp);
	printf("voltage en = %d\r\n",temp);
	Get_Option_Para(SLEEP_CURRENT_EN_INDEX,&temp);
	printf("sleep en = %d\r\n",temp);
	Get_Option_Para(HARD_ID_EN_INDEX,&temp);
	printf("hard id en = %d\r\n",temp);
	Get_Option_Para(HARD_ID_VAL_INDEX,&temp);
	printf("hard id val = %d\r\n",temp);
	Get_Option_Para(SOFT_ID_EN_INDEX,&temp);
	printf("soft id en = %d\r\n",temp);
	Get_Option_Para(SOFT_ID12_VAL_INDEX,&temp);
	printf("soft id12 val = %d\r\n",temp);
	Get_Option_Para(SOFT_ID34_VAL_INDEX,&temp);
	printf("soft id34 val = %d\r\n",temp);
	Get_Option_Para(OTP_EN_INDEX,&temp);
	printf("opt en = %d\r\n",temp);
	Get_Option_Para(OTP_HIGH_INDEX,&temp);
	printf("opt high = %d\r\n",temp);
	Get_Option_Para(OTP_LOW_INDEX,&temp);
	printf("opt low = %d\r\n",temp);
	Get_Option_Para(DUIWEI_EN_INDEX,&temp);
	printf("duiwei en = %d\r\n",temp);
	Get_Option_Para(PWM_EN_INDEX,&temp);
	printf("pwm en = %d\r\n",temp);
	Get_Option_Para(PWM_SEQ_HIGH_INDEX,&temp);
	printf("pwm seq high = %d\r\n",temp);
	Get_Option_Para(PWM_SEQ_LOW_INDEX,&temp);
	printf("pwm seq low = %d\r\n",temp);
	Get_Option_Para(PWM_DUTY_HIGH_INDEX,&temp);
	printf("pwm duty high = %d\r\n",temp);
	Get_Option_Para(PWM_DUTY_LOW_INDEX,&temp);
	printf("pwm duty low = %d\r\n",temp);
	Get_Option_Para(PWM_VOLT_HIGH_INDEX,&temp);
	printf("pwm volt high = %d\r\n",temp);
	Get_Option_Para(PWM_VOLT_LOW_INDEX,&temp);
	printf("pwm volt low = %d\r\n",temp);
	Get_Option_Para(TE_EN_INDEX,&temp);
	printf("te en = %d\r\n",temp);
	Get_Option_Para(TE_SEQ_HIGH_INDEX,&temp);
	printf("te seq high = %d\r\n",temp);
	Get_Option_Para(TE_SEQ_LOW_INDEX,&temp);
	printf("te seq low = %d\r\n",temp);
	Get_Option_Para(TE_DUTY_HIGH_INDEX,&temp);
	printf("te duty high = %d\r\n",temp);
	Get_Option_Para(TE_DUTY_LOW_INDEX,&temp);
	printf("te duty low = %d\r\n",temp);
	Get_Option_Para(TE_VOLT_HIGH_INDEX,&temp);
	printf("te volt high = %d\r\n",temp);
	Get_Option_Para(TE_VOLT_LOW_INDEX,&temp);
	printf("te volt low = %d\r\n",temp);
	
	Get_Option_Para(KEY_ONOFF_SHORT_EN,&temp);
	printf("key onoff short = %d\r\n",temp);
	Get_Option_Para(KEY_ONOFF_LONG_EN,&temp);
	printf("key onoff long = %d\r\n",temp);
	Get_Option_Para(KEY_ONOFF_SHORT_PRESS,&temp);
	printf("key onoff short press = %d\r\n",temp);
	Get_Option_Para(KEY_ONOFF_LONG_PRESS,&temp);
	printf("key onoff long press= %d\r\n",temp);

	Get_Option_Para(KEY_LEFT_SHORT_EN,&temp);
	printf("key left short = %d\r\n",temp);
	Get_Option_Para(KEY_LEFT_LONG_EN,&temp);
	printf("key left long = %d\r\n",temp);
	Get_Option_Para(KEY_LEFT_SHORT_PRESS,&temp);
	printf("key left short press = %d\r\n",temp);
	Get_Option_Para(KEY_LEFT_LONG_PRESS,&temp);
	printf("key left long press= %d\r\n",temp);

	
	Get_Option_Para(KEY_AUTO_SHORT_EN,&temp);
	printf("key auto short = %d\r\n",temp);
	Get_Option_Para(KEY_AUTO_LONG_EN,&temp);
	printf("key auto long = %d\r\n",temp);
	Get_Option_Para(KEY_AUTO_SHORT_PRESS,&temp);
	printf("key auto short press = %d\r\n",temp);
	Get_Option_Para(KEY_AUTO_LONG_PRESS,&temp);
	printf("key auto long press= %d\r\n",temp);

	Get_Option_Para(KEY_RIGHT_SHORT_EN,&temp);
	printf("key right short = %d\r\n",temp);
	Get_Option_Para(KEY_RIGHT_LONG_EN,&temp);
	printf("key right long = %d\r\n",temp);
	Get_Option_Para(KEY_RIGHT_SHORT_PRESS,&temp);
	printf("key right short press = %d\r\n",temp);
	Get_Option_Para(KEY_RIGHT_LONG_PRESS,&temp);
	printf("key right long press= %d\r\n",temp);

	Get_Option_Para(KEY_TEST_SHORT_EN,&temp);
	printf("key test short = %d\r\n",temp);
	Get_Option_Para(KEY_TEST_LONG_EN,&temp);
	printf("key test long = %d\r\n",temp);
	Get_Option_Para(KEY_TEST_SHORT_PRESS,&temp);
	printf("key test short press = %d\r\n",temp);
	Get_Option_Para(KEY_TEST_LONG_PRESS,&temp);
	printf("key test long press= %d\r\n",temp);

	Get_Option_Para(KEY_LOCK_PIC_TIME,&temp);
	printf("key lock time= %d\r\n",temp);
	
	
#endif

}


u8 Get_Cfg_Done_Flag(void)
{
	return(Flag_Load_CFG_Done);
}



u16 Get_Pic_Lock_Time(u8 pic_index,u16 *data)
{
	u8 i = 0;
	//return(Pic_Lock_Time[pic_index]);
	*data = Pic_Lock_Time[pic_index];

}



u16 Get_Pic_Scan_Cmd(u8 pic_index,u16 *data)
{
	u8 i = 0;
	//return(Pic_Lock_Time[pic_index]);
	if(pic_index >= Get_Max_Index())
	{
		*data = 0;	
	}
	else
	{
		*data = Pic_Cmd_Index[pic_index];
	}

	//for(i = 0;i<9;i++)
	//printf("Pic_Cmd_Index[%d] = %d\r\n",i,Pic_Cmd_Index[i]);
	//printf("\r\n");
}




u8 Get_PowerOn_Pic_Index(void)
{
	return(PowerOn_Pic_Index);
}



u8 Get_Auto_Sleep_Index(void)
{
	return(Auto_Sleep_Index);
}




void Clear_Auto_Sleep_Index(void)
{
	Auto_Sleep_Index = 0xff;
}


u8 Get_Auto_Sleep_Mode(void)
{
	return(Auto_Sleep_Mode);
}




void Clear_Auto_Sleep_Mode(void)
{
	Auto_Sleep_Mode = 0x00;
}

void Clear_Flag_Port(void)
{
	Flag_Port = 0;
}


void Set_Power_Seq_Flag(void)
{
	Flag_Power_On_Seq_Start = 1;
}


u8 GetAutoTestTpVal(void)
{
	return(AutoTestTp);
}


u16 GetAutoTestTpDelayVal(void)
{
	return(AutoTestTpDelay);
}


void Print_Seq(void)
{	
	#if 1
	printf("Power_On_Seq_Delay = %d %d %d %d %d %d \r\n",Power_On_Seq_Delay[1],Power_On_Seq_Delay[2],Power_On_Seq_Delay[3],Power_On_Seq_Delay[4],Power_On_Seq_Delay[5],Power_On_Seq_Delay[6]);	
	printf("Power_On_Seq_Index = %d %d %d %d %d %d \r\n",Power_On_Seq_Index[1],Power_On_Seq_Index[2],Power_On_Seq_Index[3],Power_On_Seq_Index[4],Power_On_Seq_Index[5],Power_On_Seq_Index[6]);
	memset(Power_On_Seq_Delay,0,10);
	memset(Power_On_Seq_Index,0,10);
	Power_On_Seq_Index_Cnt = 0;
	#endif
}

void Power_BIN_Load(void)
{
	#if 1	
	//Delay_ms(200);
	LCD_Rectangle(0,0,240,320,WHITE);
	LCD_Str_O(8,80,"Update POWER.BIN",RED);
	LCD_Str_O(8,100,"Please Waiting...",RED);
	

	//Gui_DrawRect(0,0,162,128,WHITE);
	
	Lcd_Clear(BK_COLOR);
	Gui_DrawFont_GBK16(2,16*1,RED,WHITE,"Update POWER.BIN");
	Gui_DrawFont_GBK16(2,16*2,RED,WHITE,"Please Waiting...");
	#endif
}


/*******************************************************************************
* 文件名	  	 : Load_Power_Board_Runtin
* 描述	         : 
* 输入           : 0:/UPDATE/POWER.bin
* 输出           : 
* 返回           : 无
*******************************************************************************/
int Load_Power_Board_Runtin(const char *path)
{
	u32 i,br,btr,j;
	s32 res;
	u32 initCodeFileSzie = 0;
	u8 * ptr;
	FIL fdst_bin;
	FILINFO binfileInfo;
	
	//1. Read File information
	res = f_stat(path,&binfileInfo);
	if ( res == FR_OK )
	{
		initCodeFileSzie = binfileInfo.fsize;
		TESTER_MAIN_DEBUG("\r\n initCodeFileSzie Size: %d bytes\n", initCodeFileSzie);
	}

	//2. Open bin File
	res = f_open(&fdst_bin,path,FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK )
	{ 
		#ifdef DEBUG_BIN_CFG
			TESTER_MAIN_DEBUG("\r\nFAILED to Open .bin file.\r\n" ); 
		#endif
		return -1;
	}
	#ifdef DEBUG_BIN_CFG
		TESTER_MAIN_DEBUG("\r\n File successfully Open .bin file.\r\n" ); 
	#endif
	

	#if 0		//查看数据
	for(i=0;i<binfileInfo.fsize;i=i+4)
	{
		TESTER_MAIN_DEBUG("buf[%x]=%02x %02x %02x %02x\t\r\n",i,ptr[i+0],ptr[i+1],ptr[i+2],ptr[i+3]);
	}
	#endif

	#if 0
		f_close(&fdst_bin);	
		Power_BIN_Load();
		
		
		//2. Open bin File
		res = f_open(&fdst_bin,path,FA_OPEN_EXISTING | FA_READ);
		if(res != FR_OK )
		{ 
			#ifdef DEBUG_BIN_CFG
				TESTER_MAIN_DEBUG("\r\nFAILED to Open .bin file.\r\n" ); 
			#endif
			return -1;
		}
		#ifdef DEBUG_BIN_CFG
			TESTER_MAIN_DEBUG("\r\n File successfully Open .bin file.\r\n" ); 
		#endif
	#else
		Power_BIN_Load();
	#endif


	i=0;
	ptr = G_DISP_FILE_BUFFER; // 20k
	while(i<initCodeFileSzie)
	{
		//set read/write bytes number

		/*
		if(binfileInfo.fsize > (i + FILE_BUFFER_SIZE2)){
			btr = FILE_BUFFER_SIZE2;
		}else{
			btr = binfileInfo.fsize - i;
		}/////////////////////////////////
		*/



		
		//
		#if 0
		f_lseek(&fdst,10);
		f_read(&fdst, ptr, 5, &br );			
		TESTER_MAIN_DEBUG("ptr = 0x%x 0x%x 0x%x 0x%x 0x%x\r\n",*ptr,*(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4));
		#else		
		btr = binfileInfo.fsize;
		res = f_read(&fdst_bin, ptr, btr, &br ); /* 将文件里面的内容读到缓冲区 */
		#endif

		if ( (res != FR_OK) | (br != btr)  ){
			#ifdef DEBUG_LCM_COMM
				TESTER_MAIN_DEBUG("Read .bin FILE Failed\r\n");
			#endif
			return -1;
		}
		
		TESTER_MAIN_DEBUG("Read %d bytes of data.\r\n", br);

	
		
		i += br; 
	}

	
	//printf("Total Write %d bytes of data.\r\n", i);


	
	#if 0
	USART2_Send(G_DISP_FILE_BUFFER,i);	

	f_close(&fdst_bin);	

	f_unlink(path);			//更新完成后delete
	#else
	for(j = 0;j<(i/2048);j++)
	{
		Power_Bin_Send(G_DISP_FILE_BUFFER + (2048*j),i,2048);
		Delay_ms(50);
	}
	Power_Bin_Send(G_DISP_FILE_BUFFER + (2048*j),i,i%2048);

	//printf("count = %d %d\r\n",j,i%2048);

	f_close(&fdst_bin);	

	f_unlink(path);			//更新完成后delete	
	
	Delay_ms(4000);

	SystemReset();
	#endif
	
	
	return TP_OK;
		
}


void Delay_Circle_Ms1(u16 Ms)
{
#if 1
	u16 i;
	while(Ms--)
	{
		i=7200;
		while(i--);
	}
#else
	Delay_Circle_Us(1000*Ms);
#endif
}

static void  PowerUp_Init_Handle1(void)
{
	u16 temp = 0;
	
#if 0
	Delay_ms(200);
	
	i2c1_init();

	Delay_ms(20);

	I2C_Detect();

	
	//UCD9081_Init();

	Delay_ms(200);
#endif
	
	TESTER_MAIN_DEBUG("\r\nEnable ACT8846....\r\n\r\n");
	//Delay_ms(1000);
	act8846_init(); 
	Delay_ms(200);
	TESTER_MAIN_DEBUG("int ACT8846_ON();\r\n");

	//act8846_on();


	//ACT8846_PWR_ON_Cfg();	
	
	
	ACT8846_DCDC_Switch(DCDC_SW1,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW2,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW3,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW4,ACT8846_SWITCH_ON);

	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON);
	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_OFF);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_OFF);
	//ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_OFF);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_OFF);

	//PCA9557_PWR_ON_Cfg();

	//BL_LM36923_POWER_ON; 
	Delay_ms(20);

	//I2C_Detect();
	
	//Delay_ms(50);	
	
	PCA9557_PWR_ON_Cfg();

	Delay_ms(50);	

	LT3582_PWR_ON_Cfg();				//OTP_EN 拉高,里面设置了电压

	TPS22993_PWR_ON_Cfg();
	TPS22993_Sel_Dev(0);				//VSP_VSN POWER ON
	TPS22993_On(TPS22993_CHN1);
	

	Load_Soft_Init();
	
	Delay_ms(80);						//延时时间不能太小
	
	
	//////////////////////////////////////////////////////////VSP VSN设置	
#if 1
	FPGA_Write_Register(0x1C,0x0032);		//BL ADJ 拉高，使能 vsp  vsn	

	BL_LM36923_POWER_ON; 									//背光主电源开启
	LM36923_PWR_ON_Cfg( PWR7_LED1N);		//设置背光电流
	
	Delay_ms(10);		
	Tps65132_Init( PWR5_VSP_AVDD, PWR6_VSN_AVEE); //设置VSP VSN电压
	//Tps65132_Init(5000,5000); 
	/*
	tps65132_set_voltage(5000,1);			//设置VSP VSN电压
	tps65132_set_voltage(5000,0);	
	tps65132_set_mode(0,1,1);	
	*/
	Delay_ms(10);							

	FPGA_Write_Register(0x1C,0x0020);		//BL ADJ 拉高，断开vsp  vsn

	temp = FPGA_Read_Register(0x1c);
	FPGA_Write_Register(0x1C,temp | 0x80);	
#endif

	Flag_Load_CFG_Done = 1;				//初始化配置load完
	

#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("x=%d y=%d\t\r\n",ParaManage.Resolution_X,ParaManage.Resolution_Y);
	TESTER_MAIN_DEBUG("pwr1=%d %d %d %d %d\t\r\n", PWR1_IOVCC, PWR2_VDDH, PWR3_TP_VDD, PWR4_TP_VIO, PWR5_VSP_AVDD);
	TESTER_MAIN_DEBUG("pwr6=%d %d %d %d %d\t\r\n", PWR6_VSN_AVEE, PWR7_LED1N, PWR8_LED2N, PWR9_LED3N, PWR10_VMTP);
	TESTER_MAIN_DEBUG("pwr11=%d %d %d %d \t\r\n", PWR11_VNEG, PWR12_LED_PWR_5V, PWR13_EXT_PWR_A, PWR14_EXT_PWR_B);
	TESTER_MAIN_DEBUG("pwr threshold low=%d %d %d %d %d\t\r\n",UCD9081_ThersholdManage.UCD9081_RAIL1_LOW,UCD9081_ThersholdManage.UCD9081_RAIL2_LOW,UCD9081_ThersholdManage.UCD9081_RAIL3_LOW,UCD9081_ThersholdManage.UCD9081_RAIL4_LOW,UCD9081_ThersholdManage.UCD9081_RAIL5_LOW,UCD9081_ThersholdManage.UCD9081_RAIL6_LOW);
	TESTER_MAIN_DEBUG("pwr threshold high=%d %d %d %d %d\t\r\n",UCD9081_ThersholdManage.UCD9081_RAIL1_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL2_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL3_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL4_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL5_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL6_HIGH);
#endif

#if 1		//电压电流上下限
	printf("IOVCC LIMIT = %d %d %d %d %d %d\r\n",PWR1_IOVCC_HIGH,PWR1_IOVCC_LOW,PWR1_IOVCC_A_HIGH,PWR1_IOVCC_A_LOW,PWR1_IOVCC_UA_HIGH,PWR1_IOVCC_UA_LOW);
	printf("VDDH LIMIT = %d %d %d %d %d %d\r\n",PWR2_VDDH_HIGH,PWR2_VDDH_LOW,PWR2_VDDH_A_HIGH,PWR2_VDDH_A_LOW,PWR2_VDDH_UA_HIGH,PWR2_VDDH_UA_LOW);
	printf("TPVDD LIMIT = %d %d %d %d %d %d\r\n",PWR3_TP_VDD_HIGH,PWR3_TP_VDD_LOW,PWR3_TP_VDD_A_HIGH,PWR3_TP_VDD_A_LOW,PWR3_TP_VDD_UA_HIGH,PWR3_TP_VDD_UA_LOW);
	printf("TPVIO LIMIT = %d %d %d %d %d %d\r\n",PWR4_TP_VIO_HIGH,PWR4_TP_VIO_LOW,PWR4_TP_VIO_A_HIGH,PWR4_TP_VIO_A_LOW,PWR4_TP_VIO_UA_HIGH,PWR4_TP_VIO_UA_LOW);
	printf("TPVDD LIMIT = %d %d %d %d %d %d\r\n",PWR3_TP_VDD_HIGH,PWR3_TP_VDD_LOW,PWR3_TP_VDD_A_HIGH,PWR3_TP_VDD_A_LOW,PWR3_TP_VDD_UA_HIGH,PWR3_TP_VDD_UA_LOW);
	printf("VSP LIMIT = %d %d %d %d %d %d\r\n",PWR5_VSP_AVDD_HIGH,PWR5_VSP_AVDD_LOW,PWR5_VSP_AVDD_A_HIGH,PWR5_VSP_AVDD_A_LOW,PWR5_VSP_AVDD_UA_HIGH,PWR5_VSP_AVDD_UA_LOW);	
	printf("VSN LIMIT = %d %d %d %d %d %d\r\n",PWR6_VSN_AVEE_HIGH,PWR6_VSN_AVEE_LOW,PWR6_VSN_AVEE_A_HIGH,PWR6_VSN_AVEE_A_LOW,PWR6_VSN_AVEE_UA_HIGH,PWR6_VSN_AVEE_UA_LOW);
	printf("LED1 = %d %d %d %d %d %d\r\n",PWR7_LED1N_HIGH,PWR7_LED1N_LOW,PWR7_LED1N_A_HIGH,PWR7_LED1N_A_LOW,PWR7_LED1N_UA_HIGH,PWR7_LED1N_UA_LOW);
	printf("LED2 = %d %d %d %d %d %d\r\n",PWR8_LED2N_HIGH,PWR8_LED2N_LOW,PWR8_LED2N_A_HIGH,PWR8_LED2N_A_LOW,PWR8_LED2N_UA_HIGH,PWR8_LED2N_UA_LOW);
	printf("LED3 = %d %d %d %d %d %d\r\n",PWR9_LED3N_HIGH,PWR9_LED3N_LOW,PWR9_LED3N_A_HIGH,PWR9_LED3N_A_LOW,PWR9_LED3N_UA_HIGH,PWR9_LED3N_UA_LOW);
#endif
	
}


void Load_Initial_Code2(u16 len)
{	

	//USART2_Send(G_DISP_FILE_BUFFER,len);
	//Send_Bin_Len(len);
	//i2c1_init();
	//GPIO_ResetBits(GPIOF,GPIO_Pin_6);
	Clear_Flag_Port();		//ENABLE PORT CAN BE CONFIGED
	FPGA_Key_Off();
	Get_INIT_STATUS(INIT_STATUS_RESTET);		//RESET POWER BOARD
	Delay_ms(1000);
	USART2_Send(G_DISP_FILE_BUFFER,len);
	Send_Bin_Len(len);
	Delay_ms(2500);


	memset(ParaManage.Bin_Name,0,50);	//先清除bin name
	////////////////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_INFO);		//读取常规设置的地址和size	
	Info_Add =  General_Info_Add;
	Info_Add_Size =  General_Info_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("info_add = 0x%x 0x%x\r\n", General_Info_Add, General_Info_Size);	
	#endif	
	MutiBuff_Handle( General_Info_Add, General_Info_Size);	
	PowerUp_Init_Handle1();		//解析完了参数，然后上电初始化
	
	
	////////////////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_PATTEN);		//读取图片
	Pattern_Add =  Pattern_Set_Add;
	Pattern_Add_Size =  Pattern_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("patten_add = 0x%x 0x%x\r\n", Pattern_Set_Add, Pattern_Set_Size);
	#endif
	MutiBuff_Handle( Pattern_Set_Add, Pattern_Set_Size);
	//Load_Pic();
	Pic_Num_Max = PatternManage.Pic_Index_Max;
	



	///////////////////////////////////////////////////////////////////////////////	
	Read_Start_Address(READ_START_ADD_BOARD);		//读取board发送数据	
	Board_Init_Add =  Board_Add;
	Board_Init_Add_Size =  Board_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("board_add = 0x%x 0x%x\r\n", Board_Add, Board_Size);
	#endif
	MutiBuff_Handle( Board_Add, Board_Size);
	
	


	///////////////////////////////////////////////////////////////////////////////	
	Read_Start_Address(READ_START_ADD_POWER_ON);		//读取power on数据	
	PowerOn_Add =  Power_On_Set_Add;
	PowerOn_Add_Size =  Power_On_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("on_add = 0x%x 0x%x\r\n", Power_On_Set_Add, Power_On_Set_Size);
	#endif

	#if 0
	if(Get_PowerSeq_Index())		//上电状态
	{
		MutiBuff_Handle( Power_On_Set_Add, Power_On_Set_Size);		//执行上电程序
	}
	else
	{
		MutiBuff_Handle( Power_Off_Set_Add, Power_Off_Set_Size);	//执行掉电程序
	}
	#endif	


	///////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_INIT_CODE);		//读取lcd init数据
	Code_Init_Add =  Init_Code_Add;
	Code_Init_Add_Size =  Init_Code_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("init_add = 0x%x 0x%x\r\n", Init_Code_Add, Init_Code_Size);
	#endif
	//MutiBuff_Handle( Init_Code_Add, Init_Code_Size);			//记录下地址和size
	


	///////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_POWER_OFF);		//读取power off数据	
	PowerOff_Add =  Power_Off_Set_Add;
	PowerOff_Add_Size =  Power_Off_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("off_add = 0x%x 0x%x\r\n", Power_Off_Set_Add, Power_Off_Set_Size);
	#endif


	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_SLEEP_SEQ);		//读取休眠SLEEP数据	
	Sleep_Seq_Add =  Sleep_Seq_Set_Add;
	Sleep_Seq_Add_Size =  Sleep_Seq_Set_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("off_add = 0x%x 0x%x\r\n", Sleep_Seq_Set_Add, Sleep_Seq_Set_Size);
	#endif


	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_INIT2_COD);		//读取用户定义数据	
	Code_Init2_Add =  Init2_Code_Add;
	Code_Init2_Add_Size =  Init2_Code_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("off_add = 0x%x 0x%x\r\n", Init2_Code_Add, Init2_Code_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE1);		//读取cmd1数据	
	Code_Cmd1_Add =  Cmd_Code1_Add;
	Code_Cmd1_Add_Size =  Cmd_Code1_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code1 = 0x%x 0x%x\r\n", Cmd_Code1_Add, Cmd_Code1_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE2);		//读取cmd2数据	
	Code_Cmd2_Add =  Cmd_Code2_Add;
	Code_Cmd2_Add_Size =  Cmd_Code2_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code2 = 0x%x 0x%x\r\n", Cmd_Code2_Add, Cmd_Code2_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE3);		//读取cmd3数据	
	Code_Cmd3_Add =  Cmd_Code3_Add;
	Code_Cmd3_Add_Size =  Cmd_Code3_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code3 = 0x%x 0x%x\r\n", Cmd_Code3_Add, Cmd_Code3_Size);
	#endif

	////////////////////////////////////////////////////////////////////////////
	Read_Start_Address(READ_START_ADD_CMD_CODE4);		//读取cmd4数据	
	Code_Cmd4_Add =  Cmd_Code4_Add;
	Code_Cmd4_Add_Size =  Cmd_Code4_Size;
	#if DEBUG_BIN_CFG
	TESTER_MAIN_DEBUG("Cmd_Code4 = 0x%x 0x%x\r\n", Cmd_Code4_Add, Cmd_Code4_Size);
	#endif
	
	#if 0
	if(Get_PowerSeq_Index())		//上电状态
	{
		MutiBuff_Handle( Power_On_Set_Add, Power_On_Set_Size);		//执行上电程序
	}
	else
	{
		MutiBuff_Handle( Power_Off_Set_Add, Power_Off_Set_Size);	//执行掉电程序
	}
	#endif

	
	if(G_Select_Port == 3)
	{
		LCD1_8_Str_6x12(BL1_LABLE_X,BL1_LABLE_Y,"B",BLACK,GREEN);
		LCD1_8_Str_6x12(BL2_LABLE_X,BL2_LABLE_Y,"A",BLACK,GREEN);
	}
	else
	{
		LCD1_8_Str_6x12(BL1_LABLE_X,BL1_LABLE_Y,"B",BLACK,BK_COLOR);
		LCD1_8_Str_6x12(BL2_LABLE_X,BL2_LABLE_Y,"A",BLACK,GREEN);
	}

	Connect_Fresh();
	

	Show_Bin_Name_Version();

	//Initial_Code_Handle();
}


void printf_message(void)
{
	u32 i;
	char *temp_arr;
	u16 *lcmp = (u16 *)&lcm_para;
	u16 *pp;
	
	Get_Bin_Version(temp_arr);//

	
	TESTER_MAIN_DEBUG("###############printf_message############\r\n###############printf_message############\r\n");

	TESTER_MAIN_DEBUG("\r\n  G_Select_Port = %d ",G_Select_Port);	
	TESTER_MAIN_DEBUG("\r\n  G_disp_width = %d ",G_disp_width);	
	TESTER_MAIN_DEBUG("\r\n  G_disp_height = %d ",G_disp_height);
	TESTER_MAIN_DEBUG("\r\nBin version = %s \r\n",&ParaManage.Bin_Name);

	TESTER_MAIN_DEBUG("\r\nIOVCC=%d VDDH=%d TP_VDD=%d TP_VIO=%d VSP_AVDD=%d\t\r\n", PWR1_IOVCC, PWR2_VDDH, PWR3_TP_VDD, PWR4_TP_VIO, PWR5_VSP_AVDD);
	TESTER_MAIN_DEBUG("\r\nVSN_AVEE=%d LED1N=%d LED2N=%d LED3N=%d VMTP=%d\t\r\n", PWR6_VSN_AVEE, PWR7_LED1N, PWR8_LED2N, PWR9_LED3N, PWR10_VMTP);
	TESTER_MAIN_DEBUG("\r\nVNEG=%d LED_PWR_5V=%d EXT_PWR_A=%d EXT_PWR_B=%d \t\r\n", PWR11_VNEG, PWR12_LED_PWR_5V, PWR13_EXT_PWR_A, PWR14_EXT_PWR_B);

	TESTER_MAIN_DEBUG("\t\r\npwr threshold low=%d %d %d %d %d\t\r\n",UCD9081_ThersholdManage.UCD9081_RAIL1_LOW,UCD9081_ThersholdManage.UCD9081_RAIL2_LOW,UCD9081_ThersholdManage.UCD9081_RAIL3_LOW,UCD9081_ThersholdManage.UCD9081_RAIL4_LOW,UCD9081_ThersholdManage.UCD9081_RAIL5_LOW,UCD9081_ThersholdManage.UCD9081_RAIL6_LOW);
	TESTER_MAIN_DEBUG("\r\npwr threshold high=%d %d %d %d %d\t\r\n",UCD9081_ThersholdManage.UCD9081_RAIL1_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL2_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL3_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL4_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL5_HIGH,UCD9081_ThersholdManage.UCD9081_RAIL6_HIGH);

	TESTER_MAIN_DEBUG("\r\nIOVCC LIMIT = %d %d %d %d %d %d\r\n",PWR1_IOVCC_HIGH,PWR1_IOVCC_LOW,PWR1_IOVCC_A_HIGH,PWR1_IOVCC_A_LOW,PWR1_IOVCC_UA_HIGH,PWR1_IOVCC_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nVDDH LIMIT = %d %d %d %d %d %d\r\n",PWR2_VDDH_HIGH,PWR2_VDDH_LOW,PWR2_VDDH_A_HIGH,PWR2_VDDH_A_LOW,PWR2_VDDH_UA_HIGH,PWR2_VDDH_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nTPVDD LIMIT = %d %d %d %d %d %d\r\n",PWR3_TP_VDD_HIGH,PWR3_TP_VDD_LOW,PWR3_TP_VDD_A_HIGH,PWR3_TP_VDD_A_LOW,PWR3_TP_VDD_UA_HIGH,PWR3_TP_VDD_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nTPVIO LIMIT = %d %d %d %d %d %d\r\n",PWR4_TP_VIO_HIGH,PWR4_TP_VIO_LOW,PWR4_TP_VIO_A_HIGH,PWR4_TP_VIO_A_LOW,PWR4_TP_VIO_UA_HIGH,PWR4_TP_VIO_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nTPVDD LIMIT = %d %d %d %d %d %d\r\n",PWR3_TP_VDD_HIGH,PWR3_TP_VDD_LOW,PWR3_TP_VDD_A_HIGH,PWR3_TP_VDD_A_LOW,PWR3_TP_VDD_UA_HIGH,PWR3_TP_VDD_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nVSP LIMIT = %d %d %d %d %d %d\r\n",PWR5_VSP_AVDD_HIGH,PWR5_VSP_AVDD_LOW,PWR5_VSP_AVDD_A_HIGH,PWR5_VSP_AVDD_A_LOW,PWR5_VSP_AVDD_UA_HIGH,PWR5_VSP_AVDD_UA_LOW);	
	TESTER_MAIN_DEBUG("\r\nVSN LIMIT = %d %d %d %d %d %d\r\n",PWR6_VSN_AVEE_HIGH,PWR6_VSN_AVEE_LOW,PWR6_VSN_AVEE_A_HIGH,PWR6_VSN_AVEE_A_LOW,PWR6_VSN_AVEE_UA_HIGH,PWR6_VSN_AVEE_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nLED1 = %d %d %d %d %d %d\r\n",PWR7_LED1N_HIGH,PWR7_LED1N_LOW,PWR7_LED1N_A_HIGH,PWR7_LED1N_A_LOW,PWR7_LED1N_UA_HIGH,PWR7_LED1N_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nLED2 = %d %d %d %d %d %d\r\n",PWR8_LED2N_HIGH,PWR8_LED2N_LOW,PWR8_LED2N_A_HIGH,PWR8_LED2N_A_LOW,PWR8_LED2N_UA_HIGH,PWR8_LED2N_UA_LOW);
	TESTER_MAIN_DEBUG("\r\nLED3 = %d %d %d %d %d %d\r\n",PWR9_LED3N_HIGH,PWR9_LED3N_LOW,PWR9_LED3N_A_HIGH,PWR9_LED3N_A_LOW,PWR9_LED3N_UA_HIGH,PWR9_LED3N_UA_LOW);

	
	TESTER_MAIN_DEBUG("\r\n\r\n Flag_Current_En=%d",Flag_Current_En);
	TESTER_MAIN_DEBUG("\r\n Flag_Voltage_En=%d",Flag_Voltage_En);
	TESTER_MAIN_DEBUG("\r\n Flag_Sleep_Current_En=%d",Flag_Sleep_Current_En);
	TESTER_MAIN_DEBUG("\r\n Flag_Hard_ID_En=%d",Flag_Hard_ID_En);
	TESTER_MAIN_DEBUG("\r\n Hard_ID_En_Val=%d",Hard_ID_En_Val);
	TESTER_MAIN_DEBUG("\r\n Flag_Soft_ID_En=%d",Flag_Soft_ID_En);
	TESTER_MAIN_DEBUG("\r\n Soft_ID12_Val=%d",Soft_ID12_Val);
	TESTER_MAIN_DEBUG("\r\n Soft_ID34_Val=%d",Soft_ID34_Val);
	TESTER_MAIN_DEBUG("\r\n Flag_OTP_Times_En=%d",Flag_OTP_Times_En);
	TESTER_MAIN_DEBUG("\r\n OTP_Times_High=%d",OTP_Times_High);
	TESTER_MAIN_DEBUG("\r\n OTP_Times_Low=%d",OTP_Times_Low);
	TESTER_MAIN_DEBUG("\r\n Flag_DuiWei_En=%d",Flag_DuiWei_En);
	TESTER_MAIN_DEBUG("\r\n Flag_PWM_En=%d",Flag_PWM_En);
	TESTER_MAIN_DEBUG("\r\n PWM_Seq_High=%d",PWM_Seq_High);
	TESTER_MAIN_DEBUG("\r\n PWM_Seq_Low=%d",PWM_Seq_Low);
	TESTER_MAIN_DEBUG("\r\n PWM_Duty_High=%d",PWM_Duty_High);
	TESTER_MAIN_DEBUG("\r\n PWM_Duty_Low=%d",PWM_Duty_Low);
	TESTER_MAIN_DEBUG("\r\n PWM_Volt_High=%d",PWM_Volt_High);
	TESTER_MAIN_DEBUG("\r\n PWM_Volt_Low=%d",PWM_Volt_Low);
	TESTER_MAIN_DEBUG("\r\n Flag_TE_En=%d",Flag_TE_En);
	TESTER_MAIN_DEBUG("\r\n TE_Seq_High=%d",TE_Seq_High);
	TESTER_MAIN_DEBUG("\r\n TE_Seq_Low=%d",TE_Seq_Low);
	TESTER_MAIN_DEBUG("\r\n TE_Duty_High=%d",TE_Duty_High);
	TESTER_MAIN_DEBUG("\r\n TE_Duty_Low=%d",TE_Duty_Low);
	TESTER_MAIN_DEBUG("\r\n TE_Volt_High=%d",TE_Volt_High);
	TESTER_MAIN_DEBUG("\r\n TE_Volt_Low=%d",TE_Volt_Low);
	TESTER_MAIN_DEBUG("\r\n FLAG_BK_En=%d",FLAG_BK_En);
	
	
	TESTER_MAIN_DEBUG("\r\n Flag_Key_OnOff_Short =%d",Flag_Key_OnOff_Short);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_OnOff_Long = %d",Flag_Key_OnOff_Long);
	TESTER_MAIN_DEBUG("\r\n Key_OnOff_NormalPress %d",Key_OnOff_NormalPress);
	TESTER_MAIN_DEBUG("\r\n Key_OnOff_LongPress = %d",Key_OnOff_LongPress);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Left_Short = %d",Flag_Key_Left_Short);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Left_Long =  %d",Flag_Key_Left_Long);
	TESTER_MAIN_DEBUG("\r\n Key_Left_NormalPress =%d",Key_Left_NormalPress);
	TESTER_MAIN_DEBUG("\r\n Key_Left_LongPress =  %d",Key_Left_LongPress);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Auto_Short = %d",Flag_Key_Auto_Short);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Auto_Long =  %d",Flag_Key_Auto_Long);
	TESTER_MAIN_DEBUG("\r\n Key_Auto_NormalPress =%d",Key_Auto_NormalPress);
	TESTER_MAIN_DEBUG("\r\n Key_Auto_LongPress =  %d",Key_Auto_LongPress);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Right_Short =%d",Flag_Key_Right_Short);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Right_Long = %d",Flag_Key_Right_Long);
	TESTER_MAIN_DEBUG("\r\n Key_Right_NormalPress %d",Key_Right_NormalPress);
	TESTER_MAIN_DEBUG("\r\n Key_Right_LongPress = %d",Key_Right_LongPress);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Test_Short = %d",Flag_Key_Test_Short);
	TESTER_MAIN_DEBUG("\r\n Flag_Key_Test_Long =  %d",Flag_Key_Test_Long);
	TESTER_MAIN_DEBUG("\r\n Key_Test_NormalPress =%d",Key_Test_NormalPress);
	TESTER_MAIN_DEBUG("\r\n Key_Test_LongPress =  %d",Key_Test_LongPress);
	TESTER_MAIN_DEBUG("\r\n Key_Lock_Time =  %d",Key_Lock_Time);

	for(i=0;i<PatternManage.Pic_Index_Max;i++)
	{
		TESTER_MAIN_DEBUG("\r\n pic_%d : Index=%d  Name=%x Color_Index=%x\r\n",i,
		PatternManage.Pattern_Index_Buf[i],PatternManage.Pic_Name_Buf[i],PatternManage.Pure_Color_Index_Buf[i]);	
	}
	
	TESTER_MAIN_DEBUG("###############printf_message############\r\n###############printf_message############\r\n\r\n");

	printf("\r\nlcm_parameter:%d    lcm_para:%d   lcmp addr:0x%x\r\n",sizeof(lcm_parameter),sizeof(lcm_para),lcmp);
	
	for(i=0;i<sizeof(lcm_para)/2;i++)
	{
		printf(" %d",*(lcmp++));
	}
	
	lcmp = (u16 *)lcm_para.lcmname;
	
	
	printf("\r\n*lcmp++  %x",*(lcmp++));
	printf(" %x",*(lcmp++));
	printf(" %x",*(lcmp++));

	printf("lcm_para.name data:%x",lcm_para.lcmname[0]);
	printf(" %x",lcm_para.lcmname[1]);
	printf(" %x",lcm_para.lcmname[2]);
	
	printf("lcm_para.name addr:%x",lcm_para.lcmname);

	TESTER_MAIN_DEBUG("\r\n\r\n###############printf_message############\r\n###############printf_message############\r\n");
	
}

int Lcm_Fpga_init(void)
{
	 uint16_t G_Reg;
	 G_disp_width = 0x0438;
	 G_disp_height = 0x0780;
	 FPGA_Write_Register(0x10,0x9805);//VIDEO mode
	 FPGA_Write_Register(0x03,0x0438);//1080
	 FPGA_Write_Register(0x04,0x002b);//HBP
	 FPGA_Write_Register(0x05,0x0060);//HFP
	 FPGA_Write_Register(0x06,0x0002);//HPW
	 FPGA_Write_Register(0x07,0x0780);//1920
	 FPGA_Write_Register(0x08,0x0003);//VBP
	 FPGA_Write_Register(0x09,0x0004);//VFP
	 FPGA_Write_Register(0x0a,0x001);//VPW
	 FPGA_Write_Register(0x0b,0x000);//rgb pluse event
	 FPGA_Write_Register(0x0c,0x071);//M
	 FPGA_Write_Register(0x0d,0x008);//N
	 FPGA_Write_Register(0x0e,0x005);//C
	 FPGA_Write_Register(0x0f,0x000);//PHASE
	 FPGA_Write_Register(0x12,0x001);//able rgb pll
	 Delay_ms(1);
	 FPGA_Write_Register(0x13,0x001);//rgm put out
	 
	 G_Reg = FPGA_Read_Register(0x1c);//reset SSDD2828
	 G_Reg = G_Reg & 0xfff7;
	 
	 FPGA_Write_Register(0x1c,G_Reg);
	 if(FPGA_FSMC_GetStatus(FPGA_TIMEOUT) != 0)
	{
		TESTER_MAIN_DEBUG("FPGA_FSMC_GetStatus timeout.");
		return TP_ERR;
	}	
	Delay_ms(10);

	 G_Reg = FPGA_Read_Register(0x1c);
	 G_Reg = G_Reg | 0x0008;
	 
	 FPGA_Write_Register(0x1c,G_Reg);
	 if(FPGA_FSMC_GetStatus(FPGA_TIMEOUT) != 0)
	{
		TESTER_MAIN_DEBUG("FPGA_FSMC_GetStatus timeout.");
		return TP_ERR;
	}	
	Delay_ms(32);
	return 0;
}

//WriteSSD2828Reg(uint8_t reg,uint32_t data)
int Lcm_SSD2828_init(void)
{
	uint16_t Reg;

	uint16_t mode;

	WriteSSD2828Reg(0xb9,0x0000);
	WriteSSD2828Reg(0xba,0xc2b8);
	WriteSSD2828Reg(0xbb,0x000e);
	WriteSSD2828Reg(0xb9,0x0001);
	Delay_ms(10);

	WriteSSD2828Reg(0xb8,0x0000);
	WriteSSD2828Reg(0xde,0x0003);
	WriteSSD2828Reg(0xb1,0x0102);
	WriteSSD2828Reg(0xb2,0x042d);
	WriteSSD2828Reg(0xb3,0x0460);
	WriteSSD2828Reg(0xb4,0x0438);
	WriteSSD2828Reg(0xb5,0x0780);
	WriteSSD2828Reg(0xb6,0x000b);
	WriteSSD2828Reg(0xc9,0x1f0a);
	WriteSSD2828Reg(0xca,0x490d);
	WriteSSD2828Reg(0xcb,0x0321);
	WriteSSD2828Reg(0xcc,0x1112);
	WriteSSD2828Reg(0xdf,0x001c);
	WriteSSD2828Reg(0xb7,0x0300);


	Reg = FPGA_Read_Register(0x1c);//lcm reset = 0
	Reg = Reg & 0xfffb;
	FPGA_Write_Register(0x1c,Reg);
	if(FPGA_FSMC_GetStatus(FPGA_TIMEOUT) != 0)
	{
		TESTER_MAIN_DEBUG("FPGA_FSMC_GetStatus timeout.");
		return TP_ERR;
	}	
	Delay_ms(32);
	
	return 0;
}



