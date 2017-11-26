#ifndef __LUXDB_H
#define __LUXDB_H

#include "GY2561.h"

//  defaultvcom  basevcom  
struct db_data{
	u8 current;
	u8 first;
	u8 second;
	u8 cvcom;
	u8 fvcom;
	u8 svcom;
	u8 flag;		//向前 或向后
} ;
struct GY_DB{
	double gyper;
	u8 db;
};




float get_dbdb(u8 num);
double getLux(u8 gain, u16 ms);
//double get_db(u8 num);
void check_db(void);
double get_luxdb(u8 num);
float transformdb(double perlux);
u8 get_bestvcom(u8 base,u8 begin,u8 end);
u8 getbest_vcom(u8 base,u8 begin,u8 end);
u8 get_whilebestvcom(u8 base,u8 begin);
float gain_db(u8 num);
u8 lcm_search_vcom(void);



#endif





