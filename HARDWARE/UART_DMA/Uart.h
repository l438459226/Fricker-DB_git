/*********************************************************************************************************
* 文件名称: Uart.h
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: Cth
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/
#ifndef  _UART_H
#define  _UART_H


/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "sys.h"
#include "stm32f10x.h"



/*********************************************************************************************************
*                                              宏定义定义
*********************************************************************************************************/
//一开始要根据所用的芯片对容量进行定义，在mdk界面设置Options->c/c++->Preprocsor Symbols
//或者在stm32f10x.h的46行设置

//STM32F10X_LD 小容量是指闪存存储器容量在16K至32K字节之间。2个串口
//STM32F10X_MD 中容量是指闪存存储器容量在64K至128K字节之间。3个串口
//STM32F10X_HD 大容量是指闪存存储器容量在256K至512K字节之间。5个串口

//编译出的hex 的ram占用大小，可以通过在startup.c  文件中的heap中裁剪
//heap供给dma环形队列使用，实际最大大小(16+SEND_BUFFER_MAX_SIZE)*4 = 144*4 =576。


#ifdef STM32F10X_HD		
#define USART_COM1_EN   0			//串口1使能
#define USART_COM2_EN   1
#define USART_COM3_EN   0
#define USART_COM4_EN   0			
#define USART_COM5_EN   0
#endif

#ifdef STM32F10X_MD		
#define USART_COM1_EN   1			//串口1使能
#define USART_COM2_EN   1
#define USART_COM3_EN   1
#endif

#ifdef STM32F10X_LD		
#define USART_COM1_EN   1			//串口1使能
#define USART_COM2_EN   1
#endif



#define USART_COM1_BARD		115200
#define USART_COM2_BARD		115200
#define USART_COM3_BARD		115200
#define USART_COM4_BARD		115200
#define USART_COM5_BARD		115200



//#define ENABLE_UART2_IDLE_IRQ







/*在当前数据没发完的情况下暂时保存在缓冲中的最大数据量*/
#define	SEND_BUFFER_MAX_SIZE		250		
#define	RECEVIE_BUFFER_MAX_SIZE		250		//DMA用FIFO接收缓冲区大小




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
*                                              API函数定义
*********************************************************************************************************/

void InitUart(void);

void USARTx_Timer_Proc(void);		//放定时器中，1ms执行一次



//测试后得一下结论

//写入串口数据，Usart_Comx = USART_PORT_COM5,的时候不能放在中断中，防止和其他中断发生嵌套导致数据
//丢失或者错误，其他port用的都是dma，可以放在其他中断中
u8 WriteUart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len);

//读取串口数据，要保证每4ms或之内能运行一次，防止数据覆盖
//Usart_Comx = USART_PORT_COM5,的时候不能放在中断中，防止和其他中断发生嵌套导致数据
//丢失或者错误，其他port用的都是dma，可以放在其他中断中
u8	ReadUart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len); 

u8	Read_Uart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len);

//建议读取或者写入都放在主函数中运行，特殊情况也要遵守上述规则


void USARTx_Test(USART_PORT_COMX Usart_Comx);			//串口自发自收测试，要保证4ms运行



//u8 USARTx_Rx_GetFIFO_Status(USART_PORT_COMX Usart_Comx,FIFOTYPE *fifo,u8 * len);





#endif


