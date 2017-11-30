/*********************************************************************************************************
* 文件名称: VoltCurrentProc.h
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: CMS
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/
#ifndef __VOLT_CURRENT_PROC_H
#define __VOLT_CURRENT_PROC_H



/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "stm32f10x.h"




/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define	OLED_EN		0x00






/*********************************************************************************************************
*                                              结构体定义
*********************************************************************************************************/
typedef enum 
{
	IOVCC_ALARM_V_INDEX = 0,
	VDDH_ALARM_V_INDEX,
	TPVIO_ALARM_V_INDEX,
	TPVCC_ALARM_V_INDEX,
	VSP_ALARM_V_INDEX,
	VSN_ALARM_V_INDEX,
	VSP_ALARM_SLEEP_V_INDEX,
	VSN_ALARM_SLEEP_V_INDEX,
	IOVCC_ALARM_SLEEP_V_INDEX,
	VDDH_ALARM_SLEEP_V_INDEX,
	MAX_ALARM_V_INDEX,
}ALARM_V_INDEX_TypeDef;





typedef enum 
{
	IOVCC_ALARM_A_INDEX = 0,
	VDDH_ALARM_A_INDEX,
	TPVIO_ALARM_A_INDEX,
	TPVCC_ALARM_A_INDEX,
	VSP_ALARM_A_INDEX,
	VSN_ALARM_A_INDEX,
	VSP_ALARM_SLEEP_A_INDEX,
	VSN_ALARM_SLEEP_A_INDEX,
	IOVCC_ALARM_SLEEP_A_INDEX,
	VDDH_ALARM_SLEEP_A_INDEX,
	MAX_ALARM_A_INDEX,
}ALARM_A_INDEX_TypeDef;





void INA226_Volt(void);
void  Current_Volt(void);




#endif



