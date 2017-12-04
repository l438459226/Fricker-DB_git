/*********************************************************************************************************
* �ļ�����: Fifo.c
* �ļ�����:����δ��
* ��ǰ�汾: 01.00.00
* ����ģ��: Cth
* ��������: 2016��03��22��
* �ļ�ժҪ: Fifo���еײ�����
* ע������: 
* ����˵��: ��ʼ����
**********************************************************************************************************/

/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "Common.h"
#include "Fifo.h"
#include "Uart.h"
#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include <stdlib.h>









/*********************************************************************************************************
*                                              ��������
*********************************************************************************************************/





/*******************************************************************************
* �ļ���	    : FIFO_Init
* ����	           : FIFO��ʼ��
* ����           : 
* ���           : ��
* ����           : ��
*******************************************************************************/
i32 FIFO_Init(FIFOTYPE * *fifo,u32 fifosize)
{
    volatile u32 da;
    if(fifo==NULL || fifosize == 0)
    {
		return FIFO_ERROR_PARAM;
    }
    (*fifo) = (FIFOTYPE *)malloc(16+fifosize);
    if((*fifo) == NULL)                
    {
            //�Ѿ��ڶ����������˵�ַ
		return FIFO_ERROR_MEM;
    }
    (*fifo)->size = fifosize;
    (*fifo)->staraddr = (u32)(&(*fifo)->buffer[0]);                //��¼FIFO��������ʼ��ַ
    (*fifo)->endaddr = (u32)(&(*fifo)->buffer[fifosize-1]);        //��¼FIFO������������ַ
    (*fifo)->front = (*fifo)->staraddr;                            //FIFO��һ��ȡ���ݵ�ַ
    memset((*fifo)->buffer,0,(*fifo)->size);                       //�����������������ݣ���ʡ��
    
    return FIFO_OK;
}



/*******************************************************************************
* �ļ���	    : FIFO_Status
* ����	           : ��ȡFIFO����״̬
* ����           : 
* ���           : ��
* ����           : ��
*******************************************************************************/
i32 FIFO_Status(FIFOTYPE *fifo)
{
        i32 res;
        i32 nextsave = (i32)fifo->endaddr + 1 - (i32)DMA1_Channel5->CNDTR;
        res = nextsave- (i32)(fifo->front);
        if(res < 0)
        {
                res = ( (i32)(fifo->endaddr)+1 - (i32)(fifo->front) ) + (nextsave - (i32)fifo->staraddr);
        }
        return res;
}




/*******************************************************************************
* �ļ���	    : FIFO_Read
* ����	           : ��FIFO�����ж���1byte����
* ����           : 
* ���           : ��
* ����           : ��
*******************************************************************************/
i32 FIFO_Read(FIFOTYPE *fifo,u8 *data,u32 len)
{


	u8 i,dlen,rLen=0;

	if(len > 0)
	{
		dlen = FIFO_Status(fifo);
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
	return rLen;	
    
}
