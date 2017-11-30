#include "myiic.h"
#include "delay.h"
#include "usart.h"
#include "i2c1_bitbang.h"
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
#define IIC_SPEED		100
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
	
	IIC_Send_Byte(0xff);	
	IIC_Stop();
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
	
	SDA_OUT();     //sda线输出
	IIC_SDA=1;
	IIC_SCL=1;
	delay_us(IIC_HOLDTIME);
	// Check bus
  while((READ_SCL==0)||(READ_SDA==0))	 //READ_SDA
  {
    for(i=0;i<9;i++)				//SDA拉高，SCL连续发送9个脉冲复位I2c设备，一定要重新复位，否则上电容易出错
	  {
			IIC_SCL=0;
			delay_us(IIC_HOLDTIME);
			IIC_SCL=1;
			delay_us(IIC_HOLDTIME);
	  }
	  if(outtime++>3)	
		{
			printf("i2c levevl is low!!\r\n");
			return -1;
		}
  }
	return 0;
}

//产生IIC起始信号
int IIC_Start(void)
{	

	if(IIC_Buscheck()<0)	
		return -1;
	
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
    return i2c1_write_u8(sla_adr);
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

	if(IIC_Buscheck()<0)
		printf("IIC_Stop error\r\n");

}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功

int IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	delay_us(IIC_HOLDTIME);	   
	IIC_Scl(1);
	//delay_us(IIC_HOLDTIME);	 
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
	SDA_OUT();	//BUG
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//不产生ACK应答
void IIC_NAck(void)//
{
	IIC_SCL=0;
	SDA_OUT();
	delay_us(IIC_HOLDTIME);
	IIC_SCL=1;
	delay_us(IIC_HOLDTIME);
	IIC_SCL=0;
}
//产生ACK应答		    
void IIC_Ack(void)//
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
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
	IIC_SCL = bit;
	
	if(READ_SCL != bit)
		printf("IIC SCL error\r\n");
}

void IIC_Sda(u8 bit)   	//0 or 1
{
	
  IIC_SDA = bit;//拉低时钟开始数据传输
	IIC_SDA = bit;


	if(READ_SDA != bit)
	{
		printf("IIC SDA error :%d\r\n",bit);
	}
}


void i2c1_init(void)
{
	IIC_Init();
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
		delay_us(20);	//对TEA5767这三个延时都是必须的IIC_HOLDTIME
		IIC_Scl(1);
		delay_us(20); 
		IIC_Scl(0);	
		delay_us(20);
  }
	
	if(IIC_Wait_Ack()<0)
		return -1;

	return 0;	 
} 
	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(void)
{
	unsigned char i,receive = 0;
	
	SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(IIC_HOLDTIME);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
					receive++;   
				delay_us(IIC_HOLDTIME); 
    }					 

    IIC_Ack(); //发送ACK   
		SDA_OUT();
    return receive;
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte_no_ack(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(IIC_HOLDTIME);
				IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
					receive++;   
				delay_us(IIC_HOLDTIME); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
		SDA_OUT();
    return receive;
}

int i2c1_stop(void)
{
	SDA_OUT();	//sda线输出
	IIC_Scl(0);
	IIC_Sda(0);
	//STOP:when CLK is high DATA change form low to high
 	delay_us(IIC_HOLDTIME);
	IIC_Scl(1);
	delay_us(IIC_HOLDTIME);
	IIC_Sda(1);	//发送I2C总线结束信号
	delay_us(IIC_HOLDTIME);
	return 0;
}


int i2c1_start_repeat(u8 sla_adr)
{		
	
    return i2c1_start(sla_adr);
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 iic_read_byte(void)
{
	
	unsigned char i,receive = 0;
	
	SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
        IIC_Scl(0);
        delay_us(IIC_HOLDTIME);
				IIC_Scl(1);
        receive <<= 1;
				if(READ_SDA)
						receive++;				
				delay_us(IIC_HOLDTIME); 
    }					 

    IIC_Ack(); //发送ACK 
		SDA_OUT();
    return receive;
}

int i2c1_read_u8(u8 *data, u8 nak)
{
    u8	i;
    u8	t = 12;

		SDA_IN();	//SDA设置为输入
		for(i=0;i<8;i++ )
		{
					IIC_Scl(0);
					delay_us(IIC_HOLDTIME);
					IIC_Scl(1);
					*data <<= 1;
					delay_us(IIC_HOLDTIME);
					while(t--)
					{
						if(READ_SDA)
							*data |= 0x01; 
					}						
					delay_us(IIC_HOLDTIME); 
		}		

    // Send ACK/NAK
		if (!nak)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK 
		SDA_OUT();
		
    return 0;
}


int i2c1_read_data(u8 *data, u32 nr_of_bytes, u8 nak_last_byte)
{
    while(nr_of_bytes != 0)
    {
        if((nr_of_bytes == 1) && (nak_last_byte))
        {
            if(i2c1_read_u8(data, 1))
            {
                // Error
                return -1;
            }
        }
        else
        {
            if(i2c11__read_u8(data, 0))
            {
                // Error
                return -1;
            }
        }
        
        // Next byte
        data++;
        nr_of_bytes--;
    }
    return 0;
}


/*******************************************************************************
* 文件名	     : i2c1_Detect  检测IIC总线器件
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void i2c1_Detect(void)
{
	int retval;
	u8 i = 0;
	
	TESTER_MAIN_DEBUG("Scan i2c address:\r\n");	
	for(i=0x30;i<0x7F;i++)
	{
		retval = i2c1_start(i<<1);
		if(retval >= 0)
		{
			TESTER_MAIN_DEBUG("i2c address = 0x%x detected.\r\n",i);
		}
		i2c1_stop();
		//Delay_ms(10);
	}
}


















