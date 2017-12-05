/*********************************************************************************************************
* 文件名称: Uart.c
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: Cth
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/


/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "sys.h"
#include "Uart.h"
#include "Fifo.h"
#include <stdarg.h>
#include <stdio.h>
#include "usart.h"
#include "string.h"
#include <stdlib.h>
#include "UartQue.h"
#include "stm32f10x_dma.h"
#include "sys.h"
#include "usart.h"

/*********************************************************************************************************
*                                              宏定义定义
*********************************************************************************************************/	
#define USART1_DR_BASE_ADDRESS		(u32)(USART1_BASE+4)
#define USART2_DR_BASE_ADDRESS		(u32)(USART2_BASE+4)
#define USART3_DR_BASE_ADDRESS		(u32)(USART3_BASE+4)
#define UART4_DR_BASE_ADDRESS		  (u32)(UART4_BASE+4)


/*********************************************************************************************************
*                                              结构体定义
*********************************************************************************************************/

typedef struct
{
	u16 NoSendCnt;			//未发送数量
	u8 	FirstSendFlag; 		//首次发送信号量	
	u8 	NoSendBuffer[SEND_BUFFER_MAX_SIZE]; //发送端二级缓存，存储的是未来得及发出去的数据				 
	
}UsartxSend;



typedef struct
{
	DMA_Channel_TypeDef *DMA_RX_CHN;				//DMA 发送通道
	DMA_Channel_TypeDef *DMA_TX_CHN;				//DMA接收通道
	u32 USART_DR_BASE_ADDR;
	USART_TypeDef * USART_PORT;
	u32 USART_BAUD;
	u32 USART_DMA_TCx_FLAG;
	
}UsartxConfig;



#define ENABLE_UART2_IDLE_IRQ



/*********************************************************************************************************
*                                              变量定义
*********************************************************************************************************/
static FIFOTYPE * sFifo_Comx[USART_PORT_COM5];		//定义接收结构体,USART_PORT_COM5不支持dma模式

u32 ReadPoint = 0;	//用于USART助手



static UsartxSend sUsartxSendManage[USART_PORT_COM5];

static u8 sUart5_Send_Flag = 0;
static u8 sUsartx_Config_Flag[USART_PORT_COM_MAX];
static u8 sUsartx_Config_Nums = 0;




static const UsartxConfig  UsartxConfigManage[USART_PORT_COM_MAX] = 
{
	{DMA1_Channel5,DMA1_Channel4,USART1_DR_BASE_ADDRESS,USART1,USART_COM1_BARD,DMA1_FLAG_TC4},
	{DMA1_Channel6,DMA1_Channel7,USART2_DR_BASE_ADDRESS,USART2,USART_COM2_BARD,DMA1_FLAG_TC7},
	{DMA1_Channel3,DMA1_Channel2,USART3_DR_BASE_ADDRESS,USART3,USART_COM3_BARD,DMA1_FLAG_TC2},
	{DMA2_Channel3,DMA2_Channel5,UART4_DR_BASE_ADDRESS,UART4,USART_COM4_BARD,DMA2_FLAG_TC5},
};














/*******************************************************************************
* 文件名	    : USARTxPinConfiguration
* 描述	           : 串口引脚配置函数
* 输入           : Usart_Comx:端口号
* 输出           : 无
* 返回           : 1--执行成功，0--失败
*******************************************************************************/
u8 USARTxPinConfiguration(USART_PORT_COMX Usart_Comx)
{	
	u8 ok = 0;	
	GPIO_InitTypeDef GPIO_InitStructure;


	if(Usart_Comx < USART_PORT_COM_MAX)
	{
		ok = 1;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
		
		switch(Usart_Comx)  
		{
		#if USART_COM1_EN
			case USART_PORT_COM1:
				 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	 			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	 			 //配置USART1的GPIO：USART1-Tx(PA.09)/USART1-Rx(PA.10)
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;
	 			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 			 GPIO_Init( GPIOA, &GPIO_InitStructure );	
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	 			 GPIO_Init( GPIOA, &GPIO_InitStructure );
				 break;
		#endif
		#if USART_COM2_EN
			case USART_PORT_COM2:
				 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	 			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	 			 //配置USART2的GPIO：USART2-Tx(PA.02)/USART2-Rx(PA.03)
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	 			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 			 GPIO_Init( GPIOA, &GPIO_InitStructure );	
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	 			 GPIO_Init( GPIOA, &GPIO_InitStructure );
				 break;
		#endif
		#if USART_COM3_EN
			case USART_PORT_COM3:
				 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	 			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	 			 //配置USART3的GPIO：USART3-Tx(PB.10)/USART3-Rx(PB.11)
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	 			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 			 GPIO_Init( GPIOB, &GPIO_InitStructure );	
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	 			 GPIO_Init( GPIOB, &GPIO_InitStructure );
				 break;
		#endif
		#if USART_COM4_EN
			case USART_PORT_COM4:
				 RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	 			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	 			 //配置UART4的GPIO：UART4-Tx(PC.10)/UART4-Rx(PC.11)
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	 			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 			 GPIO_Init( GPIOC, &GPIO_InitStructure );	
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	 			 GPIO_Init( GPIOC, &GPIO_InitStructure );
				 break;
		#endif
		#if USART_COM5_EN
			case USART_PORT_COM5:
				 RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	 			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
				 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	 			 //配置UART5的GPIO：UART5-Tx(PC.10)/UART5-Rx(PD.2)
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	 			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 			 GPIO_Init( GPIOC, &GPIO_InitStructure );	
	 			 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	 			 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	 			 GPIO_Init( GPIOD, &GPIO_InitStructure );
				 break;
		#endif
			default:
				break;
		}
		
	}

	return ok;

}





/*******************************************************************************
* 文件名	    : USART1_DMA_Rx_init
* 描述	           : 串口1DMA接收初始化
* 输入           : Usart_Comx:端口号，RxBuffer :接收缓冲区，num :接收缓冲区大小
* 输出           : 无
* 返回           : 1--执行成功，0--失败
*******************************************************************************/
u8 USARTx_DMA_Rx_init(USART_PORT_COMX Usart_Comx,u8 *RxBuffer,u16 num)
{
	u8 ok = 0;	
	DMA_InitTypeDef    DMA_Initstructure;
	

	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;

		/*开启DMA时钟*/
		if(Usart_Comx < USART_PORT_COM4)
		{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
		}
		else
		{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE); 
		}

		//根据不同串口设置
		DMA_DeInit((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN);
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(UsartxConfigManage+Usart_Comx)->USART_DR_BASE_ADDR;
		//DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)&(USART2->DR);
		
		//所有串口公共设置
		DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)RxBuffer;
		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_Initstructure.DMA_BufferSize = num;
		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_Initstructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_Initstructure.DMA_Mode = DMA_Mode_Circular;
		DMA_Initstructure.DMA_Priority = DMA_Priority_High;
		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;

		//根据不同串口设置
		DMA_Init((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN,&DMA_Initstructure);
		DMA_Cmd((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN,ENABLE);		
	}

	return ok;
   
}


/*******************************************************************************
* 文件名	    : USARTx_DMA_Tx_init
* 描述	           : 串口1DMA发送初始化
* 输入           : Usart_Comx:端口号，TxBuffer :发送缓冲区，num :发送缓冲区大小
* 输出           : 无
* 返回           : 1--执行成功，0--失败
*******************************************************************************/
u8 USARTx_DMA_Tx_init(USART_PORT_COMX Usart_Comx,u8 *TxBuffer,u16 num)
{
	u8 ok = 0;	
    DMA_InitTypeDef DMA_InitStructure; 
	

	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	

		//根据不同串口设置
		DMA_DeInit((UsartxConfigManage+Usart_Comx)->DMA_TX_CHN);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(UsartxConfigManage+Usart_Comx)->USART_DR_BASE_ADDR;
		//DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&(USART2->DR);

		//所有串口公共设置
	    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuffer;
	    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	    DMA_InitStructure.DMA_BufferSize = num;
	    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		
		//根据不同串口设置
		DMA_Init((UsartxConfigManage+Usart_Comx)->DMA_TX_CHN, &DMA_InitStructure);
		DMA_Cmd((UsartxConfigManage+Usart_Comx)->DMA_TX_CHN, ENABLE);
	}

	return ok;

}


/*******************************************************************************
* 文件名	    : FIFO_Init
* 描述	           : FIFO初始化
* 输入           : 
* 输出           : 无
* 返回           : 1--执行成功，0--失败
*******************************************************************************/
u8 USARTx_Rx_Init_FIFO(USART_PORT_COMX Usart_Comx,FIFOTYPE * *fifo,u32 fifosize)
{
	u8 ok = 0;
    volatile u32 da;
	
	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
		
	    if(fifo==NULL || fifosize == 0)
	    {
			return 0;
	    }
	    (*fifo) = (FIFOTYPE *)malloc(16+fifosize);
	    if((*fifo) == NULL)                
	    {
	        //已经在堆里面申请了地址
			return 0;
	    }
	    (*fifo)->size = fifosize;
	    (*fifo)->staraddr = (u32)(&(*fifo)->buffer[0]);                //记录FIFO缓冲区起始地址
	    (*fifo)->endaddr = (u32)(&(*fifo)->buffer[fifosize-1]);        //记录FIFO缓冲区结束地址
	    (*fifo)->front = (*fifo)->staraddr;                            //FIFO下一读取数据地址
	    ReadPoint = (*fifo)->staraddr;
	    memset((*fifo)->buffer,0,(*fifo)->size);                       //清除缓冲区里面的数据，可省略

		USARTx_DMA_Rx_init(Usart_Comx,(*fifo)->buffer,RECEVIE_BUFFER_MAX_SIZE);	//串口rx dma设置
	}
    
    return ok;
}



/*******************************************************************************
* 文件名	    : USARTx_Rx_Get_FIFO_Status
* 描述	           : 获取FIFO队列状态
* 输入           : 
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 USARTx_Rx_Get_FIFO_Status(USART_PORT_COMX Usart_Comx,FIFOTYPE *fifo,u8 * len)
{	
	u8 ok = 0;
    i32 res,nextsave;
	
	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
        //i32 nextsave = (i32)fifo->endaddr + 1 - (i32)(UsartxConfigManage+Usart_Comx)->DMA_RX_CHN->CNDTR;
        
        nextsave = (i32)fifo->endaddr + 1 - DMA_GetCurrDataCounter((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN);
        res = nextsave- (i32)(fifo->front);
		
        if(res < 0)
        {
                res = ( (i32)(fifo->endaddr)+1 - (i32)(fifo->front) ) + (nextsave - (i32)fifo->staraddr);
        }
		
        *len =  (u8)res;
	}

	return ok;
}

u8 USARTx_Rx_GetFIFO_Status(USART_PORT_COMX Usart_Comx,FIFOTYPE *fifo,u8 * len)
{	
	u8 ok = 0;
    i32 res,nextsave;
	
	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
        //i32 nextsave = (i32)fifo->endaddr + 1 - (i32)(UsartxConfigManage+Usart_Comx)->DMA_RX_CHN->CNDTR;
        
        nextsave = (i32)fifo->endaddr + 1 - DMA_GetCurrDataCounter((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN);
        res = nextsave- (i32)(ReadPoint);
		
        if(res < 0)
        {
                res = ( (i32)(fifo->endaddr)+1 - (i32)(ReadPoint) ) + (nextsave - (i32)fifo->staraddr);
        }
		
        *len =  (u8)res;
	}

	return ok;
}

/*******************************************************************************
* 文件名	    : USARTx_Rx_Read_FIFO
* 描述	           : 从FIFO队列中读出1byte数据
* 输入           : 
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 USARTx_Rx_Read_FIFO(USART_PORT_COMX Usart_Comx,FIFOTYPE *fifo,u8 *data,u8 len,u8 *reallen)
{
	u8 i = 0,dlen = 0,rLen = 0,ok = 0;
	
	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
		if(len > 0)
		{
			USARTx_Rx_Get_FIFO_Status(Usart_Comx,fifo,&dlen);
			//printf("fifo data num:%d start:0x%x front:0x%x end:0x%x\r\n",dlen,fifo->staraddr,fifo->front,fifo->endaddr);
			if(dlen > 0)
			{
				rLen = len;
				if(dlen < len)
					rLen = dlen; 
				for(i=0;i<rLen;i++) 
				{
					*(data+i) = (u8)(*((u8 *)(fifo->front)));
					if(fifo->front == fifo->endaddr)
				    {
				        fifo->front = fifo->staraddr;
				    }
				    else
				    {
				        fifo->front++;
				    }
				}
			}
			else
			{
				rLen = dlen;
			}
		}
		*reallen = rLen;	
	}

	return ok;
    
}





/*******************************************************************************
* 文件名	    : USARTx_Rx_Read_FIFO
* 描述	           : 从FIFO队列中读出1byte数据
* 输入           : 
* 输出           : 无
* 返回           : 无
*******************************************************************************/
u8 USARTx_Rx_Read_NOTFIFO(USART_PORT_COMX Usart_Comx,FIFOTYPE *fifo,u8 *data,u8 len,u8 *reallen)
{
	u8 i = 0,dlen = 0,rLen = 0,ok = 0;
	
	
	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
		if(len > 0)
		{
			USARTx_Rx_GetFIFO_Status(Usart_Comx,fifo,&dlen);
			if(dlen > 0)
			{
				rLen = len;
				if(dlen < len)
					rLen = dlen; 
				
				//ReadPoint = fifo->front;
				for(i=0;i<rLen;i++) 
				{
					*(data+i) = (u8)(*((u8 *)(ReadPoint)));
					if(ReadPoint == fifo->endaddr)
				    {
				        ReadPoint = fifo->staraddr;
				    }
				    else
				    {
				        ReadPoint++;
				    }
				}
			}
			else
			{
				rLen = dlen;
			}
		}
		*reallen = rLen;	
	}

	return ok;
    
}



/*******************************************************************************
* 文件名	    : UARTx_Configuration
* 描述	           : Usart_Comx:端口号
* 输入           : 无
* 输出           : 无
* 返回           : 1--执行成功，0--失败
*******************************************************************************/
u8 USARTx_Configuration(USART_PORT_COMX Usart_Comx)
{
	u8 ok = 0;	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	USART_DeInit((UsartxConfigManage+Usart_Comx)->USART_PORT);
	
	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
		USARTxPinConfiguration(Usart_Comx);		//端口，引脚配置
		
		
		if(Usart_Comx == USART_PORT_COM2)
		{
			    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
					NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;//抢占优先级3
					NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//子优先级3
					NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
					NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
			
					
		}
		
		
		USART_StructInit(&USART_InitStructure);	
		USART_InitStructure.USART_BaudRate = (UsartxConfigManage+Usart_Comx)->USART_BAUD;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;			 
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
		USART_Init((UsartxConfigManage+Usart_Comx)->USART_PORT,&USART_InitStructure);
		
		USART_DMACmd((UsartxConfigManage+Usart_Comx)->USART_PORT,USART_DMAReq_Rx,ENABLE);
		USART_DMACmd((UsartxConfigManage+Usart_Comx)->USART_PORT,USART_DMAReq_Tx,ENABLE);
		USART_Cmd((UsartxConfigManage+Usart_Comx)->USART_PORT, ENABLE ); 
#ifdef ENABLE_UART2_IDLE_IRQ
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//开启中断
#endif
		USARTx_Rx_Init_FIFO(Usart_Comx,&sFifo_Comx[Usart_Comx],RECEVIE_BUFFER_MAX_SIZE);			//串口dma接收fifo初始化
		

		
	}
	else if(Usart_Comx == USART_PORT_COM5)
	{
		//串口5配置
		USARTxPinConfiguration(USART_PORT_COM5);	
		USART_StructInit(&USART_InitStructure);	
		USART_InitStructure.USART_BaudRate = USART_COM5_BARD;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	    USART_InitStructure.USART_StopBits = USART_StopBits_1;			 
	    USART_InitStructure.USART_Parity = USART_Parity_No;
	    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
		USART_Init(UART5,&USART_InitStructure);

		//发送接收串口都用中断方式，Uart5没有DMA方式
		USART_ITConfig( UART5, USART_IT_RXNE, ENABLE );           //使能串口接收中断     
		USART_ITConfig( UART5, USART_IT_TC,  ENABLE );       	  //使能串口发送中断 

		USART_Cmd(UART5, ENABLE );  
	}
	return ok;
}


void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t Clear = Clear,tt=0,dlen=0;
	
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)  //接收中断		(接收到的数据必须'\r'结尾)
	{
			tt++;
			//printf("uart2 USART_IT_RXNE\r\n");
   } 
	 if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)  //接收中断		(接收到的数据必须'\r'结尾)
	{
			Clear = USART2->SR;		//READ SR Reg
			Clear = USART2->DR;		//READ DR Reg

			USARTx_Rx_GetFIFO_Status(USART_PORT_COM2,sFifo_Comx[USART_PORT_COM2],&dlen);
			USART_RX_STA = dlen;
			USART_RX_STA |= 0x8000;
			
			Read_Uart(USART_PORT_COM2,USART_RX_BUF,dlen);

			
			
			printf("\r\nuart idle IRQ:%s,%d\r\n",USART_RX_BUF,dlen);
			
   } 
	 //printf("\r\n uart2 irq\r\n");
} 

u8 Get_Usartx_Config_Sts(USART_PORT_COMX Usart_Comx)
{	
	if(sUsartx_Config_Flag[Usart_Comx])	
	{
		return 1;			//对应的串口已经配置
	}

	return 0;
}




/*******************************************************************************
* 文件名	    : USARTxSendPack
* 描述	           : 检查发送缓冲区的大小，若空间足够，将待发送的数据放入到发送缓冲
				   区中去,并且启动发送,与USART1WriteDataToBuffer不同的是，启动发送
				   函数世不需要指定文件大小的，这就给调用提供了方便.
* 输入           : buffer待发送的数据的指针
* 输出           : 无
* 返回           : 返回写入的字节数
*******************************************************************************/
u8 WriteUart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len)
{
	u8 i = 0,temp = 0;


	if(Get_Usartx_Config_Sts(Usart_Comx) == 0)
	{
		return 0;
	}
	

	if(Usart_Comx < USART_PORT_COM5)
	{
		DISABLE_IRQ();	//临界段保护
		
		for(i = 0;i < len; i++)
		{			
			if(sUsartxSendManage[Usart_Comx].FirstSendFlag)	//发送第一次后再写入缓冲区
			{
			 	WriteUartTxCycQue(Usart_Comx,buffer[i]);
			}			
		}

		
		if(sUsartxSendManage[Usart_Comx].FirstSendFlag == 0)	//一开始只运行一次
		{
			 sUsartxSendManage[Usart_Comx].FirstSendFlag = 1;
			 USARTx_DMA_Tx_init(Usart_Comx,buffer,len);
		}	
		
		ENABLE_IRQ(); //临界段保护
		
	}
	else if(Usart_Comx == USART_PORT_COM5)
	{
		//USART_ITConfig(UART5,USART_IT_TC,DISABLE);	  
		for(i = 0;i < len; i++)
		{			
			//if(sUart5_Send_Flag)	//发送第一次后再写入缓冲区
			//{
			 	WriteUartTxCycQue(USART_PORT_COM5,buffer[i]);
			//}			
		}

		//USART_ITConfig(UART5,USART_IT_TC,ENABLE);
		if(sUart5_Send_Flag == 0)	//一开始只运行一次
		{
			 sUart5_Send_Flag = 1;
			 ReadUartTxCycQue(USART_PORT_COM5,&temp);
			 USART_SendData(UART5,temp);  
		}
	}

	return len;
	
}

void fifo_datanum(USART_PORT_COMX Usart_Comx, u8 dlen)
{
	USARTx_Rx_Get_FIFO_Status(Usart_Comx,sFifo_Comx[Usart_Comx],&dlen);
}



u8	ReadUart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len)
{
	u8 i = 0,rLen = 0, n = 0;

	if(Get_Usartx_Config_Sts(Usart_Comx) == 0)
	{
		return 0;
	}
	
	

	if(Usart_Comx < USART_PORT_COM5)                    
	{
		USARTx_Rx_Read_FIFO(Usart_Comx,sFifo_Comx[Usart_Comx],buffer,len,&rLen);
	}
	else if(Usart_Comx == USART_PORT_COM5)
	{
		
		n = CountUartRxCycQue(UART_RX_QUE_COM5);			
		rLen = n;		

		for(i = 0;i < n; i++)
		{			
			
			ReadUartRxCycQue(UART_RX_QUE_COM5,buffer+i);
					
		}
	}

	return rLen;
}



u8	Read_Uart(USART_PORT_COMX Usart_Comx, u8 *buffer, u8 len)
{
	u8 i = 0,rLen = 0, n = 0;

	if(Get_Usartx_Config_Sts(Usart_Comx) == 0)
	{
		return 0;
	}
	
	

	if(Usart_Comx < USART_PORT_COM5)                    
	{
		USARTx_Rx_Read_NOTFIFO(Usart_Comx,sFifo_Comx[Usart_Comx],buffer,len,&rLen);
	}
	else if(Usart_Comx == USART_PORT_COM5)
	{
		
		n = CountUartRxCycQue(UART_RX_QUE_COM5);			
		rLen = n;		

		for(i = 0;i < n; i++)
		{			
			
			ReadUartRxCycQue(UART_RX_QUE_COM5,buffer+i);
					
		}
	}

	return rLen;
}



#if	USART_COM5_EN


/*******************************************************************************
* 文件名	    : UART5_NVIC_Configuration
* 描述	           : 串口5中断配置
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
static void UART5_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  													
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART5_IRQ_PrePriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void UART5SendUpdata(void)
{
	u8 n = 0,buffer = 0;	
	n = CountUartTxCycQue(USART_PORT_COM5);	
	if(n != 0)	//队列中还有数据未发送
	{
		ReadUartTxCycQue(USART_PORT_COM5,&buffer);
		USART_SendData(UART5,buffer);  
	}
	else
	{
		sUart5_Send_Flag = 0;
	}
	
}




void UART5_IRQHandler(void)                
{
	static u8 Flag = 0;

	if(USART_GetFlagStatus(UART5,USART_FLAG_RXNE)==SET)
	{
		USART_ClearFlag(UART5,USART_FLAG_RXNE);			//TCIE,TE,RE	

		//下面数据接收
		if(Flag)
		{	
			WriteUartRxCycQue(UART_RX_QUE_COM5,USART_ReceiveData(UART5));
		}
		
	}
	if(USART_GetFlagStatus(UART5,USART_FLAG_TC)==SET)
	{
		USART_ClearFlag(UART5,USART_FLAG_TC);//TCIE,TE,RE		
		//下面数据发送
		if(Flag)
		{	
			UART5SendUpdata();
		}
	}
	
	Flag = 1;	
		

}
#endif








/*******************************************************************************
* 文件名	    : USARTx_DMA_Tx_init
* 描述	           : 串口1DMA发送初始化
* 输入           : Usart_Comx:端口号，TxBuffer :发送缓冲区，num :发送缓冲区大小
* 输出           : 无
* 返回           : 1--执行成功，0--失败
*******************************************************************************/
u8 USARTxSendUpdata(USART_PORT_COMX Usart_Comx)
{
	u8 n = 0,i = 0,ok = 0;

	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
		n = CountUartTxCycQue(Usart_Comx);	//读取存在发送队列中的数据，队列的大小定义在UartQue.h中22行
		for(i = 0;i < n; i++)
		{
			if((i+sUsartxSendManage[Usart_Comx].NoSendCnt) < SEND_BUFFER_MAX_SIZE)	//计数和不能超过缓冲
			{
				ReadUartTxCycQue(Usart_Comx,sUsartxSendManage[Usart_Comx].NoSendBuffer+i+sUsartxSendManage[Usart_Comx].NoSendCnt);
			}
			else			//未发送的数据超过了缓冲区
			{
				return	0;	//内存溢出
			}
		
		}
		
		sUsartxSendManage[Usart_Comx].NoSendCnt += n;
		
		
		if(DMA_GetFlagStatus((UsartxConfigManage+Usart_Comx)->USART_DMA_TCx_FLAG) == SET)
		{
			//DMA_ClearFlag(DMA1_FLAG_TC4);		//不能清除
			

			if(sUsartxSendManage[Usart_Comx].NoSendCnt != 0)
			{	
				USARTx_DMA_Tx_init(Usart_Comx,sUsartxSendManage[Usart_Comx].NoSendBuffer,sUsartxSendManage[Usart_Comx].NoSendCnt);
				sUsartxSendManage[Usart_Comx].NoSendCnt = 0;
			}
			
		}
	}

	return ok;	
	
}


/*******************************************************************************
* 文件名	    : Uart_Init
* 描述	           : 串口初始化
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void InitUart(void)
{

	u8 i = 0;

	for(i = 0;i < USART_PORT_COM_MAX;i++)
	{
		if(sUsartx_Config_Flag[i])
		{
			sUsartx_Config_Nums++;		
		}
	}
	
	InitUartTxCycQue();		//初始化发送缓冲一级队列
	
#if USART_COM1_EN
	USARTx_Configuration(USART_PORT_COM1);	
	sUsartx_Config_Flag[USART_PORT_COM1] = 1;		//表明com1配置完成
#endif

#if USART_COM2_EN
	USARTx_Configuration(USART_PORT_COM2);	
	sUsartx_Config_Flag[USART_PORT_COM2] = 1;
#endif

#if USART_COM3_EN
	USARTx_Configuration(USART_PORT_COM3);
	sUsartx_Config_Flag[USART_PORT_COM3] = 1;
#endif	

#if USART_COM4_EN
	USARTx_Configuration(USART_PORT_COM4);
	sUsartx_Config_Flag[USART_PORT_COM4] = 1;

#endif	

#if USART_COM5_EN
	InitUartRxCycQue();			//COM5没有用dma，采用中断方式
	USARTx_Configuration(USART_PORT_COM5);	
	UART5_NVIC_Configuration();
	sUsartx_Config_Flag[USART_PORT_COM5] = 1;
#endif	

}



void USARTx_Timer_Proc(void)
{
	static u8 cnt = 0;
	cnt++;	
	switch(cnt)
	{
		case 1:	
			#if USART_COM1_EN
			USARTxSendUpdata(USART_PORT_COM1);	
			#endif
			
			//USARTx_Test(USART_PORT_COM1);
			break;
		case 2:						
			#if USART_COM2_EN
			USARTxSendUpdata(USART_PORT_COM2);	
			#endif			
			break;
		case 3:					
			#if USART_COM3_EN
			USARTxSendUpdata(USART_PORT_COM3);	
			#endif			
			break;
		case 4:	
			cnt = 0;
			#if USART_COM4_EN
			USARTxSendUpdata(USART_PORT_COM4);	
			#endif			
			break;	
	}
}



void USARTx_Test(USART_PORT_COMX Usart_Comx)
{
	u8 temp[80];		//4ms周期，在115200的模式下最多会有40个字节，取双倍余量
	i32 len = 0;
	
	len = ReadUart(Usart_Comx,temp,10);
	if(len > 0)	
	WriteUart(Usart_Comx,temp,len);
}











