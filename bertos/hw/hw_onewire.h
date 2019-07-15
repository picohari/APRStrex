

#ifndef HW_ONEWIRE_H
#define HW_ONEWIRE_H



#include "cfg/cfg_onewire.h"


#include <cpu/detect.h>
#include <cpu/attr.h>     		/* CPU_HARVARD */
#include <cfg/cfg_arch.h>
#include <cfg/compiler.h>
#include <cfg/macros.h>


#if CPU_AVR
#include <avr/io.h>
#define F_CPU					CPU_FREQ
#include <hw/hw_cpufreq.h>  	/* CPU_FREQ */
#endif


#define ONEWIRE_BUSCOUNT 		1
#define ONEWIRE_BUSMASK 		128U

#define ONEWIRE_STARTPIN 		7
#define ONEWIRE_PORT 			PORTD
#define ONEWIRE_DDR 			DDRD
#define ONEWIRE_PIN 			PIND




/*
 * macros
 */
#define OW_CONFIG_INPUT(busmask)                              \
  /* enable pullup */                                         \
  ONEWIRE_PORT = (uint8_t)(ONEWIRE_PORT | busmask);           \
  /* configure as input */                                    \
  ONEWIRE_DDR = (uint8_t)(ONEWIRE_DDR & (uint8_t)~busmask);


#define OW_CONFIG_OUTPUT(busmask)                             \
  /* configure as output */                                   \
  ONEWIRE_DDR = (uint8_t)(ONEWIRE_DDR | busmask);


#define OW_LOW(busmask)                                       \
  /* drive pin low */                                         \
  ONEWIRE_PORT = (uint8_t)(ONEWIRE_PORT & (uint8_t)~busmask);


#define OW_HIGH(busmask)                                      \
  /* drive pin high */                                        \
  ONEWIRE_PORT = (uint8_t)(ONEWIRE_PORT | busmask);


#define OW_PULLUP(busmask)                                    \
  /* pull up resistor */                                      \
  ONEWIRE_PORT = (uint8_t)(ONEWIRE_PORT | busmask);


#define OW_GET_INPUT(busmask)                                 \
  (ONEWIRE_PIN & busmask)



#endif /* HW_ONEWIRE_H */