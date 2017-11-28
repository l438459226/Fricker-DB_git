#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/10 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

   	   		   
//IO��������
//#define SDA_IN()  {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=8<<8;}
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFF0FF;GPIOB->CRH|=3<<8;}

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SCL   PBin(6)  //����SDA
#define READ_SDA   PBin(7)

#define I2C_SCL_CHECK()     (GPIOB->IDR&GPIO_Pin_6)

//#define I2C_SDA_SET_HIZ()   GPIOB->BSRR=GPIO_Pin_7
//#define I2C_SDA_SET_LO()    GPIOB->BRR=GPIO_Pin_7
#define I2C_SDA_CHECK()     (GPIOB->IDR&GPIO_Pin_7)

#define 	I2C_ERR 	-1
#define 	I2C_OK		0

void SDA_OUT(void);
void SDA_IN(void);

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
int IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
int IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(void);//IIC��ȡһ���ֽ�
int IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�


void i2c1_init(void);
int i2c1_start(u8 sla_adr);
int i2c1_write_u8(u8 txd);
int i2c1_stop(void);
int i2c1_start_repeat(u8 sla_adr);
int i2c1_read_u8(u8 *data, u8 nak);

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















