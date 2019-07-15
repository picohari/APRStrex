/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2003, 2004, 2005, 2006 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2001 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 *
 * \brief Graphic KS0108 LCD driver
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 * \author Harald W. Leschner <hari@h9l.net>
 *
 */


#include "lcd_ks0108.h"

#include "hw/hw_lcd_ks0108.h"

#include "cfg/cfg_lcd_ks0108.h"

#include <cfg/macros.h> /* BV() */
#include <cfg/debug.h>
#include <cfg/module.h>

#include <gfx/gfx.h>

#include <drv/timer.h>
//#include <drv/pwm.h>

#include <cpu/irq.h>
#include <cpu/types.h>

/**
#pragma message(VAR_NAME_VALUE(CPU_FREQ))

#pragma message(VAR_NAME_VALUE(GLCD_tDDR))
#pragma message(VAR_NAME_VALUE(GLCD_tASU))
#pragma message(VAR_NAME_VALUE(GLCD_tWH))

#pragma message(VAR_NAME_VALUE(LCD_CHIP_COUNT))
#pragma message(VAR_NAME_VALUE(LCD_PAGESIZE))
#pragma message("Display size: " VAR_NAME_VALUE(LCD_WIDTH) " x " VAR_NAME_VALUE(LCD_HEIGHT))
**/

#if CONFIG_LCD_WAIT
	/**
	 * \code
	 *      __              __
	 * RS   __\____________/__
	 *         ____________
	 * R/W  __/            \__
	 *            _______
	 * E1   _____/       \____
	 *        ______      ____
	 * DATA X/      \====/
	 *
	 * \endcode
	 */
	#define WAIT_LCD(chip) \
		do { \
			uint8_t status; \
			LCD_DB_IN; \
			do { \
				LCD_SET_CS(chip); \
				LCD_SET_RW; \
				LCD_CLR_RS; \
				LCD_DELAY_NS(GLCD_tASU); \
				LCD_SET_ENA; \
				LCD_DELAY_NS(GLCD_tDDR); \
				status = LCD_READ; \
				LCD_CLR_ENA; \
			} while (status & (LCDF_BUSY | LCDF_RESET)); \
			LCD_DB_OUT; \
		} while (0)
#else /* CONFIG_LCD_WAIT */
	#define WAIT_LCD(x) do {(void)x; } while(0)
#endif /* CONFIG_LCD_WAIT */


/**
 * Raster buffer to draw into.
 *
 * Bits in the bitmap bytes have vertical orientation,
 * as required by the LCD driver.
 */
//static uint8_t raster[RAST_SIZE(LCD_WIDTH, LCD_HEIGHT)];

/** Default LCD bitmap */
//static Bitmap lcd_bitmap;


#if CONFIG_LCD_SOFTINT_REFRESH

/** Timer for regular LCD refresh */
static Timer lcd_refresh_timer;

#endif /* CONFIG_LCD_SOFTINT_REFRESH */


INLINE void lcd_ks0108_cmd(uint8_t cmd, uint8_t chip)
{
	switch(chip){
		case 0: WAIT_LCD(0); break;
		case 1: WAIT_LCD(1); break;
	}

	/*      __              __
	 * RS   __\____________/__
	 *
	 * R/W  __________________
	 *            ______
	 * CS   _____/      \_____
	 *
	 * DATA --<============>--
	 */

	//LCD_SET_CS(chip);
	LCD_DB_OUT;
	LCD_CLR_RW;
	LCD_CLR_RS;
	LCD_WRITE(cmd);
	LCD_DELAY_NS(GLCD_tASU);

	LCD_SET_ENA;
	LCD_DELAY_NS(GLCD_tWH);
	LCD_CLR_ENA;

}


INLINE uint8_t lcd_ks0108_read(uint8_t chip)
{
	uint8_t data;

	switch(chip){
		case 0: WAIT_LCD(0); break;
		case 1: WAIT_LCD(1); break;
	}

	/**
	 * \code
	 *      __________________
	 * A0   __/            \__
	 *         ____________
	 * R/W  __/            \__
	 *            _______
	 * E1   _____/       \____
	 *
	 * DATA -------<=====>----
	 *
	 * \endcode
	 */

	//LCD_SET_CS(chip);
	LCD_DB_IN;
	LCD_SET_RW;
	LCD_SET_RS;
	LCD_DELAY_NS(GLCD_tASU);

	LCD_SET_ENA;
	LCD_DELAY_NS(GLCD_tDDR);
	data = LCD_READ;
	LCD_CLR_ENA;

	return data;
}

INLINE void lcd_ks0108_write(uint8_t data, uint8_t chip)
{
	switch(chip){
		case 0: WAIT_LCD(0); break;
		case 1: WAIT_LCD(1); break;
	}

	/**
	 * \code
	 *      __________________
	 * A0   ___/          \___
	 *
	 * R/W  __________________
	 *            ______
	 * E1   _____/      \_____
	 *
	 * DATA -<==============>-
	 *
	 * \endcode
	 */

	//LCD_SET_CS(chip);
	LCD_DB_OUT;
	LCD_CLR_RW;
	LCD_SET_RS;
	LCD_WRITE(data);
	LCD_DELAY_NS(GLCD_tASU);

	LCD_SET_ENA;
	LCD_DELAY_NS(GLCD_tWH);
	LCD_CLR_ENA;

}

#ifdef testing
void lcd_ks0108_testing_A(void)
{
	uint8_t page, j;

	uint8_t chip = 0;
	for (uint8_t chip=0; chip < LCD_CHIP_COUNT; chip++)
	{
		for (page = 0; page < LCD_PAGES; ++page)
		{
			lcd_ks0108_cmd(LCD_CMD_COLADDR, chip);
			lcd_ks0108_cmd(LCD_CMD_PAGEADDR | page, chip);
			for (j = 0; j < LCD_PAGESIZE; j++)
			{
				/* Show debug pattern */
				j % 2 ? lcd_ks0108_write(0xAA, chip) : lcd_ks0108_write(0x55, chip);
			}
		}
	}

}

void lcd_ks0108_testing_B(void)
{
	uint8_t page, j;

	for (uint8_t chip=0; chip < LCD_CHIP_COUNT; chip++)
	{
		for (page = 0; page < LCD_PAGES; ++page)
		{
			lcd_ks0108_cmd(LCD_CMD_COLADDR, chip);
			lcd_ks0108_cmd(LCD_CMD_PAGEADDR | page, chip);
			for (j = 0; j < LCD_PAGESIZE; j++)
			{
				/* Show debug pattern */
				j % 2 ? lcd_ks0108_write(0x55, chip) : lcd_ks0108_write(0xAA, chip);
			}
		}
	}

}
#endif

static void lcd_ks0108_clear(void)
{
	uint8_t page, j;

	for (uint8_t chip = 0; chip < LCD_CHIP_COUNT; chip++)
	{
		for (page = 0; page < LCD_PAGES; page++)
		{
			lcd_ks0108_cmd(LCD_CMD_COLADDR, chip);
			lcd_ks0108_cmd(LCD_CMD_PAGEADDR | page, chip);
			for (j = 0; j < LCD_PAGESIZE; j++)
			{
				/* Clear Display */
				lcd_ks0108_write(0x00, chip);

				/* Show debug pattern */
				//j % 2 ? lcd_ks0108_write(0xAA, chip) : lcd_ks0108_write(0x55, chip);
			}
		}
	}

}


#if CONFIG_LCD_SOFTINT_REFRESH

static void lcd_ks0108_refreshSoftint(void)
{
	lcd_ks0108_blitBitmap(&lcd_bitmap);
	timer_setDelay(&lcd_refresh_timer, ms_to_ticks(CONFIG_LCD_REFRESH));
	timer_add(&lcd_refresh_timer);
}

#endif /* CONFIG_LCD_SOFTINT_REFRESH */

/**
 * Set LCD contrast PWM.
 */
void lcd_ks0108_setPwm(int duty)
{
	ASSERT(duty >= LCD_MIN_PWM);
	ASSERT(duty <= LCD_MAX_PWM);

	//pwm_setDuty(LCD_PWM_CH, duty);
	//pwm_enable(LCD_PWM_CH, true);
}



void lcd_ks0108_blitBitmap(const Bitmap *bm)
{
	uint8_t page = 0;		/* Page (Row) of LCD */
	uint8_t col = 0;		/* X-Position inside page */
	uint16_t bufferpos = 0;		/* Buffer-Index of Bitmap Raster */

	for (page = 0; page < LCD_PAGES; page++)
	{
		/* Set both chips to XY position */
		lcd_ks0108_cmd(LCD_CMD_COLADDR, LCD_BCS1);
		lcd_ks0108_cmd(LCD_CMD_PAGEADDR | page, LCD_BCS1);

		lcd_ks0108_cmd(LCD_CMD_COLADDR, LCD_BCS2);
		lcd_ks0108_cmd(LCD_CMD_PAGEADDR | page, LCD_BCS2);
		
		/* Whole raster row is written (with offset) to both chips */
		for (col = 0; col < LCD_PAGESIZE; col++)
		{
			/* Left side of  display */
			lcd_ks0108_write(bm->raster[bufferpos], LCD_BCS1);
			
			/* Right side of display */
			lcd_ks0108_write(bm->raster[bufferpos + LCD_PAGESIZE], LCD_BCS2);

			bufferpos++;
		}
		/* Calculate raster offset for the next loop */
		bufferpos += LCD_PAGESIZE;
	}
}


/**
 * Initialize LCD subsystem.
 *
 * \note The PWM used for LCD contrast is initialized in drv/pwm.c
 *       because it is the same PWM used for output attenuation.
 */
void lcd_ks0108_init(void)
{
	MOD_CHECK(timer);

	//pwm_init();

	/* Setup Hardware Configuration */
	lcd_ks0108_hw_bus_init();
	LCD_CMD_RESET;

	/* Additional extra blind delay to get things started up */
	timer_delay(100);

	/* Send initialization sequence to each chip */

	for (uint8_t chip=0; chip < LCD_CHIP_COUNT; chip++){

		lcd_ks0108_cmd(LCD_CMD_DISPLAY_ON, chip);
		lcd_ks0108_cmd(LCD_CMD_STARTLINE, chip);
		lcd_ks0108_cmd(LCD_CMD_PAGEADDR, chip);
		lcd_ks0108_cmd(LCD_CMD_COLADDR, chip);
	}

	lcd_ks0108_clear();
	//lcd_ks0108_setPwm(LCD_DEF_PWM);

	//gfx_bitmapInit(&lcd_bitmap, raster, LCD_WIDTH, LCD_HEIGHT);
	//gfx_bitmapClear(&lcd_bitmap);

#if CONFIG_LCD_SOFTINT_REFRESH
	/* Init IRQ driven LCD refresh */
	timer_setSoftint(&lcd_refresh_timer, (Hook)lcd_ks0108_refreshSoftint, 0);
	timer_setDelay(&lcd_refresh_timer, ms_to_ticks(CONFIG_LCD_REFRESH));
	timer_add(&lcd_refresh_timer);
#endif /* CONFIG_LCD_SOFTINT_REFRESH */


}

