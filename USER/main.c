
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
#include "tps65132.h"

#include "INA226.h"
#include "AD715.h"
#include "Uart.h"
#include "VoltCurrentProc.h"

u8 bufer[512];

int main(void)
{ 
	
  NVIC_Configuration();
	delay_init();	    	 //��ʱ������ʼ��

	System_GPIO_Config();	
	//uart2_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	InitUart();
 	
	usmart_dev.init(72); 	//��ʼ��USMART				 	
	mem_init(SRAMIN);
	
	
	
	IIC_Init();
	Flicker_init();//ʹ���ڴ����
	printf("uart2 init ok!\r\n");
	//I2C_Detect();

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

	
	TPS22993_Init();							//���ٷŵ�
	TPS22993_Set_All_Mode(1);			//���ø��ؿ���Ϊio����
	TPS22993_TPVDD_ON_OFF(1);
	TPS22993_TPVIO_ON_OFF(1);
	TPS22993_VDDH_ON_OFF(1);
	TPS22993_EXT1_ON_OFF(1);
	TPS22993_IOVCC_ON_OFF(1);
	TPS22993_VSP_VSN_ON_OFF(1);
	
	Delay_ms(200);
	
	OTP_P_ON_OFF(1);					//MOS�ܿ���

	OLEDLCD_MAIN_POWER_OFF(); 	//LCD OLED ��ѹ�л�   ����mos��  ͨ���̵����л���Դ

	BL_POWER_ON(); 	//mos�� ����  ��bl��5V��Դ
	Delay_ms(100);
	BL_ADJ_OFF();	//����BLģ��

	BL_DETECT_ON();	//�������mos�ܿ��ر���ģ��BL+ 
	
	printf("bei guang dianliushezhi \r\n");
	LM36923_Init(20);
	
	
	VSP_ON();	//��ʹ�� vsp  vsn
	VSP_PWR_ON();	//	
	VSN_ON();
	
	Delay_ms(10);
	Tps65132_Init(Get_Volt_Val(PWR5_VSP_AVDD_INDEX), Get_Volt_Val(PWR6_VSN_AVEE_INDEX)); //����VSP VSN��ѹ
	Delay_ms(10);


	RELAY_UA_ALL_ON_OFF(1);
	Delay_ms(500);
	INA226_Volt();

  while(1) 
	{
		//WriteUart(USART_PORT_COM2,"SDOIFUHODSFCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",60);
		Current_Volt();
		Delay_ms(50);
		if(ReadUart(USART_PORT_COM2,bufer,64))
			printf("read uart:%s\r\n",bufer);
	}			
	
}	




