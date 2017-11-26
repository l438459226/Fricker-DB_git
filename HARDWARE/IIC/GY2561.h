#ifndef __GY2561_H
#define __GY2561_H
#include "myiic.h"   
//Mini STM32开发板
//24CXX驱动函数(适合24C01~24C16,24C32~256未经过测试!有待验证!)
//正点原子@ALIENTEK
//2010/6/10
//V1.2

//??????
#define SLAVE_ADDR_WR 0x72
#define SLAVE_ADDR_RD 0x73
//???????
#define CONTROL 0x80
#define TIMING 0x81
#define GYINIT 0x86
#define DATA0LOW 0x8C
#define DATA0HIGH 0x8D
#define DATA1LOW 0x8E
#define DATA1HIGH 0x8F





void GY2561_Init(void);
u8 GY2561_RD(u8 reg);
u16 Read_Light(u8 Channel);
u8 GY2561_WR(u8 reg,u8 date);


#endif
















