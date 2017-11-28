/*********************************************************************************************************
* 文件名称: tps65651.h
* 文件作者: zq
* 当前版本: V1.00
* 所属模块: 
* 完成日期: 2017年03月30日
* 文件摘要: 
* 注意事项:	
* 更改说明: 
**********************************************************************************************************/
#ifndef __TPS6561_H
#define	__TPS6561_H





/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "stm32f10x.h"


#define	AMOLED_AVDD58			0
#define	AMOLED_AVDD77			1


#define	VELVSS_TRANSMIT_FAST	52
#define	VELVSS_TRANSMIT_12MS	53

#define	DISCHARGE_ON			50
#define	DISCHARGE_OFF			51




#define AMOLED_ENABLE()			GPIO_SetBits(GPIOF,GPIO_Pin_10)
#define AMOLED_DISABLE()		GPIO_ResetBits(GPIOF,GPIO_Pin_10)

#define AMOLED_FD_DISABLE()		GPIO_ResetBits(GPIOF,GPIO_Pin_11)
#define AMOLED_FD_ENABLE()		GPIO_SetBits(GPIOF,GPIO_Pin_11)


typedef enum 
{
	AMOLED_VNEG_NONE = 0,
	AMOLED_VNEG_54,
	AMOLED_VNEG_53,
	AMOLED_VNEG_52,
	AMOLED_VNEG_51,
	AMOLED_VNEG_50,
	AMOLED_VNEG_49,
	AMOLED_VNEG_48,
	AMOLED_VNEG_47,
	AMOLED_VNEG_46,
	AMOLED_VNEG_45,
	AMOLED_VNEG_44,
	AMOLED_VNEG_43,
	AMOLED_VNEG_42,
	AMOLED_VNEG_41,
	AMOLED_VNEG_40,
	AMOLED_VNEG_39,
	AMOLED_VNEG_38,
	AMOLED_VNEG_37,
	AMOLED_VNEG_36,
	AMOLED_VNEG_35,
	AMOLED_VNEG_34,
	AMOLED_VNEG_33,
	AMOLED_VNEG_32,
	AMOLED_VNEG_31,
	AMOLED_VNEG_30,
	AMOLED_VNEG_29,
	AMOLED_VNEG_28,
	AMOLED_VNEG_27,
	AMOLED_VNEG_26,
	AMOLED_VNEG_25,
	AMOLED_VNEG_24,
	AMOLED_VNEG_23,
	AMOLED_VNEG_22,
	AMOLED_VNEG_21,
	AMOLED_VNEG_20,
	AMOLED_VNEG_19,
	AMOLED_VNEG_18,
	AMOLED_VNEG_17,
	AMOLED_VNEG_16,
	AMOLED_VNEG_15,
	AMOLED_VNEG_14,
	AMOLED_VNEG_MAX
}AMOLED_VNEG_TypeDef;



typedef enum 
{
	AMOLED_AVDD_NONE = 0,
	AMOLED_AVDD_79 = 42,
	AMOLED_AVDD_76,
	AMOLED_AVDD_73,
	AMOLED_AVDD_70,
	AMOLED_AVDD_67,
	AMOLED_AVDD_64,
	AMOLED_AVDD_61,
	AMOLED_AVDD_58,
	AMOLED_AVDD_MAX
}AMOLED_AVDD_TypeDef;


typedef enum 
{
	AMOLED_VPOS_46 = 0,
	AMOLED_VPOS_47 = 54,
	AMOLED_VPOS_48,
	AMOLED_VPOS_49,
	AMOLED_VPOS_50,
	AMOLED_VPOS_MAX
}AMOLED_VPOS_TypeDef;




void tps65651_Init(void);
void tps65651_On(void);
void tps65651_Off(void);






#endif


