

#include "sys.h"
#include "delay.h"

#define TL35825_REG0	0x00	//set vp
#define TL35825_REG1	0x01	//set vn
#define TL35825_REG2	0x02
#define TL35825_CMDR	0x04


u8 TL35825_Write(u8 reg,u8 date);
u8 TL35825_Read(u8 reg);
void Set_Voutp(float voltage);
void Set_Voutn(float voltage);

void LT35825_Init(void);

void Voutp_Upfirst(void);
void Voutn_Upfirst(void);
void Voutn_UpBoth(void);
void Vout_Off(void);










