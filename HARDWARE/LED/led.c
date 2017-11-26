#include "led.h"
#include "usart.h"
#include "stdarg.h"
#include "lcd.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

/*
#define	 NATIVE_INT		 int
#define  _AUPBND         (sizeof (NATIVE_INT) - 1)
#define  _ADNBND         (sizeof (NATIVE_INT) - 1)
*/
//typedef int *__va_list[1]; 
/*
typedef int *va_list;
#define va_start(ap, A)		(ap = (int *)&(A) + 1)
#define va_arg(ap, T)		(*(T *)ap++)
#define va_end(ap)		((void)0)

*/

/*
 * Variable argument list macro definitions
 */


u8 commd;
char mbuffer[128];
struct cmmd cmd;


//初始化PB5和PE5为输出口.并使能这两个口的时钟
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;				 //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOB,GPIO_Pin_0);						 //PA.8 输出高
}


//LED状态设置函数
void led_set(u8 sta)
{
	LED1=sta;
} 
//函数参数调用测试函数
void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}  	

/*
int atoi(char *buf)		//
{
	
	int value = 0;	
	int base = 10;
	int i = 0;
	int tmp=0;
	
	if (buf[0] == '0' && buf[1] == 'x')
	{
		base = 16;
		i = 2;
	}	
	// 123 = (1 * 10 + 2) * 10 + 3
	// 0x1F = 1 * 16 + F(15)	
	while (buf[i])
	{
		if (buf[i] <= '9' && buf[i] >= '0') tmp = buf[i] - '0';
		else if(buf[i] <= 'f' && buf[i] >= 'a')	tmp = buf[i] - 'a' + 10;
		else if(buf[i] <= 'F' && buf[i] >= 'A')	tmp = buf[i] - 'A' + 10;
		
		value = value * base + tmp;
		i++;
	}
	return value;
}
*/




int readline(char *buff,char *pcmd,u8 *valu)
{
	char *buf = buff;
	int argc = 0;
	u8 i;
	char *argv[65];
	
	int state = 0;
	
	while (*buf)
	{
		if (*buf != ' ' && state == 0)
		{
			argv[argc++] = buf;
			state = 1;
		}
		if (*buf == ' ' && state == 1)
		{
			*buf = '\0';
	  	state = 0;
		}
		buf++;		
	}
	
	for(i=0;i<argc;i++)
	{
		valu[i] = atoi(argv[i]);
	}
	
	return argc;
}


int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}




u8	commd_fun(char *p)
{
		char *pp = p;
		u8 i;
		for(i=0;i<64;i++){
			if((*(pp+i))!='\0')
				cmd.cmdbuffer[i]=*(pp+i);
			else cmd.cmdbuffer[i]='\0';
		}
		printf(" cmd.cmdbuffer:%s \r\n",cmd.cmdbuffer);
		cmd.num = readline(cmd.cmdbuffer,mbuffer,cmd.pmdbuf);//返回个数
		printf(" cmd.cmdbuffer:%s \r\n",cmd.cmdbuffer);
		printf(" cmd.num:%X \r\n",cmd.num);
		return 0;
}

size_t strlenn(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}



