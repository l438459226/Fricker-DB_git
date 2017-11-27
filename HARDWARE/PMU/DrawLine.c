/*********************************************************************************************************
* �ļ�����: LT3582.c
* �ļ�����:����δ��
* ��ǰ�汾: 01.00.00
* ����ģ��: CMS
* ��������: 2017��02��22��
* �ļ�ժҪ: 
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/


/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "DrawLine.h"
#include "fsmc_fpga.h"
#include "string.h"


/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/
#define TOUCH_FINGER_NUM	(u8)(4)		//��ָ��Ŀ




/*********************************************************************************************************
*                                              �ⲿ��������
*********************************************************************************************************/
extern uint16_t G_disp_width;
extern uint16_t G_disp_height;



/*********************************************************************************************************
*                                              �ֲ���������
*********************************************************************************************************/
static u8 map[8][5];
static u8 Draw_H_Line = 0,Draw_V_Line = 0;
static u16 start_x = 0,offset_x = 0,gap_x = 0,offset_y = 0;
static u16 Draw_Line_Cnt = 0,Draw_Line_Cnt1 = 0,Draw_Line_Status = 0;
static u8 Flag_Draw_Line_Down = 0;





/*******************************************************************************
* �ļ���	     : Draw_Line_Init
* ����	         : 
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
u8 Draw_Line_Init(u32 fr_color,u32 bk_color,u8 pen_size,u8 h_line,u8 v_line)
{
	u8 i = 0,j = 0;
	
	Delay_ms(200);
	FPGA_Lcd_Clear(bk_color,30);
	if(h_line == 8 || h_line == 4 || h_line == 6)		//ֻ��4��6��8�������з�ʽ
	{
		Draw_H_Line = h_line;
		if(G_disp_height == 1920)	//1920*1080
		{
			switch(h_line)
			{
				case 4:
					start_x = 40;
					offset_x = 143;
					gap_x = 143;
					break;
				case 6:
					start_x = 30;
					offset_x = 93;
					gap_x = 93;
					break;
				case 8:
					start_x = 20;
					offset_x = 70;
					gap_x = 70;
					break;
			}
		}
		else if(G_disp_height == 1280)	//1280*720
		{
			switch(h_line)
			{
				case 4:
					start_x = 40;
					offset_x = 92;
					gap_x = 92;
					break;
				case 6:
					start_x = 30;
					offset_x = 60;
					gap_x = 60;
					break;
				case 8:
					start_x = 20;
					offset_x = 46;
					gap_x = 46;
					break;
			}
		}
		
		
		if(v_line == 24 || v_line == 40)
		{
			Draw_V_Line = v_line;

			if(G_disp_height == 1920)	//1920*1080
			{
				if(v_line == 24)
				{
					offset_y = 80;
				}
				else if(v_line == 40)
				{
					offset_y = 48;
				}
			}
			else if(G_disp_height == 1280)	//1280*720
			{
				if(v_line == 24)
				{
					offset_y = 53;
				}
				else if(v_line == 40)
				{
					offset_y = 32;
				}
			}
		}
		else
		{			
			return 0;		//���������й���
		}
	}
	else
	{		
		return 0;
	}
#if 0
	for(i = 1; i <= 8; i++)
	{		
		FPGA_Lcd_DrawLine(20+(60+80)*(i-1),20+(60+80)*(i-1),0,1920,fr_color,4*pen_size,30);
		FPGA_Lcd_DrawLine(20+60*i+80*(i-1),20+60*i+80*(i-1),0,1920,fr_color,4*pen_size,30);
		for(j = 0;j < 40;j++)
		{
			FPGA_Lcd_DrawLine(20+(60+80)*(i-1),20+60*i+80*(i-1),48*j,48*j,fr_color,4*pen_size,30);
		}
	}
	setAndShowPic(30,1);	
#else
	for(i = 1; i <= h_line; i++)
	{		
		FPGA_Lcd_DrawLine(start_x+(offset_x+gap_x)*(i-1),start_x+(offset_x+gap_x)*(i-1),0,G_disp_height,fr_color,4*pen_size,30);
		FPGA_Lcd_DrawLine(start_x+offset_x*i+gap_x*(i-1),start_x+offset_x*i+gap_x*(i-1),0,G_disp_height,fr_color,4*pen_size,30);
		for(j = 0;j < v_line;j++)
		{
			FPGA_Lcd_DrawLine(start_x+(offset_x+gap_x)*(i-1),start_x+offset_x*i+gap_x*(i-1),offset_y*j,offset_y*j,fr_color,4*pen_size,30);
		}
	}
	setAndShowPic(30,1);
#endif

	return 1;
}




/*******************************************************************************
* �ļ���	     : Draw_Rect
* ����	         : 
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
u8 Draw_Rect(u16 x,u16 y,u32 color)
{	
	s16 x1 = -1,y1 = -1;
	u8 i = 0,j = 0;
	

	if(Draw_Line_Status == 0)		//��ûʧ�ܵ������
	{
		for(i = 1;i <= Draw_H_Line;i++)
		{
			if(x >= (start_x+(offset_x+gap_x)*(i-1)) && x <= (start_x+offset_x*i+gap_x*(i-1)))		//���жϷ�Χ
			{
				Flag_Draw_Line_Down = 1;
				x1 = start_x+(offset_x+gap_x)*(i-1);
				j = y/offset_y;
				y1 = offset_y*j;
				map[i-1][j/8] |= (0x01 << (j%8));		
				break;		
			}
		}	
		
		
		if((x1 != -1) && (y1 != -1))
		{			
			if(G_disp_height == 1920)	//1920*1080
			{
				switch(Draw_H_Line)
				{
					case 4:
						if(Draw_V_Line == 24)
							FPGA_Lcd_DrawRect(x1+4,y1+4,76,139,color,30);
						else if(Draw_V_Line == 40)
							FPGA_Lcd_DrawRect(x1+4,y1+4,44,139,color,30);
						break;
					case 6:
						if(Draw_V_Line == 24)
							FPGA_Lcd_DrawRect(x1+4,y1+4,76,89,color,30);
						else if(Draw_V_Line == 40)
							FPGA_Lcd_DrawRect(x1+4,y1+4,44,89,color,30);
						break;
					case 8:
						if(Draw_V_Line == 24) 
							FPGA_Lcd_DrawRect(x1+4,y1+4,76,66,color,30);
						else if(Draw_V_Line == 40)
							FPGA_Lcd_DrawRect(x1+4,y1+4,44,66,color,30);
						break;
				}			
			}
			else if(G_disp_height == 1280)	//1280*720
			{
				switch(Draw_H_Line)
				{
					case 4:
						if(Draw_V_Line == 24)
							FPGA_Lcd_DrawRect(x1+4,y1+4,88,49,color,30);
						else if(Draw_V_Line == 40)
							FPGA_Lcd_DrawRect(x1+4,y1+4,88,28,color,30);
						break;
					case 6:
						if(Draw_V_Line == 24)
							FPGA_Lcd_DrawRect(x1+4,y1+4,56,49,color,30);
						else if(Draw_V_Line == 40)
							FPGA_Lcd_DrawRect(x1+4,y1+4,56,28,color,30);
						break;
					case 8:
						if(Draw_V_Line == 24)
							FPGA_Lcd_DrawRect(x1+4,y1+4,42,49,color,30);
						else if(Draw_V_Line == 40)
							FPGA_Lcd_DrawRect(x1+4,y1+4,42,28,color,30);
						break;
				}
			}
		}	
	}

}


/*******************************************************************************
* �ļ���	     : Is_Draw_Line_OK
* ����	         : 
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
u8 Is_Draw_Line_OK(void)
{
	
	u8 i = 0,j = 0;

	if(Draw_Line_Status == 0)
	{
		if(Draw_Line_Cnt++ >= 100)		//0.65s
		{
			Draw_Line_Cnt = 0;

			if(Flag_Draw_Line_Down)
			{
				if(Draw_Line_Cnt1++ >= 30)
				{
					Flag_Draw_Line_Down = 0;
					Draw_Line_Cnt1 = 0;
					Draw_Line_Status = 1;
					FPGA_LCD_Str_O_Trans(350,800,"FAIL",FPGA_COLOUR_RED,FPGA_COLOUR_BLUE,30);
					return 0;
				}
			}			
			
			if(Draw_H_Line != 0 && Draw_V_Line != 0)
			{				
				for(i = 0;i < Draw_H_Line;i++)
				{
					for(j = 0;j < (Draw_V_Line/8);j++)
					{
						if(map[i][j] != 0xff)
						{
							//printf("err \r\n");
							return 0;
						}
						//printf("map[%d][%d] = %d\r\n",i,j,map[i][j]);
					}
				}				
			}
			else
			{
				return 0;
			}
			FPGA_LCD_Str_O_Trans(350,800,"PASS",FPGA_COLOUR_BLUE,FPGA_COLOUR_BLUE,30);
			Draw_Line_Status = 2;
			//printf("pass \r\n");
			return 1;		
		}
	}
	else
	{
		return 0;
	}
}




/*******************************************************************************
* �ļ���	     : Clear_Draw_Line
* ����	         : 
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void Clear_Draw_Line(void)
{
#if 0			//�Ƿ���Ҫ��ӡ
	u8 i = 0,j = 0;
	
	for(i = 0;i < 8;i++)
	{
		for(j = 0;j < 5;j++)
		{
			if(map[i][j] != 0xff)
			{
				//printf("err \r\n");
				//return 0;
			}	

			printf("map[%d][%d] = %d\r\n",i,j,map[i][j]);
		}
	}
#endif	
	FPGA_LCD_Str_O_Trans(350,800,"clear",FPGA_COLOUR_BLUE,FPGA_COLOUR_BLUE,30);

	Draw_H_Line = 0;
	Draw_V_Line = 0;
	start_x = 0;
	offset_x = 0;
	gap_x = 0;
	offset_y = 0;
	Flag_Draw_Line_Down = 0;
	Draw_Line_Status = 0;
	Draw_Line_Cnt = 0;
	Draw_Line_Cnt1 = 0;
	memset(map,0,40);	
}



/*******************************************************************************
* �ļ���	     : Draw_Line_Handle
* ����	         : 
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void Draw_Line_Handle(void)
{
	static u16 cnt = 0;	
	u16 x_arr[10],y_arr[10];
	u8 status[10],i = 0;

	if(cnt++ >= 3)
	{
		cnt = 0;	
		Read_Finger_Report(x_arr,y_arr,status,TOUCH_FINGER_NUM);		
		if(status[0] == 1)
		{	
			//printf("x_arr = %d,y_arr = %d,status = %d\r\n",x_arr[0],y_arr[0],status[0]);			
			for(i = 0;i < TOUCH_FINGER_NUM;i++)
			Draw_Rect(x_arr[i],y_arr[i],FPGA_COLOUR_GREEN);
		}
		Is_Draw_Line_OK();
	}
}






