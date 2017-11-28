#ifndef __AD715_H
#define	__AD715_H


#include "stm32f10x.h"




#define	VOUT_NONE	0x00	//此时AD715 没有选通
#define	TP_AVDD		0x11
#define	TP_ABUS		0x22
#define	LCD_ABUS	0x44
#define	LCD_AVDD	0x88




#define  OLED_AVDD_OFF     		GPIO_ResetBits(GPIOF, GPIO_Pin_10);		
#define  OLED_AVDD_ON    		GPIO_SetBits(GPIOF, GPIO_Pin_10);


#define  OLED_ELVDD_OFF     	GPIO_ResetBits(GPIOD, GPIO_Pin_7);		
#define  OLED_ELVDD_ON    		GPIO_SetBits(GPIOD, GPIO_Pin_7);




void ad715_Init(void);
int ad715_i2c_write(uint8_t val);



#endif /* __AD8557_H */

