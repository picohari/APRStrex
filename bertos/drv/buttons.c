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
 * Copyright 2003, 2004, 2005 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2003 Bernie Innocenti
 *
 * -->
 *
 * \brief Keyboard driver (implementation)
 *
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 * \author Francesco Sacchi <batt@develer.com>
 *
 */
#include <stdio.h>
#include <stdlib.h>


#include <avr/io.h>
#include <avr/pgmspace.h>


#include "hw/hw_buttons.h"
#include "cfg/cfg_buttons.h"

#include <cfg/debug.h>
#include <cfg/module.h>
#include <cfg/macros.h>
#include <cfg/compiler.h>  /* uint8_t */

#include <drv/timer.h>

#include <cpu/irq.h>
#include <cpu/types.h>
 
#include <mware/event.h>
#include <drv/buttons.h>


static Timer encoder_timer;            /**< Encoder softtimer */

volatile int8_t enc_delta;			// -127 .0. 127
static int8_t enc_last;

//volatile int16_t rotary_value = 0;

#if CPU_AVR


int8_t encoder_read1(void){		// read single step encoders
	int8_t val;
	cli();
	val = enc_delta;
	enc_delta = 0;
	sei();
	return val;					// counts since last call
}

int8_t encoder_read2(void){		// read two step encoders
	int8_t val;
	cli();
	val = enc_delta;
	enc_delta = val & 1;
	sei();
	return val >> 1;
}

int8_t encoder_read4(void){		// read four step encoders
	int8_t val;
	cli();
	val = enc_delta;
	enc_delta = val & 3;
	sei();
	return val >> 2;
}


int8_t encoder_get_input(void) {

	int8_t cur, diff;
	//int8_t temp;

	cur = 0;
	//temp = 0;

	if( PHASE_A )
		cur = 3;
	if( PHASE_B )
		cur ^= 1;						// convert gray to binary
		diff = enc_last - cur;			// difference last - new
	if( diff & 1 ){						// bit 0 = value (1)
		enc_last = cur;					// store new as next last
		enc_delta += (diff & 2) - 1;	// bit 1 = direction (+/-)
	}

	return encoder_read2();			// read a dual step encoder

	/*
	temp = encoder_read2();			// read a dual step encoder

	if(temp > 0){
		rotary_value++;
		if (rotary_value > 1023) rotary_value = 1023;

	}

	if(temp < 0){
		rotary_value--;
		if (rotary_value < 0) rotary_value = 0;

	}

	return temp;
	*/
}


int8_t encoder_get_button(void) {

	if( !PHASE_C ){
		return 1;
	}
	return 0;
}


static void encoder_read_isr(void)
{
	int8_t cur, diff;

	cur = 0;

	if( PHASE_A )
		cur = 3;
	if( PHASE_B )
		cur ^= 1;						// convert gray to binary
		diff = enc_last - cur;			// difference last - new
	if( diff & 1 ){						// bit 0 = value (1)
		enc_last = cur;					// store new as next last
		enc_delta += (diff & 2) - 1;	// bit 1 = direction (+/-)
	}	
}


static void encoder_softint(UNUSED_ARG(iptr_t, arg))
{
	encoder_read_isr();
	timer_add(&encoder_timer);
}


void encoder_init(void) {

	BTN_HW_INIT;

	int8_t cur = 0;

	if( PHASE_A )
		cur = 3;
	if( PHASE_B )
		cur ^= 1;				// convert gray to binary
	enc_last = cur;				// power on state
	enc_delta = 0;

	event_initSoftint(&encoder_timer.expire, encoder_softint, NULL);
	timer_setDelay(&encoder_timer, ms_to_ticks(5));
	timer_add(&encoder_timer);
}











/*
volatile uint16_t key_state;	// debounced key state (bit = 1: key pressed)
volatile uint16_t key_press;	// key press detect
volatile uint16_t key_release;	// key release detect
volatile uint16_t key_rpt;		// key long press and repeat
*/

/**
uint8_t Key_Code = 0;				// Letzter empfangener KEYCODE

void get_key_code(void){

	//if( get_key_press( 1<<KEY_DIAL )) key_screen_set(DISPLAY_SCREEN_TOGGLE);

	if( get_key_press_all( 1<<KEY_0 ))    Key_Code = KEY_CODE_0;
	if( get_key_press_all( 1<<KEY_1 ))    Key_Code = KEY_CODE_1;
	if( get_key_press_all( 1<<KEY_2 ))    Key_Code = KEY_CODE_2;
	if( get_key_press_all( 1<<KEY_3 ))    Key_Code = KEY_CODE_3;
	if( get_key_press_all( 1<<KEY_4 ))    Key_Code = KEY_CODE_4;
	if( get_key_press_all( 1<<KEY_5 ))    Key_Code = KEY_CODE_5;
	if( get_key_press_all( 1<<KEY_6 ))    Key_Code = KEY_CODE_6;
	if( get_key_press_all( 1<<KEY_7 ))    Key_Code = KEY_CODE_7;
	if( get_key_press_all( 1<<KEY_8 ))    Key_Code = KEY_CODE_8;
	if( get_key_press_all( 1<<KEY_9 ))    Key_Code = KEY_CODE_9;

	if( get_key_press_all( 1<<KEY_STAR )) Key_Code = KEY_CODE_STAR;
	if( get_key_press_all( 1<<KEY_DIAL )) Key_Code = KEY_CODE_DIAL;

}


void default_key_handler(void){

	if( get_key_press( 1<<KEY_DIAL )) key_screen_set(DISPLAY_SCREEN_TOGGLE);
}
**/

/*
void key_control(void)
{
	static uint16_t ct0, ct1, rpt;
	uint16_t i;

	i = key_state ^ key_scan();		// key changed?
	ct0 = ~( ct0 & i );				// reset or count ct0
	ct1 = ct0 ^ (ct1 & i);			// reset or count ct1
	i &= ct0 & ct1;					// count until roll over?
	key_state ^= i;					// then toggle debounced state
	key_press |= key_state & i;		// 0->1: key press detect
	key_release |= ~key_state & i;	// 1->0: key release detect

	if( (key_state & REPEAT_MASK) == 0 )	// check repeat function
		rpt = REPEAT_START;					// start delay
	if( --rpt == 0 )						// repeat waittime = 0
	  {
	    rpt = REPEAT_NEXT;					// repeat delay
	    key_rpt |= key_state & REPEAT_MASK;	// repeat pressed key
	  }

	//get_key_code();
}

uint16_t get_key_long( uint16_t key_mask )
{
	return get_key_press( get_key_rpt( key_mask ));
}

uint16_t get_key_short(uint16_t key_mask)
{
	cli();						// read and clear atomic begin (IMPORTANT)
	return get_key_press( ~key_state & key_mask );
}

uint16_t get_key_rpt(uint16_t key_mask)
{
	cli();						// read and clear atomic begin (IMPORTANT)
	key_mask &= key_rpt;		// read key(s)
	key_rpt ^= key_mask;		// clear key(s)
	sei();						// read and clear atomic end (IMPORTANT)
	return key_mask;
}

uint16_t get_key_release(uint16_t key_mask)
{
	cli();						// read and clear atomic begin (IMPORTANT)
	key_mask &= key_release;	// read key(s)
	key_release ^= key_mask;	// clear key(s)
	sei();						// read and clear atomic end (IMPORTANT)
	return key_mask;
}

uint16_t get_key_press(uint16_t key_mask)
{
	cli();						// read and clear atomic begin (IMPORTANT)
	key_mask &= key_press;    	// read key(s)
	key_press ^= key_mask;    	// clear key(s)
	sei();						// read and clear atomic end (IMPORTANT)
	return key_mask;
}

uint16_t get_key_press_all(uint16_t key_mask)
{
	cli();						// read and clear atomic begin (IMPORTANT)
	key_mask &= key_press;    	// read key(s)
	key_press ^= key_mask;    	// clear key(s)
	sei();						// read and clear atomic end (IMPORTANT)
	return key_mask;
}





static void col_set(void)
{
	DDRA = 0xF8;  	// bit 0.2 are columns (0 is input)
	PORTA = 0x07;  	//pullups on these bits
	//_delay_ms(1);
}

static void row_set(void)
{
	DDRA = 0x87;  	// bit 3..6 used for rows
	PORTA = 0x78;  	//pullups on these bits
	//_delay_ms(1);
}

uint16_t key_scan(void)
{
	uint8_t value;
	uint16_t bitset;

	value = 0;       // init value

	col_set();      //set up columns to read

	// read columns first - 0 if key in that column pressed
	if (!(PINA & 0x01))			{value = 0;}		// Col2 = bit0 is low
	else if (!(PINA & 0x02))	{value = 1;}   		// Col1 = bit1 is low
	else if (!(PINA & 0x04))	{value = 2;}		// Col0 = bit2 is low

	row_set();   //set up rows to read

	// read rows now - 0 if key in that row pressed
	if (!(PINA & 0x08))			{value += 0x01;}	//row0 = bit 3 is low
	else if (!(PINA & 0x10))	{value += 0x04;} 	//row1 = bit 4 is low
	else if (!(PINA & 0x20))	{value += 0x07;} 	//row2 = bit 5 is low
	else if (!(PINA & 0x40))	{value += 0x0A;} 	//row3 = bit 6 is low

	bitset = (1 << value);		// value is sum of row and column

	return bitset;
}
*/

#endif /* CPU_AVR */
