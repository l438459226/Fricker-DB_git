#include "LT35825.h"
#include "myiic.h"
#include "usart.h"
//#include "i2c1_bitbang.h"
#include "LT3582.h"

//Voutp = 3.2v+(Vp*50mV)+(Vplus*25mV)
//Voutn = -1.2V-(Vn*50mV)  -1200-vn*50

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
	ret = IIC_Wait_Ack();
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

	date = IIC_Read_Byte(); 

	IIC_Stop();
	return date;
}

void Set_Voutp(float voltage)//3.2  - 12.75  6.75
{
	u8 vp;
	u16 value;
	u8 vplus;
	float VOUTP;
	
	value = (u16)((voltage-3.2)*1000);
	vp = value/VP_BASIC;
	
	printf("vp:0x%x  \r\n",vp);
	//TL35825_Write(TL35825_CMDR,0x07);	//Voutp = 3.2v+(Vp*50mV)+(Vplus*25mV)
	TL35825_Write(TL35825_REG0,vp);
	//delay_ms(100);
	
	printf("REG0:0x%x\r\n",TL35825_Read(TL35825_REG0));
	printf("REG2:0x%x\r\n",TL35825_Read(TL35825_REG2));
	
	if(value%VP_BASIC){
		TL35825_Write(TL35825_REG2,(TL35825_Read(TL35825_REG2)|SET_VPLUS));
		vplus = 1;//
	}else{
		TL35825_Write(TL35825_REG2,TL35825_Read(TL35825_REG2)&RESET_VPLUS);
		vplus = 0;
	}
	VOUTP = 3200 + (vp * 50) + (vplus * 25);
	printf("VSP输出电压：%f\r\n",(VOUTP/1000));
}

void Set_Voutn(float voltage)
{
	u8 vn;
	u16 value;
	u8 temp;
	float VOUTN;
	
	//value = (voltage*1000)+1200;//0-(voltage+1.2)*1000;
	value = (u16)(0-(voltage+1.2)*1000);
	
	//printf("value:0x%x  \r\n",value);
	vn = (value/50);
	if(value%50)	vn +=1;
	printf("vn:0x%x  \r\n",vn);
	
	TL35825_Write(TL35825_REG1,vn);
	//delay_ms(100);
	//temp = TL35825_Read(TL35825_REG1);
	temp = TL35825_Read(TL35825_REG1);
	printf("REG1:0x%x\r\n",temp);
	
	VOUTN = (-1200) - (vn * 50);
	printf("VSN输出电压：%f\r\n",(VOUTN/1000));
}


void Voutp_Upfirst(void)
{
	TL35825_Write(TL35825_CMDR,0x17);
	TL35825_Write(TL35825_REG2,((TL35825_Read(TL35825_REG2)&(~0x03))|VOUTP_UP));
	TL35825_Write(TL35825_CMDR,0x07);
	
	printf("cmdr:0x%x\r\n",TL35825_Read(TL35825_REG1));
}

void Voutn_Upfirst(void)
{
	TL35825_Write(TL35825_CMDR,0x17);
	TL35825_Write(TL35825_REG2,((TL35825_Read(TL35825_REG2)&(~0x03))|VOUTN_UP));
	TL35825_Write(TL35825_CMDR,0x07);
}

void TL35825_reset()
{
	TL35825_Write(TL35825_CMDR,0x70);//复位芯片
}

void TL35825_init(void)
{
	TL35825_reset();	//	
	delay_ms(1);
	TL35825_Write(TL35825_CMDR,0x57);
}

void Voutn_UpBoth(void)
{
	
	//TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)|SWON);
	TL35825_Write(TL35825_CMDR,0x17);
	TL35825_Write(TL35825_REG2,((TL35825_Read(TL35825_REG2)&(~0x03))|VOUTPN_UP));
	TL35825_Write(TL35825_CMDR,0x07);
	//TL35825_Write(TL35825_CMDR,TL35825_Read(TL35825_CMDR)&SWOFF);
}


void Vout_Off(void)
{
	TL35825_Write(TL35825_CMDR,0x17);
	TL35825_Write(TL35825_REG2,((TL35825_Read(TL35825_REG2)&(~0x03))|OUTPUTS_DISABLE));
	TL35825_Write(TL35825_CMDR,0x07);
}


