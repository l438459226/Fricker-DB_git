/*********************************************************************************************************
* 文件名称: LT3582.c
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: CMS
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/


//LT3582 DRIVER

/******************************************
Vin 2.55 -- 5.5V
Voutp  3.2v -- 12.775v   Imax 350ma

Voutp = 3.2+ Vp*50mv + Vpuls*25mv

Vp : 0--191
Vpulse : 0 or 1



Voutn  -1.2v -- - 13.95v  (-50mv) Imax 600ma
 
Voutn = -1.2v - (Vn*50mv)

Vn:0--255
******************************************/




/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "LT3582.h"
//#include "i2c1_bitbang.h"
#include "myiic.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"



#define	REG_CMDR		0x04
#define	REG_VP_VAL		0x00
#define	REG_VN_VAL		0x01
#define	REG_PARA		0x02

static u8 LT3582_Address = LT3582_ADD;


/*******************************************************************************
* 文件名	     : LT3582_I2c_write
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
int LT3582_I2c_write(u8 reg,u8 val)
{
	int retval;
	
	retval = i2c1_start(LT3582_Address<<1);
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_write_u8((u8)(reg));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_write_u8(reg) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_write_u8((u8)(val));
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_write_u8(val) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_stop();
	if(retval<0)
	{
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c1_stop Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	return I2C_OK;	
}
/*
u8 TL35825_Write(u8 reg,u8 date)
{
	u8 ret=0;
	IIC_Start();	
 	IIC_Send_Byte(TL35825_ADDR<<1);   
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   	
	IIC_Wait_Ack();  
  IIC_Send_Byte(date);  	
	ret=IIC_Wait_Ack();
	if(ret)	return 255;  
	IIC_Stop();					
	return ret; 
}
*/

int LT3582_I2c_Read(uint8_t reg, uint8_t* pBuffer)
{
	int retval;
	retval = i2c1_start(LT3582_Address<<1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_start Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	retval = i2c1_write_u8((uint8_t)(reg));
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_write_u8(reg) Failed\r\n");
		#endif
		return I2C_ERR; 
	}
	
	retval = i2c1_start_repeat((LT3582_Address<<1)|0x01);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c1_start_repeat Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_read_u8(pBuffer, 1);
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c_read_data Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	retval = i2c1_stop();
	if(retval<0){
		#ifdef ENABLE_ERROR_PRINT
			printf("fun: i2c1_stop Failed\r\n");
		#endif
		return I2C_ERR; 
	}

	return I2C_OK;
}




/*******************************************************************************
* 文件名	     : LT3582_Reset
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_Reset(void)
{	
	LT3582_I2c_write(REG_CMDR,0x70);	//清除错误标志，复位，并关闭输出			
}






/*******************************************************************************
* 文件名	     : LT3582_On
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_On(void)
{
//	PCA9557_Sel_Dev(0);
//	PCA9557_Dev0_Set(IO_DEV0_LCD_VMTP_ON,IO_SWITCH_ON);
	LT3582_I2c_write(REG_CMDR,0x07);	
}



	

/*******************************************************************************
* 文件名	     : LT3582_Off
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_Off(void)
{
	LT3582_I2c_write(REG_CMDR,0x57);
//	PCA9557_Sel_Dev(0);
//	PCA9557_Dev0_Set(IO_DEV0_LCD_VMTP_ON,IO_SWITCH_OFF);
}


u16 Get_VoutP_Val(u16 volt)
{
	float temp = 0;

	temp = ((float)volt/1000 -3.2)*100/5;

	return(temp);
}


u16 Get_VoutN_Val(u16 volt)
{
	float temp = 0;

	temp = ((float)volt/1000 -1.2)*100/5;

	return(temp);
}





/*******************************************************************************
* 文件名	     : LT3582_Init
* 描述	         : 
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void LT3582_Init(void)
{
	u8 temp = 0;
	
	i2c1_init();
	
	LT3582_Reset();
	delay_ms(1);
	LT3582_I2c_write(REG_CMDR,0x57);			//选择REG0
	
	LT3582_I2c_write(REG_PARA,0x57);			//选择REG0
	LT3582_I2c_Read(REG_PARA,&temp);			//返回值
	printf("REG_VP_VAL = 0x%x\r\n",temp);
	
	delay_ms(1);
	
	LT3582_I2c_write(REG_VP_VAL,0x33);			//设置VP电压
	LT3582_I2c_Read(REG_VP_VAL,&temp);
	printf("reg_vp = 0x%x\r\n",temp);

	//LT3582_I2c_write(REG_CMDR,0x12);			//选择REG1
	delay_ms(1);
	
	LT3582_I2c_write(REG_VN_VAL,0x00);			//设置VN电压
	
	LT3582_I2c_Read(REG_VN_VAL,&temp);
	//printf("REG_VN_VAL = 0x%x\r\n",temp);

	//LT3582_I2c_write(REG_CMDR,0x14);			//选择REG2
	delay_ms(1);
	LT3582_I2c_write(REG_PARA,0x07);			//设置参数，
	LT3582_I2c_Read(REG_PARA,&temp);
	//printf("REG_PARA = 0x%x\r\n",temp);

	LT3582_Off();								//默认关闭

}

















