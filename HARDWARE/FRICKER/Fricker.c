#include "Fricker.h"
#include "adc.h"
#include "usart.h"
#include "timer.h"


u32 sampling_count;
q31_t *fft_ibuf = NULL,*fft_obuf = NULL;		//int32_t  fft_ibuf[FFT_LENGTH*2];	fft_obuf[FFT_LENGTH];

static q31_t *FFT_InputBuffer(u32 fft_lenth)	//
{
	q31_t *fft_inputbuf;
	fft_inputbuf=(q31_t*)mymalloc(SRAMIN,fft_lenth);//申请2K字节
	return fft_inputbuf;
}

static q31_t *FFT_OutputBuffer(u32 fft_lenth)
{
	q31_t *fft_Outputbuf;
	fft_Outputbuf=(q31_t*)mymalloc(SRAMIN,fft_lenth);//申请2K字节
	return fft_Outputbuf;
}

static void fft_Malloc_Buffer(void)
{
	fft_ibuf = FFT_InputBuffer(FFT_LENGTH*2*4);
	fft_obuf = FFT_OutputBuffer(FFT_LENGTH*4);
}

void collection_data(void)	//在定时器中采集数据
{
	if(sampling_count<FFT_LENGTH)	//FFT_LENGTH
	{
		fft_ibuf[2*sampling_count] = get_adc_value();
		fft_ibuf[2*sampling_count+1]=0;
		sampling_count++;
	}
}

void Flicker_init(void)
{
	fft_Malloc_Buffer();//分配内存
	//fft_Malloc_Buffer();//
	ADC1_Init();
	TIM3_Init(1000000/FFT_FS);//2000
	start_time3();	//开启定时采集数据
}

void RMS_percent(void)
{
	u16 i;
	float rms_percent;
	float average;
	double sum_dvalue=0;
	u32 sum_value=0;
	float temp;
	
	for(i=0;i<FFT_LENGTH;i++)
	{
			sum_value = sum_value + fft_ibuf[2*i];
			//printf("%d ",Adcbuf[i]);
	}
	
	//printf("\r\nsumvalue:%d\r\n",sum_value);
	average = sum_value/FFT_LENGTH;
	//printf("average:%f\r\n",average);
	
	for(i=0;i<FFT_LENGTH;i++)
	{
		if(average>fft_ibuf[2*i]) 
			temp = average-fft_ibuf[2*i];
		else 
			temp = fft_ibuf[2*i]-average;
		
			sum_dvalue += 	pow(temp,2);
	}
	//printf("square:%f\r\n",sum_dvalue);
	temp = sqrt((sum_dvalue/FFT_LENGTH));//开方
	//printf("sqrt:%f\r\n",temp);

	rms_percent = temp/average*100;
	printf("rms_percent:%f%% \r\n\r\n",rms_percent);
}

void get_jeita_db(void)
{
	u16 i;
	float jeita_db,per;
	float amplitude,frequency; 
	float direct;

	arm_cfft_radix2_instance_q31 SC;
	//arm_cfft_radix4_instance_q31 SC;
	
	arm_cfft_radix2_init_q31(&SC,FFT_LENGTH,0,1);
	//arm_cfft_radix4_init_q31(&SC,FFT_LENGTH,0,1);
	
	for(i=0;i<FFT_LENGTH;i++)//生成信号序列
	{

			fft_ibuf[2*i] = fft_ibuf[2*i]<<16;
			//fft_ibuf[2*i+1]=0;//虚部全部为0
	}
	
	arm_cfft_radix2_q31(&SC,fft_ibuf);//傅里叶
	arm_cmplx_mag_q31(fft_ibuf,fft_obuf,FFT_LENGTH);//计算模值
	//arm_cfft_radix4_q31(&SC,fft_ibuf);//傅里叶
	//arm_cmplx_mag_q31(fft_ibuf,fft_obuf,FFT_LENGTH);//计算模值
	
#ifdef FRICKER_DUGE
	for(i=0;i<72;i++)
	{
				printf("fft_outputbuf[%d]:%d \r\n\r\n",i*FFT_FS/FFT_LENGTH,fft_obuf[i]/16306);///16306
	}
#endif
	direct = fft_obuf[0]/2;//直流分量

	amplitude = fft_obuf[1];
	frequency = 1;
	for(i=1;i<80;i++)
	{
		if(fft_obuf[i]>amplitude) 
		{
			amplitude = fft_obuf[i];
			frequency = i;
		}
	}
	frequency = frequency*FFT_FS/FFT_LENGTH;
	jeita_db = 20*log10(amplitude*0.708/direct);	
	per = (amplitude/direct*200 );
	printf("jeita_db:%f db \r\n\r\n per:%f%% \r\n\r\n ***direct:%f		fs:%f    fu:%f	***\r\n\r\n\r\n\r\n",jeita_db,per,direct,frequency,amplitude);///16306
}

void clear_samplingdata(void)
{
	//myfree(SRAMIN,fft_ibuf);
	//myfree(SRAMIN,fft_obuf);
	//fft_ibuf = NULL;
	//fft_obuf = NULL;
	sampling_count = 0;
	//stop_time3();
}

void	FlickerMeter(void)
{
	/*
	if((fft_ibuf==NULL)&&(fft_obuf==NULL)&&(sampling_count==0))//没有分配内存
	{
			fft_Malloc_Buffer();//分配内存
			start_time3();	//开启定时采集数据
	}*/
	
	if((sampling_count==FFT_LENGTH)||(sampling_count>FFT_LENGTH))	//采集数据完成 if(sampling_count<FFT_LENGTH)	
	{
		RMS_percent();
		get_jeita_db();
		clear_samplingdata();
	}
}
























