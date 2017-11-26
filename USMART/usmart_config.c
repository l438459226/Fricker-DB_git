#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"	
#include "usart.h"		
#include "sys.h"
#include "lcd.h"
#include "ssd2828.h"
												 
extern void led_set(u8 sta);
extern void test_fun(void(*ledset)(u8),u8 sta); 

extern void check_db(void);
extern u8	commd_fun(char *p);

extern void pc_ssd_rdata(u8 reg);
extern void pc_DCS_Short_Read_NP(u16 cmd, u8 cnt, u8 *val);
extern void  PC_GP_R(uint8_t adr, uint16_t l, uint8_t *p);
extern void PC_DCS_Long_Write_FIFO(u16 NUM,char *P);
extern void PC_Generic_Long_Write_FIFO(u16 NUM,char *P);

extern void burn_otp(u8 best);

//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		

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
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















