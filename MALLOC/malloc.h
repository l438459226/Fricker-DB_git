#ifndef __MALLOC_H
#define __MALLOC_H

#include "stm32f10x.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

/*
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
*/
#ifndef NULL
#define NULL 0
#endif

#define SRAMIN	0	//�ڲ��ڴ��


//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����
#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			54*1024  						//�������ڴ� 40K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ����С

//�ڴ����������
struct _m_mallco_dev
{
	void (*init)(u8);					//��ʼ��
	u8 (*perused)(u8);		  	    	//�ڴ�ʹ����
	u8 	*membase[1];					//�ڴ�� ����2��������ڴ�
	u16 *memmap[1]; 					//�ڴ����״̬��
	u8  memrdy[1]; 						//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mymemset(void *s,u8 c,u32 count);	 //�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void mem_init(u8 memx);					 //�ڴ������ʼ������(��/�ڲ�����)
u32 mem_malloc(u8 memx,u32 size);		 //�ڴ����(�ڲ�����)
u8 mem_free(u8 memx,u32 offset);		 //�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(u8 memx);				 //����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(u8 memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u8 memx,u32 size);			//�ڴ����(�ⲿ����)
void *myrealloc(u8 memx,void *ptr,u32 size);//���·����ڴ�(�ⲿ����)
#endif












