#ifndef _I2C1_BITBANG_H_
#define _I2C1_BITBANG_H_

#include "stm32f10x.h"

#define I2C_OK 0
#define I2C_ERR -1	

void i2c1_init(void);
int i2c1_send_start(void);
int i2c1_start(u8 sla_adr);
int i2c1_start_repeat(u8 sla_adr);
int i2c1_stop(void);
int i2c1_write_u8(u8 data);
int i2c1_read_u8(u8 *data, u8 nak);
int i2c1_write_data(const u8 *data, u32 nr_of_bytes);
int i2c1_read_data(u8 *data, u32 nr_of_bytes, u8 nak_last_byte);
void i2c1_anti_death(void);

#endif
/* _I2C1_BITBANG_H_ */
