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
u8 iic_buscheck()
{
  u8 outtime;
	// Check bus
  while((I2C_SCL_CHECK()==0)||(I2C_SDA_CHECK()==0))	 //READ_SDA
  {
       // Bus error
      printf("i2c levevl is low!!\r\n");
      IIC_Send_Byte(0xff);				//������������Իָ�����	  reset����
	  IIC_Stop();
	  if((outtime++)>3)	
	  	return 255;
  }
	return 0;
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{	
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	iic_buscheck();

	delay_us(IIC_HOLDTIME);
 	IIC_SDA=0;	//START:when CLK is high,DATA change form high to low 
	delay_us(IIC_HOLDTIME);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
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

u8 IIC_Wait_Ack(void)
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
			IIC_Stop();
			return 1;
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
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
				delay_us(IIC_HOLDTIME);   //��TEA5767��������ʱ���Ǳ����
				IIC_SCL=1;
				delay_us(IIC_HOLDTIME); 
				IIC_SCL=0;	
				delay_us(IIC_HOLDTIME);
    }	 
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



























