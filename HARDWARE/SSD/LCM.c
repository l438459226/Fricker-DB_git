#include "LCM.h" 
#include	"ssd2828.h"
#include "usart.h"
#include "lcmburn.h"

vidinfo_t panel_info;

u16 dsi_clock(u16 speed)//DSI	speed
{
		u16 pll;
		if(speed<1000)		pll = 0xc300;
		else if(speed<500)	pll = 0x8300;
			else if(speed<250)	pll = 0x4300;
				else if(speed<125)	pll = 0x0300;
	pll |= ((speed>>3)&0xff);	// 除8
	return pll;
}


void hs_settime(u16 pll)
{
		u16 hzd,hpd,czd,nibble;
		u16 czdcpd,cpedtd,chtd,wud;
	
		hzd = (pll/32) + 8;
		if(hzd < 3)	hzd = hzd;
			else if(hzd < 15)	hzd -= 1;
				else if(hzd < 17)	hzd -= 2;
					else if(hzd < 27)	hzd -= 3;
	
		hpd = (pll/80) + 1;
		nibble = (hzd<<8) | (hpd & 0xFF);
	
		if(pll>125)	czd = (pll/16) + 0x0A;
		else 	czd = 0x28;
		czdcpd = (czd<<8) | 0x0A;
		cpedtd = 0x0844;
	
		if(pll>125)	chtd = 0x0309;
		else chtd = (((pll/64)+3)<<8) | (((pll/64)+9));
		wud = 0x2ebb;
	
	Write_SSPI_REG(0xC9, nibble);
	Write_SSPI_REG(0xCA, czdcpd);
	Write_SSPI_REG(0xCB, cpedtd);
	Write_SSPI_REG(0xCC, chtd);
	Write_SSPI_REG(0xCD, wud);
}

void print_panelinfo(void)
{
		printf("\r\n panel_info.width:%d",panel_info.width);
		printf("\r\n panel_info.height:%d",panel_info.height);
		printf("\r\n panel_info.pixel:%d",panel_info.pixel);
		printf("\r\n panel_info.fps:%d",panel_info.fps);
		printf("\r\n panel_info.pclk:%d",panel_info.pclk);	
	
		printf("\r\n panel_info.hsync:%d",panel_info.hsync);
		printf("\r\n panel_info.hbp:%d",panel_info.hbp);
		printf("\r\n panel_info.hfp:%d",panel_info.hfp);
	
		printf("\r\n panel_info.vsync:%d",panel_info.vsync);
		printf("\r\n panel_info.vbp:%d",panel_info.vbp);
		printf("\r\n panel_info.vfp:%d",panel_info.vfp);
	
		printf("\r\n dsi_lane:%d",panel_info.dsi_lane);
		printf("\r\n dsi_mbps:%d\r\n",panel_info.dsi_mbps);
}


void SSD_LANE(u8 lane,u16 clock)
{
	Write_SSPI_REG(0xB9, 0x0000);
	Write_SSPI_REG(0xB6, 0x000B);	//non burst sync event, 24bpp
	if(clock)
			Write_SSPI_REG(0xBA, dsi_clock(clock));//  0xC367--824
	else 	Write_SSPI_REG(0xBA, dsi_clock(panel_info.pclk*24/panel_info.dsi_lane + 8));
	Write_SSPI_REG(0xDE, (lane-1));	//LANE  0--1 lane  1--2 lane  2--3 lane  3--4 lane
	Write_SSPI_REG(0x00B9,0x0001);
	delay_us(100);
}

	//	//Tips:	SSD_MODE([0], [1])
	//		[0]	Video Mode:	0 - Non burst mode with sync pulses
	//						1 - Non burst mode with sync events
	//						2 - Burst mode
	//						3 - Command mode
	//		[1]	HS Mode:		0 - No operation
	//						1 - Enable HS mode
void SSD_MODE(u8 vido_mode,u8 mode)
{
		
	Write_SSPI_REG(0xB1, (panel_info.vsync<<8)|panel_info.hsync);			//VSA  HAS
	Write_SSPI_REG(0xB2, (panel_info.vbp<<8)|panel_info.hbp);			//VBP+10 HBP						
	Write_SSPI_REG(0xB3, (panel_info.vfp<<8)|(panel_info.hfp));			//VFP HFP								
	Write_SSPI_REG(0xB4, panel_info.width);					//						        	
	Write_SSPI_REG(0xB5, panel_info.height);				//
	hs_settime(panel_info.dsi_mbps);
	
	Write_SSPI_REG(0xB6, 0x0D0|(vido_mode<<2)|0x03);	//non burst sync event, 24bpp
	
	if(mode)	Write_SSPI_REG(0xB7, 0x030B);
	else 	Write_SSPI_REG(0xB7, 0x0301);
}

void InitMipi_PanelInit23(void)///////
{
	SSD2828GPIOConfig();
  SSD_RESET = 1;	
	delay_us(50);
  SSD_RESET = 0;	
	delay_us(50);
  SSD_RESET = 1;	
	
	panel_info.width = 1080;
	panel_info.height = 1920;
	panel_info.pixel = 24;
	panel_info.fps = 60;		//60赫兹
	
	panel_info.hsync = 10;
	panel_info.hbp = 24;
	panel_info.hfp = 48;
	
	panel_info.vsync = 2;
	panel_info.vbp = 17;
	panel_info.vfp = 24;
	
	panel_info.pclk = panel_info.fps * ((panel_info.width + panel_info.hsync + panel_info.hbp + panel_info.hfp) * (panel_info.height + panel_info.vsync + panel_info.vbp + panel_info.vfp))/1000000;
	//panel_info.fps = (panel_info.pclk*1000000)/((panel_info.width + panel_info.hsync + panel_info.hbp + panel_info.hfp) * (panel_info.height + panel_info.vsync + panel_info.vbp + panel_info.vfp));	      
	
	panel_info.dsi_lane = 4;			//
	//panel_info.dsi_mbps = 824;		//自己设置mbps   自动计算mbps
	panel_info.dsi_mbps = panel_info.pclk*24/panel_info.dsi_lane + 8;		//多8m校验数据
	
	print_panelinfo();
	
/*	
	Write_SSPI_REG(0xB9, 0x0000);
	Write_SSPI_REG(0xB6, 0x000B);	//non burst sync event, 24bpp
	Write_SSPI_REG(0xBA, dsi_clock(panel_info.dsi_mbps));	//  0xC367--824
	Write_SSPI_REG(0xDE, (panel_info.dsi_lane-1));	//LANE  0--1 lane  1--2 lane  2--3 lane  3--4 lane
	Write_SSPI_REG(0x00B9,0x0001);
	delay_us(100);
*/
	SSD_LANE(4,0);
	/**************************************************/		
	//LCDD (Peripheral) Setting		
   //**************************************************/	


/*
	Write_SSPI_REG(0xB1, (panel_info.vsync<<8)|panel_info.hsync);			//VSA  HAS
	Write_SSPI_REG(0xB2, (panel_info.vbp<<8)|panel_info.hbp);			//VBP+10 HBP						
	Write_SSPI_REG(0xB3, (panel_info.vfp<<8)|(panel_info.hfp));			//VFP HFP								
	Write_SSPI_REG(0xB4, panel_info.width);					//						        	
	Write_SSPI_REG(0xB5, panel_info.height);				//
	
	hs_settime(panel_info.dsi_mbps);
	
	
	Write_SSPI_REG(0xB6, 0x00D3);	//non burst sync event, 24bpp
	Write_SSPI_REG(0xB7, 0x030B);
	*/
	SSD_MODE(0,1);	
}


void lcm_9881c(void)
{
	
//9881c
Generic_Long_Write_3P(0xFF,0x98,0x81,0x03);
//GIP_1

Generic_Short_Write_1P(0x01,0x00);
Generic_Short_Write_1P(0x02,0x00);
Generic_Short_Write_1P(0x03,0x53);
Generic_Short_Write_1P(0x04,0x13);
Generic_Short_Write_1P(0x05,0x13);
Generic_Short_Write_1P(0x06,0x06);
Generic_Short_Write_1P(0x07,0x00);
Generic_Short_Write_1P(0x08,0x04);
Generic_Short_Write_1P(0x09,0x04);
Generic_Short_Write_1P(0x0a,0x03);
Generic_Short_Write_1P(0x0b,0x03);
Generic_Short_Write_1P(0x0c,0x00);
Generic_Short_Write_1P(0x0d,0x00);
Generic_Short_Write_1P(0x0e,0x00);
Generic_Short_Write_1P(0x0f,0x04);
Generic_Short_Write_1P(0x10,0x04);
Generic_Short_Write_1P(0x11,0x00);
Generic_Short_Write_1P(0x12,0x00);
Generic_Short_Write_1P(0x13,0x00);
Generic_Short_Write_1P(0x14,0x00);
Generic_Short_Write_1P(0x15,0x00);
Generic_Short_Write_1P(0x16,0x00);
Generic_Short_Write_1P(0x17,0x00);
Generic_Short_Write_1P(0x18,0x00);
Generic_Short_Write_1P(0x19,0x00);
Generic_Short_Write_1P(0x1a,0x00);
Generic_Short_Write_1P(0x1b,0x00);
Generic_Short_Write_1P(0x1c,0x00);
Generic_Short_Write_1P(0x1d,0x00);
Generic_Short_Write_1P(0x1e,0xC0);
Generic_Short_Write_1P(0x1f,0x80);
Generic_Short_Write_1P(0x20,0x04);
Generic_Short_Write_1P(0x21,0x0B);
Generic_Short_Write_1P(0x22,0x00);
Generic_Short_Write_1P(0x23,0x00);
Generic_Short_Write_1P(0x24,0x00);
Generic_Short_Write_1P(0x25,0x00);
Generic_Short_Write_1P(0x26,0x00);
Generic_Short_Write_1P(0x27,0x00);
Generic_Short_Write_1P(0x28,0x55);
Generic_Short_Write_1P(0x29,0x03);
Generic_Short_Write_1P(0x2a,0x00);
Generic_Short_Write_1P(0x2b,0x00);
Generic_Short_Write_1P(0x2c,0x00);
Generic_Short_Write_1P(0x2d,0x00);
Generic_Short_Write_1P(0x2e,0x00);
Generic_Short_Write_1P(0x2f,0x00);
Generic_Short_Write_1P(0x30,0x00);
Generic_Short_Write_1P(0x31,0x00);
Generic_Short_Write_1P(0x32,0x00);
Generic_Short_Write_1P(0x33,0x00);
Generic_Short_Write_1P(0x34,0x04);
Generic_Short_Write_1P(0x35,0x00);
Generic_Short_Write_1P(0x36,0x05);
Generic_Short_Write_1P(0x37,0x00);
Generic_Short_Write_1P(0x38,0x3c);
Generic_Short_Write_1P(0x39,0x00);
Generic_Short_Write_1P(0x3a,0x40);
Generic_Short_Write_1P(0x3b,0x40);
Generic_Short_Write_1P(0x3c,0x00);
Generic_Short_Write_1P(0x3d,0x00);
Generic_Short_Write_1P(0x3e,0x00);
Generic_Short_Write_1P(0x3f,0x00);
Generic_Short_Write_1P(0x40,0x00);
Generic_Short_Write_1P(0x41,0x00);
Generic_Short_Write_1P(0x42,0x00);
Generic_Short_Write_1P(0x43,0x00);
Generic_Short_Write_1P(0x44,0x00);


//GIP_2
Generic_Short_Write_1P(0x50,0x01);
Generic_Short_Write_1P(0x51,0x23);
Generic_Short_Write_1P(0x52,0x45);
Generic_Short_Write_1P(0x53,0x67);
Generic_Short_Write_1P(0x54,0x89);
Generic_Short_Write_1P(0x55,0xab);
Generic_Short_Write_1P(0x56,0x01);
Generic_Short_Write_1P(0x57,0x23);
Generic_Short_Write_1P(0x58,0x45);
Generic_Short_Write_1P(0x59,0x67);
Generic_Short_Write_1P(0x5a,0x89);
Generic_Short_Write_1P(0x5b,0xab);
Generic_Short_Write_1P(0x5c,0xcd);
Generic_Short_Write_1P(0x5d,0xef);

//GIP_3
Generic_Short_Write_1P(0x5e,0x01);
Generic_Short_Write_1P(0x5f,0x14);
Generic_Short_Write_1P(0x60,0x15);
Generic_Short_Write_1P(0x61,0x0C);
Generic_Short_Write_1P(0x62,0x0D);
Generic_Short_Write_1P(0x63,0x0E);
Generic_Short_Write_1P(0x64,0x0F);
Generic_Short_Write_1P(0x65,0x10);
Generic_Short_Write_1P(0x66,0x11);
Generic_Short_Write_1P(0x67,0x08);
Generic_Short_Write_1P(0x68,0x02);
Generic_Short_Write_1P(0x69,0x0A);
Generic_Short_Write_1P(0x6a,0x02);
Generic_Short_Write_1P(0x6b,0x02);
Generic_Short_Write_1P(0x6c,0x02);
Generic_Short_Write_1P(0x6d,0x02);
Generic_Short_Write_1P(0x6e,0x02);
Generic_Short_Write_1P(0x6f,0x02);
Generic_Short_Write_1P(0x70,0x02);
Generic_Short_Write_1P(0x71,0x02);
Generic_Short_Write_1P(0x72,0x06);
Generic_Short_Write_1P(0x73,0x02);
Generic_Short_Write_1P(0x74,0x02);
Generic_Short_Write_1P(0x75,0x14);
Generic_Short_Write_1P(0x76,0x15);
Generic_Short_Write_1P(0x77,0x11);
Generic_Short_Write_1P(0x78,0x10);
Generic_Short_Write_1P(0x79,0x0F);
Generic_Short_Write_1P(0x7a,0x0E);
Generic_Short_Write_1P(0x7b,0x0D);
Generic_Short_Write_1P(0x7c,0x0C);
Generic_Short_Write_1P(0x7d,0x06);
Generic_Short_Write_1P(0x7e,0x02);
Generic_Short_Write_1P(0x7f,0x0A);
Generic_Short_Write_1P(0x80,0x02);
Generic_Short_Write_1P(0x81,0x02);
Generic_Short_Write_1P(0x82,0x02);
Generic_Short_Write_1P(0x83,0x02);
Generic_Short_Write_1P(0x84,0x02);
Generic_Short_Write_1P(0x85,0x02);
Generic_Short_Write_1P(0x86,0x02);
Generic_Short_Write_1P(0x87,0x02);
Generic_Short_Write_1P(0x88,0x08);
Generic_Short_Write_1P(0x89,0x02);
Generic_Short_Write_1P(0x8A,0x02);


//CMD_Page 4
Generic_Long_Write_3P(0xFF,0x98,0x81,0x04);
Generic_Short_Write_1P(0x6C,0x15);               //Set VCORE voltage =1.5V
Generic_Short_Write_1P(0x6E,0x33);               //di_pwr_reg=0 for power mode 2A //VGH clamp 18V
Generic_Short_Write_1P(0x6F,0x45);               // reg vcl + pumping ratio VGH=4x VGL=-2.5x
Generic_Short_Write_1P(0x3A,0xA4);               //POWER SAVING
Generic_Short_Write_1P(0x8D,0x1F);              //VGL clamp -12V
Generic_Short_Write_1P(0x87,0xBA);              //ESD               
Generic_Short_Write_1P(0x26,0x76);           
Generic_Short_Write_1P(0xB2,0xD1);
Generic_Short_Write_1P(0x88,0x0B);


//CMD_Page 1
Generic_Long_Write_3P(0xFF,0x98,0x81,0x01);
Generic_Short_Write_1P(0x22,0x0A);              //BGR, SS
Generic_Short_Write_1P(0x31,0x00);              //column inversion
//Generic_Short_Write_1P(0x53,0x72);              //VCOM1
//Generic_Short_Write_1P(0x55,0x99);              //VCOM2
Generic_Short_Write_1P(0x50,0xa0);              // VREG1OUT
Generic_Short_Write_1P(0x51,0xa0);              // VREG2OUT
Generic_Short_Write_1P(0x60,0x14);              //SDT

Generic_Short_Write_1P(0xA0,0x2B);              //VP255 Gamma P  
Generic_Short_Write_1P(0xA1,0x46);              //VP251
Generic_Short_Write_1P(0xA2,0x4E);              //VP247
Generic_Short_Write_1P(0xA3,0x13);              //VP243
Generic_Short_Write_1P(0xA4,0x17);              //VP239
Generic_Short_Write_1P(0xA5,0x2A);              //VP231
Generic_Short_Write_1P(0xA6,0x1D);              //VP219
Generic_Short_Write_1P(0xA7,0x1E);              //VP203
Generic_Short_Write_1P(0xA8,0xBA);              //VP175
Generic_Short_Write_1P(0xA9,0x1B);              //VP144
Generic_Short_Write_1P(0xAA,0x2A);              //VP111
Generic_Short_Write_1P(0xAB,0x9A);              //VP80
Generic_Short_Write_1P(0xAC,0x19);              //VP52
Generic_Short_Write_1P(0xAD,0x1A);              //VP36
Generic_Short_Write_1P(0xAE,0x4D);              //VP24
Generic_Short_Write_1P(0xAF,0x22);              //VP16
Generic_Short_Write_1P(0xB0,0x25);              //VP12
Generic_Short_Write_1P(0xB1,0x52);              //VP8
Generic_Short_Write_1P(0xB2,0x5F);              //VP4
Generic_Short_Write_1P(0xB3,0x31);              //VP0

Generic_Short_Write_1P(0xC0,0x1E);              //VN255 GAMMA N   
Generic_Short_Write_1P(0xC1,0x2B);              //VN251
Generic_Short_Write_1P(0xC2,0x3E);              //VN247
Generic_Short_Write_1P(0xC3,0x13);              //VN243
Generic_Short_Write_1P(0xC4,0x18);              //VN239
Generic_Short_Write_1P(0xC5,0x1C);              //VN231
Generic_Short_Write_1P(0xC6,0x1D);              //VN219
Generic_Short_Write_1P(0xC7,0x20);              //VN203
Generic_Short_Write_1P(0xC8,0xB6);              //VN175
Generic_Short_Write_1P(0xC9,0x1F);              //VN144
Generic_Short_Write_1P(0xCA,0x2A);              //VN111
Generic_Short_Write_1P(0xCB,0x97);              //VN80
Generic_Short_Write_1P(0xCC,0x1D);              //VN52
Generic_Short_Write_1P(0xCD,0x1D);              //VN36
Generic_Short_Write_1P(0xCE,0x50);              //VN24
Generic_Short_Write_1P(0xCF,0x23);              //VN16
Generic_Short_Write_1P(0xD0,0x25);              //VN12
Generic_Short_Write_1P(0xD1,0x58);              //VN8
Generic_Short_Write_1P(0xD2,0x65);              //VN4
Generic_Short_Write_1P(0xD3,0x31);              //VN0


	DCS_Long_Write_3P(0xFF,0x98,0x81,0x00);

	DCS_Short_Write_1P(0x11,0x00);
	DCS_Short_Write_1P(0x35,0x01);
	delay_ms(150);
	DCS_Short_Write_1P(0x29,0x00);
	delay_ms(20);
}


void InitMipi_PanelInitt(void)
{
	
	SSD2828GPIOConfig();
  SSD_RESET = 1;	
	delay_us(50);
  SSD_RESET = 0;	
	delay_us(50);
  SSD_RESET = 1;	
	
	
	panel_info.width = 720;
	panel_info.height = 1280;
	panel_info.pixel = 24;
	
	panel_info.hsync = 24;
	panel_info.hbp = 24;
	panel_info.hfp = 48;
	
	panel_info.vsync = 8;
	panel_info.vbp = 12;
	panel_info.vfp = 24;
	
	panel_info.dsi_lane = 4;
	panel_info.dsi_speed = 0x23;

	panel_info.pclk = 0;
	
	////////////////////////////////SSD2828  mipi
	Write_SSPI_REG(0xB9, 0x0000);
	Write_SSPI_REG(0xB7, 0x0150);
	Write_SSPI_REG(0xB8, 0x0000);

	Write_SSPI_REG(0xBA, 0x8337);
	Write_SSPI_REG(0xBB, 0x0005);
	Write_SSPI_REG(0xBC, 0x0001);


	Write_SSPI_REG(0xC6, 0xCF86);
	Write_SSPI_REG(0xC9, 0x1307);
	Write_SSPI_REG(0xCA, 0x2204);
	Write_SSPI_REG(0xCB, 0x083D);
	Write_SSPI_REG(0xCC, 0x090C);
	Write_SSPI_REG(0xCD, 0x2CC4);

	Write_SSPI_REG(0xD5, 0x30C0);
	Write_SSPI_REG(0xDE, 0x0003);
	Write_SSPI_REG(0xB9, 0x0001);
	
	
	lcm_9881c();			//lcd driver ic   lcm_9881c
	
	
	Write_SSPI_REG(0xB7, 0x0159);
	Write_SSPI_REG(0xB1, (panel_info.vsync<<8)|panel_info.hsync);			//VSA  HAS
	Write_SSPI_REG(0xB2, (panel_info.vbp<<8)|panel_info.hbp);			//VBP+10 HBP						
	Write_SSPI_REG(0xB3, (panel_info.vfp<<8)|panel_info.hfp);			//VFP HFP								
	Write_SSPI_REG(0xB4, panel_info.width);				//						        	
	Write_SSPI_REG(0xB5, panel_info.height);				//
	Write_SSPI_REG(0xB6, 0x00D3);			//non burst sync event, 24bpp
	
}



void InitMipi_PanelInit(void)
{
	SSD2828GPIOConfig();
  SSD_RESET = 1;	
	delay_us(50);
  SSD_RESET = 0;	
	delay_us(50);
  SSD_RESET = 1;	
	
	yassylcd_init();
}



