/*********************************************************************************************************
* �ļ�����: UartQue.h
* �ļ�����: ���ڴ�ѧ�������ģ�忪���Ŷ�
* ��ǰ�汾: 01.00.00
* ����ģ��: Uart
* ��������: 2015��12��11��
* �ļ�ժҪ: 
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/
#ifndef _UART_QUE_H_
#define _UART_QUE_H_

/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "sys.h"
#include "Uart.h"

/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/
#define	UARTRX_QUE_SIZE		300
#define	UARTTX_QUE_SIZE		300

/*********************************************************************************************************
*                                              ö�ٶ���
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
*                                              API��������
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

