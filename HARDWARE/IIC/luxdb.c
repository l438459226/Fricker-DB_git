#include "GY2561.h" 
#include "delay.h"
#include "math.h"
#include "usart.h"
#include "ssd2828.h"
#include "lcd.h"
#include "lcmburn.h"
#include "luxdb.h"

//								百分百
double const gydb[48]={54,34,28,24,22,19.23,17.47,15.38,13.35,12.2,11.11,9.19,8.47,7.62,6.75,5.65,5.14,4.70,4.54,3.71,3.54,2.49,2.44,2.38,2.25,1.75,1.67,1.42,1.28,1.27,1.26,1.25,1.245,1.24,1.23,1.22,1.20,1.18,1.17,1.16,1.00,0.70,0.60,0.40,0.30,0.25,0.20};
//					DB		0  1  2  3  4   5    6		  7	    8     9   10    11   12   13   14   15   16   17   18   19   20   21    22   23  24   25    27   28  29   30   31    32   33   34   35    36  37   38    39   40	41   42   43    44   45

u8 ssdwrite_vcom(u8 vcom)	//---
{
		u8 rvcom;
		rvcom = write_otp(vcom);
		delay_ms(96);
		return rvcom;
}	
	
float transformdb(double perlux)	//将测试值映射成DB值
{
		u8 i;
		for(i=0;i<47;i++){
			if(gydb[i] <= perlux)  return (i);//db值
		}
		return perlux;
}


double get_luxdb(u8 num)	//获取亮暗的百分百
{
		u8 i;
		double lowlight,hightlight;
		double lux,d,dd;
		double lightdata;
	
		delay_ms(14);
		lowlight = getLux(0,14);
		hightlight = lowlight;
		for(i=0;i<num;i++){
			lightdata = getLux(0,14);
			delay_ms(14);
			if(lightdata < lowlight)	lowlight = lightdata;
			if(lightdata > hightlight)	hightlight = lightdata;
		}
		d = (double)(hightlight-lowlight);
		dd = (double)(hightlight+lowlight);
		lux = d/dd/2;
		return (lux*100);
}
float get_dbdb(u8 num)//--
{
		double lux;
		float db;
		u8 i;

		for(i=0;i<4;i++){
			lux = get_luxdb(num);		//获取亮暗的百分百  取样个数
			db = transformdb(lux);	//转换出错  返回255	//如果转换出错 则再测试
			if(db < 45)	return db;	//DB 0-40  将百分比转换成DB值 
		}
		printf("lux-db:%lf",db);
		return db;	//出错
}

float gain_db(u8 num)//--
{
	float db1,db2;
	u8 i;
	for(i=0;i<5;i++){
		db1 = get_dbdb(num);	//如果转换出错 则再测试
		db2 = get_dbdb(num);	//	
		if(db1>db2)	{if(((db1-db2)<3)&&(db2>=1))	return db2;}
		if(db1<=db2) {if(((db2-db1)<3)&&(db1>=1))	return db1;}	//返回较小的那个
	}
	return ((db1+db2)/2);
}

void check_db(void)//--
{
		float dbvale;
		double light;
		dbvale = get_dbdb(20);
		light = getLux(0,14);
		sprintf((char*)fb_buffer,"flicker:%ddb    ",(u8)dbvale);	//
		LCD_ShowString(0,2,176,16,16,fb_buffer);//x y 长 宽 字体大小
		sprintf((char*)fb_buffer,"light:%d lux    ",(u8)light);
		LCD_ShowString(0,3,176,16,16,fb_buffer);
}


u8 three_point(float *searbuf)//--
{
	if((searbuf[0]<=searbuf[1])&&(searbuf[1]<=searbuf[2]))	return 0x03;	//升
	
	if((searbuf[1]<=searbuf[0])&&(searbuf[0]<=searbuf[2]))	return 0x03;	//升
	
	if((searbuf[2]<=searbuf[1])&&(searbuf[1]<=searbuf[0]))	return 0x00;	//降
	
	if((searbuf[0]<=searbuf[1])&&(searbuf[2]<=searbuf[1]))	return 0x01;	//ok 
	
	printf("three_point:error\r\n");
	return 254;
}

u8 getbest_vcom(u8 base,u8 begin,u8 end)	///---
{
		u8 vcom,rvcom,bestvcom;
		u8 flag1,flag2,flag,i;
		float searbuf[3],tmpsear;;	//0 1 2
	
		flag2 = 0x03;
		bestvcom = 0;
	
		vcom = begin;
		if(begin == 0)	vcom = VCOM_BASE;

		printf("开始搜索：searbuf:");
		sprintf((char*)fb_buffer,"start search!");	//
		LCD_ShowString(0,0,176,16,16,fb_buffer);//x y 长 宽 字体大小
	
		vcom -= base;
		for(i=0;i<3;i++){			//默认上升 保存三个点
			vcom += base;
			rvcom = ssdwrite_vcom(vcom);
			searbuf[i] = gain_db(LIGHTNUM);	
			printf("  %lf",searbuf[i]);
		}
		
		flag1 = three_point(searbuf);
		printf("  VCOM:%X & flag1:%d\r\n",rvcom,flag1);
		sprintf((char*)fb_buffer,"DB:%ddb VCOM:%X    ",(u8)searbuf[2],rvcom);	//
		LCD_ShowString(0,1,176,16,16,fb_buffer);//x y 长 宽 字体大小
		
		if(flag1==0x03)		flag=1;	//升
		if(flag1==0x00)		{flag=0;	vcom-=(3*base); flag1=0x03; searbuf[0]=searbuf[1];searbuf[1]=searbuf[2];}//降
		if(flag1==0x01)		{return (vcom-(2*base));}
			
		if(flag1==254)	{printf("searbuf:%lf :%lf :%lf  point error\r\n",searbuf[0],searbuf[1],searbuf[2]);return vcom;}//////

		while(1)
		{
			rvcom = ssdwrite_vcom(vcom);
			searbuf[2] = gain_db(LIGHTNUM);	//测试点保存在searbuf[2]
			sprintf((char*)fb_buffer,"DB:%ddb VCOM:%X    ",(u8)searbuf[2],rvcom);
			LCD_ShowString(0,1,176,16,16,fb_buffer);
			
			flag2 = three_point(searbuf);		
			if(flag2==254)	{printf("searbuf:%lf :%lf :%lf  point error\r\n",searbuf[0],searbuf[1],searbuf[2]);return (vcom);}///
			
			printf("searbuf:%lf :%lf :%lf ### flag:%d %d %d ### vcom:%X\r\n",searbuf[0],searbuf[1],searbuf[2],flag1,flag2,flag,rvcom);
			
			if((flag1==0x03)&&(flag2==0x00)){	//升降
					if(flag)	bestvcom = (vcom-base); else bestvcom = (vcom+base);
					rvcom = ssdwrite_vcom(bestvcom);
					return bestvcom;
			}
			if((flag1==0x03)&&(flag2==0x01)){	//升降
					if(flag)	bestvcom = (vcom-base); else bestvcom = (vcom+base);
					rvcom = ssdwrite_vcom(bestvcom);
					return bestvcom;
			}
			if(flag1==0x01){	//升升
					flag1 = flag2;
					searbuf[0] = searbuf[1];
					searbuf[1] = searbuf[2];
			}
			if((flag1==0x03)&&(flag2==0x03)){	//升升
					flag1 = flag2;
					searbuf[0] = searbuf[1];
					searbuf[1] = searbuf[2];
			}
			if((flag1==0x00)&&(flag2==0x00)){	//降降
					flag1 = 0x03;
					tmpsear = searbuf[0];
					searbuf[0] = searbuf[2];
					searbuf[2] = tmpsear;	
					if(flag) flag = 0; else flag = 1;
			}
			if(flag == 1)		vcom += base;	
				else vcom -= base;			
	}
}

u8 get_whilebestvcom(u8 base,u8 begin)
{
		u8 vcom,rvcom,bestdb,bestvcom;
		double fdb,cdb;
		u8 flag = 1,times,outime;
	
		vcom = begin;
		rvcom = ssdwrite_vcom(vcom);	
		bestdb = fdb = gain_db(LIGHTNUM);
		sprintf((char*)fb_buffer,"SDB:%ddb VCOM:%X ",(u8)bestdb,rvcom);	
		LCD_ShowString(0,1,176,16,16,fb_buffer);
		printf("STEP search: fdb:%lf vcom:%X & flag:%d times:%d,bestdb:%d\r\n",fdb,rvcom,flag,times,bestdb);

		times = 0;
		outime = 0;
		bestvcom = vcom;
		while(1)
		{
			rvcom = ssdwrite_vcom(vcom);
			cdb = gain_db(LIGHTNUM-5);
			sprintf((char*)fb_buffer,"SDB:%ddb VCOM:%X T:%d ",(u8)cdb,rvcom,times);	//
			LCD_ShowString(0,1,176,16,16,fb_buffer);

			if(bestdb < cdb)	{bestdb = cdb;times = 0;bestvcom = vcom;}	//
			if(cdb == bestdb)	{times++;}
			if(times > 3)	return vcom;
			
			sprintf((char*)fb_buffer,"BDB:%d BVCOM:%x",(u8)bestdb,bestvcom);	//
			LCD_ShowString(0,2,176,16,16,fb_buffer);
			
			if(times >0) outime++;
			
			if(outime > 6)	{		
				outime = 0;
				bestvcom = 0;
				bestdb = 0;
				times = 0;
				sprintf((char*)fb_buffer,"outime try again!");	
				LCD_ShowString(0,1,176,16,16,fb_buffer);
			}
			
			if(cdb < fdb ) 
					{if(flag) flag = 0; else flag = 1;}
			
			fdb = cdb;
			if(flag == 1)		vcom += base;	
			else vcom -= base;
	}
}
	

u8 lcm_search_vcom(void)//--
{
			u8 vcom,bend;
	
			before_otp_read();
	
			vcom = getbest_vcom(6,0,0);	//u8 getbest_vcom(u8 base,u8 begin,u8 end)
			
			bend = vcom;
			vcom = getbest_vcom(3,(bend-4),(bend+4));

			bend = vcom;
			vcom = getbest_vcom(1,(bend-2),(bend+2));
			
			write_otp(vcom);
			burn_otp(vcom);
			after_otp_read();
	
			return vcom;
}











