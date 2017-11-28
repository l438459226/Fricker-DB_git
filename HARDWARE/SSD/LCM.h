#ifndef __LCM_H
#define __LCM_H

#include "sys.h"



typedef struct vidinfo {

	
	u16	width;	/* Width of display area in millimeters */
	u16	height;	/* Height of display area in millimeters */
	u8 fps;
	u16 pclk;
	u8 pixel;
	/* LCD configuration register */
	u8	hsync;		/* Horizontal Sync polarity */
	u8	hbp;		/* Vertical Sync polarity */
	u8	hfp;		/* Data polarity */
	u8	vsync;	/* Bits per pixel, 0 = 1, 1 = 2, 2 = 4, 3 = 8 */
	u8	vbp;
	u8	vfp;
	
	u8 dsi_lane;
	u16 dsi_mbps;
	u8 dsi_speed;
	
} vidinfo_t;




#ifndef CONFIG_SYS_DEFAULT_VIDEO_MODE
#define CONFIG_SYS_DEFAULT_VIDEO_MODE	0x301
#endif

/* Some mode definitions */
#define FB_SYNC_HOR_HIGH_ACT	1	/* horizontal sync high active	*/
#define FB_SYNC_VERT_HIGH_ACT	2	/* vertical sync high active	*/
#define FB_SYNC_EXT		4	/* external sync		*/
#define FB_SYNC_COMP_HIGH_ACT	8	/* composite sync high active	*/
#define FB_SYNC_BROADCAST	16	/* broadcast video timings	*/
					/* vtotal = 144d/288n/576i => PAL  */
					/* vtotal = 121d/242n/484i => NTSC */
#define FB_SYNC_ON_GREEN	32	/* sync on green */
#define FB_VMODE_NONINTERLACED	0	/* non interlaced */
#define FB_VMODE_INTERLACED	1	/* interlaced	*/
#define FB_VMODE_DOUBLE		2	/* double scan */
#define FB_VMODE_MASK		255

#define FB_VMODE_YWRAP		256	/* ywrap instead of panning	*/
#define FB_VMODE_SMOOTH_XPAN	512	/* smooth xpan possible (internally used) */
#define FB_VMODE_CONUPDATE	512	/* don't update x/yoffset	*/


/******************************************************************
 * Resolution Struct
 ******************************************************************/
struct ctfb_res_modes {
	int xres;		/* visible resolution		*/
	int yres;
	int refresh;			/* vertical refresh rate in hz  */
	/* Timing: All values in pixclocks, except pixclock (of course) */
	int pixclock;			/* pixel clock in ps (pico seconds) */
	int pixclock_khz;	/* pixel clock in kHz*/
	
	int left_margin;	/* time from sync to picture	*/
	int right_margin;	/* time from picture to sync	*/
	int upper_margin;	/* time from sync to picture	*/
	int lower_margin;
	
	int hsync_len;		/* length of horizontal sync	*/
	int vsync_len;		/* length of vertical sync	*/
	int sync;					/* see FB_SYNC_*/
	int vmode;				/* see FB_VMODE_*		*/
};

/******************************************************************
 * Vesa Mode Struct
 ******************************************************************/
struct ctfb_vesa_modes {
	int vesanr;		/* Vesa number as in LILO (VESA Nr + 0x200} */
	int resindex;		/* index to resolution struct */
	int bits_per_pixel;	/* bpp */
};

#define RES_MODE_640x480		0
#define RES_MODE_800x600		1
#define RES_MODE_1024x768		2
#define RES_MODE_960_720		3
#define RES_MODE_1152x864		4
#define RES_MODE_1280x1024	5
#define RES_MODE_1280x720		6
#define RES_MODE_1360x768		7
#define RES_MODE_1920x1080	8
#define RES_MODE_1920x1200	9
#define RES_MODES_COUNT			10

#define VESA_MODES_COUNT 		19

extern const struct ctfb_vesa_modes vesa_modes[];
extern const struct ctfb_res_modes res_mode_init[];

extern vidinfo_t panel_info;
void SSD_MODE(u8 vido_mode,u8 mode);
void SSD_LANE(u8 lane,u16 clock);


void InitMipi_PanelInit(void);
void InitMipi_PanelInitt(void);

#endif