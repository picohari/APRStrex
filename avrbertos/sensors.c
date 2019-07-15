/*
* measurement functions of the AQUALOOP Control Unit
* not intended for use from outside the control unit (monitoring)
* main measurement functions
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


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include <stdbool.h>

#include <drv/onewire.h>
#include <cfg/cfg_onewire.h>

#include "sensors.h"


// Define logging setting (for cfg/log.h module).
#define LOG_LEVEL             3
#define LOG_FORMAT            0
#include <cfg/log.h>


#if ONEWIRE_SUPPORT
int16_t aprs_get_temperature(ow_rom_code_t * rom) {
  int16_t retval = 0x7FFF;      /* error */
  ow_temp_scratchpad_t  sp;
  int8_t ret;

  // TODO: Aktualisiert dann nicht mehr das Display ??
  //ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    ret = ow_temp_read_scratchpad(rom, &sp);
  //}

  if (ret == 1) {
    ow_temp_t temp = ow_temp_normalize(rom, &sp);
    //retval = (temp.twodigits ? temp.val / 10 : temp.val);
    retval = temp.val;
    ret = temp.twodigits;
    //LOG_INFO("%d\n", temp.val);
  }


  //debug_printf("temp: %d digits: %d \n", retval, ret);

  return retval;
}
#endif



void sensors_init(void) {

  #if ONEWIRE_SUPPORT
  if (parse_ow_rom(CONF_SENSOR_EXTERNAL, &romcodeExtTemp) == -1) {
      LOG_INFO("sensor error ow temp\n");
  }
  #endif

}





void sensors_poll(void)
{





}


/*
  -- Ethersex META --
  header(services/intewa/aqualoop/aqualoop_sensors.h)
  timer(10, aqualoop_sensors_periodic())

*/

