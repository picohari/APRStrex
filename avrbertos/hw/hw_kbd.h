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
 * \brief Keyboard hardware-specific definitions
 *
 *
 * \author Francesco Sacchi <batt@develer.com>
 * \author Stefano Fedrigo <a@develer.com>
 */

#ifndef HW_KBD_H
#define HW_KBD_H


#include "hw/kbd_map.h"

#include <drv/buttons.h>
#include <cfg/cfg_buttons.h>

#include <cfg/macros.h>

#if CPU_AVR
#include <avr/io.h>
#endif
 
//#warning TODO:This is an example implementation, you must implement it!


/*

// Version mit vier Tastern für die Eingabe

#define KEYS_PORT		PORTG
#define KEYS_DDR		DDRG
#define KEYS_PINS		PING

#define K_UP_PIN		PG0
#define K_DOWN_PIN		PG1
#define K_ENTER_PIN		PG2

#define K_UP_PINS		PG0
#define K_DOWN_PINS		PG1
#define K_ENTER_PINS	PG2



// Pins  als Eingang
// internen Pull-Up aktivieren

		KEYS_DDR  &= ~(1<<K_UP_PIN); \
		KEYS_PORT |= (1<<K_UP_PIN); \
		KEYS_DDR  &= ~(1<<K_DOWN_PIN); \
		KEYS_PORT |= (1<<K_DOWN_PIN); \
		KEYS_DDR  &= ~(1<<K_ENTER_PIN); \
		KEYS_PORT |= (1<<K_ENTER_PIN); \


	// Get input keys first...
	if ( !(KEYS_PINS & (1<<K_UP_PINS)) )    mask |= K_UP;
	if ( !(KEYS_PINS & (1<<K_DOWN_PINS)) )  mask |= K_DOWN;
	if ( !(KEYS_PINS & (1<<K_ENTER_PINS)) ) mask |= K_OK;

*/


#if 0
#define K_RPT_MASK	( K_ONE | K_TWO | K_THREE |	K_FOUR | K_FIVE | K_SIX | K_SEVEN |	K_EIGHT | K_NINE | K_CANCEL | K_ZERO | K_OK )	
#else
//#define K_RPT_MASK	( K_OK )	
#define K_RPT_MASK (K_UP | K_DOWN | K_OK | K_CANCEL)
#endif

// Set PORT G as columns drivers output and active pullups (PIN is logic "1")
// Set PORT B as rows inputs and active pullups (PIN is logic "1")

/*
*/


#if 0
#define KBD_HW_INIT \
	do { \
		DDRB  &=  ~( (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3 ) ); \
		PORTB |=   ( (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3 ) ); \
		DDRG  |=   ( (1 << PG0) | (1 << PG1) | (1 << PG2) ); \
		PORTG |=   ( (1 << PG0) | (1 << PG1) | (1 << PG2) ); \
		} while (0)
#else
#define KBD_HW_INIT \
	do { \
		} while (0)
#endif

EXTERN_C int emul_kbdReadCols(void);

/*
 * Read the keyboard ports and return the mask of
 * depressed keys.
 */
INLINE keymask_t kbd_readkeys(void)
{

	keymask_t mask = 0;


#if CPU_AVR

	#if 0
	// Drive column to LOW, so if pressed, we got a PINx = 0
	PORTG  |= ( (1 << PG1) | (1 << PG2) );
	PORTG  &=  ~(1 << PG0);										
	asm volatile ("nop");					

	// read rows now - 0 if key in that row pressed
	if (!(PINB & 0x01))			{mask |= K_ONE;}	//row0 = bit 1 is low
	else if (!(PINB & 0x02))	{mask |= K_FOUR;} 	//row1 = bit 2 is low
	else if (!(PINB & 0x04))	{mask |= K_SEVEN;} 	//row2 = bit 3 is low
	else if (!(PINB & 0x08))	{mask |= K_CANCEL;} //row3 = bit 4 is low

	PORTG  |=  ( (1 << PG0) | (1 << PG2) );					
	PORTG  &=   ~(1 << PG1);										
	asm volatile ("nop");					

	// read rows now - 0 if key in that row pressed
	if (!(PINB & 0x01))			{mask |= K_TWO;}	//row0 = bit 1 is low
	else if (!(PINB & 0x02))	{mask |= K_FIVE;} 	//row1 = bit 2 is low
	else if (!(PINB & 0x04))	{mask |= K_EIGHT;} 	//row2 = bit 3 is low
	else if (!(PINB & 0x08))	{mask |= K_ZERO;} 	//row3 = bit 4 is low

	PORTG  |=  ( (1 << PG0) | (1 << PG1) );					
	PORTG  &=   ~(1 << PG2);										
	asm volatile ("nop");					

	// read rows now - 0 if key in that row pressed
	if (!(PINB & 0x01))			{mask |= K_THREE;}	//row0 = bit 1 is low
	else if (!(PINB & 0x02))	{mask |= K_SIX;} 	//row1 = bit 2 is low
	else if (!(PINB & 0x04))	{mask |= K_NINE;} 	//row2 = bit 3 is low
	else if (!(PINB & 0x08))	{mask |= K_OK;} 	//row3 = bit 4 is low

	// Activate all PORTG
	PORTG  |=  ( (1 << PG0) | (1 << PG1) | (1 << PG2) );		// PORTG Pullups OFF

	#else

	int8_t result = 0;

	result = encoder_get_input();

	if (result > 0){
		mask |= K_RIGHT;
	}

	else if (result < 0){
		mask |= K_LEFT;
	}

	else if (!PHASE_C){
		mask |= K_OK;
	}

	#endif


	return mask;

#else
	//Only for test remove when implement this function
	return (keymask_t)emul_kbdReadCols();
	//return 0;
#endif

}

#endif /* HW_KBD_H */








