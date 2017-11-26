#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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


void LED_Init(void);//��ʼ��
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
