/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��adc.c
 * ����    ��adcӦ�ú�����        
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�----------------------------------------------------
 *          |                                                    |
 *          | PC1 - ADC1 �����ⲿ��ѹ(ͨ��һ��������������ѹ����)|
 *          |                                                    |
 *           ----------------------------------------------------
 * ��汾  ��ST3.5.0 
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/


#include "adc.h"
#include "stdio.h"
#include "stm32_dsp.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"


#define ADC1_DR_Address    ((u32)0x40012400+0x4c)
#define ADC3_DR_Address    ((u32)ADC3_BASE+0x4c)

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
uint16_t ADCBuffer[1];//65536

/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PC.00
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
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


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪDMAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
void ADC1_Mode_Config(uint8_t ADC_Channel)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 5;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 5;	 	//Ҫת����ͨ����Ŀ1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��6��Ƶ����12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	/*����ADC1��ͨ��10Ϊ55.	5���������ڣ�����Ϊ1 */ 
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
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



void ADC1_Mode_Configg(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_TIMES*ADCCHANLE_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
	DMA_DeInit(DMA2_Channel5);//ADC 3 DMA
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	 //ADC3��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_TIMES*ADC3_CHANLE_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel5, ENABLE);
	
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = ADCCHANLE_NUM;	 	//Ҫת����ͨ����Ŀ9
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//ADC 3 
	ADC_InitStructure.ADC_NbrOfChannel = ADC3_CHANLE_NUM;	 	//Ҫת����ͨ����Ŀ4   
	ADC_Init(ADC3, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����12MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
	/*����ADC1��ͨ��10Ϊ55.	5���������ڣ�����Ϊ1 */ 
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
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	
	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);
	
	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC3);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC3));
	/* ADCУ׼ */
	ADC_StartCalibration(ADC3);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC3));
	
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCBuffer;//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;	//
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�̶�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//Ҫת����ͨ����Ŀ9
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��10Ϊ55.	5���������ڣ�����Ϊ1 (13.5 + 12.5)/12 = 26/12 = 3us*/ 
	//ADC_Channel_10 -> VDD; ADC_Channel_11 -> VIO; ADC_Channel_12 -> VTX; ADC_Channel_13 -> VLED;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);//(28.5 + 12.5)/9 = 4.5us

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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






