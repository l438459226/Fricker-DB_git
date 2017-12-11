
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

u8 bufer[512];

typedef  void (*pFunction0)(void);

pFunction0 iapmain = NULL;


#define	PACK_HEAD 					0x80		//包头
#define	PACK_TAIL 					0x88		//包尾

//下位机 --> 上位机
#define	CMD_UA_CAL_STATUS			0x02
#define	CMD_LOADBIN_STATUS			0x03
#define	CMD_VA_VAL					0x04
#define	CMD_SYS_ERR					0x05
#define	CMD_VERSION					0x40

#define	CMD_UA_CAL_STATUS_LEN		0x01
#define	CMD_LOADBIN_STATUS_LEN		0x01
#define	CMD_VA_VAL_LEN				0x04
#define	CMD_VERSION_LEN				0x02
#define	CMD_SYS_ERR_LEN				0x01



//上位机  --> 下位机
#define	CMD_POWER_SEQ				0x41
#define	CMD_INIT_STATUS				0x42
#define	CMD_VA_STATUS				0x43
//#define	CMD_SYS_STATUS				0x44



#define	CMD_POWER_SEQ_LEN			0x01
#define	CMD_INIT_STATUS_LEN			0x01
#define	CMD_VA_STATUS_LEN			0x01
//#define	CMD_SYS_STATUS_LEN			0x01


void test(u8 ii)
{
	printf("QQ AA HH YY I :%d\r\n",ii);
}


void Clear_buffer(u8 *buf,u8 len)
{
	u32 i;
	for(i=0;i<len;i++)
	{
		buf[i] = 0x00;
	}
}


int UnPack(u8 *package,u8 len)
{
	static u8 i = 0,start = 0;
	u8 lenth;

	if(ReadUart(USART_PORT_COM2,&package[i],1)==0)//没有读到数据
	{
			return -1;
	}
	
	if(start)	//
	{
	#if 0 
		//printf("start \r\n");
		if(package[i] == PACK_HEAD)	//
		{
			printf("package agan\r\n");
			Clear_buffer(package,len);
			
			start = 1;
			i = 0;	//重新接收包
			return 0;
		}
		#endif
		
		if(package[i] == PACK_TAIL)
		{		
			lenth = i;
			start = 0;
			i = 0;
			return lenth;
		}
		else
		{
			i++;
			if(i==(len-1)) {start = 0;i = 0;Clear_buffer(package,len);}	//超出数值 重新接收包
			
		}
	}
	if((package[i] == PACK_HEAD)&&(i==0))
	{
		i = 0;
		start = 1;
		//printf("start \r\n");
		return 0 ;
	}
	return 0;
}

void Ymode(void)
{
	Main_Menu();
}


typedef char * va_list; 

#define _INTSIZEOF(n) \
((sizeof(n)+sizeof(int)-1)&~(sizeof(int) - 1) ) 

#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) ) 

#define va_arg(ap,t) \
( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) ) 

#define va_end(ap) ( ap = (va_list)0 ) 

void simple_va_fun(int reg,...);

void simple_va_fun(int reg,...)   
{   
    va_list   arg_ptr;     //定义可变参数指针 
		int i,regdata,n;
	
	
    va_start(arg_ptr,reg);   // i为最后一个固定参数，这里i为100
		n = va_arg(arg_ptr,int);
		printf( " 0x%x 0x%x",reg,n); 
		for(i=0;i<n;i++)
		{
			regdata = va_arg(arg_ptr,int);
			printf( " 0x%x",regdata); 
		}
		printf("\r\n");
	
    va_end(arg_ptr);        //  清空参数指针

    return;   
}


extern Interface IAP_W25QXX_Init ;


const u32 *p;

Interface IAP_W25QXX_Init = (u32(*)())(IIC_Start);




int main(void)
{ 
	u32 len=0x5674574,t;
	u32 *p=(u32 *)simple_va_fun;
  NVIC_Configuration();
	delay_init();	    	 //延时函数初始化

	System_GPIO_Config();	
	//uart2_init(115200);	 	//串口初始化为9600
	InitUart();
 	
	usmart_dev.init(72); 	//初始化USMART				 	
	mem_init(SRAMIN);
	
	p = &len;
	
	t = *p;
	
	IIC_Init();
	Flicker_init();//使用内存管理
	printf("uart2 init ok!\r\n");
	//I2C_Detect();

	TL35825_init();
	Set_Voutp(5.11);//4.95v~12.75v
	Set_Voutn(-2.3);//-1.2v~-13.95v
	Voutp_Upfirst();
	


	
	printf("\r\nEnable ACT8846....\r\n");
	act8846_init(); 
	Delay_ms(200);
	printf("int ACT8846_ON();\r\n");
	
	ACT8846_DCDC_Switch(DCDC_SW1,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW2,ACT8846_SWITCH_ON);
	ACT8846_DCDC_Switch(DCDC_SW3,ACT8846_SWITCH_ON);//off san lu
	ACT8846_DCDC_Switch(DCDC_SW4,ACT8846_SWITCH_ON);

	
	ACT8846_LDO_Switch(LDO_SW1,ACT8846_SWITCH_ON);				//对应OUT5。 注意 :OUT6 和 OUT10没有经过TPS22993
	ACT8846_LDO_Switch(LDO_SW2,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW3,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW4,ACT8846_SWITCH_ON);				//VSP VSN ON
	ACT8846_LDO_Switch(LDO_SW5,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW6,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW7,ACT8846_SWITCH_ON);
	ACT8846_LDO_Switch(LDO_SW8,ACT8846_SWITCH_ON);

	
	TPS22993_Init();							//快速放电
	TPS22993_Set_All_Mode(1);			//设置负载开关为io控制
	TPS22993_TPVDD_ON_OFF(1);
	TPS22993_TPVIO_ON_OFF(1);
	TPS22993_VDDH_ON_OFF(1);
	TPS22993_EXT1_ON_OFF(1);
	TPS22993_IOVCC_ON_OFF(1);
	TPS22993_VSP_VSN_ON_OFF(1);
	
	Delay_ms(200);
	
	OTP_P_ON_OFF(1);					//MOS管开关

	OLEDLCD_MAIN_POWER_OFF(); 	//LCD OLED 电压切换   控制mos管  通过继电器切换电源

	BL_POWER_ON(); 	//mos管 控制  给bl供5V电源
	Delay_ms(100);
	BL_ADJ_OFF();	//接入BL模块

	BL_DETECT_ON();	//背光控制mos管开关背光模块BL+ 
	
	printf("bei guang dianliushezhi \r\n");
	LM36923_Init(20);
	
	
	VSP_ON();	//，使能 vsp  vsn
	VSP_PWR_ON();	//	
	VSN_ON();
	
	Delay_ms(10);
	Tps65132_Init(Get_Volt_Val(PWR5_VSP_AVDD_INDEX), Get_Volt_Val(PWR6_VSN_AVEE_INDEX)); //设置VSP VSN电压
	Delay_ms(10);


	RELAY_UA_ALL_ON_OFF(1);
	Delay_ms(500);
	INA226_Volt();

	//Ymode();
	
	
	//iapmain = (pFunction0)(*p);
	//printf("running....app:0x%x",(u32)iapmain);
	//iapmain = (pFunction0)(*p);
	
	//iapmain();
	
  while(1) 
	{
		//Current_Volt();
		IAP_W25QXX_Init(0xD0,14,0x33,0x5C,0xCB,0x31,0x01,0x10,0x10,0x10,0x19,0x29,0xD0,0x33,0x5C,0xCB);
//		printf(0xD0,14,0x33,0x5C,0xCB,0x31,0x01,0x10,0x10,0x10,0x19,0x29,0xD0,0x33,0x5C,0xCB);
		Delay_ms(1000);
		len = ReadUart(USART_PORT_COM2,bufer,1);
		if(len)
			printf("read uart:%d   0x%x  \r\n",len,bufer[0]);
	}			
	
}	




