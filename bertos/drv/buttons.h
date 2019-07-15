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
 * \defgroup kbd Keyboard handling driver
 * \ingroup drivers
 * \{
 * \brief Keyboard driver.
 *
 * <b>Configuration file</b>: cfg_kbd.h
 *
 * <b>HAL files</b>: hw_kbd.h, kbd_map.h
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 * \author Francesco Sacchi <batt@develer.com>
 *
 * $WIZ$ module_name = "kbd"
 * $WIZ$ module_depends = "timer"
 * $WIZ$ module_configuration = "bertos/cfg/cfg_kbd.h"
 * $WIZ$ module_hw = "bertos/hw/hw_kbd.h", "bertos/hw/kbd_map.h"
 */

#ifndef DRV_BUTTONS_H
#define DRV_BUTTONS_H

#include "hw/hw_buttons.h"
#include "cfg/cfg_buttons.h" // CONFIG_KBD_OBSERVER

#include <cfg/compiler.h>

#include <struct/list.h>



#define PHASE_A		(ENCODER_PIN & 1 << ENC_A)
#define PHASE_B		(ENCODER_PIN & 1 << ENC_B)
#define PHASE_C		(ENCODER_PIN & 1 << ENC_C)


//extern int8_t enc_delta;			// -127 .0. 127
//extern int8_t enc_last;

//volatile int16_t rotary_value;


void encoder_init(void);

int8_t encoder_read1(void);
int8_t encoder_read2(void);
int8_t encoder_read4(void);

int8_t encoder_get_input(void);
int8_t encoder_get_button(void);

//void encoder_read_isr(void);





#if 0
extern uint8_t Key_Code;        /*!< Letzter empfangener RC5-Code */

/* Physikalische Tastenzuordnung für Biterkennung bei Entprellung */
#define KEY_1			(0x01)		/*!< Taste 1 */
#define KEY_2			(0x02)		/*!< Taste 2 */
#define KEY_3			(0x03)		/*!< Taste 3 */
#define KEY_4			(0x04)		/*!< Taste 4 */
#define KEY_5			(0x05)		/*!< Taste 5 */
#define KEY_6			(0x06)		/*!< Taste 6 */
#define KEY_7			(0x07)		/*!< Taste 7 */
#define KEY_8			(0x08)		/*!< Taste 8 */
#define KEY_9			(0x09)		/*!< Taste 9 */
#define KEY_STAR		(0x0A)		/*!< Taste * */
#define KEY_0			(0x0B)		/*!< Taste 0 */
#define KEY_DIAL		(0x0C)		/*!< Taste # */

/* Logische Zuordnung für Berechnungen, Auswertung etc. */
#define KEY_CODE_0			(0x10)		/*!< Taste 0 */
#define KEY_CODE_1			(0x11)		/*!< Taste 1 */
#define KEY_CODE_2			(0x12)		/*!< Taste 2 */
#define KEY_CODE_3			(0x13)		/*!< Taste 3 */
#define KEY_CODE_4			(0x14)		/*!< Taste 4 */
#define KEY_CODE_5			(0x15)		/*!< Taste 5 */
#define KEY_CODE_6			(0x16)		/*!< Taste 6 */
#define KEY_CODE_7			(0x17)		/*!< Taste 7 */
#define KEY_CODE_8			(0x18)		/*!< Taste 8 */
#define KEY_CODE_9			(0x19)		/*!< Taste 9 */
#define KEY_CODE_STAR		(0x1A)		/*!< Taste * */
#define KEY_CODE_DIAL		(0x1B)		/*!< Taste # */


#define REPEAT_MASK  (1<<KEY_1^1<<KEY_2)	// repeat: key1, key2



/*!
 * @brief	Liest ein Key-Codeword und wertet es aus
 */
void key_control(void);

/*!
 * @brief	Ordnet den Tasten eine Aktion zu und fuehrt diese aus.
 */
void get_key_code(void);

void default_key_handler(void);


uint16_t get_key_long( uint16_t key_mask );
uint16_t get_key_short(uint16_t key_mask);
uint16_t get_key_rpt(uint16_t key_mask);
uint16_t get_key_release(uint16_t key_mask);
uint16_t get_key_press(uint16_t key_mask);
uint16_t get_key_press_all(uint16_t key_mask);

uint16_t key_scan(void);
#endif





#endif