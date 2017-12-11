#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "boardcfg.h"
#include "arm_math.h"
#include "math.h"
#include "Fricker.h"
#include "LT35825.h"
#include "LT3582.h"
#include "act8846.h"
#include "parm.h"

#include "GPIO_Config.h"
#include "TPS22993.h"
#include "LM36923.h"
#include "tps65132.h"

#include "INA226.h"
#include "AD715.h"
#include "Uart.h"
#include "VoltCurrentProc.h"

#include "common.h"

extern void simple_va_fun(int i,...);


//typedef  u32 (*Interface)();






struct function{
   void *func;
};


#define 		INTERFACEADDR				0x20000550
struct function *Interface_addr  __attribute__((at(INTERFACEADDR))) = NULL;


 void *interface[]={
	(void*)delay_ms,
	(void*)delay_us,
	(void*)printf,

	(void*)mymemset,
	(void*)mymemcpy,
	(void*)mem_malloc,
	(void*)mem_free,
	(void*)mem_perused,

	(void*)IIC_Start,
	(void*)IIC_Stop,
	(void*)IIC_Send_Byte,
	(void*)IIC_Read_Byte,
	(void*)IIC_Wait_Ack,
	(void*)IIC_Ack,
	(void*)IIC_NAck,
	(void*)Set_Voutp,
	(void*)Set_Voutn,
	(void*)Voutp_Upfirst,
	(void*)LM36923_Init,
	(void*)&lcm_para,	   //µ×²ã²ÎÊý
	0,
};


const struct function interfacee={
	 	delay_ms

 };

/*
const u32 od=(u32)delay_ms;
*/

//const void * pg = IIC_Start;













