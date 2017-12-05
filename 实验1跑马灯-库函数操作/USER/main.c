#include "led.h"
#include "delay.h"
#include "sys.h"
//ALIENTEK战舰STM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司




 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	while(1)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
	}
 }

