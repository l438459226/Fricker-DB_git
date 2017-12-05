/*********************************************************************************************************
* �ļ�����: Uart.c
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
*                                              �궨�嶨��
*********************************************************************************************************/	
#define USART1_DR_BASE_ADDRESS		(u32)(USART1_BASE+4)
#define USART2_DR_BASE_ADDRESS		(u32)(USART2_BASE+4)
#define USART3_DR_BASE_ADDRESS		(u32)(USART3_BASE+4)
#define UART4_DR_BASE_ADDRESS		  (u32)(UART4_BASE+4)


/*********************************************************************************************************
*                                              �ṹ�嶨��
*********************************************************************************************************/

typedef struct
{
	u16 NoSendCnt;			//δ��������
	u8 	FirstSendFlag; 		//�״η����ź���	
	u8 	NoSendBuffer[SEND_BUFFER_MAX_SIZE]; //���Ͷ˶������棬�洢����δ���ü�����ȥ������				 
	
}UsartxSend;



typedef struct
{
	DMA_Channel_TypeDef *DMA_RX_CHN;				//DMA ����ͨ��
	DMA_Channel_TypeDef *DMA_TX_CHN;				//DMA����ͨ��
	u32 USART_DR_BASE_ADDR;
	USART_TypeDef * USART_PORT;
	u32 USART_BAUD;
	u32 USART_DMA_TCx_FLAG;
	
}UsartxConfig;



#define ENABLE_UART2_IDLE_IRQ



/*********************************************************************************************************
*                                              ��������
*********************************************************************************************************/
static FIFOTYPE * sFifo_Comx[USART_PORT_COM5];		//������սṹ��,USART_PORT_COM5��֧��dmaģʽ

u32 ReadPoint = 0;	//����USART����



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
* �ļ���	    : USARTxPinConfiguration
* ����	           : �����������ú���
* ����           : Usart_Comx:�˿ں�
* ���           : ��
* ����           : 1--ִ�гɹ���0--ʧ��
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
	 			 //����USART1��GPIO��USART1-Tx(PA.09)/USART1-Rx(PA.10)
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
	 			 //����USART2��GPIO��USART2-Tx(PA.02)/USART2-Rx(PA.03)
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
	 			 //����USART3��GPIO��USART3-Tx(PB.10)/USART3-Rx(PB.11)
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
	 			 //����UART4��GPIO��UART4-Tx(PC.10)/UART4-Rx(PC.11)
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
	 			 //����UART5��GPIO��UART5-Tx(PC.10)/UART5-Rx(PD.2)
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
* �ļ���	    : USART1_DMA_Rx_init
* ����	           : ����1DMA���ճ�ʼ��
* ����           : Usart_Comx:�˿ںţ�RxBuffer :���ջ�������num :���ջ�������С
* ���           : ��
* ����           : 1--ִ�гɹ���0--ʧ��
*******************************************************************************/
u8 USARTx_DMA_Rx_init(USART_PORT_COMX Usart_Comx,u8 *RxBuffer,u16 num)
{
	u8 ok = 0;	
	DMA_InitTypeDef    DMA_Initstructure;
	

	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;

		/*����DMAʱ��*/
		if(Usart_Comx < USART_PORT_COM4)
		{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE); 
		}
		else
		{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE); 
		}

		//���ݲ�ͬ��������
		DMA_DeInit((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN);
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(UsartxConfigManage+Usart_Comx)->USART_DR_BASE_ADDR;
		//DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)&(USART2->DR);
		
		//���д��ڹ�������
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

		//���ݲ�ͬ��������
		DMA_Init((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN,&DMA_Initstructure);
		DMA_Cmd((UsartxConfigManage+Usart_Comx)->DMA_RX_CHN,ENABLE);		
	}

	return ok;
   
}


/*******************************************************************************
* �ļ���	    : USARTx_DMA_Tx_init
* ����	           : ����1DMA���ͳ�ʼ��
* ����           : Usart_Comx:�˿ںţ�TxBuffer :���ͻ�������num :���ͻ�������С
* ���           : ��
* ����           : 1--ִ�гɹ���0--ʧ��
*******************************************************************************/
u8 USARTx_DMA_Tx_init(USART_PORT_COMX Usart_Comx,u8 *TxBuffer,u16 num)
{
	u8 ok = 0;	
    DMA_InitTypeDef DMA_InitStructure; 
	

	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	

		//���ݲ�ͬ��������
		DMA_DeInit((UsartxConfigManage+Usart_Comx)->DMA_TX_CHN);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(UsartxConfigManage+Usart_Comx)->USART_DR_BASE_ADDR;
		//DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&(USART2->DR);

		//���д��ڹ�������
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

		
		//���ݲ�ͬ��������
		DMA_Init((UsartxConfigManage+Usart_Comx)->DMA_TX_CHN, &DMA_InitStructure);
		DMA_Cmd((UsartxConfigManage+Usart_Comx)->DMA_TX_CHN, ENABLE);
	}

	return ok;

}


/*******************************************************************************
* �ļ���	    : FIFO_Init
* ����	           : FIFO��ʼ��
* ����           : 
* ���           : ��
* ����           : 1--ִ�гɹ���0--ʧ��
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
	        //�Ѿ��ڶ����������˵�ַ
			return 0;
	    }
	    (*fifo)->size = fifosize;
	    (*fifo)->staraddr = (u32)(&(*fifo)->buffer[0]);                //��¼FIFO��������ʼ��ַ
	    (*fifo)->endaddr = (u32)(&(*fifo)->buffer[fifosize-1]);        //��¼FIFO������������ַ
	    (*fifo)->front = (*fifo)->staraddr;                            //FIFO��һ��ȡ���ݵ�ַ
	    ReadPoint = (*fifo)->staraddr;
	    memset((*fifo)->buffer,0,(*fifo)->size);                       //�����������������ݣ���ʡ��

		USARTx_DMA_Rx_init(Usart_Comx,(*fifo)->buffer,RECEVIE_BUFFER_MAX_SIZE);	//����rx dma����
	}
    
    return ok;
}



/*******************************************************************************
* �ļ���	    : USARTx_Rx_Get_FIFO_Status
* ����	           : ��ȡFIFO����״̬
* ����           : 
* ���           : ��
* ����           : ��
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
* �ļ���	    : USARTx_Rx_Read_FIFO
* ����	           : ��FIFO�����ж���1byte����
* ����           : 
* ���           : ��
* ����           : ��
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
* �ļ���	    : USARTx_Rx_Read_FIFO
* ����	           : ��FIFO�����ж���1byte����
* ����           : 
* ���           : ��
* ����           : ��
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
* �ļ���	    : UARTx_Configuration
* ����	           : Usart_Comx:�˿ں�
* ����           : ��
* ���           : ��
* ����           : 1--ִ�гɹ���0--ʧ��
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
		USARTxPinConfiguration(Usart_Comx);		//�˿ڣ���������
		
		
		if(Usart_Comx == USART_PORT_COM2)
		{
			    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
					NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;//��ռ���ȼ�3
					NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//�����ȼ�3
					NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
					NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
			
					
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
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�����ж�
#endif
		USARTx_Rx_Init_FIFO(Usart_Comx,&sFifo_Comx[Usart_Comx],RECEVIE_BUFFER_MAX_SIZE);			//����dma����fifo��ʼ��
		

		
	}
	else if(Usart_Comx == USART_PORT_COM5)
	{
		//����5����
		USARTxPinConfiguration(USART_PORT_COM5);	
		USART_StructInit(&USART_InitStructure);	
		USART_InitStructure.USART_BaudRate = USART_COM5_BARD;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	    USART_InitStructure.USART_StopBits = USART_StopBits_1;			 
	    USART_InitStructure.USART_Parity = USART_Parity_No;
	    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
		USART_Init(UART5,&USART_InitStructure);

		//���ͽ��մ��ڶ����жϷ�ʽ��Uart5û��DMA��ʽ
		USART_ITConfig( UART5, USART_IT_RXNE, ENABLE );           //ʹ�ܴ��ڽ����ж�     
		USART_ITConfig( UART5, USART_IT_TC,  ENABLE );       	  //ʹ�ܴ��ڷ����ж� 

		USART_Cmd(UART5, ENABLE );  
	}
	return ok;
}


void USART2_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Clear = Clear,tt=0,dlen=0;
	
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)  //�����ж�		(���յ������ݱ���'\r'��β)
	{
			tt++;
			//printf("uart2 USART_IT_RXNE\r\n");
   } 
	 if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)  //�����ж�		(���յ������ݱ���'\r'��β)
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
		return 1;			//��Ӧ�Ĵ����Ѿ�����
	}

	return 0;
}




/*******************************************************************************
* �ļ���	    : USARTxSendPack
* ����	           : ��鷢�ͻ������Ĵ�С�����ռ��㹻���������͵����ݷ��뵽���ͻ���
				   ����ȥ,������������,��USART1WriteDataToBuffer��ͬ���ǣ���������
				   ����������Ҫָ���ļ���С�ģ���͸������ṩ�˷���.
* ����           : buffer�����͵����ݵ�ָ��
* ���           : ��
* ����           : ����д����ֽ���
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
		DISABLE_IRQ();	//�ٽ�α���
		
		for(i = 0;i < len; i++)
		{			
			if(sUsartxSendManage[Usart_Comx].FirstSendFlag)	//���͵�һ�κ���д�뻺����
			{
			 	WriteUartTxCycQue(Usart_Comx,buffer[i]);
			}			
		}

		
		if(sUsartxSendManage[Usart_Comx].FirstSendFlag == 0)	//һ��ʼֻ����һ��
		{
			 sUsartxSendManage[Usart_Comx].FirstSendFlag = 1;
			 USARTx_DMA_Tx_init(Usart_Comx,buffer,len);
		}	
		
		ENABLE_IRQ(); //�ٽ�α���
		
	}
	else if(Usart_Comx == USART_PORT_COM5)
	{
		//USART_ITConfig(UART5,USART_IT_TC,DISABLE);	  
		for(i = 0;i < len; i++)
		{			
			//if(sUart5_Send_Flag)	//���͵�һ�κ���д�뻺����
			//{
			 	WriteUartTxCycQue(USART_PORT_COM5,buffer[i]);
			//}			
		}

		//USART_ITConfig(UART5,USART_IT_TC,ENABLE);
		if(sUart5_Send_Flag == 0)	//һ��ʼֻ����һ��
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
* �ļ���	    : UART5_NVIC_Configuration
* ����	           : ����5�ж�����
* ����           : ��
* ���           : ��
* ����           : ��
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
	if(n != 0)	//�����л�������δ����
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

		//�������ݽ���
		if(Flag)
		{	
			WriteUartRxCycQue(UART_RX_QUE_COM5,USART_ReceiveData(UART5));
		}
		
	}
	if(USART_GetFlagStatus(UART5,USART_FLAG_TC)==SET)
	{
		USART_ClearFlag(UART5,USART_FLAG_TC);//TCIE,TE,RE		
		//�������ݷ���
		if(Flag)
		{	
			UART5SendUpdata();
		}
	}
	
	Flag = 1;	
		

}
#endif








/*******************************************************************************
* �ļ���	    : USARTx_DMA_Tx_init
* ����	           : ����1DMA���ͳ�ʼ��
* ����           : Usart_Comx:�˿ںţ�TxBuffer :���ͻ�������num :���ͻ�������С
* ���           : ��
* ����           : 1--ִ�гɹ���0--ʧ��
*******************************************************************************/
u8 USARTxSendUpdata(USART_PORT_COMX Usart_Comx)
{
	u8 n = 0,i = 0,ok = 0;

	if(Usart_Comx < USART_PORT_COM5)
	{
		ok = 1;	
		n = CountUartTxCycQue(Usart_Comx);	//��ȡ���ڷ��Ͷ����е����ݣ����еĴ�С������UartQue.h��22��
		for(i = 0;i < n; i++)
		{
			if((i+sUsartxSendManage[Usart_Comx].NoSendCnt) < SEND_BUFFER_MAX_SIZE)	//�����Ͳ��ܳ�������
			{
				ReadUartTxCycQue(Usart_Comx,sUsartxSendManage[Usart_Comx].NoSendBuffer+i+sUsartxSendManage[Usart_Comx].NoSendCnt);
			}
			else			//δ���͵����ݳ����˻�����
			{
				return	0;	//�ڴ����
			}
		
		}
		
		sUsartxSendManage[Usart_Comx].NoSendCnt += n;
		
		
		if(DMA_GetFlagStatus((UsartxConfigManage+Usart_Comx)->USART_DMA_TCx_FLAG) == SET)
		{
			//DMA_ClearFlag(DMA1_FLAG_TC4);		//�������
			

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
* �ļ���	    : Uart_Init
* ����	           : ���ڳ�ʼ��
* ����           : ��
* ���           : ��
* ����           : ��
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
	
	InitUartTxCycQue();		//��ʼ�����ͻ���һ������
	
#if USART_COM1_EN
	USARTx_Configuration(USART_PORT_COM1);	
	sUsartx_Config_Flag[USART_PORT_COM1] = 1;		//����com1�������
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
	InitUartRxCycQue();			//COM5û����dma�������жϷ�ʽ
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
	u8 temp[80];		//4ms���ڣ���115200��ģʽ��������40���ֽڣ�ȡ˫������
	i32 len = 0;
	
	len = ReadUart(Usart_Comx,temp,10);
	if(len > 0)	
	WriteUart(Usart_Comx,temp,len);
}











