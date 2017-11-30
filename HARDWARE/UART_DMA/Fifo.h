/*********************************************************************************************************
* 文件名称: Fifo.h
* 文件作者:相信未来
* 当前版本: 01.00.00
* 所属模块: Cth
* 创建日期: 2016年03月22日
* 文件摘要: 
* 注意事项: 
* 更改说明: 初始创建
**********************************************************************************************************/
#ifndef  _FIFO_H
#define  _FIFO_H


/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "sys.h"
#include "Uart.h"



/*********************************************************************************************************
*                                              宏定义定义
*********************************************************************************************************/
#define ERR_NO_SPACE			0xff
	
	
#define FIFO_OK                  0
#define FIFO_ERROR_PARAM        -1
#define FIFO_ERROR_MEM          -2
#define FIFO_ERROR_FULL         -3
#define FIFO_ERROR_EMPTY        -4
#define FIFO_ERROR_BUSY         -5



/*********************************************************************************************************
*                                              结构体定义
*********************************************************************************************************/
typedef struct
{
    u32 size;                        //FIFO缓冲区大小
    u32 front;                       //FIFO下一读取位置
    u32 staraddr;                    //FIFO缓冲区起始地址
    u32 endaddr;                     //FIFO缓冲区结束地址
    u8  buffer[RECEVIE_BUFFER_MAX_SIZE];     //实际长度由初始化分配内存!(memloc的时候确定)
    //u8  *buffer;
}FIFOTYPE;





/*********************************************************************************************************
*                                              API函数定义
*********************************************************************************************************/

u32 FIFO_Init(FIFOTYPE * *fifo,u32 fifosize);
u32 FIFO_Read(FIFOTYPE *fifo,u8 *data,u32 len);









#endif



