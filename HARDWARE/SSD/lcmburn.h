#include "delay.h"


#define ILI9881C
//#define LCMBRUN


#define VCOM_BASE    0x34
#define LIGHTNUM  14			//采集亮度个数

extern void burn_otp(u8 best);
extern u8 Read_otp(void);
extern u8 after_otp_read(void);
extern u8 write_otp(u8 vcom);
extern u8 before_otp_read(void);
extern void yassylcd_init(void);



