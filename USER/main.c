
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "boardcfg.h"
#include "arm_math.h"
#include "math.h"
#include "Fricker.h"
#include "LT35825.h"
#include "LT3582.h"
#include "act8846.h"
#include "parm.h"

#include "GPIO_Config.h"
#include "TPS22993.h"
#include "LM36923.h"



int main(void)
{ 
	//u32 iitime;
//	u8 sram_per;
//	u8 data,i,temp;
	
	
  NVIC_Configuration();
	delay_init();	    	 //延时函数初始化

	System_GPIO_Config();	
	uart2_init(115200);	 	//串口初始化为9600
 	
	usmart_dev.init(72); 	//初始化USMART				 	
	mem_init(SRAMIN);
	
	
	
	IIC_Init();
	Flicker_init();//使用内存管理
	printf("uart2 init ok!\r\n");
	

	TL35825_init();
	Set_Voutp(5.11);//4.95v~12.75v
	Set_Voutn(-2.3);//-1.2v~-13.95v
	Voutp_Upfirst();
	


	
	printf("\r\nEnable ACT8846....\r\n");
	act8846_init(); 
	Delay_ms(200);
	printf("int ACT8846_ON();\r\n");
	
	ACT8846_DCDC_Switch(DCDC_SW1,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW2,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW3,ACT8846_SWITCH_ON);//off san lu
	ACT8846_DCDC_Switch(DCDC_SW4,ACT8846_SWITCH_ON);

	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON);

	
	TPS22993_Init();							//快速放电
	TPS22993_Set_All_Mode(1);			//设置负载开关为io控制
	TPS22993_TPVDD_ON_OFF(1);
	TPS22993_TPVIO_ON_OFF(1);
	TPS22993_VDDH_ON_OFF(1);
	TPS22993_EXT1_ON_OFF(1);
	TPS22993_IOVCC_ON_OFF(1);
	TPS22993_VSP_VSN_ON_OFF(1);
	
	delay_ms(200);
	
	OTP_P_ON_OFF(1);					//MOS管开关
	
	
	BL_POWER_ON(); 						//mos管 控制  给bl供5V电源
	Delay_ms(100);

	OLEDLCD_MAIN_POWER_OFF(); 	//LCD OLED 电压切换   控制mos管  通过继电器切换电源

	BL_POWER_ON(); 	//mos管 控制  给bl供5V电源
	Delay_ms(100);
	BL_ADJ_OFF();	//接入BL模块

	BL_DETECT_ON();	//背光控制mos管开关背光模块BL+ 
	
	printf("bei guang dianliushezhi \r\n");
	LM36923_Init(20);
	
  while(1) 
	{
		delay_ms(1000);
		
	}											    
}	




