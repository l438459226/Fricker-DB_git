#ifndef _i2c11_BITBANG_H_
#define _i2c11_BITBANG_H_

#include "stm32f10x.h"
#include "myiic.h"


#define I2C_OK 0
#define I2C_ERR -1	

//void i2c11__init(void);


//int i2c11__send_start(void);
int i2c11__start(u8 sla_adr);


int i2c11__start_repeat(u8 sla_adr);
int i2c11__stop(void);	//i2c11__stop
int i2c11__write_u8(u8 data);
int i2c11__read_u8(u8 *data, u8 nak);
int i2c11__write_data(const u8 *data, u32 nr_of_bytes);
int i2c11__read_data(u8 *data, u32 nr_of_bytes, u8 nak_last_byte);
void i2c11__anti_death(void);

int i2c_rx_bit(u8 *bit);
int i2c_tx_bit(u8 bit);
#endif
/* _i2c11_BITBANG_H_ */
