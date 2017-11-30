/*********************************************************************************************************
* 文件名称: Fifo.c
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: Cth
* 创建日期: 2016年03月22日
* 文件摘要: Fifo队列底层驱动
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "Common.h"
#include "Fifo.h"
#include "Uart.h"
#include <stdarg.h>
#include <stdio.h>
#include "string.h"
#include <stdlib.h>









/*********************************************************************************************************
*                                              变量定义
*********************************************************************************************************/





/*******************************************************************************
* 文件名	    : FIFO_Init
* 描述	           : FIFO初始化
* 输入           : 
* 输出           : 无
* 返回           : 无
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
            //已经在堆里面申请了地址
		return FIFO_ERROR_MEM;
    }
    (*fifo)->size = fifosize;
    (*fifo)->staraddr = (u32)(&(*fifo)->buffer[0]);                //记录FIFO缓冲区起始地址
    (*fifo)->endaddr = (u32)(&(*fifo)->buffer[fifosize-1]);        //记录FIFO缓冲区结束地址
    (*fifo)->front = (*fifo)->staraddr;                            //FIFO下一读取数据地址
    memset((*fifo)->buffer,0,(*fifo)->size);                       //清除缓冲区里面的数据，可省略
    
    return FIFO_OK;
}



/*******************************************************************************
* 文件名	    : FIFO_Status
* 描述	           : 获取FIFO队列状态
* 输入           : 
* 输出           : 无
* 返回           : 无
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
* 文件名	    : FIFO_Read
* 描述	           : 从FIFO队列中读出1byte数据
* 输入           : 
* 输出           : 无
* 返回           : 无
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

