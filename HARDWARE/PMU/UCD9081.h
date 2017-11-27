/*********************************************************************************************************
* �ļ�����: UCD9081.h
* �ļ�����:����δ��
* ��ǰ�汾: 01.00.00
* ����ģ��: CMS
* ��������: 2016��03��22��
* �ļ�ժҪ: 
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/
#ifndef __UCD9081_H
#define __UCD9081_H



/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "stm32f10x.h"




/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/
#define  UCD9081_DEVEICE0_ADD		0x6F			//UCD9081�豸1��ַ


#define  REG_UCD9081_RAIL1H			0x00
#define  REG_UCD9081_RAIL1L			0x01
#define  REG_UCD9081_RAIL2H			0x02
#define  REG_UCD9081_RAIL2L			0x03
#define  REG_UCD9081_RAIL3H			0x04
#define  REG_UCD9081_RAIL3L			0x05
#define  REG_UCD9081_RAIL4H			0x06
#define  REG_UCD9081_RAIL4L			0x07
#define  REG_UCD9081_RAIL5H			0x08
#define  REG_UCD9081_RAIL5L			0x09
#define  REG_UCD9081_RAIL6H			0x0A
#define  REG_UCD9081_RAIL6L			0x0B
#define  REG_UCD9081_RAIL7H			0x0C
#define  REG_UCD9081_RAIL7L			0x0D
#define  REG_UCD9081_RAIL8H			0x0E
#define  REG_UCD9081_RAIL8L			0x0F


#define  REG_UCD9081_RESET			0x2F		//��λ
#define  REG_UCD9081_WADDR1			0x30		//��ַ�Ĵ���
#define  REG_UCD9081_WADDR2			0x31
#define  REG_UCD9081_WDATA1			0x32
#define  REG_UCD9081_WDATA2			0x33


#define  REG_GPIOVALL				0x1A		//���gpIOֱ�ӿ���
#define  REG_GPIOVALH				0x1B		//enʹ�ܿ�



////////////////////////////////////////////////
//��ѹ����������
#define RAIL1_UNDER_THRESHOLD      (u16)1500			//IOVCC
#define RAIL1_OVER_THRESHOLD       (u16)2200			
#define RAIL2_UNDER_THRESHOLD      (u16)1500			//TPDIO
#define RAIL2_OVER_THRESHOLD       (u16)2200	
#define RAIL3_UNDER_THRESHOLD      (u16)4600			//VSP
#define RAIL3_OVER_THRESHOLD       (u16)5600	
#define RAIL4_UNDER_THRESHOLD      (u16)2600			//VDDH
#define RAIL4_OVER_THRESHOLD       (u16)3000	
#define RAIL5_UNDER_THRESHOLD      (u16)1600			//EXPORT1
#define RAIL5_OVER_THRESHOLD       (u16)3300	
#define RAIL6_UNDER_THRESHOLD      (u16)2600			//TPVDD
#define RAIL6_OVER_THRESHOLD       (u16)3000	
#define RAIL7_UNDER_THRESHOLD      (u16)5000			//OPT
#define RAIL7_OVER_THRESHOLD       (u16)10000	
#define RAIL8_UNDER_THRESHOLD      (u16)4500			//+5V
#define RAIL8_OVER_THRESHOLD       (u16)6000	












/*********************************************************************************************************
*                                              �ṹ�嶨��
*********************************************************************************************************/
typedef enum 
{
	
	UCD9081_RAILS_NONE = 0,
	UCD9081_RAIL1,
	UCD9081_RAIL2,
	UCD9081_RAIL3,
	UCD9081_RAIL4,
	UCD9081_RAIL5,
	UCD9081_RAIL6,
	UCD9081_RAIL7,
	UCD9081_RAIL8,
	
}UCD9081_RAIL_TypeDef;




typedef struct
{
	u16 UCD9081_RAIL1_HIGH;
	u16 UCD9081_RAIL1_LOW;
	u16 UCD9081_RAIL2_HIGH;
	u16 UCD9081_RAIL2_LOW;
	u16 UCD9081_RAIL3_HIGH;
	u16 UCD9081_RAIL3_LOW;
	u16 UCD9081_RAIL4_HIGH;
	u16 UCD9081_RAIL4_LOW;
	u16 UCD9081_RAIL5_HIGH;
	u16 UCD9081_RAIL5_LOW;
	u16 UCD9081_RAIL6_HIGH;
	u16 UCD9081_RAIL6_LOW;
	u16 UCD9081_RAIL7_HIGH;
	u16 UCD9081_RAIL7_LOW;
	u16 UCD9081_RAIL8_HIGH;
	u16 UCD9081_RAIL8_LOW;
}UCD9081_VOLT_THRESHOLD_STRUCT_TypeDef;		//��ѹ������



typedef struct
{
	u16 UCD9081_RAIL1_HIGH;
	u16 UCD9081_RAIL1_LOW;
	u16 UCD9081_RAIL2_HIGH;
	u16 UCD9081_RAIL2_LOW;
	u16 UCD9081_RAIL3_HIGH;
	u16 UCD9081_RAIL3_LOW;
	u16 UCD9081_RAIL4_HIGH;
	u16 UCD9081_RAIL4_LOW;
	u16 UCD9081_RAIL5_HIGH;
	u16 UCD9081_RAIL5_LOW;
	u16 UCD9081_RAIL6_HIGH;
	u16 UCD9081_RAIL6_LOW;
	u16 UCD9081_RAIL7_HIGH;
	u16 UCD9081_RAIL7_LOW;
	u16 UCD9081_RAIL8_HIGH;
	u16 UCD9081_RAIL8_LOW;
}UCD9081_CURR_THRESHOLD_STRUCT_TypeDef;		//����������



/*********************************************************************************************************
*                                              �ⲿ��������
*********************************************************************************************************/
extern UCD9081_VOLT_THRESHOLD_STRUCT_TypeDef UCD9081_ThersholdManage;






/*********************************************************************************************************
*                                              API��������
*********************************************************************************************************/
void UCD9081_Init(void);
void Err_Handle(void);
void UCD9081_Set_Enx(UCD9081_RAIL_TypeDef rail,u8 val);
void UCD9081_Reset(u8 val);




















#endif


