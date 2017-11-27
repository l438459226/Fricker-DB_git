#include "myiic.h"
#include "delay.h"
#include "usart.h"
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

/*************************/
#define IIC_SPEED		400
#define IIC_HOLDTIME	((1000/IIC_SPEED)+1)

//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	IIC_SCL=1;
	IIC_SDA=1;
}

void SDA_OUT(void)
{				
	GPIOB->CRL=(GPIOB->CRL&(~((u32)0xF<<28)))|((u32)0x3<<28);//
	IIC_SDA=1;
}

void SDA_IN(void)
{			
	GPIOB->CRL=(GPIOB->CRL&(~((u32)0xF<<28)))|(u32)((u32)0x8<<28);//
	IIC_SDA=1;
}

/********检测总线是否被占用*********************/
int IIC_Buscheck()
{
  u8 outtime,i;
	// Check bus
  while((I2C_SCL_CHECK()==0)||(I2C_SDA_CHECK()==0))	 //READ_SDA
  {
       // Bus error
	  SDA_OUT();     //sda线输出
      printf("i2c levevl is low!!\r\n");
	  IIC_SDA=1;
      for(i=0;i<9;i++)				//SDA拉高，SCL连续发送9个脉冲复位I2c设备，一定要重新复位，否则上电容易出错
	  {
		IIC_SCL=1;
		delay_us(IIC_HOLDTIME);
		IIC_SCL=0;
		delay_us(IIC_HOLDTIME);
		IIC_SCL=1;
	  }
	  if(outtime++>3)	return -1;
  }
	return 0;
}

//产生IIC起始信号
int IIC_Start(void)
{	
	if(IIC_Buscheck()<0)	return -1;
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;

	delay_us(IIC_HOLDTIME);
 	IIC_SDA=0;	//START:when CLK is high,DATA change form high to low 
	delay_us(IIC_HOLDTIME);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(IIC_HOLDTIME);
	return 0;
}


int i2c1_start(u8 sla_adr)
{   
	if(IIC_Start()<0) 
		return  -1;

    // Send address
    return i2c1_write_u8(sla_adr<<1);
}

//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();	//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;	//STOP:when CLK is high DATA change form low to high
 	delay_us(IIC_HOLDTIME);
	IIC_SCL=1; 
	IIC_SDA=1;	//发送I2C总线结束信号
	delay_us(IIC_HOLDTIME);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功

int IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;
	delay_us(IIC_HOLDTIME);	   
	IIC_SCL=1;
	delay_us(IIC_HOLDTIME);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{	 
			printf("IIC NO back ACK\r\n ");
			IIC_Stop();
			return -1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(IIC_HOLDTIME);
	IIC_SCL=1;
	delay_us(IIC_HOLDTIME);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(IIC_HOLDTIME);
	IIC_SCL=1;
	delay_us(IIC_HOLDTIME);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
int IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd <<= 1; 	  
		delay_us(IIC_HOLDTIME);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(IIC_HOLDTIME); 
		IIC_SCL=0;	
		delay_us(IIC_HOLDTIME);
    }
	return 0;	 
} 

void IIC_Scl(u8 bit)   	//0 or 1
{
    IIC_SCL = bit;//拉低时钟开始数据传输
	if(I2C_SCL_CHECK() != bit)
		printf("IIC SCL error\r\n");
}

void IIC_Sda(u8 bit)   	//0 or 1
{
    IIC_SDA = bit;//拉低时钟开始数据传输
	if(I2C_SDA_CHECK() != bit)
		printf("IIC SDA error\r\n");
}

int i2c1_write_u8(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
	IIC_Scl(0);
    for(t=0;t<8;t++)
    {              
        IIC_Sda((txd&0x80)>>7);
        txd <<= 1; 	  
		delay_us(IIC_HOLDTIME);   //对TEA5767这三个延时都是必须的
		IIC_Scl(1);
		delay_us(IIC_HOLDTIME); 
		IIC_Scl(0);	
		delay_us(IIC_HOLDTIME);
    }
	IIC_Wait_Ack();
	return 0;	 
} 
	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(IIC_HOLDTIME);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(IIC_HOLDTIME); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

int i2c1_stop(void)
{
	SDA_OUT();	//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;	//STOP:when CLK is high DATA change form low to high
 	delay_us(IIC_HOLDTIME);
	IIC_SCL=1;
	IIC_SDA=1;	//发送I2C总线结束信号
	delay_us(IIC_HOLDTIME);
}

























