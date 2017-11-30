/*********************************************************************************************************
* 文件名称: UartQue.h
* 文件作者: 深圳大学智能软件模板开发团队
* 当前版本: 01.00.00
* 所属模块: Uart
* 创建日期: 2015年12月11日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/
#ifndef _UART_QUE_H_
#define _UART_QUE_H_

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "sys.h"
#include "Uart.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define	UARTRX_QUE_SIZE		300
#define	UARTTX_QUE_SIZE		300

/*********************************************************************************************************
*                                              枚举定义
*********************************************************************************************************/
typedef enum
{
	//UART_RX_QUE_COM1,
	//UART_RX_QUE_COM2,
	//UART_RX_QUE_COM3,
	//UART_RX_QUE_COM4,		
	UART_RX_QUE_COM5,
	UART_RX_QUE_MAX
}UartRxQue;		//Uart Rx Que

typedef enum
{
	UART_TX_QUE_COM1,		//COM1 uart tx que
	UART_TX_QUE_COM2,		//COM1 uart tx que
	UART_TX_QUE_COM3,		//COM1 uart tx que
	UART_TX_QUE_COM4,		//COM1 uart tx que
	UART_TX_QUE_COM5,		//COM1 uart tx que
	UART_TX_QUE_MAX
}UartTxQue;		//Uart Tx Que


/*********************************************************************************************************
*                                              API函数定义
*********************************************************************************************************/
void InitUartRxCycQue( void );
int ClearUartRxCycQue( int n );
int WriteUartRxCycQue( int n, u8 d );
int ReadUartRxCycQue( int n, u8 *p );
int CountUartRxCycQue( int n );
void InitUartTxCycQue( void );
int ClearUartTxCycQue( int n );
int WriteUartTxCycQue( int n, u8 d );
int ReadUartTxCycQue( int n, u8 *p );
int CountUartTxCycQue( int n );

#endif

