#include "led.h"
#include "delay.h"
#include "sys.h"
//ALIENTEK战舰STM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司


u8 app[32];

//pFunction0 IAP_W25QXX_Init = 0;

void led_flash(void)
{
		u8 i=5;
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
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
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
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
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	while(i--)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);

		delay_ms(200);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
	}
}

