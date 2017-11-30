/*

*/


#include "INA226.h"
#include "i2c1_bitbang.h"
#include "myiic.h"
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"


#define INA226_0_I2C_7BIT_ADDRESS 0x40
#define INA226_1_I2C_7BIT_ADDRESS 0x41
#define INA226_2_I2C_7BIT_ADDRESS 0x44			//背光部分电流

#define ENABLE_ERROR_PRINT

uint8_t INA226_I2C_7BIT_ADDRESS = 0x40;
int8_t inaAddress;
double currentLSB, powerLSB;
float vShuntMax, vBusMax, rShunt;

void INA226_selectDevice(uint8_t ina226_index)
{
    if(ina226_index == 0)
	{
		INA226_I2C_7BIT_ADDRESS = INA226_0_I2C_7BIT_ADDRESS;
	}
	else if(ina226_index == 1)
	{
		INA226_I2C_7BIT_ADDRESS = INA226_1_I2C_7BIT_ADDRESS;
	}
	else if(ina226_index == 2)
	{
		INA226_I2C_7BIT_ADDRESS = INA226_2_I2C_7BIT_ADDRESS;
	}
}



int INA226_writeRegister16(uint8_t reg, uint16_t val)
{
	int retval;
    uint8_t vla;
    vla = (uint8_t)val;
    val >>= 8;
	
		retval = i2c1_start(INA226_I2C_7BIT_ADDRESS<<1);
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
					printf("fun: i2c1_start Failed\r\n");
			#endif
			return I2C_ERR; 
		}
		
		retval = i2c1_write_u8((uint8_t)(reg));
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
					printf("fun: i2c1_write_u8(reg) Failed\r\n");
			#endif
			return I2C_ERR; 
		}
		
		retval = i2c1_write_u8((uint8_t)(val));
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
					printf("fun: i2c1_write_u8(val) Failed\r\n");
			#endif
			return I2C_ERR; 
		}
		
		retval = i2c1_write_u8((uint8_t)(vla));
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
					printf("fun: i2c1_write_u8(vla) Failed\r\n");
			#endif
			return I2C_ERR; 
		}
		
		retval = i2c1_stop();
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_stop Failed\r\n");
			#endif
			return I2C_ERR; 
		}
		return I2C_OK;
}


int INA226_readRegister16(uint8_t reg, int16_t* val)
{
		int retval;
	  int16_t value;
		uint8_t pBuffer[2];
	
		retval = i2c1_start(INA226_I2C_7BIT_ADDRESS<<1);
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
					printf("fun: i2c1_start Failed\r\n");
			#endif
			return I2C_ERR; 
		}
		
		retval = i2c1_write_u8((uint8_t)(reg));
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
					printf("fun: i2c1_write_u8(reg) Failed\r\n");
			#endif
			return I2C_ERR; 
		}
		
		retval = i2c1_stop();
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_stop Failed\r\n");
			#endif
			return I2C_ERR; 
		}

    //Delay_ms(1);
		
		retval = i2c1_start((INA226_I2C_7BIT_ADDRESS<<1)|0x01);
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_start_repeat Failed\r\n");
			#endif
			return I2C_ERR; 
		}

		retval = i2c1_read_data(pBuffer, 2, 1);
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_read_data Failed\r\n");
			#endif
			return I2C_ERR; 
		}

		retval = i2c1_stop();
		if(retval<0){
			#ifdef ENABLE_ERROR_PRINT
				printf("fun: i2c1_stop Failed\r\n");
			#endif
			return I2C_ERR; 
		}
	
		value = pBuffer[0];
		value = value << 8;
		value = value | pBuffer[1];
		*val = value;
		
    return I2C_OK;
}



int INA226_configure(ina226_averages_t avg, ina226_busConvTime_t busConvTime, ina226_shuntConvTime_t shuntConvTime, ina226_mode_t mode)
{
		int retval;
    uint16_t config = 0;

    config |= (avg << 9 | busConvTime << 6 | shuntConvTime << 3 | mode);

    vBusMax = 36;
    vShuntMax = 0.08192f;

		
    retval = INA226_writeRegister16(INA226_REG_CONFIG, config);

    return retval;
}

int INA226_calibrate(float rShuntValue, float iMaxExpected)
{
		int retval;
		double  minimumLSB;	//iMaxPossible
	
    uint16_t calibrationValue;
    rShunt = rShuntValue;

		//iMaxPossible = vShuntMax / rShunt;

    minimumLSB = ((double)iMaxExpected)/ 32768;

	/*

    currentLSB = (uint16_t)(minimumLSB * 100000000);
    currentLSB /= 100000000;
    currentLSB /= 0.0001;
    currentLSB = ceil(currentLSB);
    currentLSB *= 0.0001;  

    powerLSB = currentLSB * 25;

     */

  calibrationValue = (uint16_t)((0.00512) / (minimumLSB * rShunt));
	currentLSB = minimumLSB;
	//currentLSB = 0;
	powerLSB = currentLSB * 25;

	//printf("currentLSB = %0.9f\r\n",currentLSB);

	//printf("powerLSB = %0.9f\r\n",powerLSB);

    retval = INA226_writeRegister16(INA226_REG_CALIBRATION, calibrationValue);

    return retval;
}

float INA226_getMaxPossibleCurrent(void)
{
    return (vShuntMax / rShunt);
}

float INA226_getMaxCurrent(void)
{
    float maxCurrent = (currentLSB * 32767);
    float maxPossible = INA226_getMaxPossibleCurrent();

    if (maxCurrent > maxPossible)
    {
        return maxPossible;
    } else
    {
        return maxCurrent;
    }
}

float INA226_getMaxShuntVoltage(void)
{
    float maxVoltage = INA226_getMaxCurrent() * rShunt;

    if (maxVoltage >= vShuntMax)
    {
        return vShuntMax;
    } else
    {
        return maxVoltage;
    }
}

float INA226_getMaxPower(void)
{
    return (INA226_getMaxCurrent() * vBusMax);
}

float INA226_readBusPower(void)
{
		int retval;
		int16_t val;		
		retval = INA226_readRegister16(INA226_REG_POWER, &val);		
		if(retval<0){
				printf("INA226_readRegister16(INA226_REG_POWER, &val) Failed\r\n");
		}
		
    return (((float)val) * powerLSB);
}

float INA226_readShuntCurrent(void)
{
		int retval;
		int16_t val;		
		retval = INA226_readRegister16(INA226_REG_CURRENT, &val);	
		if(retval<0){
				printf("INA226_readRegister16(INA226_REG_CURRENT, &val) Failed\r\n");
		}

		//printf("shunt ad = %d\r\n",val);
    return (((float)val) *currentLSB);
}

float INA226_readShuntVoltage(void)
{
		int retval;
		int16_t val;
    float voltage;
		retval = INA226_readRegister16(INA226_REG_SHUNTVOLTAGE, &val);
		if(retval<0){
				printf("INA226_readRegister16(INA226_REG_SHUNTVOLTAGE, &val) Failed\r\n");
		}
    voltage = (float)val;

    return (voltage * 0.0000025);
}

float INA226_readBusVoltage(void)
{
		int retval;
		int16_t val;
    	float voltage;
		retval = INA226_readRegister16(INA226_REG_BUSVOLTAGE, &val);
		if(retval<0){
				printf("INA226_readRegister16(INA226_REG_BUSVOLTAGE, &val) Failed\r\n");
		}
		voltage = (float)val;
    return (voltage * 0.00125);
}

ina226_averages_t INA226_getAverages(void)
{
    int16_t value;
		INA226_readRegister16(INA226_REG_CONFIG, &value);
    value &= 0xE00;//0b0000111000000000;
    value >>= 9;

    return (ina226_averages_t)value;
}

ina226_busConvTime_t INA226_getBusConversionTime(void)
{
    int16_t value;

		INA226_readRegister16(INA226_REG_CONFIG, &value);
    value &= 0x1C0;//0b0000000111000000;
    value >>= 6;

    return (ina226_busConvTime_t)value;
}

ina226_shuntConvTime_t INA226_getShuntConversionTime(void)
{
    int16_t value;

		INA226_readRegister16(INA226_REG_CONFIG, &value);
    value &= 0x38;//0b0000000000111000;
    value >>= 3;

    return (ina226_shuntConvTime_t)value;
}

ina226_mode_t INA226_getMode(void)
{
    int16_t value;

		INA226_readRegister16(INA226_REG_CONFIG, &value);
    value &= 0x07;//0b0000000000000111;

    return (ina226_mode_t)value;
}

void INA226_setMaskEnable(uint16_t mask)
{
    INA226_writeRegister16(INA226_REG_MASKENABLE, mask);
}

uint16_t INA226_getMaskEnable(void)
{
		int16_t value;
		INA226_readRegister16(INA226_REG_MASKENABLE, &value);
    return value;
}

void INA226_enableShuntOverLimitAlert(void)
{
    INA226_writeRegister16(INA226_REG_MASKENABLE, INA226_BIT_SOL);
}

void INA226_enableShuntUnderLimitAlert(void)
{
    INA226_writeRegister16(INA226_REG_MASKENABLE, INA226_BIT_SUL);
}

void INA226_enableBusOvertLimitAlert(void)
{
    INA226_writeRegister16(INA226_REG_MASKENABLE, INA226_BIT_BOL);
}

void INA226_enableBusUnderLimitAlert(void)
{
    INA226_writeRegister16(INA226_REG_MASKENABLE, INA226_BIT_BUL);
}

void INA226_enableOverPowerLimitAlert(void)
{
    INA226_writeRegister16(INA226_REG_MASKENABLE, INA226_BIT_POL);
}

void INA226_enableConversionReadyAlert(void)
{
    INA226_writeRegister16(INA226_REG_MASKENABLE, INA226_BIT_CNVR);
}

void INA226_setBusVoltageLimit(float voltage)
{
    uint16_t value = voltage / 0.00125;
    INA226_writeRegister16(INA226_REG_ALERTLIMIT, value);
}

void INA226_setShuntVoltageLimit(float voltage)
{
    uint16_t value = voltage * 25000;
    INA226_writeRegister16(INA226_REG_ALERTLIMIT, value);
}

void INA226_setPowerLimit(float watts)
{
    uint16_t value = watts / powerLSB;
    INA226_writeRegister16(INA226_REG_ALERTLIMIT, value);
}

void INA226_setAlertInvertedPolarity(uint8_t inverted)
{
    uint16_t temp = INA226_getMaskEnable();

    if (inverted)
    {
        temp |= INA226_BIT_APOL;
    } else
    {
        temp &= ~INA226_BIT_APOL;
    }

    INA226_setMaskEnable(temp);
}

void INA226_setAlertLatch(uint8_t latch)
{
    uint16_t temp = INA226_getMaskEnable();

    if (latch)
    {
        temp |= INA226_BIT_LEN;
    } else
    {
        temp &= ~INA226_BIT_LEN;
    }

    INA226_setMaskEnable(temp);
}

uint8_t INA226_isMathOverflow(void)
{
    return ((INA226_getMaskEnable() & INA226_BIT_OVF) == INA226_BIT_OVF);
}

uint8_t INA226_isAlert(void)
{
    return ((INA226_getMaskEnable() & INA226_BIT_AFF) == INA226_BIT_AFF);
}


