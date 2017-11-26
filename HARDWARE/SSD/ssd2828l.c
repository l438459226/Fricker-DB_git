/*
 * (C) 2015 Siarhei Siamashka <siarhei.siamashka@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Support for the SSD2828 bridge chip, which can take pixel data coming
 * from a parallel LCD interface and translate it on the flight into MIPI DSI
 * interface for driving a MIPI compatible TFT display.
 */
#include "sys.h"
#include "delay.h"
#include "mipi_display.h"
#include "usart.h"
#include "spi.h"	
#include "videomodes.h"
#include "LCM.h"
#include "lcd.h"
#include "ssd2828l.h"

u8 buffer[128]; 
//add 
struct ssd2828_config ssd_parm;

void gpio_set_value(u32 pin,u8 val)
{
	if(val)	GPIO_SetBits(((GPIO_TypeDef *)( (pin>>16) + APB2PERIPH_BASE)),pin);
		else 	GPIO_ResetBits(((GPIO_TypeDef *)( (pin>>16) + APB2PERIPH_BASE)),pin);	
}

u8 gpio_get_value(u32 pin)
{
		return GPIO_ReadInputDataBit(((GPIO_TypeDef *)( (pin>>16) + APB2PERIPH_BASE)),pin);
}

void gpio_free(u16 pin)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA,和AFIO时钟
	
	GPIO_InitStructure.GPIO_Pin = pin;	   ///PORTA 3 5 6 7 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				//GPIOA	
}

void gpio_direction_output(u16 pin,u8 val)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA,和AFIO时钟
	
	GPIO_InitStructure.GPIO_Pin = pin;	   ///PORTA 3 5 6 7 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				//GPIOA	
	if(val)	GPIO_SetBits(GPIOA,pin);
		else 	GPIO_ResetBits(GPIOA,pin);
}

void gpio_direction_input(u16 pin)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA,和AFIO时钟
	
	GPIO_InitStructure.GPIO_Pin = pin;	   ///PORTA 3 5 6 7 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				//GPIOA	
}	

void yassy_lcm()
{
	//yt50f285();
}


/*
 * SPI transfer, using the "24-bit 3 wire" mode (that's how it is called in
 * the SSD2828 documentation). The 'dout' input parameter specifies 24-bits
 * of data to be written to SSD2828. Returns the lowest 16-bits of data,
 * that is received back.
 */
static u32 soft_spi_xfer_24bit_3wire(const struct ssd2828_config *drv, u32 dout)
{
	int j, bitlen = 24;
	u32 tmpdin = 0;
	/*
	 * According to the "24 Bit 3 Wire SPI Interface Timing Characteristics"
	 * and "TX_CLK Timing Characteristics" tables in the SSD2828 datasheet,
	 * the lowest possible 'tx_clk' clock frequency is 8MHz, and SPI runs
	 * at 1/8 of that after reset. So using 1 microsecond delays is safe in
	 * the main loop. But the delays around chip select pin manipulations
	 * need to be longer (up to 16 'tx_clk' cycles, or 2 microseconds in
	 * the worst case).
	 */
	const int spi_delay_us = 1;
	const int spi_cs_delay_us = 2;

	gpio_set_value(drv->csx_pin, 0);
	delay_us(spi_cs_delay_us);
	for (j = bitlen - 1; j >= 0; j--) {
		gpio_set_value(drv->sck_pin, 0);
		gpio_set_value(drv->sdi_pin, (dout & (1 << j)) != 0);
		delay_us(spi_delay_us);
		if (drv->sdo_pin != -1)
			tmpdin = (tmpdin << 1) | gpio_get_value(drv->sdo_pin);
		gpio_set_value(drv->sck_pin, 1);
		delay_us(spi_delay_us);
	}
	delay_us(spi_cs_delay_us);
	gpio_set_value(drv->csx_pin, 1);
	delay_us(spi_cs_delay_us);
	return tmpdin & 0xFFFF;
}

/*
 * Read from a SSD2828 hardware register (regnum >= 0xB0)
 */
u32 read_hw_register(const struct ssd2828_config *cfg, u8 regnum)
{
	soft_spi_xfer_24bit_3wire(cfg, 0x700000 | regnum);
	return soft_spi_xfer_24bit_3wire(cfg, 0x730000);
}

/*
 * Write to a SSD2828 hardware register (regnum >= 0xB0)
 */
static void write_hw_register(const struct ssd2828_config *cfg, u8 regnum,
			      u16 val)
{
	soft_spi_xfer_24bit_3wire(cfg, 0x700000 | regnum);
	soft_spi_xfer_24bit_3wire(cfg, 0x720000 | val);
}

/*
 * Send MIPI command to the LCD panel (cmdnum < 0xB0)
 */
static void send_mipi_dcs_command(const struct ssd2828_config *cfg, u8 cmdnum)
{
	/* Set packet size to 1 (a single command with no parameters) */
	write_hw_register(cfg, SSD2828_PSCR1, 1);
	/* Send the command */
	write_hw_register(cfg, SSD2828_PDR, cmdnum);
}

/*
 * Reset SSD2828
 */
static void ssd2828_reset(const struct ssd2828_config *cfg)
{
	/* RESET needs 10 milliseconds according to the datasheet */
	gpio_set_value(cfg->reset_pin, 0);
	delay_ms(10);
	gpio_set_value(cfg->reset_pin, 1);
	delay_ms(10);
}
int ssd2828_enable_gpio(const struct ssd2828_config *cfg)
{
	/*
	gpio_direction_output(cfg->reset_pin, 0);
	gpio_direction_output(cfg->csx_pin, 1);
	gpio_direction_output(cfg->sck_pin, 1);
	gpio_direction_output(cfg->sdi_pin, 1);
	gpio_direction_input(cfg->sdo_pin);
*/
	
	GPIO_InitTypeDef  GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //使能PORTA,和AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7;	   ///PORTC 3 5 6 7 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	   ///PORTC 3 5 6 7 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA	
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PA6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //???????
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//???GPIOA6
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	
	return 0;
}

int ssd2828_free_gpio(const struct ssd2828_config *cfg)
{
	/*
	gpio_free(cfg->csx_pin);
	gpio_free(cfg->sck_pin);
	gpio_free(cfg->sdi_pin);
	gpio_free(cfg->reset_pin);
	if (cfg->sdo_pin != -1)
		gpio_free(cfg->sdo_pin);*/
	
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); //使能PORTA,和AFIO时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_6;	   ///PORTA 3 5 6 7 复用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 				//GPIOA	
	
	return 1;
}

/*
 * PLL configuration register settings.
 *
 * See the "PLL Configuration Register Description" in the SSD2828 datasheet.
 */
static u32 construct_pll_config(u32 desired_pll_freq_kbps,
				u32 reference_freq_khz)
{
	u32 div_factor = 1, mul_factor, fr = 0;
	u32 output_freq_kbps;

	/* The intermediate clock after division can't be less than 5MHz */
	while (reference_freq_khz / (div_factor + 1) >= 5000)
		div_factor++;
	if (div_factor > 31)
		div_factor = 31;

	mul_factor = DIV_ROUND_UP(desired_pll_freq_kbps * div_factor,
				  reference_freq_khz);

	output_freq_kbps = reference_freq_khz * mul_factor / div_factor;

	if (output_freq_kbps >= 501000)
		fr = 3;
	else if (output_freq_kbps >= 251000)
		fr = 2;
	else if (output_freq_kbps >= 126000)
		fr = 1;

	return (fr << 14) | (div_factor << 8) | mul_factor;
}

static u32 decode_pll_config(u32 pll_config, u32 reference_freq_khz)
{
	u32 mul_factor = pll_config & 0xFF;
	u32 div_factor = (pll_config >> 8) & 0x1F;
	if (mul_factor == 0)
		mul_factor = 1;
	if (div_factor == 0)
		div_factor = 1;
	return reference_freq_khz * mul_factor / div_factor;
}

static int ssd2828_configure_video_interface(const struct ssd2828_config *cfg,
					     const struct ctfb_res_modes *mode)
{
	u32 val;
	u32 vbp;
	u32 hbp;
	
	/* RGB Interface Control Register 1 */
	write_hw_register(cfg, SSD2828_VICR1, (mode->vsync_len << 8) |
					      (mode->hsync_len));

	/* RGB Interface Control Register 2 */
	vbp = mode->vsync_len + mode->upper_margin;
	hbp = mode->hsync_len + mode->left_margin;
	write_hw_register(cfg, SSD2828_VICR2, (vbp << 8) | hbp);

	/* RGB Interface Control Register 3 */
	write_hw_register(cfg, SSD2828_VICR3, (mode->lower_margin << 8) |
					      (mode->right_margin));

	/* RGB Interface Control Register 4 */
	write_hw_register(cfg, SSD2828_VICR4, mode->xres);

	/* RGB Interface Control Register 5 */
	write_hw_register(cfg, SSD2828_VICR5, mode->yres);

	/* RGB Interface Control Register 6 */
	val = SSD2828_VIDEO_MODE_BURST;
	switch (cfg->ssd2828_color_depth) {
	case 16:
		val |= SSD2828_VIDEO_PIXEL_FORMAT_16BPP;
		break;
	case 18:
		val |= cfg->mipi_dsi_loosely_packed_pixel_format ?
			SSD2828_VIDEO_PIXEL_FORMAT_18BPP_LOOSELY_PACKED :
			SSD2828_VIDEO_PIXEL_FORMAT_18BPP_PACKED;
		break;
	case 24:
		val |= SSD2828_VIDEO_PIXEL_FORMAT_24BPP;
		break;
	default:
		printf("SSD2828: unsupported color depth\n");
		return 1;
	}
	write_hw_register(cfg, SSD2828_VICR6, val);

	/* Lane Configuration Register */
	write_hw_register(cfg, SSD2828_LCFR,
			  cfg->mipi_dsi_number_of_data_lanes - 1);

	return 0;
}

int ssd2828_init(const struct ssd2828_config *cfg,
		 const struct ctfb_res_modes *mode)
{
	u32 lp_div, pll_freq_kbps, reference_freq_khz, pll_config;
	/* The LP clock speed is limited by 10MHz */
	const u32 mipi_dsi_low_power_clk_khz = 10000;
	u32 t;
	/*
	 * This is just the reset default value of CFGR register (0x301).
	 * Because we are not always able to read back from SPI, have
	 * it initialized here.
	 */
	u32 cfgr_reg = SSD2828_CFGR_EOT | /* EOT Packet Enable */
		       SSD2828_CFGR_ECD | /* Disable ECC and CRC */
		       SSD2828_CFGR_HS;   /* Data lanes are in HS mode */
	
	/* Initialize the pins */
	if (ssd2828_enable_gpio(cfg) != 0)
		return 1;

	/* Reset the chip */
	ssd2828_reset(cfg);
	
	/*
	 * If there is a pin to read data back from SPI, then we are lucky. Try
	 * to check if SPI is configured correctly and SSD2828 is actually able
	 * to talk back.
	 */

	if (cfg->sdo_pin != -1) {
		if (read_hw_register(cfg, SSD2828_DIR) != 0x2828) {
				t =	read_hw_register(cfg, SSD2828_CFGR);
				sprintf((char*)buffer,"t=%04X reg=%04X",t,cfgr_reg);
			LCD_ShowString(0,70,176,24,16,buffer);
			ssd2828_free_gpio(cfg);
			return 1;
		}
	}

	/*
	 * Pick the reference clock for PLL. If we know the exact 'tx_clk'
	 * clock speed, then everything is good. If not, then we can fallback
	 * to 'pclk' (pixel clock from the parallel LCD interface). In the
	 * case of using this fallback, it is necessary to have parallel LCD
	 * already initialized and running at this point.
	 */
	reference_freq_khz = cfg->ssd2828_tx_clk_khz;
	if (reference_freq_khz  == 0) {
		reference_freq_khz = mode->pixclock_khz;
		/* Use 'pclk' as the reference clock for PLL */
		cfgr_reg |= SSD2828_CFGR_CSS;
	}

	/*
	 * Setup the parallel LCD timings in the appropriate registers.
	 */
	if (ssd2828_configure_video_interface(cfg, mode) != 0) {
		ssd2828_free_gpio(cfg);
		return 1;
	}

	/* Configuration Register */
	cfgr_reg &= ~SSD2828_CFGR_HS;  /* Data lanes are in LP mode */
	cfgr_reg |= SSD2828_CFGR_CKE;  /* Clock lane is in HS mode */
	cfgr_reg |= SSD2828_CFGR_DCS;  /* Only use DCS packets */
	write_hw_register(cfg, SSD2828_CFGR, cfgr_reg);

	/* PLL Configuration Register */
	pll_config = construct_pll_config(
				cfg->mipi_dsi_bitrate_per_data_lane_mbps * 1000,
				reference_freq_khz);
	write_hw_register(cfg, SSD2828_PLCR, pll_config);

	pll_freq_kbps = decode_pll_config(pll_config, reference_freq_khz);
	lp_div = DIV_ROUND_UP(pll_freq_kbps, mipi_dsi_low_power_clk_khz * 8);

	/* VC Control Register */
	write_hw_register(cfg, SSD2828_VCR, 0);

	/* Clock Control Register */
	write_hw_register(cfg, SSD2828_CCR, SSD2828_LP_CLOCK_DIVIDER(lp_div));

	/* PLL Control Register */
	write_hw_register(cfg, SSD2828_PCR, 1); /* Enable PLL */

	/* Wait for PLL lock */
	delay_us(500);
	
	yassy_lcm();
	

	send_mipi_dcs_command(cfg, MIPI_DCS_EXIT_SLEEP_MODE);			//11
	delay_ms(cfg->mipi_dsi_delay_after_exit_sleep_mode_ms);

	send_mipi_dcs_command(cfg, MIPI_DCS_SET_DISPLAY_ON);			//29
	delay_ms(cfg->mipi_dsi_delay_after_set_display_on_ms);

	cfgr_reg |= SSD2828_CFGR_HS;    /* Enable HS mode for data lanes */
	cfgr_reg |= SSD2828_CFGR_VEN;   /* Enable video pipeline */
	write_hw_register(cfg, SSD2828_CFGR, cfgr_reg);

	return 0;
}
/*#define SPI_SDO		   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)  		//输入		PA3-SPI_CS    PA5-SPI_CLK   PA6-MISO   PA7-MOSI
#define SSD_CS       PAout(3)		//GPIO_SetBits(GPIOA,GPIO_Pin_3)		//PAout(3) 		//
#define SPI_SCLK     PAout(5)
#define SPI_SDI      PAout(7)
#define SSD_RESET    PBout(4) */
int yassy_ssd2828_init(const struct ctfb_res_modes *mode)
{
		ssd_parm.csx_pin = GPA_PIN3,
		ssd_parm.sck_pin = GPA_PIN5,
		ssd_parm.sdi_pin = GPA_PIN7,
		ssd_parm.sdo_pin = GPA_PIN6,
		ssd_parm.reset_pin = GPA_PIN3,
		ssd_parm.ssd2828_tx_clk_khz  = 24 * 1000,	//CONFIG_VIDEO_LCD_SSD2828_TX_CLK
		ssd_parm.ssd2828_color_depth = 24,
		ssd_parm.mipi_dsi_number_of_data_lanes           = 3,
		ssd_parm.mipi_dsi_bitrate_per_data_lane_mbps     = 520,
		ssd_parm.mipi_dsi_delay_after_exit_sleep_mode_ms = 100,
		ssd_parm.mipi_dsi_delay_after_set_display_on_ms  = 200;
	  
		return ssd2828_init(&ssd_parm, mode);
}

/*
#ifdef CONFIG_VIDEO_LCD_SSD2828
static int sunxi_ssd2828_init(const struct ctfb_res_modes *mode)
{
	struct ssd2828_config cfg = {
		.csx_pin = name_to_gpio(CONFIG_VIDEO_LCD_SPI_CS),
		.sck_pin = name_to_gpio(CONFIG_VIDEO_LCD_SPI_SCLK),
		.sdi_pin = name_to_gpio(CONFIG_VIDEO_LCD_SPI_MOSI),
		.sdo_pin = name_to_gpio(CONFIG_VIDEO_LCD_SPI_MISO),
		.reset_pin = name_to_gpio(CONFIG_VIDEO_LCD_SSD2828_RESET),
		.ssd2828_tx_clk_khz  = CONFIG_VIDEO_LCD_SSD2828_TX_CLK * 1000,
		.ssd2828_color_depth = 24,
#ifdef CONFIG_VIDEO_LCD_PANEL_MIPI_4_LANE_513_MBPS_VIA_SSD2828
		.mipi_dsi_number_of_data_lanes           = 4,
		.mipi_dsi_bitrate_per_data_lane_mbps     = 513,
		.mipi_dsi_delay_after_exit_sleep_mode_ms = 100,
		.mipi_dsi_delay_after_set_display_on_ms  = 200
#else
#error MIPI LCD panel needs configuration parameters
#endif
	};

	if (cfg.csx_pin == -1 || cfg.sck_pin == -1 || cfg.sdi_pin == -1) {
		printf("SSD2828: SPI pins are not properly configured\n");
		return 1;
	}
	if (cfg.reset_pin == -1) {
		printf("SSD2828: Reset pin is not properly configured\n");
		return 1;
	}

	return ssd2828_init(&cfg, mode);
}
#endif  CONFIG_VIDEO_LCD_SSD2828 */


