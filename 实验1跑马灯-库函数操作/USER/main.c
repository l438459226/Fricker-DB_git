#include "led.h"
#include "delay.h"
#include "sys.h"
<<<<<<< HEAD
#include "stdio.h"
=======
>>>>>>> origin/master
//ALIENTEKս��STM32������ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾


<<<<<<< HEAD
u8 app[32];

//pFunction0 IAP_W25QXX_Init = 0;

void Delay_us(u32 val)
{
	int i = 0,j = 0;

	for(i = 0; i < val ; i++)
	{
		for(j = 0;j < 36;j++)
		{
			;
		}
	}
}
void Delay_ms(u16 nms)
{
	Delay_us(nms*1000);
}

void led_flash(void)
{
	u8 i=5;
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(i--)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		Delay_ms(1000);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		Delay_ms(1000);
	}
}



typedef  void (*pFunction0)(void);
u32 rw[12] __attribute__ ((section ("codeinterge"))) ={1,23,4,5,544,4323}; 




int main(void)
{
	u8 i=5;
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	app[0] = (u32)led_flash;
	while(i--)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		rw[i] = i;
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
=======


 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(1)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
>>>>>>> origin/master
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
	}
<<<<<<< HEAD
}
=======
 }
>>>>>>> origin/master

