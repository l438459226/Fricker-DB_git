#include "LT35825.h"
#include "myiic.h"
#include "usart.h"
#include "i2c1_bitbang.h"
#include "LT3582.h"

//Voutp = 3.2v+(Vp*50mV)+(Vplus*25mV)
//Voutn = -1.2V-(Vn*50mV)

#define TL35825_ADDR	0x45



#define VP_BASIC	50
#define VPLUS_BASIC	25

#define SET_VPLUS		(1<<5)
#define RESET_VPLUS		(~(1<<5))


#define OUTPUTS_DISABLE		0x00

#define VOUTN_UP					0x01
#define VOUTP_UP					0x02
#define VOUTPN_UP					0x02


#define RESET	(1<<5)
#define SWON 	(1<<4)
#define SWOFF (~(1<<4))
#define LOAD_REG	(0x07)


u8 TL35825_Write(u8 reg,u8 date)
{
	u8 ret=0;
	IIC_Start();	
 	IIC_Send_Byte(TL35825_ADDR<<1);   // 写- 地址最低位为0
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   	
	IIC_Wait_Ack();  
  IIC_Send_Byte(date);  	
	ret=IIC_Wait_Ack();
	if(ret)	return 255;  
	IIC_Stop();					
	return ret; 
}

		  
u8 TL35825_Read(u8 reg)
{
	u8 date = 0x5a;
 	IIC_Start();	
 	IIC_Send_Byte(TL35825_ADDR<<1);  
	IIC_Wait_Ack();
 	IIC_Send_Byte(reg);   	
	IIC_Wait_Ack();  
 	IIC_Start();  	 	   
	IIC_Send_Byte((TL35825_ADDR<<1)|0x01);   //读 - 地址最低位为1    写- 地址最低位为0
	IIC_Wait_Ack();	

  date = IIC_Read_Byte(1); 
  
  IIC_Stop();
	return date;
}

void Set_Voutp(float voltage)//3.2  - 12.75  6.75
{
	u8 vp,vplus;
	u16 value;
	u8 temp;
	
	value = (voltage-3.2)*1000;
	vp = value/VP_BASIC;
	printf("vp:0x%x  ",vp);
	
	TL35825_Write(TL35825_REG0,vp);
	delay_us(100);
	temp = TL35825_Read(TL35825_REG0);
	printf("temp:0x%x\r\n",temp);
	
	
	temp = TL35825_Read(TL35825_REG2);
	printf("TL35825_REG21:0x%x\r\n",temp);
	if(value%VP_BASIC){
		vplus = TL35825_Read(TL35825_REG2)|SET_VPLUS;
		TL35825_Write(TL35825_REG2,vplus);	//
	}else{
		vplus = TL35825_Read(TL35825_REG2)&RESET_VPLUS;
		TL35825_Write(TL35825_REG2,vplus);
	}
	temp = TL35825_Read(TL35825_REG2);
	printf("TL35825_REG22:0x%x\r\n",temp);
}

void lt35825_init(void)
{
	u8 data;
	
	TL35825_Write(TL35825_CMDR,0x70);
	delay_ms(10);
	TL35825_Write(TL35825_CMDR,0x57);
	delay_ms(10);
	data = TL35825_Read(TL35825_CMDR);
	printf("data=%d \r\n",data);
}


void Set_Voutn(float voltage)
{
	u8 vn;
	short value;
	u8 temp;
	
	value = (voltage+1.2)*1000;
	vn = 0-(value/50);
	printf("vn:0x%x  ",vn);
	TL35825_Write(TL35825_REG1,vn);
	delay_ms(100);
	temp = TL35825_Read(TL35825_REG1);
	temp = TL35825_Read(TL35825_REG1);
	printf("temp:0x%x\r\n",temp);
}


void Voutp_Upfirst(void)
{
	TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)|SWON);

	TL35825_Write(TL35825_REG2,((TL35825_Read(TL35825_REG2)&(~0x03))|VOUTP_UP));
	
	TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)&SWOFF);
}

void Voutn_Upfirst(void)
{
	TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)|SWON);

	TL35825_Write(TL35825_REG2,((TL35825_Read(TL35825_REG2)&(~0x03))|VOUTN_UP));
	
	TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)&SWOFF);
}

void TL35825_reset()
{
	TL35825_Write(TL35825_CMDR,0x20);//复位芯片
}

void TL35825_init(void)
{
	TL35825_reset();	//	
	delay_ms(1);
	TL35825_Write(TL35825_CMDR,0x07);
}

void Voutn_UpBoth(void)
{

	TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)|SWON);

	TL35825_Write(TL35825_REG2,((TL35825_Read(TL35825_REG2)&(~0x03))|VOUTPN_UP));
	
	TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)&SWOFF);
}


void Vout_Off(void)
{
	u8 data;
	data = TL35825_Read(TL35825_REG2);
	data = (data&0xf8)|(OUTPUTS_DISABLE);
	printf("Vout_Off:0x%x",data);
	TL35825_Write(TL35825_REG2,data);
}


