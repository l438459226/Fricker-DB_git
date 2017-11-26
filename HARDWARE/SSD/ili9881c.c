
#include "lcmburn.h"

#ifdef ILI9881C

#include "delay.h"
#include "math.h"
#include "usart.h"
#include "ssd2828.h"
#include "lcd.h"
#include "LCM.h"


u8 write_otp(u8 vcom)
{
		u8 rvcom;
		Generic_Long_Write_3P(0xFF,0x98,0x81,0x01);
		Generic_Short_Write_1P(0x56, 0x00);
		Generic_Short_Write_1P(0x53, vcom);
		DCS_Short_Read_NP(0x53, 1, &rvcom);	
		return rvcom;
}

u8 Read_otp(void)//读取OTP值
{    
	u8 rvcom; 
	Generic_Long_Write_3P(0xFF,0x98,0x81,0x00);
	DCS_Short_Read_NP(0xDA, 1, &rvcom);
	return 0;
}


u8 before_otp_read(void)//读取OTP值
{ 
	
//	u8 rvcom; 
	u8 BUFFER[8];
	BUFFER[0]=0;

	Generic_Long_Write_3P(0xFF,0x98,0x81,0x00);
	DCS_Short_Read_NP(0xDA, 1, BUFFER+0);	
	
	Generic_Long_Write_3P(0xFF,0x98,0x81,0x01);
	DCS_Short_Read_NP(0x53, 1, BUFFER+1);
	DCS_Short_Read_NP(0xE6, 1, BUFFER+2);	//ID_MK
 	DCS_Short_Read_NP(0xE8, 1, BUFFER+3);	
	
	DCS_Long_Write_3P(0xFF,0x98,0x81,0x04);
	DCS_Short_Read_NP(0xC5, 1, BUFFER+4);

	sprintf((char*)fb_buffer,"B ID:%X MK:%X ",BUFFER[0],BUFFER[2]);
	LCD_ShowString(0,7,176,16,16,fb_buffer);
	sprintf((char*)fb_buffer,"VC:%X OVC:%X MK:%X ",BUFFER[1],BUFFER[4],BUFFER[3]);
	LCD_ShowString(0,8,176,16,16,fb_buffer);
	return 0;
}

u8 after_otp_read(void)//读取OTP值
{    
	u8 BUFFER[8];
	
	Generic_Long_Write_3P(0xFF,0x98,0x81,0x00);
	DCS_Short_Read_NP(0xDA, 1, BUFFER+0);	
	
	Generic_Long_Write_3P(0xFF,0x98,0x81,0x01);
	DCS_Short_Read_NP(0x53, 1, BUFFER+1);
	DCS_Short_Read_NP(0xE6, 1, BUFFER+2);	//ID_MK
 	DCS_Short_Read_NP(0xE8, 1, BUFFER+3);	
	
	DCS_Long_Write_3P(0xFF,0x98,0x81,0x04);
	DCS_Short_Read_NP(0xC5, 1, BUFFER+4);

	sprintf((char*)fb_buffer,"A ID:%X MK:%X ",BUFFER[0],BUFFER[2]);
	LCD_ShowString(0,7,176,16,16,fb_buffer);
	sprintf((char*)fb_buffer,"VC:%X OVC:%X MK:%X ",BUFFER[1],BUFFER[4],BUFFER[3]);
	LCD_ShowString(0,8,176,16,16,fb_buffer);
	return 0;
}

void burn_otp(u8 best)
{
  Generic_Long_Write_3P(0xFF,0x98,0x81,0x04);
  Generic_Short_Write_1P(0x6E,0x2A);
  Generic_Short_Write_1P(0x6F,0x35);
  Generic_Short_Write_1P(0xD7,0x04);
  Generic_Short_Write_1P(0x8B,0xEB);
  
  Generic_Long_Write_3P(0xFF,0x98,0x81,0x00);
	Generic_Short_Write_1P(0x11,0x00);			//sleep out
	delay_ms(150);

	Generic_Long_Write_3P(0xFF,0x98,0x81,0x01);
	delay_ms(50);
	Generic_Short_Write_1P(0xE0,0x55);	//??ID
	Generic_Short_Write_1P(0xE1,0x01);
	Generic_Short_Write_1P(0xE2,0x00);
  delay_ms(50);
                  
	Generic_Short_Write_1P(0xE3,0x55);
	Generic_Short_Write_1P(0xE4,0xAA);
	Generic_Short_Write_1P(0xE5,0x66);
	delay_ms(50);

	Generic_Short_Write_1P(0xE0,best);	//??VCOM1
	Generic_Short_Write_1P(0xE1,0x05);
	Generic_Short_Write_1P(0xE2,0x00);
  delay_ms(50);
        
	Generic_Short_Write_1P(0xE3,0x55);
	Generic_Short_Write_1P(0xE4,0xAA);
	Generic_Short_Write_1P(0xE5,0x66);
	delay_ms(50);
}



void YT55F96A0(void)
{
	
	DCS_Long_Write_3P(0xFF,0x98,0x81,0x03);
//GIP_1

DCS_Short_Write_1P(0x01,0x00);
DCS_Short_Write_1P(0x02,0x00);
DCS_Short_Write_1P(0x03,0x53);
DCS_Short_Write_1P(0x04,0x14);
DCS_Short_Write_1P(0x05,0x00);
DCS_Short_Write_1P(0x06,0x06);
DCS_Short_Write_1P(0x07,0x01);
DCS_Short_Write_1P(0x08,0x00);
DCS_Short_Write_1P(0x09,0x01);
DCS_Short_Write_1P(0x0a,0x19);
DCS_Short_Write_1P(0x0b,0x01);
DCS_Short_Write_1P(0x0c,0x00);
DCS_Short_Write_1P(0x0d,0x00);
DCS_Short_Write_1P(0x0e,0x00);
DCS_Short_Write_1P(0x0f,0x19);
DCS_Short_Write_1P(0x10,0x19);
DCS_Short_Write_1P(0x11,0x00);
DCS_Short_Write_1P(0x12,0x00);
DCS_Short_Write_1P(0x13,0x00);
DCS_Short_Write_1P(0x14,0x00);
DCS_Short_Write_1P(0x15,0x00);
DCS_Short_Write_1P(0x16,0x00);
DCS_Short_Write_1P(0x17,0x00);
DCS_Short_Write_1P(0x18,0x00);
DCS_Short_Write_1P(0x19,0x00);
DCS_Short_Write_1P(0x1a,0x00);
DCS_Short_Write_1P(0x1b,0x00);
DCS_Short_Write_1P(0x1c,0x00);
DCS_Short_Write_1P(0x1d,0x00);
DCS_Short_Write_1P(0x1e,0x40);
DCS_Short_Write_1P(0x1f,0x40); 
DCS_Short_Write_1P(0x20,0x02);
DCS_Short_Write_1P(0x21,0x05);
DCS_Short_Write_1P(0x22,0x02);  
DCS_Short_Write_1P(0x23,0x00);
DCS_Short_Write_1P(0x24,0x87); 
DCS_Short_Write_1P(0x25,0x87);
DCS_Short_Write_1P(0x26,0x00);
DCS_Short_Write_1P(0x27,0x00);
DCS_Short_Write_1P(0x28,0x3B); 
DCS_Short_Write_1P(0x29,0x03);
DCS_Short_Write_1P(0x2a,0x00);
DCS_Short_Write_1P(0x2b,0x00);
DCS_Short_Write_1P(0x2c,0x00);
DCS_Short_Write_1P(0x2d,0x00);
DCS_Short_Write_1P(0x2e,0x00);
DCS_Short_Write_1P(0x2f,0x00);
DCS_Short_Write_1P(0x30,0x00);
DCS_Short_Write_1P(0x31,0x00);
DCS_Short_Write_1P(0x32,0x00);
DCS_Short_Write_1P(0x33,0x00);
DCS_Short_Write_1P(0x34,0x04);
DCS_Short_Write_1P(0x35,0x00);
DCS_Short_Write_1P(0x36,0x00);
DCS_Short_Write_1P(0x37,0x00);
DCS_Short_Write_1P(0x38,0x01);
DCS_Short_Write_1P(0x39,0x01);
DCS_Short_Write_1P(0x3a,0x40);
DCS_Short_Write_1P(0x3b,0x40);
DCS_Short_Write_1P(0x3c,0x00);
DCS_Short_Write_1P(0x3d,0x00);
DCS_Short_Write_1P(0x3e,0x00);
DCS_Short_Write_1P(0x3f,0x00);
DCS_Short_Write_1P(0x40,0x00);
DCS_Short_Write_1P(0x41,0x88);
DCS_Short_Write_1P(0x42,0x00);
DCS_Short_Write_1P(0x43,0x00);
DCS_Short_Write_1P(0x44,0x00);
                                
                               
//GIP_2                         
DCS_Short_Write_1P(0x50,0x01);
DCS_Short_Write_1P(0x51,0x23);
DCS_Short_Write_1P(0x52,0x45);
DCS_Short_Write_1P(0x53,0x67);
DCS_Short_Write_1P(0x54,0x89);
DCS_Short_Write_1P(0x55,0xab);
DCS_Short_Write_1P(0x56,0x01);
DCS_Short_Write_1P(0x57,0x23);
DCS_Short_Write_1P(0x58,0x45);
DCS_Short_Write_1P(0x59,0x67);
DCS_Short_Write_1P(0x5a,0x89);
DCS_Short_Write_1P(0x5b,0xab);
DCS_Short_Write_1P(0x5c,0xcd);
DCS_Short_Write_1P(0x5d,0xef);
                                
//GIP_3                         
DCS_Short_Write_1P(0x5e,0x11);
DCS_Short_Write_1P(0x5f,0x06);
DCS_Short_Write_1P(0x60,0x0C);
DCS_Short_Write_1P(0x61,0x0D);
DCS_Short_Write_1P(0x62,0x0E);
DCS_Short_Write_1P(0x63,0x0F);
DCS_Short_Write_1P(0x64,0x02);
DCS_Short_Write_1P(0x65,0x02);
DCS_Short_Write_1P(0x66,0x02);
DCS_Short_Write_1P(0x67,0x02);
DCS_Short_Write_1P(0x68,0x02);
DCS_Short_Write_1P(0x69,0x02);
DCS_Short_Write_1P(0x6a,0x02);
DCS_Short_Write_1P(0x6b,0x02);
DCS_Short_Write_1P(0x6c,0x02);
DCS_Short_Write_1P(0x6d,0x02);
DCS_Short_Write_1P(0x6e,0x05);
DCS_Short_Write_1P(0x6f,0x05);
DCS_Short_Write_1P(0x70,0x05);
DCS_Short_Write_1P(0x71,0x02);
DCS_Short_Write_1P(0x72,0x01);
DCS_Short_Write_1P(0x73,0x00);
DCS_Short_Write_1P(0x74,0x08);
DCS_Short_Write_1P(0x75,0x08);
DCS_Short_Write_1P(0x76,0x0C);
DCS_Short_Write_1P(0x77,0x0D);
DCS_Short_Write_1P(0x78,0x0E);
DCS_Short_Write_1P(0x79,0x0F);
DCS_Short_Write_1P(0x7a,0x02);
DCS_Short_Write_1P(0x7b,0x02);
DCS_Short_Write_1P(0x7c,0x02);
DCS_Short_Write_1P(0x7d,0x02);
DCS_Short_Write_1P(0x7e,0x02);
DCS_Short_Write_1P(0x7f,0x02);
DCS_Short_Write_1P(0x80,0x02);
DCS_Short_Write_1P(0x81,0x02);
DCS_Short_Write_1P(0x82,0x02);
DCS_Short_Write_1P(0x83,0x02);
DCS_Short_Write_1P(0x84,0x05);
DCS_Short_Write_1P(0x85,0x05);
DCS_Short_Write_1P(0x86,0x05);
DCS_Short_Write_1P(0x87,0x02);
DCS_Short_Write_1P(0x88,0x01);
DCS_Short_Write_1P(0x89,0x00);
DCS_Short_Write_1P(0x8A,0x06);


//CMD_Page 4
DCS_Long_Write_3P(0xFF,0x98,0x81,0x04);
DCS_Short_Write_1P(0x6C,0x15);
DCS_Short_Write_1P(0x6E,0x3B);              //di_pwr_reg=0 VGH 18V clamp
DCS_Short_Write_1P(0x6F,0x45);              // reg vcl + VGH pumping ratio 4x VGL=-3x
DCS_Short_Write_1P(0x3A,0xA4);               //POWER SAVING
DCS_Short_Write_1P(0x8D,0x1F);              //VGL clamp -12V
DCS_Short_Write_1P(0x87,0xBA);              //ESD
DCS_Short_Write_1P(0x26,0x76);           
DCS_Short_Write_1P(0xB2,0xD1);

//CMD_Page 1
DCS_Long_Write_3P(0xFF,0x98,0x81,0x01);
DCS_Short_Write_1P(0x22,0x0A);	//BGR, SS
//DCS_Short_Write_1P(0x53,0xff);	//VCOM1
//DCS_Short_Write_1P(0x55,0x08);	//VCOM2
DCS_Short_Write_1P(0x50,0xA6);       	//VREG1OUT=4.7V
DCS_Short_Write_1P(0x51,0x9E);       	//VREG2OUT=-4.65V
DCS_Short_Write_1P(0x31,0x00);	//column inversion
DCS_Short_Write_1P(0x60,0x14);             //SDT
                                
DCS_Short_Write_1P(0xA0,0x0F);	//VP255	Gamma P
DCS_Short_Write_1P(0xA1,0x22);             //VP251        
DCS_Short_Write_1P(0xA2,0x30);             //VP247        
DCS_Short_Write_1P(0xA3,0x14);             //VP243        
DCS_Short_Write_1P(0xA4,0x17);             //VP239        
DCS_Short_Write_1P(0xA5,0x29);             //VP231        
DCS_Short_Write_1P(0xA6,0x1C);             //VP219        
DCS_Short_Write_1P(0xA7,0x1E);             //VP203        
DCS_Short_Write_1P(0xA8,0x9E);             //VP175        
DCS_Short_Write_1P(0xA9,0x1C);             //VP144        
DCS_Short_Write_1P(0xAA,0x29);             //VP111        
DCS_Short_Write_1P(0xAB,0x92);             //VP80         
DCS_Short_Write_1P(0xAC,0x1B);             //VP52         
DCS_Short_Write_1P(0xAD,0x1B);             //VP36         
DCS_Short_Write_1P(0xAE,0x4D);             //VP24         
DCS_Short_Write_1P(0xAF,0x22);             //VP16         
DCS_Short_Write_1P(0xB0,0x26);             //VP12         
DCS_Short_Write_1P(0xB1,0x5C);             //VP8          
DCS_Short_Write_1P(0xB2,0x6C);             //VP4          
DCS_Short_Write_1P(0xB3,0x3F);             //VP0          
                                           
DCS_Short_Write_1P(0xC0,0x01);							//VN255 GAMMA N
DCS_Short_Write_1P(0xC1,0x22);             //VN251        
DCS_Short_Write_1P(0xC2,0x30);             //VN247        
DCS_Short_Write_1P(0xC3,0x14);             //VN243        
DCS_Short_Write_1P(0xC4,0x17);             //VN239        
DCS_Short_Write_1P(0xC5,0x29);             //VN231        
DCS_Short_Write_1P(0xC6,0x1C);             //VN219        
DCS_Short_Write_1P(0xC7,0x1E);             //VN203        
DCS_Short_Write_1P(0xC8,0x9E);             //VN175        
DCS_Short_Write_1P(0xC9,0x1C);             //VN144        
DCS_Short_Write_1P(0xCA,0x29);             //VN111        
DCS_Short_Write_1P(0xCB,0x92);             //VN80         
DCS_Short_Write_1P(0xCC,0x1B);             //VN52        
DCS_Short_Write_1P(0xCD,0x1B);             //VN36         
DCS_Short_Write_1P(0xCE,0x4D);             //VN24         
DCS_Short_Write_1P(0xCF,0x22);             //VN16         
DCS_Short_Write_1P(0xD0,0x26);             //VN12         
DCS_Short_Write_1P(0xD1,0x5C);             //VN8          
DCS_Short_Write_1P(0xD2,0x6C);             //VN4          
DCS_Short_Write_1P(0xD3,0x3F);             //VN0  


	DCS_Long_Write_3P(0xFF,0x98,0x81,0x00);

	DCS_Short_Write_1P(0x11,0x00);
	DCS_Short_Write_1P(0x35,0x01);
	delay_ms(150);
	DCS_Short_Write_1P(0x29,0x00);
	delay_ms(20);
}



void yassylcd_init(void)
{
	
	SSD2828GPIOConfig();
  SSD_RESET = 1;	
	delay_us(50);
  SSD_RESET = 0;	
	delay_us(50);
  SSD_RESET = 1;	
	printfk(8,9,12,"11223");
	/*
	panel_info.width = 720;
	panel_info.height = 1280;
	panel_info.pixel = 24;
	panel_info.fps = 60;		//60赫兹
	
	panel_info.hsync = 24;
	panel_info.hbp = 24;
	panel_info.hfp = 48;
	
	panel_info.vsync = 8;
	panel_info.vbp = 12;
	panel_info.vfp = 24;
	panel_info.dsi_lane = 4;			//
	
	panel_info.pclk = panel_info.fps * ((panel_info.width + panel_info.hsync + panel_info.hbp + panel_info.hfp) * (panel_info.height + panel_info.vsync + panel_info.vbp + panel_info.vfp))/1000000;
	panel_info.dsi_mbps = panel_info.pclk*24/panel_info.dsi_lane + 8;		//多8m校验数据
	*/
	
	
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
	
	//SSD_LANE(4,0);
	
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
	
	
	
		DCS_Long_Write_3P(0xFF,0x98,0x81,0x03);
//GIP_1

DCS_Short_Write_1P(0x01,0x00);
DCS_Short_Write_1P(0x02,0x00);
DCS_Short_Write_1P(0x03,0x53);
DCS_Short_Write_1P(0x04,0x14);
DCS_Short_Write_1P(0x05,0x00);
DCS_Short_Write_1P(0x06,0x06);
DCS_Short_Write_1P(0x07,0x01);
DCS_Short_Write_1P(0x08,0x00);
DCS_Short_Write_1P(0x09,0x01);
DCS_Short_Write_1P(0x0a,0x19);
DCS_Short_Write_1P(0x0b,0x01);
DCS_Short_Write_1P(0x0c,0x00);
DCS_Short_Write_1P(0x0d,0x00);
DCS_Short_Write_1P(0x0e,0x00);
DCS_Short_Write_1P(0x0f,0x19);
DCS_Short_Write_1P(0x10,0x19);
DCS_Short_Write_1P(0x11,0x00);
DCS_Short_Write_1P(0x12,0x00);
DCS_Short_Write_1P(0x13,0x00);
DCS_Short_Write_1P(0x14,0x00);
DCS_Short_Write_1P(0x15,0x00);
DCS_Short_Write_1P(0x16,0x00);
DCS_Short_Write_1P(0x17,0x00);
DCS_Short_Write_1P(0x18,0x00);
DCS_Short_Write_1P(0x19,0x00);
DCS_Short_Write_1P(0x1a,0x00);
DCS_Short_Write_1P(0x1b,0x00);
DCS_Short_Write_1P(0x1c,0x00);
DCS_Short_Write_1P(0x1d,0x00);
DCS_Short_Write_1P(0x1e,0x40);
DCS_Short_Write_1P(0x1f,0x40); 
DCS_Short_Write_1P(0x20,0x02);
DCS_Short_Write_1P(0x21,0x05);
DCS_Short_Write_1P(0x22,0x02);  
DCS_Short_Write_1P(0x23,0x00);
DCS_Short_Write_1P(0x24,0x87); 
DCS_Short_Write_1P(0x25,0x87);
DCS_Short_Write_1P(0x26,0x00);
DCS_Short_Write_1P(0x27,0x00);
DCS_Short_Write_1P(0x28,0x3B); 
DCS_Short_Write_1P(0x29,0x03);
DCS_Short_Write_1P(0x2a,0x00);
DCS_Short_Write_1P(0x2b,0x00);
DCS_Short_Write_1P(0x2c,0x00);
DCS_Short_Write_1P(0x2d,0x00);
DCS_Short_Write_1P(0x2e,0x00);
DCS_Short_Write_1P(0x2f,0x00);
DCS_Short_Write_1P(0x30,0x00);
DCS_Short_Write_1P(0x31,0x00);
DCS_Short_Write_1P(0x32,0x00);
DCS_Short_Write_1P(0x33,0x00);
DCS_Short_Write_1P(0x34,0x04);
DCS_Short_Write_1P(0x35,0x00);
DCS_Short_Write_1P(0x36,0x00);
DCS_Short_Write_1P(0x37,0x00);
DCS_Short_Write_1P(0x38,0x01);
DCS_Short_Write_1P(0x39,0x01);
DCS_Short_Write_1P(0x3a,0x40);
DCS_Short_Write_1P(0x3b,0x40);
DCS_Short_Write_1P(0x3c,0x00);
DCS_Short_Write_1P(0x3d,0x00);
DCS_Short_Write_1P(0x3e,0x00);
DCS_Short_Write_1P(0x3f,0x00);
DCS_Short_Write_1P(0x40,0x00);
DCS_Short_Write_1P(0x41,0x88);
DCS_Short_Write_1P(0x42,0x00);
DCS_Short_Write_1P(0x43,0x00);
DCS_Short_Write_1P(0x44,0x00);
                                
                               
//GIP_2                         
DCS_Short_Write_1P(0x50,0x01);
DCS_Short_Write_1P(0x51,0x23);
DCS_Short_Write_1P(0x52,0x45);
DCS_Short_Write_1P(0x53,0x67);
DCS_Short_Write_1P(0x54,0x89);
DCS_Short_Write_1P(0x55,0xab);
DCS_Short_Write_1P(0x56,0x01);
DCS_Short_Write_1P(0x57,0x23);
DCS_Short_Write_1P(0x58,0x45);
DCS_Short_Write_1P(0x59,0x67);
DCS_Short_Write_1P(0x5a,0x89);
DCS_Short_Write_1P(0x5b,0xab);
DCS_Short_Write_1P(0x5c,0xcd);
DCS_Short_Write_1P(0x5d,0xef);
                                
//GIP_3                         
DCS_Short_Write_1P(0x5e,0x11);
DCS_Short_Write_1P(0x5f,0x06);
DCS_Short_Write_1P(0x60,0x0C);
DCS_Short_Write_1P(0x61,0x0D);
DCS_Short_Write_1P(0x62,0x0E);
DCS_Short_Write_1P(0x63,0x0F);
DCS_Short_Write_1P(0x64,0x02);
DCS_Short_Write_1P(0x65,0x02);
DCS_Short_Write_1P(0x66,0x02);
DCS_Short_Write_1P(0x67,0x02);
DCS_Short_Write_1P(0x68,0x02);
DCS_Short_Write_1P(0x69,0x02);
DCS_Short_Write_1P(0x6a,0x02);
DCS_Short_Write_1P(0x6b,0x02);
DCS_Short_Write_1P(0x6c,0x02);
DCS_Short_Write_1P(0x6d,0x02);
DCS_Short_Write_1P(0x6e,0x05);
DCS_Short_Write_1P(0x6f,0x05);
DCS_Short_Write_1P(0x70,0x05);
DCS_Short_Write_1P(0x71,0x02);
DCS_Short_Write_1P(0x72,0x01);
DCS_Short_Write_1P(0x73,0x00);
DCS_Short_Write_1P(0x74,0x08);
DCS_Short_Write_1P(0x75,0x08);
DCS_Short_Write_1P(0x76,0x0C);
DCS_Short_Write_1P(0x77,0x0D);
DCS_Short_Write_1P(0x78,0x0E);
DCS_Short_Write_1P(0x79,0x0F);
DCS_Short_Write_1P(0x7a,0x02);
DCS_Short_Write_1P(0x7b,0x02);
DCS_Short_Write_1P(0x7c,0x02);
DCS_Short_Write_1P(0x7d,0x02);
DCS_Short_Write_1P(0x7e,0x02);
DCS_Short_Write_1P(0x7f,0x02);
DCS_Short_Write_1P(0x80,0x02);
DCS_Short_Write_1P(0x81,0x02);
DCS_Short_Write_1P(0x82,0x02);
DCS_Short_Write_1P(0x83,0x02);
DCS_Short_Write_1P(0x84,0x05);
DCS_Short_Write_1P(0x85,0x05);
DCS_Short_Write_1P(0x86,0x05);
DCS_Short_Write_1P(0x87,0x02);
DCS_Short_Write_1P(0x88,0x01);
DCS_Short_Write_1P(0x89,0x00);
DCS_Short_Write_1P(0x8A,0x06);


//CMD_Page 4
DCS_Long_Write_3P(0xFF,0x98,0x81,0x04);
DCS_Short_Write_1P(0x6C,0x15);
DCS_Short_Write_1P(0x6E,0x3B);              //di_pwr_reg=0 VGH 18V clamp
DCS_Short_Write_1P(0x6F,0x45);              // reg vcl + VGH pumping ratio 4x VGL=-3x
DCS_Short_Write_1P(0x3A,0xA4);               //POWER SAVING
DCS_Short_Write_1P(0x8D,0x1F);              //VGL clamp -12V
DCS_Short_Write_1P(0x87,0xBA);              //ESD
DCS_Short_Write_1P(0x26,0x76);           
DCS_Short_Write_1P(0xB2,0xD1);

//CMD_Page 1
DCS_Long_Write_3P(0xFF,0x98,0x81,0x01);
DCS_Short_Write_1P(0x22,0x0A);	//BGR, SS
//DCS_Short_Write_1P(0x53,0xff);	//VCOM1
//DCS_Short_Write_1P(0x55,0x08);	//VCOM2
DCS_Short_Write_1P(0x50,0xA6);       	//VREG1OUT=4.7V
DCS_Short_Write_1P(0x51,0x9E);       	//VREG2OUT=-4.65V
DCS_Short_Write_1P(0x31,0x00);	//column inversion
DCS_Short_Write_1P(0x60,0x14);             //SDT
                                
DCS_Short_Write_1P(0xA0,0x0F);	//VP255	Gamma P
DCS_Short_Write_1P(0xA1,0x22);             //VP251        
DCS_Short_Write_1P(0xA2,0x30);             //VP247        
DCS_Short_Write_1P(0xA3,0x14);             //VP243        
DCS_Short_Write_1P(0xA4,0x17);             //VP239        
DCS_Short_Write_1P(0xA5,0x29);             //VP231        
DCS_Short_Write_1P(0xA6,0x1C);             //VP219        
DCS_Short_Write_1P(0xA7,0x1E);             //VP203        
DCS_Short_Write_1P(0xA8,0x9E);             //VP175        
DCS_Short_Write_1P(0xA9,0x1C);             //VP144        
DCS_Short_Write_1P(0xAA,0x29);             //VP111        
DCS_Short_Write_1P(0xAB,0x92);             //VP80         
DCS_Short_Write_1P(0xAC,0x1B);             //VP52         
DCS_Short_Write_1P(0xAD,0x1B);             //VP36         
DCS_Short_Write_1P(0xAE,0x4D);             //VP24         
DCS_Short_Write_1P(0xAF,0x22);             //VP16         
DCS_Short_Write_1P(0xB0,0x26);             //VP12         
DCS_Short_Write_1P(0xB1,0x5C);             //VP8          
DCS_Short_Write_1P(0xB2,0x6C);             //VP4          
DCS_Short_Write_1P(0xB3,0x3F);             //VP0          
                                           
DCS_Short_Write_1P(0xC0,0x01);							//VN255 GAMMA N
DCS_Short_Write_1P(0xC1,0x22);             //VN251        
DCS_Short_Write_1P(0xC2,0x30);             //VN247        
DCS_Short_Write_1P(0xC3,0x14);             //VN243        
DCS_Short_Write_1P(0xC4,0x17);             //VN239        
DCS_Short_Write_1P(0xC5,0x29);             //VN231        
DCS_Short_Write_1P(0xC6,0x1C);             //VN219        
DCS_Short_Write_1P(0xC7,0x1E);             //VN203        
DCS_Short_Write_1P(0xC8,0x9E);             //VN175        
DCS_Short_Write_1P(0xC9,0x1C);             //VN144        
DCS_Short_Write_1P(0xCA,0x29);             //VN111        
DCS_Short_Write_1P(0xCB,0x92);             //VN80         
DCS_Short_Write_1P(0xCC,0x1B);             //VN52        
DCS_Short_Write_1P(0xCD,0x1B);             //VN36         
DCS_Short_Write_1P(0xCE,0x4D);             //VN24         
DCS_Short_Write_1P(0xCF,0x22);             //VN16         
DCS_Short_Write_1P(0xD0,0x26);             //VN12         
DCS_Short_Write_1P(0xD1,0x5C);             //VN8          
DCS_Short_Write_1P(0xD2,0x6C);             //VN4          
DCS_Short_Write_1P(0xD3,0x3F);             //VN0  


	DCS_Long_Write_3P(0xFF,0x98,0x81,0x00);

	DCS_Short_Write_1P(0x11,0x00);
	DCS_Short_Write_1P(0x35,0x01);
	delay_ms(150);
	DCS_Short_Write_1P(0x29,0x00);
	delay_ms(20);
	
	//SSD_MODE(0,1);
		Write_SSPI_REG(0xB7, 0x0159);
	Write_SSPI_REG(0xB1, (panel_info.vsync<<8)|panel_info.hsync);			//VSA  HAS
	Write_SSPI_REG(0xB2, (panel_info.vbp<<8)|panel_info.hbp);			//VBP+10 HBP						
	Write_SSPI_REG(0xB3, (panel_info.vfp<<8)|panel_info.hfp);			//VFP HFP								
	Write_SSPI_REG(0xB4, panel_info.width);				//						        	
	Write_SSPI_REG(0xB5, panel_info.height);				//
	Write_SSPI_REG(0xB6, 0x00D3);			//non burst sync event, 24bpp
}









#endif
















