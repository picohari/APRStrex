/*
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
 * Copyright(c) 2008 by Stefan Siegl <stesie@brokenpipe.de>
 * Copyright(c) 2008 by Christian Dietrich <stettberger@dokucode.de>
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

#ifndef AVR_EEPROM_H
#define AVR_EEPROM_H


#include <stdint.h>
#include <stddef.h>
#include <avr/eeprom.h>

#include <cfg/cfg_onewire.h>

#define CRC_SUPPORT     1
#define EEPROM_SUPPORT  1



#if ONEWIRE_NAMING_SUPPORT
#include <hw/hw_onewire.h>
#include <drv/onewire.h>
#endif



struct eeprom_config_t
{


#if ONEWIRE_NAMING_SUPPORT
  ow_name_t ow_names[OW_SENSORS_COUNT];
#endif


  uint8_t crc;
};


#define EEPROM_CONFIG_BASE  (uint8_t *)0x0000


uint8_t crc_checksum (void *data, uint8_t length);
void eeprom_write_block_hack (void *dst, const void *src, size_t n);

/* Reset the EEPROM to sane defaults. */
void eeprom_reset (void);

/* Initialize EEPROM cruft. */
void eeprom_init (void);

/* Calculate crc value, from config saved in eeprom */
uint8_t eeprom_get_chksum (void);

#define eeprom_save(dst, data, len) \
  eeprom_write_block_hack(EEPROM_CONFIG_BASE + offsetof(struct eeprom_config_t, dst), data, len)

#define eeprom_save_offset(dst, off, data, len) \
  eeprom_write_block_hack(EEPROM_CONFIG_BASE + offsetof(struct eeprom_config_t, dst) + off, data, len)

#define eeprom_save_P(dst,data_pgm,len) \
    do { char data[len]; memcpy_P(data, data_pgm, len); eeprom_save(dst, data, len);} while(0)

#define eeprom_save_char(dst,data) \
    do { uint8_t _t = data; eeprom_save(dst, &_t, 1); } while(0)

#define eeprom_save_int(dst, data) \
    do { uint16_t _t = data; eeprom_save(dst, &_t, 2); } while(0)

/* Reads len byte from eeprom at dst into mem */
#define eeprom_restore(dst, mem, len) \
  eeprom_read_block(mem, EEPROM_CONFIG_BASE + offsetof(struct eeprom_config_t, dst), len)

#define eeprom_restore_offset(dst, off, mem, len) \
  eeprom_read_block(mem, EEPROM_CONFIG_BASE + offsetof(struct eeprom_config_t, dst) + off, len)

#define eeprom_restore_ip(dst,mem) \
    eeprom_restore(dst, mem, IPADDR_LEN)

#define eeprom_restore_char(dst,mem) \
    eeprom_restore(dst, mem, 1)

#define eeprom_restore_int(dst, mem) \
    eeprom_restore(dst, mem, 2)

/* Update the eeprom crc */
#define eeprom_update_chksum() eeprom_save_char(crc, eeprom_get_chksum())


//#define read_eeprom_array(address,value_p,length) eeprom_read_block ((void *)value_p, (const void *)address, length)
//#define write_eeprom_array(address,value_p,length) eeprom_write_block ((const void *)value_p, (void *)address, length)

#endif /* AVR_EEPROM_H */
