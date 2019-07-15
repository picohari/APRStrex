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
 * Copyright 2003, 2004, 2005, 2006, 2010 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2001 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 *
 *
 * \brief Graphic KS0108 LCD driver
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 * \author Harald W. Leschner <hari@h9l.net>
 * 
 */

#ifndef HW_LCD_KS0108_H
#define HW_LCD_KS0108_H

#include <cpu/types.h>
#include <cpu/irq.h>

#include <avr/io.h>

//#warning TODO:This is an example implementation, you must implement it!


/*
 * The following defines are for panel specific low level timing.
 *
 * See your data sheet for the exact timing and waveforms.
 * All defines below are in nanoseconds.
 */

#define GLCD_tDDR   380    /* Data Delay time (E high to valid read data)        */
#define GLCD_tASU   140    /* Address setup time (ctrl line changes to E HIGH    */
#define GLCD_tWH    450    /* E hi level width (minimum E hi pulse width)        */
#define GLCD_tRST  2000    /* Reset Delay time  */

#define GLCD_tDSU   200    /* Data setup time (data lines setup to dropping E)   */
#define GLCD_tWL    450    /* E lo level width (minimum E lo pulse width)        */


/**
 * Predefined LCD PWM contrast values
 */
#define LCD_DEF_PWM 	145
#define LCD_MAX_PWM 	505
#define LCD_MIN_PWM 	130
#define LCD_PWM_CH    	0


/**
 * \name LCD I/O pins/ports
 * @{
 */
#define LCD_RS		/* Implement me! */		/* Data or Instruction */
#define LCD_RW		/* Implement me! */		/* Read or Write */
#define LCD_CS1		/* Implement me! */		/* Chip Select 1 */
#define LCD_CS2		/* Implement me! */		/* Chip Select 2 */
#define LCD_ENA		/* Implement me! */		/* Strobe Enable */
#define LCD_RST		/* Implement me! */		/* Reset */

#define LCD_DB0		/* Implement me! */
#define LCD_DB1		/* Implement me! */
#define LCD_DB2		/* Implement me! */
#define LCD_DB3		/* Implement me! */
#define LCD_DB4		/* Implement me! */
#define LCD_DB5		/* Implement me! */
#define LCD_DB6		/* Implement me! me! */
#define LCD_DB7		/* SET */

//#define LCD_PF_DB0	/* Implement me! */
/*@}*/

/**
 * \name LCD bus control macros
 * @{
 */
#define LCD_CLR_RS		do { /* Implement me! */ } while (0)
#define LCD_SET_RS		do { /* Implement me! */ } while (0)

#define LCD_CLR_RW		do { /* Implement me! */ } while (0)
#define LCD_SET_RW		do { /* Implement me! */ } while (0)

#define LCD_CLR_ENA		do { /* Implement me! */ } while (0)
#define LCD_SET_ENA		do { /* Implement me! */ } while (0)

#define LCD_CLR_CS1		do { /* Implement me! */ } while (0)
#define LCD_SET_CS1		do { /* Implement me! */ } while (0)

#define LCD_CLR_CS2		do { /* Implement me! */ } while (0)
#define LCD_SET_CS2		do { /* Implement me! */ } while (0)

#define LCD_SET_CS(x)				\
	do {							\
	LCD_CLR_CS1;					\
	LCD_CLR_CS2;					\
	switch(x) {						\
		case 0: LCD_SET_CS1; break;	\
		case 1: LCD_SET_CS2; break;	\
	default: break;					\
	}								\
	} while (0)

#define LCD_CLR_CS(x)				\
	do {							\
	switch(x) {						\
		case 0: LCD_CLR_CS1; break;	\
		case 1: LCD_CLR_CS2; break;	\
	default: break;					\
	}								\
	} while (0)
/*@}*/

/**
 * \name Chip select bits for LCD_SET_E()
 * @{
 */
#define LCD_BCS1                 ( 0 )
#define LCD_BCS2                 ( 1 )
/*@}*/

/** Set data bus direction to output (write to display) */
#define LCD_DB_OUT		( /* Implement me! */ )

/** Set data bus direction to input (read from display) */
#define LCD_DB_IN		( /* Implement me! */ )

/** Write from the LCD data bus (DB[0-7]) */
#define LCD_WRITE(x)	( /* Implement me! */ )

/** Read to the LCD data bus (DB[0-7]) */
#define LCD_READ		( /* Implement me! */ )



/** Delay for reset (Enable pulse width, 2000ns) */
#define LCD_CMD_RESET	\
	do { \
		/* Implement me! */
	} while (0)


INLINE void lcd_ks0108_hw_bus_init(void)
{
	cpu_flags_t flags;
	IRQ_SAVE_DISABLE(flags);

	/*
	 * Data bus is in output state most of the time:
	 * LCD r/w functions assume it is left in output state
	 */
	LCD_DB_OUT;

	IRQ_RESTORE(flags);
}



#endif /* HW_LCD_KS0108_H */

