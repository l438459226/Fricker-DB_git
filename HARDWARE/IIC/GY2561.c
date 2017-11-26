#include "GY2561.h" 
#include "delay.h"
#include "math.h"
#include "usart.h"
#include "ssd2828.h"
#include "lcd.h"
#include "lcmburn.h"
//Mini STM32开发板
//正点原子@ALIENTEK
//2010/6/10
//V1.2


u8 GY2561_WR(u8 reg,u8 date)
{
	u8 ret=0;
	IIC_Start();	
 	IIC_Send_Byte(SLAVE_ADDR_WR);   	//发送写命令 	 
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   	//发送reg
	IIC_Wait_Ack();  
  IIC_Send_Byte(date);  	//发数据
	ret=IIC_Wait_Ack();
	if(ret)	return 255;  
	IIC_Stop();					//产生一个停止条件	    
	return ret; 
}

//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
u8 GY2561_RD(u8 reg)
{
	u8 date = 0x5a;
 	IIC_Start();	
 	IIC_Send_Byte(SLAVE_ADDR_WR);   //发送写命令 	 
	IIC_Wait_Ack();
 	IIC_Send_Byte(reg);   	//发送高8位地址
	IIC_Wait_Ack();  
 	IIC_Start();  	 	   
	IIC_Send_Byte(SLAVE_ADDR_RD);   //发送读命令		   
	IIC_Wait_Ack();	

  date = IIC_Read_Byte(1); //发数据	
  
  IIC_Stop();//产生一个停止条件   
	return date;
} 

void GY_setpowerup(void)
{
	GY2561_WR(CONTROL,0x03);
}
void GY_setpowerdown(void)
{
	GY2561_WR(CONTROL,0x00);
}

char GY_setTiming(u8 gain, u8 time)
	// If gain = false (0), device is set to low gain (1X)
	// If gain = high (1), device is set to high gain (16X)
	// If time = 0, integration will be 13.7ms
	// If time = 1, integration will be 101ms
	// If time = 2, integration will be 402ms
	// If time = 3, use manual start / stop
{
	u8 timing;
	
	timing = GY2561_RD(TIMING);
	// Get timing byte
		// Set gain (0 or 1)
		if (gain)
			timing |= 0x10;			//device is set to low gain (16X)
		else
			timing &= ~0x10;		//device is set to low gain (1X)

		// Set integration time (0 to 3)
		timing &= ~0x03;
		timing |= (time & 0x03);

		// Write modified timing byte back to device
		GY2561_WR(TIMING,timing);
		
		return 0;
}


void GY_manualStart(void)
{
	u8 timing;
	timing = GY2561_RD(TIMING);
	timing |= 0x03;
	GY2561_WR(TIMING,timing);
	timing |= 0x08;
	GY2561_WR(TIMING,timing);
}

void GY_manualStop(void)
{
	u8 timing;
	timing = GY2561_RD(TIMING);
	timing &= ~0x08;
	GY2561_WR(TIMING,timing);
}

void GY_setInterruptControl(u8 control,u8 persist)
	// If control = 0, interrupt output disabled
	// If control = 1, use level interrupt, see setInterruptThreshold()
	// If persist = 0, every integration cycle generates an interrupt
	// If persist = 1, any value outside of threshold generates an interrupt
	// If persist = 2 to 15, value must be outside of threshold for 2 to 15 integration cycles
{
	GY2561_WR(GYINIT,((control | 0x03) << 4) & (persist | 0x0F));
}
void GY_setInterruptThreshold(u16 low, u16 high)		
	// Set interrupt thresholds (channel 0 only)
	// low, high: 16-bit threshold values
	// Returns true (1) if successful, false (0) if there was an I2C error
	// (Also see getError() below)
{
	// Write low and high threshold values
	GY2561_WR(DATA0LOW,low);
	GY2561_WR(DATA0HIGH,(low>>8));	//

	GY2561_WR(DATA1LOW,high);
	GY2561_WR(DATA1HIGH,(high>>8));	//
}
u16 Read_Light(u8 Channel)
{
	u8 DataLow = 0,DataHigh = 0;
	u16 light;
	if(Channel == 1){
		DataLow = GY2561_RD(DATA1LOW);
		DataHigh = GY2561_RD(DATA1HIGH);
	}else if(Channel == 0){
		DataLow = GY2561_RD(DATA0LOW);
		DataHigh = GY2561_RD(DATA0HIGH);
	}
	light = 256*DataHigh + DataLow;
	return light;
}

double getLux(u8 gain, u16 ms)	//16x
{
		double ratio, d0, d1,lux;
		u16 CH0, CH1;
		
		CH0 = Read_Light(0);
		CH1 = Read_Light(1);
		//printf("ch0:%d ch1:%d\r\n",CH0,CH1);
	
		if ((CH0 == 0xFFFF) || (CH1 == 0xFFFF))
		{
			lux = 0.0;
			return(0.0);
		}
		
		d0 = (double)CH0; 
		d1 = (double)CH1;
		ratio = d1 / d0;
		d0 *= (402.0/ms);
		d1 *= (402.0/ms);		
		if (gain)
		{
			d0 /= 16;
			d1 /= 16;
		}
		
		
	//printf("d0:%lf d1:%lf ratio:%lf\r\n",d0,d1,ratio);
	if (ratio < 0.5)
	{
		lux = 0.0304 * d0 - 0.062 * d0 * pow(ratio,1.4);
		return(lux);
	}

	if (ratio < 0.61)
	{
		lux = 0.0224 * d0 - 0.031 * d1;
		return(lux);
	}

	if (ratio < 0.80)
	{
		lux = 0.0128 * d0 - 0.0153 * d1;
		return(lux);
	}

	if (ratio < 1.30)
	{
		lux = 0.00146 * d0 - 0.00112 * d1;
		return(lux);
	}

	lux = 0.0;
	return(0.0);
}



//初始化GT9147触摸屏
//返回值:0,初始化成功;1,初始化失败 
void GY2561_Init(void)
{
	IIC_Init();      	//初始化电容屏的I2C总线  
 	delay_ms(2);

	GY2561_WR(CONTROL,0x03);
	GY2561_WR(TIMING,0x00);
	GY_setTiming(1,0);
	GY2561_WR(GYINIT,0x10);
	
 	delay_ms(100);
}

