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
 * Copyright 2003, 2004, 2005, 2006, 2008 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2000 Bernie Innocenti
 * All Rights Reserved.
 * -->
 *
 * \brief Buttons hardware-specific definitions
 *
 *
 * \author Francesco Sacchi <batt@develer.com>
 * \author Stefano Fedrigo <a@develer.com>
 */

#ifndef HW_BUTTONS_H
#define HW_BUTTONS_H

//#include "hw/buttons_map.h"

#include <cfg/cfg_buttons.h>
#include <cfg/macros.h>

#if CPU_AVR
#include <avr/io.h>
#endif

//#warning TODO:This is an example implementation, you must implement it!

//#define B_RPT_MASK (K_UP | K_DOWN | K_OK | K_CANCEL)

#if CPU_AVR
#define ENCODER_PORT	PORTG
#define ENCODER_DDR		DDRG
#define ENCODER_PIN		PING

#define ENC_A			PG0		// Phase A Pin
#define ENC_B			PG1		// Phase B Pin
#define ENC_C			PG2		// OPTIONAL: Button switch
 
#define BTN_HW_INIT \
	do { \
		ENCODER_DDR &= ~( 1 << ENC_A ); \
		ENCODER_PORT |= ( 1 << ENC_A ); \
	    ENCODER_DDR &= ~( 1 << ENC_B ); \
	    ENCODER_PORT |= ( 1 << ENC_B ); \
	    ENCODER_DDR &= ~( 1 << ENC_C ); \
	    ENCODER_PORT |= ( 1 << ENC_C ); \
	} while (0)

#else
#define BTN_HW_INIT		do { /* Nothing */ } while (0)
#endif


#define REPEAT_START 	50			// after 500ms
#define REPEAT_NEXT  	20			// every 200ms



/**
 * Read the keyboard ports and return the mask of
 * depressed keys.
 */
 //INLINE keymask_t btn_readbuttons(void)
//{
	/* Implement me! */

	//Only for test remove when implement this function
	//return 0;
//}

#endif /* HW_BUTTONS_H */