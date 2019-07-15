/*
*
* internal functions of the AQUALOOP Control Unit
* not intended for use from outside the control unit (monitoring)
*
* Copyright (c) 2014 by Harald W. Leschner <leschner@intewa.de>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 3
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* For more information on the GPL, please go to:
* http://www.gnu.org/copyleft/gpl.html
*/

#ifndef APRS_SENSORS_H
#define APRS_SENSORS_H


#include <drv/onewire.h>
#include <cfg/cfg_onewire.h>


#define APRS_ADC_12V_CHANNEL		1	/* PORT PF0 (ADC0) connected to 24 V measurement voltage divider */

//#define CONF_SENSOR_EXTERNAL	"28cffcd405000018" /* V2 Board bei Haraldo */
#define CONF_SENSOR_EXTERNAL	"28ff878a70140449" /* V2 Board bei Haraldo */
								 



/* Eingangsspannungen am A/D Wandler */
//extern volatile uint16_t capt_adc_24v;



/* 1W Temperatursensor */
#if ONEWIRE_SUPPORT

ow_rom_code_t romcodeExtTemp;

int16_t aprs_get_temperature(ow_rom_code_t *rom);

#endif


void sensors_init(void);

void sensors_poll(void);



#endif /* APRS_SENSORS_H */
