/* =============================================================================
    Title:          i2c_bitbang.h : Bit bang I2C master driver
    Author(s):      Pieter Conradie
    Creation Date:  2012-06-25
    Revision Info:  $Id$

============================================================================= */

/* _____STANDARD INCLUDES____________________________________________________ */

/* _____PROJECT INCLUDES_____________________________________________________ */
#include "i2c1_bitbang.h"
#include "myiic.h"

/* _____LOCAL DEFINITIONS____________________________________________________ */
/// Select I2C clock frequency


/// @name I2C Clock GPIO pin manipulation macros
//@{
#define I2C_SCL_SET_HIZ()   GPIOB->BSRR=GPIO_Pin_6
#define I2C_SCL_SET_LO()    GPIOB->BRR=GPIO_Pin_6
#define I2C_SCL_IS_HI()     ((GPIOB->IDR&GPIO_Pin_6)!=0)
#define I2C_SCL_IS_LO()     ((GPIOB->IDR&GPIO_Pin_6)==0)
//@}

int i2c11__write_u8(u8 data);
/// @name I2C Data GPIO pin manipulation macros
//@{
#define I2C_SDA_SET_HIZ()   GPIOB->BSRR=GPIO_Pin_7
#define I2C_SDA_SET_LO()    GPIOB->BRR=GPIO_Pin_7
#define I2C_SDA_IS_HI()     ((GPIOB->IDR&GPIO_Pin_7)!=0)
#define I2C_SDA_IS_LO()     ((GPIOB->IDR&GPIO_Pin_7)==0)
//@} 
/* _____LOCAL VARIABLES______________________________________________________ */

//G_i2c11__nop_count: 10 -> 4706.8K (T = 2.459us)  55 -> 100.4k (T = 10.05us)  11 -> 381K
uint8_t G_i2c11__nop_count = 55;		//不能取更小的值了

/* _____PRIVATE FUNCTIONS____________________________________________________ */
static void i2c_delay_half_clk(void)
{
	//board_delay_us(UDIV_ROUND(1E6,2*I2C_BIT_RATE_HZ));
	u32 i = G_i2c11__nop_count;
	while(i--);
}

//return: 0 - success; -1 - fail/error
static int i2c_scl_set_hiz_and_wait_clk_stretch(void)
{
    u8 i;

    I2C_SCL_SET_HIZ();
    for(i=100; i != 0; i--)
    {
        i2c_delay_half_clk();
        if(I2C_SCL_IS_HI())
        {
			//printf("stretch i = %d\r\n",i);
            return I2C_OK;
        }        
    }
	
    // Timed out waiting for SCL to go high
    return I2C_ERR;
}

int i2c_tx_bit(u8 bit)
{
    // Output bit
    if(bit)
    {
        I2C_SDA_SET_HIZ();
    }
    else
    {
        I2C_SDA_SET_LO();
    }
    i2c_delay_half_clk();

    if(i2c_scl_set_hiz_and_wait_clk_stretch())
    {
        // Error
        I2C_SDA_SET_HIZ();
        return I2C_ERR;
    }

    // Check bit
    if(bit)
    {
        if(I2C_SDA_IS_LO())
        {
            // Error
            return I2C_ERR;
        }
    }

    I2C_SCL_SET_LO();

    return I2C_OK;
}

int i2c_rx_bit(u8 *bit)
{
    // Release data line so that slave can drive it
    I2C_SDA_SET_HIZ();
    i2c_delay_half_clk();

    if(i2c_scl_set_hiz_and_wait_clk_stretch())
    {
        // Timed out waiting for SCL to go high
        return I2C_ERR;
    }
    // Read bit
	//printf("READ BIT:%d",I2C_SDA_IS_HI());
    *bit = I2C_SDA_IS_HI();

    I2C_SCL_SET_LO();

    return I2C_OK;
}


void i2c11__anti_death(void)		//防死锁
{
	u8 i = 0;
	for(i=0;i<9;i++)				//SDA拉高，SCL连续发送9个脉冲复位I2c设备，一定要重新复位，否则上电容易出错
	{
		I2C_SCL_SET_HIZ();
		i2c_delay_half_clk();
		I2C_SCL_SET_LO();
		i2c_delay_half_clk();
		I2C_SCL_SET_HIZ();
	}
}


/* _____GLOBAL FUNCTIONS_____________________________________________________ */
void i2c11__init(void)
{
//	u8 i = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;	
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); //PB6,PB7 输出高		


	I2C_SDA_SET_HIZ();
	i2c11__anti_death();				//SDA拉高，SCL连续发送9个脉冲复位I2c设备，一定要重新复位，否则上电容易出错	


	i2c_delay_half_clk();
	i2c_delay_half_clk();
	i2c_delay_half_clk();
	
	/*
    // Set bus to free state (Hi-Z)
    I2C_SDA_SET_HIZ();
	//i2c_delay_half_clk();
    I2C_SCL_SET_HIZ();
    i2c_delay_half_clk();
    */
	
}


int i2c11__start(u8 sla_adr)
{   
    // Check bus
    if(I2C_SCL_IS_LO() || I2C_SDA_IS_LO())
    {
        // Bus error
       // printf("levevl is low!!\r\n");
       	i2c11__anti_death();				//总线有问题后尝试恢复总线
       	//Set_I2C_Start_Flag(1);			//开始超时计数
        return I2C_ERR;
    }
	else
	{
		//Set_I2C_Start_Flag(0);			//停止超时计数
	}

    // Generate START
    I2C_SDA_SET_LO();
    i2c_delay_half_clk();
	//i2c_delay_half_clk();
    I2C_SCL_SET_LO();
    i2c_delay_half_clk();
	//i2c_delay_half_clk();

    // Send address
    //return i2c11__write_u8(sla_adr);
		return i2c1_write_u8(sla_adr);
}

int i2c11__send_start(void)
{   
    // Check bus
    if(I2C_SCL_IS_LO() || I2C_SDA_IS_LO())
    {
        // Bus error
       // printf("levevl is low!!\r\n");
       	i2c11__anti_death();				//总线有问题后尝试恢复总线
        return I2C_ERR;
    }

    // Generate START
    I2C_SDA_SET_LO();
    i2c_delay_half_clk();
	//i2c_delay_half_clk();
    I2C_SCL_SET_LO();
    i2c_delay_half_clk();
	//i2c_delay_half_clk();

    return I2C_OK;
}


int i2c11__start_repeat(u8 sla_adr)
{
    I2C_SDA_SET_HIZ();
    i2c_delay_half_clk();

    // Wait for SCL to be high (check for clock stretching)
    if(i2c_scl_set_hiz_and_wait_clk_stretch())
    {
        // Timed out waiting for SCL to be high
        return I2C_ERR;
    }

    return i2c11__start(sla_adr);
}

int i2c11__stop(void)
{
    I2C_SDA_SET_LO();
    i2c_delay_half_clk();

    // Wait for SCL to be high (check for clock stretching)
    if(i2c_scl_set_hiz_and_wait_clk_stretch())
    {
        // Timed out waiting for SCL to be high
        I2C_SDA_SET_HIZ();
        return I2C_ERR;
    }

    // Generate STOP condition
    I2C_SDA_SET_HIZ();
    i2c_delay_half_clk();

    if(I2C_SDA_IS_LO())
    {
        // Error
        return I2C_ERR;
    }

    return I2C_OK;
}

int i2c11__write_u8(u8 data)
{
    u8 bit_mask;
    u8 nak;

    // Send 8 bits, MSB first
    for(bit_mask = (1<<7); bit_mask != 0; bit_mask >>= 1)
    {
        // Send next bit
        if(i2c_tx_bit((data & bit_mask) != 0))
        {
            // Error
            return I2C_ERR;
        }
    }
    // Receive ACK/NAK
    if(i2c_rx_bit(&nak))
    {
        // Error
        return I2C_ERR;
    }
    // Did slave ACK byte written to it?
    if(nak)
    {
        // Release bus
		//printf("NAK\r\n");
        i2c11__stop();            
        return I2C_ERR;
    }
	//printf("ACK\r\n");

    return I2C_OK;
}

int i2c11__read_u8(u8 *data, u8 nak)
{
    u8	bit_mask;
    u8	bit;
    u8	byte;

    byte = 0;
    for(bit_mask = (1<<7); bit_mask != 0; bit_mask >>= 1)
    {
        // Receive next bit
        if(i2c_rx_bit(&bit))
        {
            // Error
            return I2C_ERR;
        }
        // Assemble byte from received bits
        if(bit)
        {
            byte |= bit_mask;
        }
    }
	//printf("byte = %x\r\n",byte);
    // Return received byte
    *data = byte;

    // Send ACK/NAK
    if(i2c_tx_bit(nak))
    {
        // Error
        return I2C_ERR;
    }
    return I2C_OK;
}

int i2c11__write_data(const u8 *data, u32 nr_of_bytes)
{
    while(nr_of_bytes != 0)
    {
        // Send data
        if(i2c11__write_u8(*data))
        {
            // Error
            return I2C_ERR;
        }
        // Next byte
        data++;
        nr_of_bytes--;        
    }

    return I2C_OK;
}

int i2c11__read_data(u8 *data, u32 nr_of_bytes, u8 nak_last_byte)
{
    while(nr_of_bytes != 0)
    {
        if((nr_of_bytes == 1) && (nak_last_byte))
        {
            if(i2c11__read_u8(data, 1))
            {
                // Error
                return I2C_ERR;
            }
        }
        else
        {
            if(i2c11__read_u8(data, 0))
            {
                // Error
                return I2C_ERR;
            }
        }
        
        // Next byte
        data++;
        nr_of_bytes--;
    }
    return I2C_OK;
}
