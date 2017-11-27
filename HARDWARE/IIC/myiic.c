#include "myiic.h"
#include "delay.h"
#include "usart.h"
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

/*************************/
#define IIC_SPEED		400
#define IIC_HOLDTIME	((1000/IIC_SPEED)+1)

//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
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

/********��������Ƿ�ռ��*********************/
int IIC_Buscheck()
{
  u8 outtime,i;
	// Check bus
  while((I2C_SCL_CHECK()==0)||(I2C_SDA_CHECK()==0))	 //READ_SDA
  {
       // Bus error
	  SDA_OUT();     //sda�����
      printf("i2c levevl is low!!\r\n");
	  IIC_SDA=1;
      for(i=0;i<9;i++)				//SDA���ߣ�SCL��������9�����帴λI2c�豸��һ��Ҫ���¸�λ�������ϵ����׳���
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

//����IIC��ʼ�ź�
int IIC_Start(void)
{	
	if(IIC_Buscheck()<0)	return -1;
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;

	delay_us(IIC_HOLDTIME);
 	IIC_SDA=0;	//START:when CLK is high,DATA change form high to low 
	delay_us(IIC_HOLDTIME);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
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

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();	//sda�����
	IIC_SCL=0;
	IIC_SDA=0;	//STOP:when CLK is high DATA change form low to high
 	delay_us(IIC_HOLDTIME);
	IIC_SCL=1; 
	IIC_SDA=1;	//����I2C���߽����ź�
	delay_us(IIC_HOLDTIME);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�

int IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
int IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd <<= 1; 	  
		delay_us(IIC_HOLDTIME);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(IIC_HOLDTIME); 
		IIC_SCL=0;	
		delay_us(IIC_HOLDTIME);
    }
	return 0;	 
} 

void IIC_Scl(u8 bit)   	//0 or 1
{
    IIC_SCL = bit;//����ʱ�ӿ�ʼ���ݴ���
	if(I2C_SCL_CHECK() != bit)
		printf("IIC SCL error\r\n");
}

void IIC_Sda(u8 bit)   	//0 or 1
{
    IIC_SDA = bit;//����ʱ�ӿ�ʼ���ݴ���
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
		delay_us(IIC_HOLDTIME);   //��TEA5767��������ʱ���Ǳ����
		IIC_Scl(1);
		delay_us(IIC_HOLDTIME); 
		IIC_Scl(0);	
		delay_us(IIC_HOLDTIME);
    }
	IIC_Wait_Ack();
	return 0;	 
} 
	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

int i2c1_stop(void)
{
	SDA_OUT();	//sda�����
	IIC_SCL=0;
	IIC_SDA=0;	//STOP:when CLK is high DATA change form low to high
 	delay_us(IIC_HOLDTIME);
	IIC_SCL=1;
	IIC_SDA=1;	//����I2C���߽����ź�
	delay_us(IIC_HOLDTIME);
}

























