#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"	
#include "usart.h"		
#include "sys.h"
#include "lcd.h"
#include "ssd2828.h"
												 
extern void test(u8 ii);
extern void test_fun(void(*ledset)(u8),u8 sta); 

extern void check_db(void);
extern u8	commd_fun(char *p);

extern void pc_ssd_rdata(u8 reg);
extern void pc_DCS_Short_Read_NP(u16 cmd, u8 cnt, u8 *val);
extern void  PC_GP_R(uint8_t adr, uint16_t l, uint8_t *p);
extern void PC_DCS_Long_Write_FIFO(u16 NUM,char *P);
extern void PC_Generic_Long_Write_FIFO(u16 NUM,char *P);

extern void burn_otp(u8 best);

extern void Ymode(void);

//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		

	(void*)test,"u16 test(u8 ii)",
	(void*)Ymode,"void Ymode(void)",
/*

	(void*)pc_ssd_rdata,"u16 ssd_rdata(u8 reg)",
	(void*)ssd_wdata,"void ssd_wdata(u8 reg,u16 parmer)",//ssd_wdata(0xb7,0x03c2);

	
	(void*)Generic_Short_Write_1P,"void Generic_Short_Write_1P(u8 Generic,u8 Parma)",
	(void*)Generic_Long_Write_2P,"void Generic_Long_Write_2P(u8 Generic,u8 Parma1,u8 Parma2)",
	(void*)Generic_Long_Write_3P,"void Generic_Long_Write_3P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3)",
	(void*)Generic_Long_Write_4P,"void Generic_Long_Write_4P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4)",
	(void*)Generic_Long_Write_5P,"void Generic_Long_Write_5P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5)",
	(void*)Generic_Long_Write_6P,"void Generic_Long_Write_6P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6)",
	(void*)Generic_Long_Write_7P,"void Generic_Long_Write_7P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6,u8 Parma7)",
	(void*)PC_Generic_Long_Write_FIFO,"void Generic_Long_Write_FIFO(u16 NUM,u8 *P)",
	(void*)PC_GP_R,"unsigned int Generic_Short_Read_NP(uint8_t adr, uint16_t l, uint8_t *p);",
	(void*)burn_otp,"void burn_otp(void)",
	
	(void*)DCS_Short_Write_1P,"void DCS_Short_Write_1P(u8 Generic,u8 Parma)",
	(void*)DCS_Long_Write_2P,"void DCS_Long_Write_2P(u8 Generic,u8 Parma1,u8 Parma2)",
	(void*)DCS_Long_Write_3P,"void DCS_Long_Write_3P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3)",
	(void*)DCS_Long_Write_4P,"void DCS_Long_Write_4P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4)",
	(void*)DCS_Long_Write_5P,"void DCS_Long_Write_5P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5)",
	(void*)DCS_Long_Write_6P,"void DCS_Long_Write_6P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6)",
	(void*)DCS_Long_Write_7P,"void DCS_Long_Write_7P(u8 Generic,u8 Parma1,u8 Parma2,u8 Parma3,u8 Parma4,u8 Parma5,u8 Parma6,u8 Parma7)",
	(void*)PC_DCS_Long_Write_FIFO,"void DCS_Long_Write_FIFO(u16 NUM,u16 *P)",
	(void*)pc_DCS_Short_Read_NP,"u8 DCS_Short_Read_NP(u16 cmd, u8 cnt, u8 *val)",	
	
	(void*)commd_fun,"u8 commd_fun(char *p)",
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)",	 
	(void*)LCD_Clear,"void LCD_Clear(u16 Color)",
	(void*)LCD_Fill,"void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)",
	(void*)LCD_DrawLine,"void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)",
	(void*)LCD_DrawRectangle,"void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)",
	(void*)LCD_Draw_Circle,"void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)",
	(void*)LCD_ShowNum,"void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)",
	(void*)LCD_ShowString,"void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)",
	(void*)led_set,"void led_set(u8 sta)",
	(void*)test_fun,"void test_fun(void(*ledset)(u8),u8 sta)",	
	(void*)LCD_Fast_DrawPoint,"void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)",
	(void*)LCD_ReadPoint,"u16 LCD_ReadPoint(u16 x,u16 y)",					 					 
	(void*)LCD_Display_Dir,"void LCD_Display_Dir(u8 dir)",
	(void*)LCD_ShowxNum,"void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)",
	*/
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















