#ifndef __TPS65132__
#define __TPS65132__

#include "stm32f10x.h"

int tps65132_set_voltage(uint16_t voltage_mV,uint8_t is_Vneg);
int tps65132_set_mode(uint8_t apps, uint8_t vpos_discharg, uint8_t vneg_discharge);

int Tps65132_Init(u16 vsp, u16 vsn);


#endif
