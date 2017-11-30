/*********************************************************************************************************
* �ļ�����: Uart.h
* �ļ�����:����δ��
* ��ǰ�汾: 01.00.00
* ����ģ��: Cth
* ��������: 2016��03��22��
* �ļ�ժҪ: 
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/
#ifndef  _UART_H
#define  _UART_H


/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "sys.h"
#include "stm32f10x.h"



/*********************************************************************************************************
*                                              �궨�嶨��
*********************************************************************************************************/
//һ��ʼҪ�������õ�оƬ���������ж��壬��mdk��������Options->c/c++->Preprocsor Symbols
//������stm32f10x.h��46������

//STM32F10X_LD С������ָ����洢��������16K��32K�ֽ�֮�䡣2������
//STM32F10X_MD ��������ָ����洢��������64K��128K�ֽ�֮�䡣3������
//STM32F10X_HD ��������ָ����洢��������256K��512K�ֽ�֮�䡣5������

//�������hex ��ramռ�ô�С������ͨ����startup.c  �ļ��е�heap�вü�
//heap����dma���ζ���ʹ�ã�ʵ������С(16+SEND_BUFFER_MAX_SIZE)*4 = 144*4 =576��


#ifdef STM32F10X_HD		
#define USART_COM1_EN   0			//����1ʹ��
#define USART_COM2_EN   1
#define USART_COM3_EN   0
#define USART_COM4_EN   0			
#define USART_COM5_EN   0
#endif

#ifdef STM32F10X_MD		
#define USART_COM1_EN   1			//����1ʹ��
#define USART_COM2_EN   1
#define USART_COM3_EN   1
#endif

#ifdef STM32F10X_LD		
#define USART_COM1_EN   1			//����1ʹ��
#define USART_COM2_EN   1
#endif



#define USART_COM1_BARD		115200
#define USART_COM2_BARD		115200
#define USART_COM3_BARD		115200
#define USART_COM4_BARD		115200
#define USART_COM5_BARD		115200











/*�ڵ�ǰ����û������������ʱ�����ڻ����е����������*/
#define	SEND_BUFFER_MAX_SIZE		250		
#define	RECEVIE_BUFFER_MAX_SIZE		250		//DMA��FIFO���ջ�������С




typedef enum
{

	USART_PORT_COM1,
	USART_PORT_COM2,
	USART_PORT_COM3,
	USART_PORT_COM4,
	USART_PORT_COM5,
	USART_PORT_COM_MAX,	
	
}USART_PORT_COMX;









/*********************************************************************************************************
*                                              API��������
*********************************************************************************************************/

void InitUart(void);

void USARTx_Timer_Proc(void);		//�Ŷ�ʱ���У�1msִ��һ��



//���Ժ��һ�½���

//д�봮�����ݣ�Usart_Comx = USART_PORT_COM5,��ʱ���ܷ����ж��У���ֹ�������жϷ���Ƕ�׵�������
//��ʧ���ߴ�������port�õĶ���dma�����Է��������ж���
u8 WriteUart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len);

//��ȡ�������ݣ�Ҫ��֤ÿ4ms��֮��������һ�Σ���ֹ���ݸ���
//Usart_Comx = USART_PORT_COM5,��ʱ���ܷ����ж��У���ֹ�������жϷ���Ƕ�׵�������
//��ʧ���ߴ�������port�õĶ���dma�����Է��������ж���
u8	ReadUart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len); 

//�����ȡ����д�붼���������������У��������ҲҪ������������


void USARTx_Test(USART_PORT_COMX Usart_Comx);			//�����Է����ղ��ԣ�Ҫ��֤4ms����








#endif


