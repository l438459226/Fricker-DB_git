
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "boardcfg.h"
#include "arm_math.h"
#include "math.h"
#include "Fricker.h"
#include "LT35825.h"
#include "LT3582.h"
#include "i2c1_bitbang.h"

int main(void)
{ 
	//u32 iitime;
//	u8 sram_per;
//	u8 data,i,temp;
	
	
  NVIC_Configuration();
	delay_init();	    	 //延时函数初始化	  
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
	
  while(1) 
	{
		delay_ms(1000);
		
	}											    
}	




