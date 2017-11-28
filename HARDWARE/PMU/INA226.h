/*
INA226.h - Header file for the Bi-directional Current/Power Monitor Arduino Library.

*/

#ifndef INA226_h
#define INA226_h

#include "stm32f10x.h"

#define INA226_REG_CONFIG           (0x00)
#define INA226_REG_SHUNTVOLTAGE     (0x01)
#define INA226_REG_BUSVOLTAGE       (0x02)
#define INA226_REG_POWER            (0x03)
#define INA226_REG_CURRENT          (0x04)
#define INA226_REG_CALIBRATION      (0x05)
#define INA226_REG_MASKENABLE       (0x06)
#define INA226_REG_ALERTLIMIT       (0x07)

#define INA226_BIT_SOL              (0x8000)
#define INA226_BIT_SUL              (0x4000)
#define INA226_BIT_BOL              (0x2000)
#define INA226_BIT_BUL              (0x1000)
#define INA226_BIT_POL              (0x0800)
#define INA226_BIT_CNVR             (0x0400)
#define INA226_BIT_AFF              (0x0010)
#define INA226_BIT_CVRF             (0x0008)
#define INA226_BIT_OVF              (0x0004)
#define INA226_BIT_APOL             (0x0002)
#define INA226_BIT_LEN              (0x0001)

typedef enum
{
    INA226_AVERAGES_1             = 0,
    INA226_AVERAGES_4             = 1,
    INA226_AVERAGES_16            = 2,
    INA226_AVERAGES_64            = 3,
    INA226_AVERAGES_128           = 4,
    INA226_AVERAGES_256           = 5,
    INA226_AVERAGES_512           = 6,
    INA226_AVERAGES_1024          = 7
} ina226_averages_t;

typedef enum
{
    INA226_BUS_CONV_TIME_140US    = 0,
    INA226_BUS_CONV_TIME_204US    = 1,
    INA226_BUS_CONV_TIME_332US    = 2,
    INA226_BUS_CONV_TIME_588US    = 3,
    INA226_BUS_CONV_TIME_1100US   = 4,
    INA226_BUS_CONV_TIME_2116US   = 5,
    INA226_BUS_CONV_TIME_4156US   = 6,
    INA226_BUS_CONV_TIME_8244US   = 7
} ina226_busConvTime_t;

typedef enum
{
    INA226_SHUNT_CONV_TIME_140US   = 0,
    INA226_SHUNT_CONV_TIME_204US   = 1,
    INA226_SHUNT_CONV_TIME_332US   = 2,
    INA226_SHUNT_CONV_TIME_588US   = 3,
    INA226_SHUNT_CONV_TIME_1100US  = 4,
    INA226_SHUNT_CONV_TIME_2116US  = 5,
    INA226_SHUNT_CONV_TIME_4156US  = 6,
    INA226_SHUNT_CONV_TIME_8244US  = 7
} ina226_shuntConvTime_t;

typedef enum
{
    INA226_MODE_POWER_DOWN      = 0,
    INA226_MODE_SHUNT_TRIG      = 1,
    INA226_MODE_BUS_TRIG        = 2,
    INA226_MODE_SHUNT_BUS_TRIG  = 3,
    INA226_MODE_ADC_OFF         = 4,
    INA226_MODE_SHUNT_CONT      = 5,
    INA226_MODE_BUS_CONT        = 6,
    INA226_MODE_SHUNT_BUS_CONT  = 7,
} ina226_mode_t;

	void INA226_selectDevice(uint8_t ina226_index);
	int INA226_configure(ina226_averages_t avg, ina226_busConvTime_t busConvTime, ina226_shuntConvTime_t shuntConvTime, ina226_mode_t mode);
	int INA226_calibrate(float rShuntValue, float iMaxExpected);

	int INA226_writeRegister16(uint8_t reg, uint16_t val);
	int INA226_readRegister16(uint8_t reg, int16_t* val);
	
	ina226_averages_t INA226_getAverages(void);
	ina226_busConvTime_t INA226_getBusConversionTime(void);
	ina226_shuntConvTime_t INA226_getShuntConversionTime(void);
	ina226_mode_t INA226_getMode(void);

	void INA226_enableShuntOverLimitAlert(void);
	void INA226_enableShuntUnderLimitAlert(void);
	void INA226_enableBusOvertLimitAlert(void);
	void INA226_enableBusUnderLimitAlert(void);
	void INA226_enableOverPowerLimitAlert(void);
	void INA226_enableConversionReadyAlert(void);

	void INA226_setBusVoltageLimit(float voltage);
	void INA226_setShuntVoltageLimit(float voltage);
	void INA226_setPowerLimit(float watts);

	void INA226_setAlertInvertedPolarity(uint8_t inverted);
	void INA226_setAlertLatch(uint8_t latch);

	uint8_t INA226_isMathOverflow(void);
	uint8_t INA226_isAlert(void);

	float INA226_readShuntCurrent(void);
	float INA226_readShuntVoltage(void);
	float INA226_readBusPower(void);
	float INA226_readBusVoltage(void);

	float INA226_getMaxPossibleCurrent(void);
	float INA226_getMaxCurrent(void);
	float INA226_getMaxShuntVoltage(void);
	float INA226_getMaxPower(void);


#endif
