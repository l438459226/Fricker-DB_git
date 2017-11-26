/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：adc.c
 * 描述    ：adc应用函数库        
 * 实验平台：野火STM32开发板
 * 硬件连接：----------------------------------------------------
 *          |                                                    |
 *          | PC1 - ADC1 连接外部电压(通过一个滑动变阻器分压而来)|
 *          |                                                    |
 *           ----------------------------------------------------
 * 库版本  ：ST3.5.0 
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/


#include "adc.h"
#include "stdio.h"
#include "stm32_dsp.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"


#define ADC1_DR_Address    ((u32)0x40012400+0x4c)
#define ADC3_DR_Address    ((u32)ADC3_BASE+0x4c)

// ADC1转换的电压值通过MDA方式传到SRAM
uint16_t ADCBuffer[1];//65536

/*
 * 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PC.00
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure PC.00 ~ PC3  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOF, &GPIO_InitStructure);				// 
}


/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为DMA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
void ADC1_Mode_Config(uint8_t ADC_Channel)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 5;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 5;	 	//要转换的通道数目1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的6分频，即12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	/*配置ADC1的通道10为55.	5个采样周期，序列为1 */ 
	//ADC_Channel_10 -> VDD; ADC_Channel_11 -> VIO; ADC_Channel_12 -> VTX; ADC_Channel_13 -> VLED;
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_1Cycles5);		//(13.5 + 12.5)/12 = 26/12 = 3us
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 3, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 5, ADC_SampleTime_28Cycles5);

	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



void ADC1_Mode_Configg(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_TIMES*ADCCHANLE_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
	DMA_DeInit(DMA2_Channel5);//ADC 3 DMA
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	 //ADC3地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_TIMES*ADC3_CHANLE_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel5, ENABLE);
	
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADCCHANLE_NUM;	 	//要转换的通道数目9
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//ADC 3 
	ADC_InitStructure.ADC_NbrOfChannel = ADC3_CHANLE_NUM;	 	//要转换的通道数目4   
	ADC_Init(ADC3, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	/*配置ADC1的通道10为55.	5个采样周期，序列为1 */ 
	//ADC_Channel_10 -> VDD; ADC_Channel_11 -> VIO; ADC_Channel_12 -> VTX; ADC_Channel_13 -> VLED;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 6, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 7, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 8, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 9, ADC_SampleTime_28Cycles5);
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 2, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 3, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 4, ADC_SampleTime_28Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	
	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);
	
	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC3);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC3));
	/* ADC校准 */
	ADC_StartCalibration(ADC3);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC3));
	
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}



void Fricker_adc_mode(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
		/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;	//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//要转换的通道数目9
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道10为55.	5个采样周期，序列为1 (13.5 + 12.5)/12 = 26/12 = 3us*/ 
	//ADC_Channel_10 -> VDD; ADC_Channel_11 -> VIO; ADC_Channel_12 -> VTX; ADC_Channel_13 -> VLED;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);//(28.5 + 12.5)/9 = 4.5us

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void ADC1_Init(void)
{
	ADC_GPIO_Config();
	Fricker_adc_mode();
}

uint16_t get_adc_value(void)
{
	return ADCBuffer[0];
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/






