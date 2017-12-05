#include "led.h"
#include "delay.h"
#include "sys.h"
//ALIENTEKս��STM32������ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾


u8 app[32];

//pFunction0 IAP_W25QXX_Init = 0;

void led_flash(void)
{
		u8 i=5;
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(i--)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		delay_ms(500);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
	}
}



typedef  void (*pFunction0)(void);

int main(void)
{
	u8 i=5;
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	app[0] = (u32)led_flash;
	while(i--)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
	}
	return 0;
}

void APPmain(void)
{	
	u8 i=3;
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(i--)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);

		delay_ms(200);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
	}
}

