#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	



#define TESTER_DBG_ON            1
#define TESTER_NET_DBG_ON        1
#define TESTER_MAIN_DBG_ON       1
#define TESTER_TP_DBG_ON         1
#define TESTER_UI_DBG_ON         1






#define TESTER_NET_DEBUG(format,...) do { \
																				 if ((TESTER_NET_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);
#define TESTER_MAIN_DEBUG(format,...) do { \
																				 if ((TESTER_MAIN_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);
#define TESTER_TP_DEBUG(format,...) do { \
																				 if ((TESTER_TP_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);
#define TESTER_UI_DEBUG(format,...) do { \
																				 if ((TESTER_UI_DBG_ON) && TESTER_DBG_ON){ \
																					 printf (format, ##__VA_ARGS__); \
																					} \
																			 } while(0);	










#define USART_REC_LEN  			256  	//�����������ֽ��� 200
//#define EN_USART1_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX	1

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void uart2_init(u32 bound);
#endif


