#include "ssd2828.h"
#include "usart.h"
#include "spi.h"			


unsigned int Lcd_DCLK = 135;	  //DCLK频率设置 单位为MHz 最大值150MH左右
unsigned int Lcd_LCDH = 1080;	  //分辨率.宽
unsigned int Lcd_LCDV = 1920;	  //分辨率.高
unsigned int Lcd_HBPD = 100;
unsigned int Lcd_HFPD = 100;
unsigned int Lcd_HSPW = 10;
unsigned int Lcd_VBPD = 7;
unsigned int Lcd_VFPD = 9;
unsigned int Lcd_VSPW = 2;
unsigned int Lcd_DCLK_EDGE = 1;		//lcd锁存数据时的DCLK边沿	 请用上面的宏定义来赋值
unsigned int Lcd_HSYNC_POLARITY = 0;  //HSYNC脉冲极性			  	 请用上面的宏定义来赋值
unsigned int Lcd_VSYNC_POLARITY = 0;  //VSYNC脉冲极性			  	 请用上面的宏定义来赋值
unsigned int Lcd_DE_POLARITY = 0;	 //DE信号有效电平			 请用上面的宏定义来赋值

unsigned int Lcd_MODE = 0X00;  //接口方式 MIPI_8LANE 或 MIPI_4LANE	 请用上面的宏定义来赋值
                               //1,2,3,4LANE的都赋值为MIPI_4LANE
															 
#define MIPI_SINGLE 0X00	  //包含1,2,3,4LANE的接口
#define MIPI_DOUBLE 0X01      //即8LANE	MIPI接口															 
						

/*---------------------------LP or HS or Video------------------------------*/
#define LP 0
#define HS 1
#define VD 2
unsigned int mode = 2; 

/*----------------------------------------------------------------------------*/
void SSD2828GPIOConfig(void)	//PA3-SPI_CS    PA5-SPI_CLK   PA6-MISO   PA7-MOSI
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //使能PORTA,和AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	   ///PORTC 3 5 6 7 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	   ///PORTC 3 5 6 7 复用推挽输出
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

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //使能PORTA,和AFIO时钟
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTA 3 5 6 7 复用推挽输出
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
			
			if((cmd & 0x80) != 0)	SPI_SDI = 1;
				else  SPI_SDI = 0;
			if((cmd & 0x80) != 0)	SPI_SDI = 1;
				else  SPI_SDI = 0;
			//delay_us(1); 
			SPI_SCLK = 1;
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
    SPI_SCLK = 1;
		if(SPI_SDO) tmp |= 0x01;
		if(SPI_SDO) tmp |= 0x01;
		if(SPI_SDO) tmp |= 0x01;
	}
	SPI_SCLK = 0;
	return tmp;
}

void ssd_wparme(u8 cmd,u8 dat1,u8 dat2)//cmd=0x70写寄存器     cmd=0x72写数据指令
{
	SSD_CS = 0;
	W_byte(cmd);	//写寄存器  W_REG
	W_byte(dat1);
	W_byte(dat2);
	SSD_CS = 1;
	//delay_us(10);
}

void ssd_wuparme(u8 cmd,u16 parmer)		//cmd=0x70写寄存器     cmd=0x72写数据指令
{
	SSD_CS = 0;
	W_byte(cmd);	//写寄存器  W_REG
	W_byte(parmer>>8);
	W_byte(parmer);
	SSD_CS = 1;
	//delay_us(10);
}

void ssd_wdata(u8 reg,u16 parmer)		//
{
	
	SSD_CS = 0;
	W_byte(0x70);	//写寄存器  W_REG
	W_byte(0x00);
	W_byte(reg);
	SSD_CS = 1;
	//delay_us(10);
	
	SSD_CS = 0;
	W_byte(0x72);	//写数据指令
	W_byte(parmer>>8);
	W_byte(parmer);
	SSD_CS = 1;
	//delay_us(10);
}



void W_REG(u8 reg,u8 dat_h,u8 dat_l)
{

	SSD_CS = 0;
	W_byte(0x70);	//写寄存器  W_REG
	W_byte(0x00);
	W_byte(reg);
	SSD_CS = 1;
	//delay_us(10);
	
	SSD_CS = 0;
	W_byte(0x72);	//写数据指令
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



u16 ssd_rdata(u8 reg)
{
	u16 parmer;
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 0;
	
	W_byte(0x70);	//写寄存器
	W_byte(0x00);
	W_byte(reg);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 1;
	delay_us(1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 0;
	
	W_byte(0x73);	//读数据指令
	//delay_us(10);
	parmer = SSD2828Read();
	parmer = (parmer<<8)|SSD2828Read();

	GPIO_SetBits(GPIOA,GPIO_Pin_3);	//SSD_CS = 1;
	//delay_us(10);
	return parmer;
}

//DCS 短包写 。 
//DT = 0x05, 0x15
//n - 数据个数； 
void DCS_SP(unsigned char n)
{
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
	
	delay_us(10);
	ssd_wdata(0xbc,n);
	ssd_wdata(0xbd,0x0000);
	ssd_wdata(0xbe,n);
}


void DCS_LP(unsigned long n)
{
	if(mode == LP)
	{
		ssd_wdata(0x00b7,0x0650);  //EOT Packet Enable,ECC CRC Check Enable, DCS Long Write, LP
	}
	else if(mode == HS)
	{
		ssd_wdata(0x00b7,(0x0650&0XEF)|0X03);  //EOT Packet Enable,ECC CRC Check Enable, DCS Long Write, HS
	}
	else if(mode == VD)
	{
		ssd_wdata(0x00b7,(0x0750&0XEF)|0X0B);  //EOT Packet Enable,ECC CRC Check Disable, DCS Long Write, HS Video
	}

	delay_us(10);
	ssd_wdata(0xbc,n);
	ssd_wdata(0xbd,n>>16);
	ssd_wdata(0xbe,0x0fff);
}


void GP_SP(unsigned char n)
{
	if(mode == LP)
	{
		ssd_wdata(0x00b7,0x0210); //EOT Packet Enable,ECC CRC Check Enable, Generic Short Write, LP
	}
	else if(mode == HS)
	{
		ssd_wdata(0x00b7,(0x0210&0xEF)|0X03);  //EOT Packet Enable,ECC CRC Check Enable, Generic Short Write, HS
	}
	else if(mode == VD)
	{
		ssd_wdata(0x00b7,(0x0310&0xEF)|0X0B);  //EOT Packet Enable,ECC CRC Check Disable, Generic Short Write, HS Video
	}

	delay_us(10);
	ssd_wdata(0xbc,n);
	ssd_wdata(0xbd,0x0000);
	ssd_wdata(0xbe,n);
}


void GP_LP(unsigned long n)
{
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
	}
	
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
	u8 datl;
	do
	{
		//ssd_wdata(0xb7,0x03c2);					//LP
		ssd_wdata(0xb7,(0x03c2&0xEF)|0x03);		//HS
		//ssd_wdata(0xb7,(0x03c2&0xEF)|0x0B);		//VD
		
		ssd_wdata(0xbb,0x0008);			//PL clock
		ssd_wdata(0xc1,cnt);		//Maximum Return Size
		ssd_wdata(0xc0,0x0001);		//取消SSD2828的操作？？
		ssd_wdata(0xBC,0x0001);
		ssd_wdata(0xBF,cmd);	   //把要讀的地址發送出去
		//delay_us(50);					   //讀需要一点点时间
		state = ssd_rdata(0xc6);		//读 ssd2828 的状态寄存器
		if(state & 0x01)
				break;    //讀成功 跳出循環
		else 
			if(++timeout_cnt > 10){
					//W_REG(0x00b7,0x03,0x0B);
					return MIPI_READ_FAIL;		//超時 讀失敗
			}
	}while(1);
	
	ssd_wuparme(0x70,0x00FF);			   //讀2828的0XFF寄存器，实际上不是个寄存器，只是假设从LCM读到的数据会存在0XFF
	for(i = 0; i < cnt;i++)
	{
		SSD_CS = 0;
		W_byte(0x73);	//读数据指令
		
	  hdat = SSD2828Read();
		if(hdat == 0xff)	continue;
		//*val++ = SSD2828Read();
		datl = SSD2828Read();
		*val++ = datl;
		*val++ = hdat;
		SSD_CS = 1;
	}
	ssd_wdata(0xb7,0x0159);
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
		ssd_wdata(0x00b7,0x0382);		//LP generic read
		
		ssd_wdata(0x00bb,0x0008);			//PL clock
		ssd_wdata(0x00c1,l);		//Maximum Return Size
		ssd_wdata(0x00c0,0x0001);		//取消SSD2828的操作？？
		ssd_wdata(0x00BC,0x0001);		 // *参数个数 
		ssd_wdata(0x00BF, adr);	   //把要讀的地址發送出去
		//delay_ms(10);					   //讀需要一点点时间
		state = ssd_rdata(0xc6);		//读 ssd2828 的状态寄存器

		if(state & 0x01)	break;     //讀成功 跳出循環
		else if(++timeout_cnt > 10){return MIPI_READ_FAIL;}		//超時 讀失敗
	}while(1);	//ssd2828的状态不对 就全部重来

	ssd_wuparme(0x70,0x00FF);				   //讀2828的0XFF寄存器，实际上不是个寄存器，只是假设从LCM读到的数据会存在0XFF
	for(i = 0; i < l;i++)
	{
		SSD_CS = 0;
		W_byte(0x73);	//读数据指令
		
	  hdat = SSD2828Read();
		if(hdat == 0xff)	continue;
		datl = SSD2828Read();
		*p++ = datl;
		*p++ = hdat;
		SSD_CS = 1;
		delay_us(1);
		Write_SSPI_REG(0xB7, 0x0159);
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

void DCS_Long_Write_FIFO(u16 NUM,u16 *P)
{
	u16 i;
	
	DCS_LP(NUM);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<NUM;){
		if((i+1) == NUM)
			ssd_wparme(0x72,0x00,P[i]);
		else
			ssd_wparme(0x72,P[i+1],P[i]);
		i += 2;
	}
}


void DCS_Long_Write_8FIFO(u16 NUM,u8 *P)
{
	u16 i;
	
	DCS_LP(NUM);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<NUM;){
		if((i+1) == NUM)
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
void Generic_Long_Write_FIFO(u16 NUM,u16 *P)
{
	u16 i;
	
	GP_LP(NUM);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<NUM;){
		if((i+1) == NUM)
			ssd_wparme(0x72,0x00,P[i]);
		else
			ssd_wparme(0x72,P[i+1],P[i]);
		i += 2;
	}
}

void Generic_Long_Write_8FIFO(u16 NUM,u8 *P)
{
	u16 i;
	
	GP_LP(NUM);
	
	ssd_wdata(0xBF,((P[1]<<8) | P[0]));
	for(i=2;i<NUM;){
		if((i+1) == NUM)
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







