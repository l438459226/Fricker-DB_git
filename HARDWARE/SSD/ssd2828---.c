#include "ssd2828.h"
#include "ssd2828l.h"
#include "usart.h"
#include "spi.h"			


unsigned int Lcd_DCLK = 135;	  //DCLKƵ������ ��λΪMHz ���ֵ150MH����
unsigned int Lcd_LCDH = 1080;	  //�ֱ���.��
unsigned int Lcd_LCDV = 1920;	  //�ֱ���.��
unsigned int Lcd_HBPD = 100;
unsigned int Lcd_HFPD = 100;
unsigned int Lcd_HSPW = 10;
unsigned int Lcd_VBPD = 7;
unsigned int Lcd_VFPD = 9;
unsigned int Lcd_VSPW = 2;
unsigned int Lcd_DCLK_EDGE = 1;		//lcd��������ʱ��DCLK����	 ��������ĺ궨������ֵ
unsigned int Lcd_HSYNC_POLARITY = 0;  //HSYNC���弫��			  	 ��������ĺ궨������ֵ
unsigned int Lcd_VSYNC_POLARITY = 0;  //VSYNC���弫��			  	 ��������ĺ궨������ֵ
unsigned int Lcd_DE_POLARITY = 0;	 //DE�ź���Ч��ƽ			 ��������ĺ궨������ֵ

unsigned int Lcd_MODE = 0X00;  //�ӿڷ�ʽ MIPI_8LANE �� MIPI_4LANE	 ��������ĺ궨������ֵ
                               //1,2,3,4LANE�Ķ���ֵΪMIPI_4LANE	
															 
/*----------------------------------------------------------------------------*/
void SSD2828GPIOConfig(void)	//PA3-SPI_CS    PA5-SPI_CLK   PA6-MISO   PA7-MOSI
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //ʹ��PORTA,��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	   ///PORTC 3 5 6 7 �����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	   ///PORTC 3 5 6 7 �����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //???????
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA6
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
	SSD_CS = 1;
	SPI_SDI = 1;
	SPI_SCLK = 0;
	SSD_RESET = 1;
	
}

void SSD2828GPIOFree(void)	//PA3-SPI_CS    PA5-SPI_CLK   PA6-MISO   PA7-MOSI
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //ʹ��PORTA,��AFIOʱ��
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTA 3 5 6 7 �����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				//GPIOA	
	
	SSD_CS =		 0;
	SPI_SDI = 	 0;
	SPI_SCLK =	 0;
	SSD_RESET =  0;
	
}


void W_byte(unsigned char cmd)
{   
		u8 i;
	
		SPI_SDI = 0;
		SPI_SCLK = 0;
    for (i=0;i<8;i++)
    {
			SPI_SCLK = 0;
			//delay_us(1);
			
			if((cmd & 0x80) != 0)	SPI_SDI = 1;
				else  SPI_SDI = 0;
			delay_us(1); 
			SPI_SCLK = 1;
			//delay_us(1); 
			cmd = cmd<<1;
    }  
		SPI_SDI = 0;
		SPI_SCLK = 0;
}

u8 SSD2828Read(void)
{
	u8 i;
	u8 tmp = 0;
	
	SPI_SCLK = 0;
  for(i = 0; i < 8; i++)
	{
		tmp <<= 1;
		SPI_SCLK = 0;
		//delay_us(1);
    SPI_SCLK = 1;
		//delay_us(1);
		if(SPI_SDO) tmp |= 0x01;
		if(SPI_SDO) tmp |= 0x01;
		if(SPI_SDO) tmp |= 0x01;
		//delay_us(10);
	}
	SPI_SCLK = 0;
	return tmp;
}

void ssd_wparme(u8 cmd,u8 dat1,u8 dat2)//cmd=0x70д�Ĵ���     cmd=0x72д����ָ��
{
	SSD_CS = 0;
	W_byte(cmd);	//д�Ĵ���  W_REG
	W_byte(dat1);
	W_byte(dat2);
	SSD_CS = 1;
	//delay_us(10);
}

void ssd_wuparme(u8 cmd,u16 parmer)		//cmd=0x70д�Ĵ���     cmd=0x72д����ָ��
{
	SSD_CS = 0;
	W_byte(cmd);	//д�Ĵ���  W_REG
	W_byte(parmer>>8);
	W_byte(parmer);
	SSD_CS = 1;
	//delay_us(10);
}

void ssd_wdata(u8 reg,u16 parmer)		//
{
	
	SSD_CS = 0;
	W_byte(0x70);	//д�Ĵ���  W_REG
	W_byte(0x00);
	W_byte(reg);
	SSD_CS = 1;
	//delay_us(10);
	
	SSD_CS = 0;
	W_byte(0x72);	//д����ָ��
	W_byte(parmer>>8);
	W_byte(parmer);
	SSD_CS = 1;
	//delay_us(10);
}



void W_REG(u8 reg,u8 dat_h,u8 dat_l)
{

	SSD_CS = 0;
	W_byte(0x70);	//д�Ĵ���  W_REG
	W_byte(0x00);
	W_byte(reg);
	SSD_CS = 1;
	//delay_us(10);
	
	SSD_CS = 0;
	W_byte(0x72);	//д����ָ��
	W_byte(dat_h);
	W_byte(dat_l);
	SSD_CS = 1;
	
	//delay_us(10);
}

void Write_SSPI_REG(u8 reg,u16 index)
{
		u8 dat_h;
		u8 dat_l;
		dat_h = index>>8;
		dat_l = index;
		W_REG(reg,dat_h,dat_l);
}



u16 ssd_rdata(u8 reg)//
{
	u16 parmer;
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 0;
	
	W_byte(0x70);	//д�Ĵ���
	W_byte(0x00);
	W_byte(reg);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 1;
	delay_us(1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 0;
	
	W_byte(0x73);	//������ָ��
	//delay_us(10);
	parmer = SSD2828Read();
	parmer = (parmer<<8)|SSD2828Read();

	GPIO_SetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 1;
	//delay_us(10);
	return parmer;
}

void SSD_mode(u8 vmode,u8 mode)
{
		u16 reg;
	
	/*
		reg = ssd_rdata(0x00b6);
		reg &= 0xFFF9;
	
		if(vmode == VPULS)	reg |= SSD2828_VIDEO_MODE_NON_BURST_WITH_SYNC_PULSES;	//bit2-3=0
		if(vmode == VEVEN)	reg |= SSD2828_VIDEO_MODE_NON_BURST_WITH_SYNC_EVENTS;	//bit2-3=1
		if(vmode == VBRUST)	reg |= SSD2828_VIDEO_MODE_BURST;
		//Write_SSPI_REG(0xb6,reg);*/
	
	
		reg = ssd_rdata(0x00b7);
		if(mode == LP)	reg &= (~SSD2828_CFGR_HS);	//bit0=0
		if(mode == HS)	reg |= SSD2828_CFGR_HS;	//bit0=1
		if(mode == VD)	reg |= SSD2828_CFGR_VEN;
		
		Write_SSPI_REG(0xb6,reg);
}

//DCS �̰�д �� 
//DT = 0x05, 0x15
//n - ���ݸ����� 
void DCS_SP(unsigned char n)
{
	/*
	if(mode == LP)
	{	
		ssd_wdata(0x00b7,0x0250);	  //EOT Packet Enable,ECC CRC Check Enable, DCS, Short packer, LP
	}
	else if(mode == HS)
	{
		ssd_wdata(0x00b7, (0x50&0XEF)|0x0203); //EOT Packet Enable,ECC CRC Check Enable, DCS, Short packer, HS
	} 
	else if(mode == VD)
	{
		ssd_wdata(0x00b7, (0x50&0XEF)|0x030B); //EOT Packet Enable,ECC CRC Check Disable, DCS, Short packer, HS Video
	}
	*/
	delay_us(10);
	ssd_wdata(0xbc,n);
	ssd_wdata(0xbd,0x0000);
	ssd_wdata(0xbe,n);
}


void DCS_LP(unsigned long n)
{
	/*
	if(mode == LP)
	{
		ssd_wdata(0x00b7,0x0650);  //EOT Packet Enable,ECC CRC Check Enable, DCS Long Write, LP
	}
	else if(mode == HS)
	{
		ssd_wdata(0x00b7,0x0650 & (0XEF|0X03));  //EOT Packet Enable,ECC CRC Check Enable, DCS Long Write, HS
	}
	else if(mode == VD)
	{
		ssd_wdata(0x00b7, 0x0750 & (0XEF|0X0B));  //EOT Packet Enable,ECC CRC Check Disable, DCS Long Write, HS Video
	}*/

	delay_us(10);
	ssd_wdata(0xbc,n);
	ssd_wdata(0xbd,n>>16);
	ssd_wdata(0xbe,0x0fff);
}


void GP_SP(unsigned char n)
{
	/*
	if(mode == LP)
	{
		ssd_wdata(0x00b7,0x0210); //EOT Packet Enable,ECC CRC Check Enable, Generic Short Write, LP
	}
	else if(mode == HS)
	{
		ssd_wdata(0x00b7,0x0210&(0XEF|0X03));  //EOT Packet Enable,ECC CRC Check Enable, Generic Short Write, HS
	}
	else if(mode == VD)
	{
		ssd_wdata(0x00b7,0x0310&(0XEF|0X0B));  //EOT Packet Enable,ECC CRC Check Disable, Generic Short Write, HS Video
	}*/

	delay_us(10);
	ssd_wdata(0xbc,n);
	ssd_wdata(0xbd,0x0000);
	ssd_wdata(0xbe,n);
}


void GP_LP(unsigned long n)
{
	/*
  if(mode == LP)
	{	
		ssd_wdata(0xb7,0x0610); //EOT Packet Enable,ECC CRC Check Enable, Generic Long Write, LP; 
	}
	else if(mode == HS)
	{
		ssd_wdata(0xb7,0x0610&(0XEF|0X03)); //EOT Packet Enable,ECC CRC Check Enable, Generic Long Write, HS;
	}
	else if(mode == VD)
	{
		ssd_wdata(0xb7,0x0710&(0XEF|0X0B));//EOT Packet Enable,ECC CRC Check Disable, Generic Long Write, HS Video
	}*/
	
	delay_us(10);
	ssd_wdata(0xbc,n);
	ssd_wdata(0xbd,n>>16);
	ssd_wdata(0xbe,0x0fff);
}

u8 DCS_Short_Read_NP(u16 cmd, u8 cnt, u8 *val)
{
	u16 state;
	u8 i;
	u8 timeout_cnt=0;
	u8 hdat;
	u8 dath,datl;
	do
	{	
		ssd_wdata(0xbb,0x0008);			//PL clock
		ssd_wdata(0xc1,cnt);		//Maximum Return Size
		ssd_wdata(0xc0,0x0001);		//ȡ��SSD2828�Ĳ�������
		ssd_wdata(0xBC,0x0001);
		ssd_wdata(0xBF,cmd);	   //��Ҫ�x�ĵ�ַ�l�ͳ�ȥ
		//delay_us(50);					   //�x��Ҫһ���ʱ��
		state = ssd_rdata(0xc6);		//�� ssd2828 ��״̬�Ĵ���
		if(state & 0x01)
				break;    //�x�ɹ� ����ѭ�h
		else 
			if(++timeout_cnt > 10)
					return MIPI_READ_FAIL;		//���r �xʧ��
	}while(1);
	
	ssd_wuparme(0x70,0x00FF);			   //�x2828��0XFF�Ĵ�����ʵ���ϲ��Ǹ��Ĵ�����ֻ�Ǽ����LCM���������ݻ����0XFF
	for(i = 0; i < cnt;i++)
	{
		SSD_CS = 0;
		W_byte(0x73);	//������ָ��
		
	  hdat = SSD2828Read();
		if(hdat == 0xff)	continue;
		//*val++ = SSD2828Read();
		datl = SSD2828Read();
		*val++ = datl;
		*val++ = hdat;
		SSD_CS = 1;
		//delay_us(5);
	}
	return MIPI_READ_SUCCEED;
}

unsigned int GP_R(uint8_t adr, uint16_t l, uint8_t *p)
{
	uint16_t state;
	u8 hdat,datl;
	unsigned int i;
	unsigned int timeout_cnt=0;
	do
	{

		ssd_wdata(0x00bb,0x0008);			//PL clock
		ssd_wdata(0x00c1,l);		//Maximum Return Size
		ssd_wdata(0x00c0,0x0001);		//ȡ��SSD2828�Ĳ�������
		ssd_wdata(0x00BC,0x0001);		 // *�������� 
		ssd_wdata(0x00BF, adr);	   //��Ҫ�x�ĵ�ַ�l�ͳ�ȥ
		//delay_ms(10);					   //�x��Ҫһ���ʱ��
		state = ssd_rdata(0xc6);		//�� ssd2828 ��״̬�Ĵ���

		if(state & 0x01)	break;     //�x�ɹ� ����ѭ�h
		else if(++timeout_cnt > 10){return MIPI_READ_FAIL;}		//���r �xʧ��
	}while(1);	//ssd2828��״̬���� ��ȫ������

	ssd_wuparme(0x70,0x00FF);				   //�x2828��0XFF�Ĵ�����ʵ���ϲ��Ǹ��Ĵ�����ֻ�Ǽ����LCM���������ݻ����0XFF
	//printf("0x%X:",adr);
	for(i = 0; i < l;i++)
	{
		SSD_CS = 0;
		W_byte(0x73);	//������ָ��
		
	  hdat = SSD2828Read();
		if(hdat == 0xff)	continue;
		datl = SSD2828Read();
		*p++ = datl;
		//printf(",0x%X",datl);
		//printf(",0x%X",hdat);
		*p++ = hdat;
		SSD_CS = 1;
		delay_us(5);
	}

	return MIPI_READ_SUCCEED;
}


void DCS_Short_Write_1P(u8 Generic,u8 Parma)	
{
	DCS_SP(2);
	ssd_wdata(0xBF,(Parma<<8)|(Generic));
	
}
void DCS_Long_Write_2P(u8 Generic,u8 Parma1,u8 Parma2)
{
	DCS_SP(3);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,0x00,Parma2);
}
void DCS_Long_Write_3P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3)
{
	DCS_SP(4);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
}
void DCS_Long_Write_4P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4)
{
	DCS_SP(5);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,0x00,Parma4);
}
void DCS_Long_Write_5P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5)
{
	DCS_SP(6);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,Parma5,Parma4);
}
void DCS_Long_Write_6P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6)
{
	DCS_SP(7);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,Parma5,Parma4);
	ssd_wparme(0x72,0x00,Parma6);
}
void DCS_Long_Write_7P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6,u8 Parma7)
{
	DCS_SP(8);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,Parma5,Parma4);
	ssd_wparme(0x72,Parma7,Parma6);
}

void DCS_Long_Write_FIFO(u16 num,u16 *P)
{
	u16 i;
	
	DCS_LP(num);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<num;){
		if((i+1) == num)
			ssd_wparme(0x72,0x00,P[i]);
		else
			ssd_wparme(0x72,P[i+1],P[i]);
		i += 2;
	}
}


void DCS_Long_Write_8FIFO(u16 num,u8 *P)
{
	u16 i;
	
	DCS_LP(num);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<num;){
		if((i+1) == num)
			ssd_wparme(0x72,0x00,P[i]);
		else
			ssd_wparme(0x72,P[i+1],P[i]);
		i += 2;
	}
	
	//Write_SSPI_REG(0xB7, 0x0159);
}


void Generic_Short_Write_1P(u8 Generic,u8 Parma)	
{
	GP_LP(2);
	ssd_wdata(0xBF,(Parma<<8)|(Generic));
	//printf("Generic_Short_Write_1P %d  %d",Parma,Generic);
}
void Generic_Long_Write_2P(u8 Generic,u8 Parma1,u8 Parma2)
{
	GP_LP(3);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,0x00,Parma2);
}
void Generic_Long_Write_3P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3)
{
	GP_LP(4);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
}
void Generic_Long_Write_4P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4)
{
	GP_LP(5);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,0x00,Parma4);
}
void Generic_Long_Write_5P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5)
{
	GP_LP(6);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,Parma5,Parma4);
}
void Generic_Long_Write_6P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6)
{
	GP_LP(7);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,Parma5,Parma4);
	ssd_wparme(0x72,0x00,Parma6);
}
void Generic_Long_Write_7P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6,u8 Parma7)
{
	GP_LP(8);
	ssd_wdata(0xBF,(Parma1<<8)|(Generic));
	ssd_wparme(0x72,Parma3,Parma2);
	ssd_wparme(0x72,Parma5,Parma4);
	ssd_wparme(0x72,Parma7,Parma6);
}
void Generic_Long_Write_FIFO(u16 num,u16 *P)
{
	u16 i;
	
	GP_LP(num);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<num;){
		if((i+1) == num)
			ssd_wparme(0x72,0x00,P[i]);
		else
			ssd_wparme(0x72,P[i+1],P[i]);
		i += 2;
	}
}

void Generic_Long_Write_8FIFO(u16 num,u8 *P)
{
	u16 i;
	
	GP_LP(num);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<num;){
		if((i+1) == num)
			ssd_wparme(0x72,0x00,P[i]);
		else
			ssd_wparme(0x72,P[i+1],P[i]);
		i += 2;
	}
}

void LCD_INIT(void)			//
{
	
}

void SSD2828Init(void)
{
	

}







