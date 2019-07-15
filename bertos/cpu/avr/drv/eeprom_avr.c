/*
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
 * Copyright(c) 2008 Christian Dietrich <stettberger@dokucode.de>
 * Copyright(c) 2008 Stefan Siegl <stesie@brokenpipe.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (either version 2 or
 * version 3) as published by the Free Software Foundation.
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


#include <cpu/detect.h>

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <avr/io.h>
#include <util/crc16.h>
#include <avr/pgmspace.h>


#include <cpu/avr/drv/eeprom_avr.h>
#include <cfg/cfg_onewire.h>


#if CRC_SUPPORT
uint8_t crc_checksum (void *data, uint8_t length)
{

  uint8_t crc = 0;
  uint8_t *p = (uint8_t *) data;

  for (uint8_t i = 0; i < length; i++)
    {
      crc = _crc_ibutton_update (crc, *p);
      p++;
    }

  return crc;
}
#endif /* CRC_SUPPORT */



#if EEPROM_SUPPORT


void eeprom_write_block_hack (void *dst, const void *src, size_t n)
{
  while (n--)
    {
      eeprom_write_byte (&((uint8_t *) dst)[n], ((uint8_t *) src)[n]);
    }
}


uint8_t eeprom_get_chksum (void)
{
  uint8_t eeprom_crc = 0;
  uint8_t *p = (uint8_t *) EEPROM_CONFIG_BASE;

  for (uint16_t i = 0; i < (sizeof (struct eeprom_config_t) - 1); i++)
    {
      eeprom_crc = _crc_ibutton_update (eeprom_crc, eeprom_read_byte (p));
      p++;
    }

  return eeprom_crc;
}


void eeprom_init (void)
{

#if ONEWIRE_NAMING_SUPPORT
  ow_name_t temp_name;
  memset(&temp_name, 0, sizeof(ow_name_t));
  for (int8_t i = 0; i < OW_SENSORS_COUNT; i++)
  {
    eeprom_save(ow_names[i], &temp_name, sizeof(ow_name_t));
  }
#endif

  eeprom_update_chksum ();
}


#endif /* EEPROM_SUPPORT */
