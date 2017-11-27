/*********************************************************************************************************
* 文件名称: LM36923i2c3.c
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: CMS
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/


//LM36923 DRIVER




/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "LM36923i2c3.h"
#include "i2c3.h"
#include "SysTick.h"
#include "tester_debug.h"







static u8 LM36923_Address1 = LM36923_ADD;






/*******************************************************************************
* 文件名	     : LM36923_I2c3_write
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
int LM36923_I2c3_write(u8 reg,u8 val)
{
	int retval;
	
	retval = i2c3_start(LM36923_Address1<<1);
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c3_start Failed\r\n");
		#endif
		return I2C3_ERR; 
	}

	retval = i2c3_write_u8((u8)(reg));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c3_write_u8(reg) Failed\r\n");
		#endif
		return I2C3_ERR; 
	}
	
	retval = i2c3_write_u8((u8)(val));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c3_write_u8(val) Failed\r\n");
		#endif
		return I2C3_ERR; 
	}
	
	retval = i2c3_stop();
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c3_stop Failed\r\n");
		#endif
		return I2C3_ERR; 
	}
	return I2C3_OK;
}





static int LM36923_I2c3_Read(uint8_t reg, uint8_t* pBuffer)
{
	int retval;
	retval = i2c3_start(LM36923_Address1<<1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c3_start Failed\r\n");
		#endif
		return I2C3_ERR; 
	}
	retval = i2c3_write_u8((uint8_t)(reg));
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				TESTER_MAIN_DEBUG("fun: i2c3_write_u8(reg) Failed\r\n");
		#endif
		return I2C3_ERR; 
	}
	
	retval = i2c3_start_repeat((LM36923_Address1<<1)|0x01);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c3_start_repeat Failed\r\n");
		#endif
		return I2C3_ERR; 
	}

	retval = i2c3_read_u8(pBuffer, 1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c_read_data Failed\r\n");
		#endif
		return I2C3_ERR; 
	}

	retval = i2c3_stop();
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			TESTER_MAIN_DEBUG("fun: i2c3_stop Failed\r\n");
		#endif
		return I2C3_ERR; 
	}

	return I2C3_OK;
}





/*******************************************************************************
* 文件名	     : LM36923_Reset
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_I2c3_Reset(void)
{
	Delay_ms(10);
	LM36923_I2c3_write(REG_DEVEICE_ON,0x01); 
	Delay_ms(10);
	LM36923_I2c3_Set_Mode(BL_MODE_I2C,LIN_MAP);
}






/*******************************************************************************
* 文件名	     : LM36923_Set_Mode
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_I2c3_Set_Mode(BL_MODE_TypeDef mode,MAP_TypeDef map)
{
	u8 temp = 0x00;
		
	temp  = (u8)(map << 7) | 0x01;			//bj_adi 0--关闭，1--输出
	LM36923_I2c3_write(REG_MODE,temp | (mode<<5));  //00 I2C控制
}


u8 Setbit1_I2c3(int num, int bit)
{  
    return num|=(1<<bit);  
}  
  
u8 Clearbit1_I2c3(int num, int bit)
{  
    return num&=~(1<<bit);  
}  




u8 LM36923_Val1 = 0;

/*******************************************************************************
* 文件名	     : LM36923_On
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_I2c3_On(BL_CHN_TypeDef chn)
{
	LM36923_Val1 = Setbit1_I2c3(LM36923_Val1,chn) | 0x01;
	LM36923_I2c3_write(REG_DEVEICE_ON,LM36923_Val1); 	
}



	

/*******************************************************************************
* 文件名	     : LM36923_Off
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_I2c3_Off(BL_CHN_TypeDef chn)
{
	u8 temp = 0;
#if 0	
	LM36923_Val1 = Clearbit1_I2c3(LM36923_Val1,chn) | 0x01;
	LM36923_I2c3_write(REG_DEVEICE_ON,LM36923_Val1); 	
	LM36923_I2c3_Read(REG_DEVEICE_ON,&temp);
	//printf("temp = %d\r\n",temp);
#endif
	LM36923_I2c3_write(REG_DEVEICE_ON,0x01); 
}


void LM36923_I2c3_AllOn(void)
{
	LM36923_I2c3_write(REG_DEVEICE_ON,0x0f); 	
}


void LM36923_I2c3_AllOff(void)
{
	LM36923_I2c3_write(REG_DEVEICE_ON,0x01); 	
}





/*******************************************************************************
* 文件名	     : LM36923_Set_Current
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
//liner map 计算公式 I = 37.806ua + 12.195ua*val,max = 25ma
//val 1 -- 2047
void LM36923_I2c3_Set_Current(u16 val)
{
	LM36923_I2c3_write(REG_CODE_L,val&0x07);	
	LM36923_I2c3_write(REG_CODE_H,(u8)(val>>3));	
}






//电感也可能要设置，先预留

/*******************************************************************************
* 文件名	     : LM36923_Init
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LM36923_I2c3_Init(u16 val)
{	
	u8 temp;
	float value = 0;
	u16 value1 = 0;
	static u8 flag_1time = 0;

	if(flag_1time == 0)
	{
		flag_1time = 1;
		LM36923_I2c3_Reset();
		//LM36923_I2c3_write(0x17,0xFF); Disable BL_ADJ, Debug Only
		LM36923_I2c3_Read(0x11,&temp);
		TESTER_MAIN_DEBUG("REG_11 = 0x%x\r\n",temp);
		Delay_ms(10);
		LM36923_I2c3_Read(0x13,&temp);
		LM36923_I2c3_write(0x13,temp | (0x01<<4)); 	//设置电感为22uh
		
		//LM36923_I2c3_write(REG_DEVEICE_ON,0x0F); 



		TESTER_MAIN_DEBUG("\r\nbl ma = %d\r\n",val);
		value = ((float)2047)/25;
		//TESTER_MAIN_DEBUG("\r\nbl ma = %f\r\n",value);
		value *= 100;
		//TESTER_MAIN_DEBUG("\r\nbl ma = %f\r\n",value);
		value *= val;
		//TESTER_MAIN_DEBUG("\r\nbl ma = %f\r\n",value);
		value1 = (u16)(((u32)(value))/100);
		TESTER_MAIN_DEBUG("\r\nbl value = %d\r\n",value1);	
		
		LM36923_I2c3_Set_Current(value1);

		#if 1
		LM36923_I2c3_Read(0x19,&temp);
		printf("REG_19 = 0x%x\r\n",temp);
		LM36923_I2c3_Read(0x18,&temp);
		printf("REG_18 = 0x%x\r\n",temp);
		LM36923_I2c3_Read(0x1f,&temp);
		printf("REG_ERR = 0x%x\r\n",temp);
		#endif	
	}
}




































