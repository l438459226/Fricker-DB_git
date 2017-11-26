/*
 * (C) 2015 Siarhei Siamashka <siarhei.siamashka@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * Support for the SSD2828 bridge chip, which can take pixel data coming
 * from a parallel LCD interface and translate it on the flight into MIPI DSI
 * interface for driving a MIPI compatible TFT display.
 *
 * Implemented as a utility function. To be used from display drivers, which are
 * responsible for driving parallel LCD hardware in front of the video pipeline.
 */

#ifndef _SSD2828_H
#define _SSD2828_H

#include "sys.h"

#define GPA_PIN3		(((GPIOA_BASE - APB2PERIPH_BASE)<<16) |GPIO_Pin_3)			//0x4001 080C  GPIOA_BASE
#define GPA_PIN5		(((GPIOA_BASE - APB2PERIPH_BASE)<<16) |GPIO_Pin_5)
#define GPA_PIN6		(((GPIOA_BASE - APB2PERIPH_BASE)<<16) |GPIO_Pin_6)
#define GPA_PIN7		(((GPIOA_BASE - APB2PERIPH_BASE)<<16) |GPIO_Pin_7)




#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define		SSD2828_DIR		0xB0
#define		SSD2828_VICR1	0xB1
#define		SSD2828_VICR2	0xB2
#define		SSD2828_VICR3	0xB3
#define		SSD2828_VICR4	0xB4
#define		SSD2828_VICR5	0xB5
#define		SSD2828_VICR6	0xB6
#define		SSD2828_CFGR	0xB7
#define		SSD2828_VCR		0xB8
#define		SSD2828_PCR		0xB9
#define		SSD2828_PLCR	0xBA
#define		SSD2828_CCR		0xBB
#define		SSD2828_PSCR1	0xBC
#define		SSD2828_PSCR2	0xBD
#define		SSD2828_PSCR3	0xBE
#define		SSD2828_PDR		0xBF
#define		SSD2828_OCR		0xC0
#define		SSD2828_MRSR	0xC1
#define		SSD2828_RDCR	0xC2
#define		SSD2828_ARSR	0xC3
#define		SSD2828_LCR		0xC4
#define		SSD2828_ICR		0xC5
#define		SSD2828_ISR		0xC6
#define		SSD2828_ESR		0xC7
#define		SSD2828_DAR1	0xC9
#define		SSD2828_DAR2	0xCA
#define		SSD2828_DAR3	0xCB
#define		SSD2828_DAR4	0xCC
#define		SSD2828_DAR5	0xCD
#define		SSD2828_DAR6	0xCE
#define		SSD2828_HTTR1	0xCF
#define		SSD2828_HTTR2	0xD0
#define		SSD2828_LRTR1	0xD1
#define		SSD2828_LRTR2	0xD2
#define		SSD2828_TSR		0xD3
#define		SSD2828_LRR		0xD4
#define		SSD2828_PLLR	0xD5
#define		SSD2828_TR		0xD6
#define		SSD2828_TECR	0xD7
#define		SSD2828_ACR1	0xD8
#define		SSD2828_ACR2	0xD9
#define		SSD2828_ACR3	0xDA
#define		SSD2828_ACR4	0xDB
#define		SSD2828_IOCR	0xDC
#define		SSD2828_VICR7	0xDD
#define		SSD2828_LCFR	0xDE
#define		SSD2828_DAR7	0xDF
#define		SSD2828_PUCR1	0xE0
#define		SSD2828_PUCR2	0xE1
#define		SSD2828_PUCR3	0xE2
#define		SSD2828_CBCR1	0xE9
#define		SSD2828_CBCR2	0xEA
#define		SSD2828_CBSR	0xEB
#define		SSD2828_ECR		0xEC
#define		SSD2828_VSDR	0xED
#define		SSD2828_TMR		0xEE
#define		SSD2828_GPIO1	0xEF
#define		SSD2828_GPIO2	0xF0
#define		SSD2828_DLYA01	0xF1
#define		SSD2828_DLYA23	0xF2
#define		SSD2828_DLYB01	0xF3
#define		SSD2828_DLYB23	0xF4
#define		SSD2828_DLYC01	0xF5
#define		SSD2828_DLYC23	0xF6
#define		SSD2828_ACR5	0xF7
#define		SSD2828_RR		0xFF

#define	SSD2828_CFGR_HS					(1 << 0)
#define	SSD2828_CFGR_CKE				(1 << 1)
#define	SSD2828_CFGR_SLP				(1 << 2)
#define	SSD2828_CFGR_VEN				(1 << 3)
#define	SSD2828_CFGR_HCLK				(1 << 4)
#define	SSD2828_CFGR_CSS				(1 << 5)
#define	SSD2828_CFGR_DCS				(1 << 6)
#define	SSD2828_CFGR_REN				(1 << 7)
#define	SSD2828_CFGR_ECD				(1 << 8)
#define	SSD2828_CFGR_EOT				(1 << 9)
#define	SSD2828_CFGR_LPE				(1 << 10)
#define	SSD2828_CFGR_TXD				(1 << 11)

#define	SSD2828_VIDEO_MODE_NON_BURST_WITH_SYNC_PULSES	(0 << 2)
#define	SSD2828_VIDEO_MODE_NON_BURST_WITH_SYNC_EVENTS	(1 << 2)
#define	SSD2828_VIDEO_MODE_BURST			(2 << 2)

#define	SSD2828_VIDEO_PIXEL_FORMAT_16BPP		0
#define	SSD2828_VIDEO_PIXEL_FORMAT_18BPP_PACKED		1
#define	SSD2828_VIDEO_PIXEL_FORMAT_18BPP_LOOSELY_PACKED	2
#define	SSD2828_VIDEO_PIXEL_FORMAT_24BPP		3

#define	SSD2828_LP_CLOCK_DIVIDER(n)			(((n) - 1) & 0x3F)















u8 gpio_get_value(u32 pin);

struct ctfb_res_modes;

struct ssd2828_config {
	/*********************************************************************/
	/* SSD2828 configuration                                             */
	/*********************************************************************/

	/*
	 * The pins, which are used for SPI communication. This is only used
	 * for configuring SSD2828, so the performance is irrelevant (only
	 * around a hundred of bytes is moved). Also these can be any arbitrary
	 * GPIO pins (not necessarily the pins having hardware SPI function).
	 * Moreover, the 'sdo' pin may be even not wired up in some devices.
	 *
	 * These configuration variables need to be set as pin numbers for
	 * the standard u-boot GPIO interface (gpio_get_value/gpio_set_value
	 * functions). Note that -1 value can be used for the pins, which are
	 * not really wired up.
	 */
	int csx_pin;
	int sck_pin;
	int sdi_pin;
	int sdo_pin;
	int spi;
	/* SSD2828 reset pin (shared with LCD panel reset) */
	int reset_pin;

	/*
	 * The SSD2828 has its own dedicated clock source 'tx_clk' (connected
	 * to TX_CLK_XIO/TX_CLK_XIN pins), which is necessary at least for
	 * clocking SPI after reset. The exact clock speed is not strictly,
	 * defined, but the datasheet says that it must be somewhere in the
	 * 8MHz - 30MHz range (see "TX_CLK Timing" section). It can be also
	 * used as a reference clock for PLL. If the exact clock frequency
	 * is known, then it can be specified here. If it is unknown, or the
	 * information is not trustworthy, then it can be set to 0.
	 *
	 * If unsure, set to 0.
	 */
	int ssd2828_tx_clk_khz;

	/*
	 * This is not a property of the used LCD panel, but more like a
	 * property of the SSD2828 wiring. See the "SSD2828QN4 RGB data
	 * arrangement" table in the datasheet. The SSD2828 pins are arranged
	 * in such a way that 18bpp and 24bpp configurations are completely
	 * incompatible with each other.
	 *
	 * Depending on the color depth, this must be set to 16, 18 or 24.
	 */
	int ssd2828_color_depth;

	/*********************************************************************/
	/* LCD panel configuration                                           */
	/*********************************************************************/

	/*
	 * The number of lanes in the MIPI DSI interface. May vary from 1 to 4.
	 *
	 * This information can be found in the LCD panel datasheet.
	 */
	int mipi_dsi_number_of_data_lanes;

	/*
	 * Data transfer bit rate per lane. Please note that it is expected
	 * to be higher than the pixel clock rate of the used video mode when
	 * multiplied by the number of lanes. This is perfectly normal because
	 * MIPI DSI handles data transfers in periodic bursts, and uses the
	 * idle time between bursts for sending configuration information and
	 * commands. Or just for saving power.
	 *
	 * The necessary Mbps/lane information can be found in the LCD panel
	 * datasheet. Note that the transfer rate can't be always set precisely
	 * and it may be rounded *up* (introducing no more than 10Mbps error).
	 */
	int mipi_dsi_bitrate_per_data_lane_mbps;

	/*
	 * Setting this to 1 enforces packing of 18bpp pixel data in 24bpp
	 * envelope when sending it over the MIPI DSI link.
	 *
	 * If unsure, set to 0.
	 */
	int mipi_dsi_loosely_packed_pixel_format;

	/*
	 * According to the "Example for system sleep in and out" section in
	 * the SSD2828 datasheet, some LCD panel specific delays are necessary
	 * after MIPI DCS commands EXIT_SLEEP_MODE and SET_DISPLAY_ON.
	 *
	 * For example, Allwinner uses 100 milliseconds delay after
	 * EXIT_SLEEP_MODE and 200 milliseconds delay after SET_DISPLAY_ON.
	 */
	int mipi_dsi_delay_after_exit_sleep_mode_ms;
	int mipi_dsi_delay_after_set_display_on_ms;
};

/*
 * Initialize the SSD2828 chip. It needs the 'ssd2828_config' structure
 * and also the video mode timings.
 *
 * The right place to insert this function call is after the parallel LCD
 * interface is initialized and before turning on the backlight. This is
 * advised in the "Example for system sleep in and out" section of the
 * SSD2828 datasheet. And also SS2828 may use 'pclk' as the clock source
 * for PLL, which means that the input signal must be already there.
 */
 
int ssd2828_init(const struct ssd2828_config *cfg,
		 const struct ctfb_res_modes *mode);

//int read_hw_register(const struct ssd2828_config *cfg, u8 regnum);
u32 read_hw_register(const struct ssd2828_config *cfg, u8 regnum);
void gpio_set_value(u32 pin,u8 val);
int yassy_ssd2828_init(const struct ctfb_res_modes *mode);

#endif
