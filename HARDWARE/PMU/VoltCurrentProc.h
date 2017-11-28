/*********************************************************************************************************
* �ļ�����: VoltCurrentProc.h
* �ļ�����:����δ��
* ��ǰ�汾: 01.00.00
* ����ģ��: CMS
* ��������: 2016��03��22��
* �ļ�ժҪ: 
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/
#ifndef __VOLT_CURRENT_PROC_H
#define __VOLT_CURRENT_PROC_H



/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "stm32f10x.h"




/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/
#define	OLED_EN		0x00






/*********************************************************************************************************
*                                              �ṹ�嶨��
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






u8 Volt_Current_Ua_Calibration(void);
void Volt_Current_Detect(void);
u8	Sys_Err_Handle(void);
u8 V_UA_Alarm_Handle(void);
u8 V_A_Alarm_Handle(void);
void LED_DETECT_Hanlde(void);




u8 Get_UA_Err_Flag(void);
void Set_UA_Err_Flag(u8 val);
u8 Get_A_Err_Flag(void);
void Set_A_Err_Flag(u8 val);
u8 Get_V_Err_Flag(void);
void Set_V_Err_Flag(u8 val);
void Set_V_UA_Detect_Over_Flag(u8 val);
u8 Get_V_UA_Detect_Over_Flag(void);




u8 Get_Flag_Sys_Ready(void);
void Clear_Flag_Sys_Ready(void);






#endif



