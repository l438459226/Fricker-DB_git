#include "lcd.h"
#include "font.h" 
#include "usart.h"
#include "delay.h"	 
#include "stdarg.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniSTM32开发板
//2.0 
//////////////////////////////////////////////////////////////////////////////////	 
u8 fb_buffer[128];
u8 LX=0,LY=0;

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;		//0x0000;	//画笔颜色
u16 BACK_COLOR=GREEN;  //背景色 

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;
	 					    
//写寄存器函数
//data:寄存器值
void LCD_WR_REG(u16 data)
{ 
	LCD_RS_CLR;		// 写  0 地址 
	LCD_CS_CLR;  
	LCD_WR_CLR; 
	DATAOUT((u8)(data>>8)); 
	LCD_WR_SET; 

	LCD_RS_CLR;		// 写  0 地址
	LCD_WR_CLR;   
	DATAOUT((u8)data); 
	LCD_WR_SET; 
 	LCD_CS_SET;  
}
//写数据函数
//可以替代LCD_WR_DATAX宏,拿时间换空间.
//data:寄存器值
void LCD_WR_DATAX(u16 data)
{
	u8 dal;
	dal = data;
	LCD_CS_CLR;
	
	LCD_RS_SET;
	DATAOUT(data>>8);
	LCD_WR_CLR;
	LCD_WR_SET;


	LCD_RS_SET;	
	DATAOUT(dal);
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
}
//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{										   
	u16 rdata;
	u8 dah,dal;
 
 	GPIOC->CRL=0x88888888; //PC0-7  上拉输入
	GPIOC->ODR=0x00;     //全部输出0

	LCD_RD_CLR;	
	LCD_RS_SET;
	LCD_CS_CLR;
	
	//读取数据(读寄存器时,需要读2次)
	LCD_RD_SET;
	LCD_RD_CLR;	
	dah = DATAIN; 

	LCD_RD_SET;
	LCD_RD_CLR;		   
	dal = DATAIN;  
	LCD_CS_SET; 
	
	rdata = (dah<<8) | (dal);
	
	GPIOC->CRL=0x33333333; //PC0-7  上拉输出
	GPIOC->ODR=0xFF;    //全部输出高
	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	return (rdata);  
}
//写寄存器
//LCD_Reg:寄存器编号
//LCD_RegValue:要写入的值
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATAX(LCD_RegValue);	  
}   
//读寄存器
//LCD_Reg:寄存器编号
//返回值:读到的值
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);  //写入要读的寄存器号  
	return LCD_RD_DATA(); 
} 
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x0022);
} 
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r,g,b;
	b++;
	if(x>=lcddev.width||y>=lcddev.height)	return 0;	//超过了范围,直接返回		  
	
	LCD_SetCursor(x,y);
  LCD_WR_REG(R34);      		 		//其他IC发送读GRAM指令
	GPIOB->CRL=0X88888888; //PB0-7  上拉输入
	GPIOB->CRH=0X88888888; //PB8-15 上拉输入
	GPIOB->ODR=0XFFFF;     //全部输出高

	LCD_RS_SET;
	LCD_CS_CLR;	    
	//读取数据(读GRAM时,第一次为假读)	
	LCD_RD_CLR;	
  delay_us(1);//延时1us					   
	LCD_RD_SET;
 	//dummy READ
	LCD_RD_CLR;					   
	delay_us(1);//延时1us					   
 	r=DATAIN;  	//实际坐标颜色
	LCD_RD_SET;
 	 
	LCD_RD_CLR;					   
	b=DATAIN;//读取蓝色值  	  
	LCD_RD_SET;
	g=r&0XFF;//对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
	g<<=8;
	
	LCD_CS_SET;
	GPIOB->CRL=0X33333333; 		//PB0-7  上拉输出
	GPIOB->CRH=0X33333333; 		//PB8-15 上拉输出
	GPIOB->ODR=0XFFFF;    		//全部输出高  
	return LCD_BGR2RGB(r);	//其他IC
}
//LCD开启显示
void LCD_DisplayOn(void)
{	
 LCD_WriteReg(R7,0x0173); 			//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(R7, 0x00);//关闭显示 
}  

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WriteReg(0x0020, Xpos);
	LCD_WriteReg(0x0021, Ypos);
	/*
		LCD_WriteReg(0x0036, Xpos+1);		//划区域
		LCD_WriteReg(0x0037, Xpos);
		LCD_WriteReg(0x0038, Ypos+1);
		LCD_WriteReg(0x0039, Ypos);
	*/
}


//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)  	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//横屏时，对6804不改变扫描方向！
	{			   
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510)//9341/6804/5310/5510,很特殊
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510))regval|=0X08;//5310/5510不需要BGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804的BIT6和9341的反了	   
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//交换X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//交换X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}  
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}
		if(lcddev.id==0x8989)//8989 IC
		{
			dirreg=0X11;
			regval|=0X6040;	//65K   
	 	}else//其他驱动IC		  
		{
			dirreg=0X03;
			regval|=1<<12;  
		}
		LCD_WriteReg(dirreg,regval);
	}
} 
//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATAX(GREEN); 
	LCD_SetCursor(0,0);
}  	 
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
 	if(lcddev.dir==1)x=lcddev.width-1-x;//横屏其实就是调转x,y坐标
	
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATAX(color);
	LCD_SetCursor(0,0);
}
//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//竖屏
	{
		lcddev.dir=0;	//竖屏
		lcddev.width=176;
		lcddev.height=220;
		
		lcddev.wramcmd=0x0022;
	 	lcddev.setxcmd=0x0020;
		lcddev.setycmd=0x0021;  
	} 
}	 
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.
//68042,横屏时不支持窗口设置!! 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	u16 hsareg,heareg,vsareg,veareg;
	u16 hsaval,heaval,vsaval,veaval; 
	width=sx+width-1;
	height=sy+height-1;
	
	if(lcddev.dir==1)//横屏
	{
			//窗口值
			hsaval=sy;				
			heaval=height;
			vsaval=lcddev.width-width-1;
			veaval=lcddev.width-sx-1;				
		}else				//竖屏
		{ 
			hsaval=sx;				
			heaval=sx + width;
			vsaval=sy;
			veaval=sy + height;
		}
			hsareg=0x0036;heareg=0x0037;//水平方向窗口寄存器
			vsareg=0x0038;veareg=0x0039;//垂直方向窗口寄存器	  							  
		//设置寄存器值
		LCD_WriteReg(hsareg,heaval);
		LCD_WriteReg(heareg,hsaval);
		LCD_WriteReg(vsareg,veaval);
		LCD_WriteReg(veareg,vsaval);		
		LCD_SetCursor(sx,sy);	//设置光标位置
} 
//初始化lcd
//该初始化函数可以初始化各种ALIENTEK出品的LCD液晶屏
//本函数占用较大flash,用户可以根据自己的实际情况,删掉未用到的LCD初始化代码.以节省空间.
void LCD_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //使能PORTB,C时钟和AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//开启SWD，失能JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	   ///PORTC6~10复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB	

	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	//  PORTC推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC
 
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	
	LCD_RESET_SET;
	
	delay_ms(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	delay_ms(50); // delay 50 ms 
	 

//--------------Display Control Setting----------------------------------------------//
LCD_WR_REG(0x0001);
LCD_WR_DATAX(0x011C);
LCD_WR_REG(0x0002);
LCD_WR_DATAX(0x0100);
LCD_WR_REG(0x0003);
LCD_WR_DATAX(0x1030);
LCD_WR_REG(0x0008);
LCD_WR_DATAX(0x0808);
LCD_WR_REG(0x000C);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x000F);
LCD_WR_DATAX(0x0001);
LCD_WR_REG(0x0020);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0021);
LCD_WR_DATAX(0x0000);
//-----------------------------------End Display Control setting-----------------------------------------//
//-------------------------------- Power Control Registers Initial --------------------------------------//
LCD_WR_REG(0x0010);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0011);
LCD_WR_DATAX(0x1000);
//---------------------------------End Power Control Registers Initial -------------------------------//
delay_ms(100);
//----------------------------------Display Windows 176 X 220----------------------------------------//
LCD_WR_REG(0x0030);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0031);
LCD_WR_DATAX(0x00DB);
LCD_WR_REG(0x0032);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0033);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0034);
LCD_WR_DATAX(0x00DB);
LCD_WR_REG(0x0035);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0036);
LCD_WR_DATAX(0x00AF);
LCD_WR_REG(0x0037);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0038);
LCD_WR_DATAX(0x00DB);
LCD_WR_REG(0x0039);
LCD_WR_DATAX(0x0000);
//----------------------------------End Display Windows 176 X 220----------------------------------//
delay_ms(10);
LCD_WR_REG(0x00ff);
LCD_WR_DATAX(0x0003);
//-------------------------------------Gamma Cluster Setting-------------------------------------------//
LCD_WR_REG(0x0050);
LCD_WR_DATAX(0x0103);
LCD_WR_REG(0x0051);
LCD_WR_DATAX(0x0808);
LCD_WR_REG(0x0052);
LCD_WR_DATAX(0x0207);
LCD_WR_REG(0x0053);
LCD_WR_DATAX(0x2222);
LCD_WR_REG(0x0054);
LCD_WR_DATAX(0x0703);
LCD_WR_REG(0x0055);
LCD_WR_DATAX(0x0103);
LCD_WR_REG(0x0056);
LCD_WR_DATAX(0x0808);
LCD_WR_REG(0x0057);
LCD_WR_DATAX(0x0207);
LCD_WR_REG(0x0058);
LCD_WR_DATAX(0x2222);		///2222
LCD_WR_REG(0x0059);
LCD_WR_DATAX(0x0603);
//---------------------------------------End Gamma Setting---------------------------------------------//
//---------------------------------------Vcom Setting---------------------------------------------//
LCD_WR_REG(0x00B0);
LCD_WR_DATAX(0x1901);//0x1901
LCD_WR_REG(0x00B2);
LCD_WR_DATAX(0x1900);//0x1901
//---------------------------------------End Vcom Setting---------------------------------------------//
LCD_WR_REG(0x00ff);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0007);
LCD_WR_DATAX(0x1017);
LCD_WR_REG(0x0022);
delay_ms(50);


LCD_WR_REG(0x0010);
LCD_WR_DATAX(0x0000);
delay_ms(120);
LCD_WR_REG(0x0007);
LCD_WR_DATAX(0x1017);

LCD_WR_REG(0x0020);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0021);
LCD_WR_DATAX(0x0000);
LCD_WR_REG(0x0022);
LCD_WR_DATAX(0x0000);
delay_ms(50); //Delay 50ms


	LCD_Display_Dir(0);		 	//默认为竖屏
	LCD_LED=1;					//点亮背光
	LCD_Clear(GREEN);
	
	lcddev.id = LCD_ReadReg(0x0000);   
 //	printf(" LCD ID:%x\r\n",lcddev.id); //打印LCD ID 
}  		  
  
//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	u16 i,j;
	LCD_SetCursor(0,0);	//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	
  for(j=lcddev.height;j>0;j--)
		for(i=lcddev.width;i>0;i--)	
			LCD_WR_DATAX(color);
}  



//在指定区域内填充指定颜色
//区域大小:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	
	LCD_Set_Window(sx,sy,ex-sx+1,ey-sy+1);
	LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(i=sy;i<=ey;i++)
			for(j=sx;j<= ex;j++)
				LCD_WR_DATAX(color);	//设置光标位置 	    
}  
//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//得到填充的宽度
	height=ey-sy+1;			//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)
			LCD_WR_DATAX(color[i*width+j]);//写入数据 
	}	  
} 
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
}								  
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(x>=lcddev.width)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}  	 
	}  	
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{     
	u8 x0=x;
	
	y = ((y)*height);
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}


int printfkk(const char *fmt, ...)
{
	int i;
	int len;
	u8 size = 12;
	__va_list ap;	

	va_start(ap, fmt);
	len = vsprintf((char*)fb_buffer,fmt,ap);
	va_end(ap);
	for (i = 0; i < strlen((char*)fb_buffer); i++)
	{
		if(fb_buffer[i]=='\n'){LX=0;LY += size;continue;}
		if(fb_buffer[i]=='\r'){LX=0;continue;}
		if(LX>=lcddev.width){LX=0;LY += size;}	//
    if(LY>=lcddev.height)	break;	//退出
		
		LCD_ShowChar(LX,LY,fb_buffer[i],size,0);
		LX+=size/2;
	}
	return len;
}

int printfk(u8 x,u8 y,u8 size,const char *fmt, ...)
{
	
	int i;
	int len;
	__va_list ap;	

	x=x*size/2;
	y=y*size;
	va_start(ap, fmt);
	len = vsprintf((char*)fb_buffer,fmt,ap);
	va_end(ap);
	for (i = 0; i < strlen((char*)fb_buffer); i++)
	{
		if(x>=lcddev.width){x=0;y += size;}	//
    if(y>=lcddev.height)	break;	//退出
		
		LCD_ShowChar(x,y,fb_buffer[i],size,0);
		x+=size/2;
	}
	return len;
}




























