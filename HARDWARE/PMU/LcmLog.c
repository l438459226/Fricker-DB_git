/*********************************************************************************************************
* 文件名称: LcmLogo.c
* 文件作者: zq
* 当前版本: V1.00
* 所属模块: 
* 完成日期: 2017年03月06日
* 文件摘要: 
* 注意事项:	
* 更改说明: 
**********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "LcmLog.h"
#include "ff.h"



u8 lcm_log_file_path[33] = "0:/LcmLog/20130717_22/221617.txt";
u8 lcm_log_file_path_new[150] = "LcmLog/20130717_22/TestLogfile_0123456789_05-06-2014_16-30-41_OK.txt";
u8 lcm_log_file_directory[22] = "0:/LcmLog/20130717_22";




u8 lcm_current_time_str[25] = "Sun Jan 06 2013 17:56:15";
//u8 lcm_log_time_str[30] = "0:/LcmLog/20130717/221617.txt";
//u8 lcm_ds_log_str[20] = "05-06-2014_16-30-41";


FIL lcm_log_fdst;

extern u8 G_TPT_SN[16];
extern u8 current_time_str[25];
extern u8 ds_log_str[20];
extern u8 log_time_str[27];
extern struct ts systmtime;

extern FRESULT f_buf_write(
	FIL *fp,			/* Pointer to the file object */
	const void *buff,	/* Pointer to the data to be written */
	u32 btw			/* Number of bytes to write */
);


extern u32 f_string_count(const u8* str);


FATFS fats;




void Lcm_Logo_Set(u8 NoOfTestItems)
{
	u16 k = 0,j = 0;
	u32 bw = 0;
	FRESULT log_res;
	u8 temp[512]="";
	

	
	/*Log function part*/ 
	/////////////////////////////////////////////////////////////////////////////////////
	u8 str_tmp[18]="";
	u8 OK_str[8]="_OK.txt";
	u8 NG_str[8]="_NG.txt";
	u8 NA_str[6]="N/A";
	char str_line_break[]="\r\n";
	char text_0[] = "BEGIN_UUT\r\nGroup Test Started\tTime: ";
	char text_1[] = "\r\nProduct Name: LCM-TP-TESTER";
	char text_2[] = "\r\nLCM-TP-TESTER P/N: ";
	char text_3[] = "\r\nTest Count: ";		
	char text_4[] = "\r\n\r\n\r\nLCM Log General TestItem: ";
	char text_5[] = "\r\nHID : ";
	char text_6[] = "\r\nSID : ";
	char text_7_title[] = "\r\n#TE Limit    High_Freq(HZ)   Low_Freq(HZ)";
	char text_7[] = "\r\nTE Freq : ";
	char text_8_title[] = "\r\n#PWM Limit    High_Freq(HZ)   Low_Freq(HZ)   High_Duty(%)   Low_Duty(%) ";
	char text_8[] = "\r\nPWM : ";
	char text_9[] = "\r\nNVM Count: ";

	/*	//这部分先预留
	char text_10[] = "\r\n\r\n\r\nLCM Log Normal Voltate/Current TestItem: ";
	char text_11[] = "\r\n#Limit    High_Voltage(mv)   Low_Voltage(mv)   High_Current(ma)   High_Current(ma)";	
	char text_12[] = "\r\n\r\n\r\nLCM Log Sleep Voltate/Current TestItem: ";
	char text_13[] = "\r\n#Limit    High_Voltage(mv)   Low_Voltage(mv)   High_Current(ua)   High_Current(ua)";
	*/
	char text_14[] = "\r\nNVM Count: ";
	char text_15[] = "\r\nGroup Test Finished\tTime: ";
	char text_16[] = "\r\nEND_UUT\r\n\r\n";
	/////////////////////////////////////////////////////////////////////////////////////		

	


#ifdef DEBUG_LCM_TEST
	printf("LcmLog function part_1 START\r\n");
#endif
		
	Get_Time_String(&systmtime);		
		
	printf("log_time_str=%s\r\n",log_time_str);
	
	for(k=10;k<21;k++)
	{
		lcm_log_file_path[k] = log_time_str[k-3];
	}	
			
	//0:/LOG/20130717_22
	lcm_log_file_path[18] = '_';
	//0:/LOG/20130717_22/221617.txt								//文件名
	for(k=18;k<30;k++)
	{
		lcm_log_file_path[k+3] = log_time_str[k-3];
	}	

	//u8 lcm_log_file_directory[19] = "0:/LOG/20130717_22";		//文件夹名
	for(k=0;k<21;k++)
	{
		lcm_log_file_directory[k] = lcm_log_file_path[k];
	}


	log_res = f_mkdir(lcm_log_file_directory);	
	#ifdef DEBUG_LCM_TEST
		printf("lcm_log_file_directory=%s\r\n",lcm_log_file_directory);
		printf("f_mkdir = %d\r\n", log_res);
	#endif

	k = 1;
	while(k--)			
	{
		log_res = f_open(&lcm_log_fdst, lcm_log_file_path, FA_CREATE_NEW | FA_WRITE); /* 打开文件 */
		#ifdef DEBUG_LCM_TEST
			printf("lcm_log_file_path=%s\r\n",lcm_log_file_path);
			printf("log_res1 = %d\r\n",log_res);		
		#endif
		if(log_res == FR_EXIST)
		{
			log_res = f_open(&lcm_log_fdst, lcm_log_file_path, FA_OPEN_EXISTING | FA_WRITE); /* 打开文件 */
			#ifdef DEBUG_LCM_TEST
				printf("log_res2 = %d\r\n",log_res);
			#endif
		}
	}
		
	strcat(temp,text_0);	
	strcat(temp,"\r\nabc");
	if(log_res == FR_OK)
	{	
		f_write(&lcm_log_fdst,temp, f_string_count(temp),&bw);
		f_write(&lcm_log_fdst,current_time_str, f_string_count(current_time_str),&bw);
		f_write(&lcm_log_fdst,text_1, f_string_count(text_1),&bw);
		f_write(&lcm_log_fdst,text_2, f_string_count(text_2),&bw);
		f_write(&lcm_log_fdst,G_TPT_SN, f_string_count(G_TPT_SN),&bw);
		f_write(&lcm_log_fdst,text_3, f_string_count(text_3),&bw);
		f_write(&lcm_log_fdst,text_4, f_string_count(text_4),&bw);
		f_write(&lcm_log_fdst,text_5, f_string_count(text_5),&bw);
		f_write(&lcm_log_fdst,text_6, f_string_count(text_6),&bw);
		f_write(&lcm_log_fdst,text_7_title, f_string_count(text_7_title),&bw);
		f_write(&lcm_log_fdst,text_7, f_string_count(text_7),&bw);
		f_write(&lcm_log_fdst,text_8_title, f_string_count(text_8_title),&bw);
		f_write(&lcm_log_fdst,text_8, f_string_count(text_8),&bw);
		f_write(&lcm_log_fdst,text_9, f_string_count(text_9),&bw);
		
		f_close(&lcm_log_fdst);

		Get_Time_String(&systmtime);		//写logo前，重新再获取一次时间
		
		for(k=0;k<15+3;k++)
		{
			lcm_log_file_path_new[k] = lcm_log_file_directory[k+3];
		}
		
		lcm_log_file_path_new[24+6] = '_';
		for(k=0;k<19;k++){
			lcm_log_file_path_new[25+6+k] = ds_log_str[k];
		}
		
		for(k=0;k<8;k++)
		{
			lcm_log_file_path_new[44+6+k] = OK_str[k];					
	    }

		//log_res = f_rename(lcm_log_file_path,lcm_log_file_path_new);

		
		log_res = f_rename(lcm_log_file_path,lcm_log_file_path_new);

		#ifdef DEBUG_LCM_TEST
			printf("lcm_log_file_path_new = %s\r\n",lcm_log_file_path_new);	
		#endif
		
		
		#ifdef DEBUG_LCM_TEST
			printf("res = %d\r\n",log_res);
			printf("white data to txt = %d\r\n",bw);	
		#endif	
	}
}	









