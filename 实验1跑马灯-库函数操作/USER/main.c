#include "led.h"
#include "delay.h"
#include "sys.h"
//ALIENTEKս��STM32������ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾




 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(1)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);

		delay_ms(500);
	}
 }

