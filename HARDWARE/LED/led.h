#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PBout(0)	// PB0
#define LED1 PBout(1)	// PB1	

#define RIIC_CLK    		PBin(6)		//GT9147????
#define RIIC_DAT    		PBin(7)		//GT9147????	
#define RIIC_DATT 			PBout(7)





struct cmmd{
	char cmdbuffer[64];
	u8 pmdbuf[64];
	u8 num;
};


void LED_Init(void);//初始化
int strcmp(const char * cs,const char * ct);
int readline(char *buff,char *pcmd,u8 *valu);
int atoi(char * buf);
	
u8 readiic(u8 *iicdata); 
u8 readiicc(u8 *iicdata);
u8 readiice(u8 *iicdata);
 
 u8 readiicd(u8 *iicdata);
 void Ri2c_Out(void);
 u8 writebyteslave(u8 data);
 
 u8 readiicdD(u8 *iicdata);
	u8 readiice(u8 *iicdat);
void clearstruct(void);	
struct parmeiic{
		u8 type;
		u16 data;
		u8 itype;
		u8 deviati;
		u8 flag;
		u8 result1;
		u8 result2;
		u8 result3;
		u8 compare;
	};
u8 readIIC(void);

#endif
