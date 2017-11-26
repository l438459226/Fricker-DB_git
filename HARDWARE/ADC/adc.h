#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"


#define ADC3_CHANLE_NUM			4
#define ADCCHANLE_NUM			9
#define ADC_TIMES					16

#define ADC_REF		2.5


void ADC1_Init(void);

uint16_t get_adc_value(void);


#endif /* __ADC_H */

