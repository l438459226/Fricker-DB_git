#include "stm32f10x.h"
#include "malloc.h"
#include "arm_math.h"
#include "math.h"

//#define FRICKER_DUGE

#define FFT_LENGTH		2048 		//FFT长度，默认是1024点FFT
#define FFT_FS			4096	//采样频率     


typedef struct {
	double Rate;
	double UserdB;
	double UserFrequency;
  unsigned int Lux;
	unsigned int PeakFrequency;
	double PeakdB;
}FlickerMeterDef;

extern u32 sampling_count;
void collection_data(void);
void Flicker_init(void);
void get_jeita_db(void);
void	FlickerMeter(void);

#ifdef FRICKER_DUGE
#include "usart.h"
#endif

































