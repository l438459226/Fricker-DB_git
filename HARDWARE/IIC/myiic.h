#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//#include "i2c1_bitbang.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

   	   		   
//IO方向设置
//#define SDA_IN()  {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=8<<8;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=3<<8;}

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SCL   PBin(6)  //输入SDA
#define READ_SDA   PBin(7)


#define 	I2C_ERR 	-1
#define 	I2C_OK		0

void SDA_OUT(void);
void SDA_IN(void);

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
int IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
int IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(void);//IIC读取一个字节
int IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Scl(u8 bit);
void IIC_Sda(u8 bit);
void i2c1_init(void);//i2c11__send_start
int i2c1_start(u8 sla_adr);
int i2c1_write_u8(u8 txd);
int i2c1_stop(void);
int i2c1_start_repeat(u8 sla_adr);
int i2c1_read_u8(u8 *data, u8 nak);

void I2C_Detect(void);
int i2c1_read_data(u8 *data, u32 nr_of_bytes, u8 nak_last_byte);

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















