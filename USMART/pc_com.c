#include "ssd2828.h"
#include "usart.h"	
#include "led.h"
#include "stdio.h"

extern u8 mbuffer[128];

void pc_ssd_rdata(u8 reg)
{
		u16 parmer;
		parmer = ssd_rdata(reg);
		printf("0x%X",parmer);
}

void pc_DCS_Short_Read_NP(u16 cmd, u8 cnt, u8 *val)
{
	u8 i;
	for(i=0;i<cnt;i++)
		mbuffer[i] = 0x00;
	DCS_Short_Read_NP(cmd,cnt,mbuffer);
	//W_REG(0x00b7,0x03,0x0B);
	printf("R:");
	for(i=0;i<cnt;i++)
				printf("%X  ",mbuffer[i]);
}

void  PC_GP_R(uint8_t adr, uint16_t l, uint8_t *p)
{
	u8 i;
	for(i=0;i<l;i++)
		mbuffer[i] = 0x00;
	GP_R(adr,l,mbuffer);
	printf("R:");
	for(i=0;i<l;i++)
			printf("%X  ",mbuffer[i]);
}


int readlin(char *buff,u8 *valu)
{//ssdr 0x34 0x43 0x44 0x66
	char *buf = buff;
	int argc = 0;
	u8 i;
	char *argv[128];
	int state = 0;
	
	while (*buf)
	{
		if (*buf != ',' && state == 0)
		{
			argv[argc++] = buf;
			state = 1;
		}
		if (*buf == ',' && state == 1)
		{
			*buf = '\0';
	  	state = 0;
		}
		buf++;		
	}
	
	for(i=0;i<argc;i++)
	{
		valu[i] = atoi(argv[i]);
	}
	
	return argc;
}

void PC_DCS_Long_Write_FIFO(u16 NUM,char *P)
{
	u8 i;
	
	for(i=0;i<NUM;i++)
		mbuffer[i] = 0x00;

	printf("%s ",P);
	readlin(P,mbuffer);
	
	printf("WFIFO:  ");
	for(i=0;i<NUM;i++)
			printf("%X  ",mbuffer[i]);
	
	DCS_Long_Write_8FIFO(NUM,mbuffer);
}

void PC_Generic_Long_Write_FIFO(u16 NUM,char *P)
{
	u8 i;
	
	for(i=0;i<NUM;i++)
		mbuffer[i] = 0x00;
	
	printf("%s ",P);
	readlin(P,mbuffer);
	
	printf("WFIFO:  ");
	for(i=0;i<NUM;i++)
			printf("%X  ",mbuffer[i]);
	
	Generic_Long_Write_8FIFO(NUM,mbuffer);
}








