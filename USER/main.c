
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



int main(void)
{ 
	//u32 iitime;
//	u8 sram_per;
//	u8 data,i,temp;
	
	
  NVIC_Configuration();
	delay_init();	    	 //��ʱ������ʼ��

	System_GPIO_Config();	
	uart2_init(115200);	 	//���ڳ�ʼ��Ϊ9600
 	
	usmart_dev.init(72); 	//��ʼ��USMART				 	
	mem_init(SRAMIN);
	
	
	
	IIC_Init();
	Flicker_init();//ʹ���ڴ����
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

	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);				//��ӦOUT5�� ע�� :OUT6 �� OUT10û�о���TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON);

	
  while(1) 
	{
		delay_ms(1000);
		
	}											    
}	




