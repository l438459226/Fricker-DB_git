/*********************************************************************************************************
* �ļ�����: GPIO_Config.c
* �ļ�����:����δ��
* ��ǰ�汾: 01.00.00
* ����ģ��: Cth
* ��������: 2016��03��22��
* �ļ�ժҪ: 
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/


/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "GPIO_Config.h"
#include "INA226.h"
#include "act8846.h"




/*******************************************************************************
* �ļ���	     : PMU_GPIO_Config
* ����	         : �ڲ�ϵͳʱ������
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void PMU_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);		


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5;	
  	GPIO_Init(GPIOE, &GPIO_InitStructure);	


		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	
  	GPIO_Init(GPIOF, &GPIO_InitStructure);		

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
  	GPIO_Init(GPIOD, &GPIO_InitStructure);		
		
		//power ON sequence
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		GPIO_SetBits(GPIOG, GPIO_Pin_8);
		GPIO_ResetBits(GPIOF,GPIO_Pin_10);		//oled control
		GPIO_ResetBits(GPIOF,GPIO_Pin_11);
		GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}


void VSP_VSN_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOC, &GPIO_InitStructure);			

		GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);	


		GPIO_ResetBits(GPIOG,GPIO_Pin_1);
}



void OTP_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure);			

		GPIO_SetBits(GPIOE,GPIO_Pin_11);	
}



void BL_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure);			

		GPIO_SetBits(GPIOE,GPIO_Pin_6);
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);

		//////////////////////////////////////��ⲿ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|\
										GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
		GPIO_Init(GPIOE, &GPIO_InitStructure);	

		GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
		GPIO_SetBits(GPIOE,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);
		GPIO_SetBits(GPIOE,GPIO_Pin_14);
		GPIO_ResetBits(GPIOE,GPIO_Pin_13);


		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9);
}




void VA_Err_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOC, &GPIO_InitStructure);			

		GPIO_SetBits(GPIOC,GPIO_Pin_5);
}	




void Usr_Led_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOB, &GPIO_InitStructure);			

		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}	





void TPS22993_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|\
								  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);			


		GPIO_ResetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
		GPIO_ResetBits(GPIOD,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8);
}



void RELAY_UA_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;

	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); 											   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);			

		GPIO_ResetBits(GPIOG,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}



void  System_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//��ֹjtag ʹ��swd
	
	PMU_GPIO_Config();
	VSP_VSN_GPIO_Config();
	OTP_GPIO_Config();
	BL_GPIO_Config();
	VA_Err_GPIO_Config();
	TPS22993_GPIO_Config();
	RELAY_UA_GPIO_Config();
	Usr_Led_GPIO_Config();
}



/*******************************************************************************
* �ļ���	     : Set_PMU_RST_IO
* ����	         : �ڲ�ϵͳʱ������
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void Set_PMU_RST_IO(u8 value)
{			
	PGout(8) = value;
}






/*******************************************************************************
* �ļ���	     : Set_PMU_PWREN_IO
* ����	         : �ڲ�ϵͳʱ������
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void Set_PMU_PWREN_IO(u8 value)
{			
	PEout(5) = value;
}


/*******************************************************************************
* �ļ���	     : ACT8846_DCDC_Switch
* ����	         : 
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void ACT8846_DCDC_Switch(ACT8846_DCDC_CHN_TypeDef chn,ACT8846_SWITCH_TypeDef onoff)
{
	if(onoff == ACT8846_SWITCH_OFF)
	{
		act8846_dcdc_disable(chn);
	}
	else
	{
		act8846_dcdc_enable(chn);
	}
}




/*******************************************************************************
* �ļ���	     : ACT8846_LDO_Switch
* ����	         : 
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void ACT8846_LDO_Switch(ACT8846_LDO_CHN_TypeDef chn,ACT8846_SWITCH_TypeDef onoff)
{
	if(onoff == ACT8846_SWITCH_OFF)
	{
		act8846_ldo_disable(chn);
	}
	else
	{
		act8846_ldo_enable(chn);
	}
}











