#include "lcd.h"
#include "font.h" 
#include "usart.h"
#include "delay.h"	 
#include "stdarg.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//2.0 
//////////////////////////////////////////////////////////////////////////////////	 
u8 fb_buffer[128];
u8 LX=0,LY=0;

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;		//0x0000;	//������ɫ
u16 BACK_COLOR=GREEN;  //����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	 					    
//д�Ĵ�������
//data:�Ĵ���ֵ
void LCD_WR_REG(u16 data)
{ 
	LCD_RS_CLR;		// д  0 ��ַ 
	LCD_CS_CLR;  
	LCD_WR_CLR; 
	DATAOUT((u8)(data>>8)); 
	LCD_WR_SET; 

	LCD_RS_CLR;		// д  0 ��ַ
	LCD_WR_CLR;   
	DATAOUT((u8)data); 
	LCD_WR_SET; 
 	LCD_CS_SET;  
}
//д���ݺ���
//�������LCD_WR_DATAX��,��ʱ�任�ռ�.
//data:�Ĵ���ֵ
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
//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{										   
	u16 rdata;
	u8 dah,dal;
 
 	GPIOC->CRL=0x88888888; //PC0-7  ��������
	GPIOC->ODR=0x00;     //ȫ�����0

	LCD_RD_CLR;	
	LCD_RS_SET;
	LCD_CS_CLR;
	
	//��ȡ����(���Ĵ���ʱ,��Ҫ��2��)
	LCD_RD_SET;
	LCD_RD_CLR;	
	dah = DATAIN; 

	LCD_RD_SET;
	LCD_RD_CLR;		   
	dal = DATAIN;  
	LCD_CS_SET; 
	
	rdata = (dah<<8) | (dal);
	
	GPIOC->CRL=0x33333333; //PC0-7  �������
	GPIOC->ODR=0xFF;    //ȫ�������
	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	return (rdata);  
}
//д�Ĵ���
//LCD_Reg:�Ĵ������
//LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATAX(LCD_RegValue);	  
}   
//���Ĵ���
//LCD_Reg:�Ĵ������
//����ֵ:������ֵ
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);  //д��Ҫ���ļĴ�����  
	return LCD_RD_DATA(); 
} 
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x0022);
} 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}		 
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r,g,b;
	b++;
	if(x>=lcddev.width||y>=lcddev.height)	return 0;	//�����˷�Χ,ֱ�ӷ���		  
	
	LCD_SetCursor(x,y);
  LCD_WR_REG(R34);      		 		//����IC���Ͷ�GRAMָ��
	GPIOB->CRL=0X88888888; //PB0-7  ��������
	GPIOB->CRH=0X88888888; //PB8-15 ��������
	GPIOB->ODR=0XFFFF;     //ȫ�������

	LCD_RS_SET;
	LCD_CS_CLR;	    
	//��ȡ����(��GRAMʱ,��һ��Ϊ�ٶ�)	
	LCD_RD_CLR;	
  delay_us(1);//��ʱ1us					   
	LCD_RD_SET;
 	//dummy READ
	LCD_RD_CLR;					   
	delay_us(1);//��ʱ1us					   
 	r=DATAIN;  	//ʵ��������ɫ
	LCD_RD_SET;
 	 
	LCD_RD_CLR;					   
	b=DATAIN;//��ȡ��ɫֵ  	  
	LCD_RD_SET;
	g=r&0XFF;//����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
	g<<=8;
	
	LCD_CS_SET;
	GPIOB->CRL=0X33333333; 		//PB0-7  �������
	GPIOB->CRH=0X33333333; 		//PB8-15 �������
	GPIOB->ODR=0XFFFF;    		//ȫ�������  
	return LCD_BGR2RGB(r);	//����IC
}
//LCD������ʾ
void LCD_DisplayOn(void)
{	
 LCD_WriteReg(R7,0x0173); 			//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	LCD_WriteReg(R7, 0x00);//�ر���ʾ 
}  

//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WriteReg(0x0020, Xpos);
	LCD_WriteReg(0x0021, Ypos);
	/*
		LCD_WriteReg(0x0036, Xpos+1);		//������
		LCD_WriteReg(0x0037, Xpos);
		LCD_WriteReg(0x0038, Ypos+1);
		LCD_WriteReg(0x0039, Ypos);
	*/
}


//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)  	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//����ʱ����6804���ı�ɨ�跽��
	{			   
		switch(dir)//����ת��
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
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510)//9341/6804/5310/5510,������
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510))regval|=0X08;//5310/5510����ҪBGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804��BIT6��9341�ķ���	   
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//����X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//����X,Y
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
			case L2R_U2D://������,���ϵ���
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		}
		if(lcddev.id==0x8989)//8989 IC
		{
			dirreg=0X11;
			regval|=0X6040;	//65K   
	 	}else//��������IC		  
		{
			dirreg=0X03;
			regval|=1<<12;  
		}
		LCD_WriteReg(dirreg,regval);
	}
} 
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATAX(GREEN); 
	LCD_SetCursor(0,0);
}  	 
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
 	if(lcddev.dir==1)x=lcddev.width-1-x;//������ʵ���ǵ�תx,y����
	
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATAX(color);
	LCD_SetCursor(0,0);
}
//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
		lcddev.width=176;
		lcddev.height=220;
		
		lcddev.wramcmd=0x0022;
	 	lcddev.setxcmd=0x0020;
		lcddev.setycmd=0x0021;  
	} 
}	 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height.
//68042,����ʱ��֧�ִ�������!! 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{   
	u16 hsareg,heareg,vsareg,veareg;
	u16 hsaval,heaval,vsaval,veaval; 
	width=sx+width-1;
	height=sy+height-1;
	
	if(lcddev.dir==1)//����
	{
			//����ֵ
			hsaval=sy;				
			heaval=height;
			vsaval=lcddev.width-width-1;
			veaval=lcddev.width-sx-1;				
		}else				//����
		{ 
			hsaval=sx;				
			heaval=sx + width;
			vsaval=sy;
			veaval=sy + height;
		}
			hsareg=0x0036;heareg=0x0037;//ˮƽ���򴰿ڼĴ���
			vsareg=0x0038;veareg=0x0039;//��ֱ���򴰿ڼĴ���	  							  
		//���üĴ���ֵ
		LCD_WriteReg(hsareg,heaval);
		LCD_WriteReg(heareg,hsaval);
		LCD_WriteReg(vsareg,veaval);
		LCD_WriteReg(veareg,vsaval);		
		LCD_SetCursor(sx,sy);	//���ù��λ��
} 
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ALIENTEK��Ʒ��LCDҺ����
//������ռ�ýϴ�flash,�û����Ը����Լ���ʵ�����,ɾ��δ�õ���LCD��ʼ������.�Խ�ʡ�ռ�.
void LCD_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //ʹ��PORTB,Cʱ�Ӻ�AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//����SWD��ʧ��JTAG
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	   ///PORTC6~10�����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB	

	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	//  PORTC�������
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


	LCD_Display_Dir(0);		 	//Ĭ��Ϊ����
	LCD_LED=1;					//��������
	LCD_Clear(GREEN);
	
	lcddev.id = LCD_ReadReg(0x0000);   
 //	printf(" LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID 
}  		  
  
//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	u16 i,j;
	LCD_SetCursor(0,0);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	
  for(j=lcddev.height;j>0;j--)
		for(i=lcddev.width;i>0;i--)	
			LCD_WR_DATAX(color);
}  



//��ָ�����������ָ����ɫ
//�����С:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	
	LCD_Set_Window(sx,sy,ex-sx+1,ey-sy+1);
	LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(i=sy;i<=ey;i++)
			for(j=sx;j<= ex;j++)
				LCD_WR_DATAX(color);	//���ù��λ�� 	    
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)
			LCD_WR_DATAX(color[i*width+j]);//д������ 
	}	  
} 
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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
//������
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
}								  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(x>=lcddev.width)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{     
	u8 x0=x;
	
	y = ((y)*height);
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
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
    if(LY>=lcddev.height)	break;	//�˳�
		
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
    if(y>=lcddev.height)	break;	//�˳�
		
		LCD_ShowChar(x,y,fb_buffer[i],size,0);
		x+=size/2;
	}
	return len;
}




























