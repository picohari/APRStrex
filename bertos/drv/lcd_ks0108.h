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
 * Copyright 2003, 2004, 2006 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2001 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 * \author Harald W. Leschner <hari@h9l.net>
 *
 * \brief Graphic KS0108 LCD driver
 *
 * $WIZ$ module_name = "lcd_ks0108"
 * $WIZ$ module_hw = "bertos/hw/hw_lcd_ks0108.h"
 * $WIZ$ module_configuration = "bertos/cfg/cfg_lcd_ks0108.h"
 * $WIZ$ module_depends = "timer", "pwm"
 */

#ifndef DRV_LCD_KS0108_H
#define DRV_LCD_KS0108_H

#include <gfx/gfx.h> /* Bitmap */

#include <cpu/types.h>

/* Display bitmap dims */
#define LCD_WIDTH   128
#define LCD_HEIGHT  64
//#define LCD_WIDTH	    16
//#define LCD_HEIGHT	8

/* Display chip dims */
#define CHIP_HEIGHT	64
#define CHIP_WIDTH	64

/* Number of LCD pages */
#define LCD_PAGES	8

/* Number of Chips and column by coordinate */
#define LCD_CHIP_COUNT ((LCD_WIDTH / CHIP_WIDTH) * (LCD_HEIGHT / CHIP_HEIGHT))

#if CHIP_HEIGHT < LCD_HEIGHT
#define LCD_XY2CHIP(x,y) 	( (x/CHIP_WIDTH) + ( (y/CHIP_HEIGHT) * (LCD_HEIGHT/CHIP_HEIGHT) ) )
#else
#define LCD_XY2CHIP(x,y)	( (x/CHIP_WIDTH) )
#endif
#define LCD_XY2CHIP_COL(x)	( (x) % CHIP_WIDTH )

/* Width of an LCD page */
#define LCD_PAGESIZE		(LCD_WIDTH / LCD_CHIP_COUNT)


/**
 * \name KS0108 Commands
 * @{
 */
#define LCD_CMD_DISPLAY_ON	0x3F
#define LCD_CMD_DISPLAY_OFF	0x3E
#define LCD_CMD_STARTLINE	0xC0
#define LCD_CMD_PAGEADDR	0xB8
#define LCD_CMD_COLADDR		0x40

//#define LCD_CMD_ADC_LEFT	0xA1
//#define LCD_CMD_ADC_RIGHT	0xA0
//#define LCD_CMD_STATIC_OFF	0xA4
//#define LCD_CMD_STATIC_ON	0xA5
//#define LCD_CMD_DUTY_32	0xA9
//#define LCD_CMD_DUTY_16	0xA8
//#define LCD_CMD_RMW_ON	0xE0
//#define LCD_CMD_RMW_OFF	0xEE

//#define LCD_CMD_RESET		0xE2
/*@}*/

/* Status flags */
#define LCDF_BUSY		BV(7)
#define LCDF_RESET 		BV(4)



//#warning __FILTER_NEXT_WARNING__
//#warning this drive is untested..

void lcd_ks0108_init(void);
void lcd_ks0108_setPwm(int duty);

void lcd_ks0108_blitBitmap(const Bitmap *bm);


/*
 * Precise Delay Timings for optimum performance
 *
 * */

#include <inttypes.h>
#include <hw/hw_cpufreq.h>  /* CPU_FREQ */


/*
#ifndef F_CPU
#warning "Macro F_CPU defined by makefile"

#ifndef CPU_FREQ
#warning "CPU_FREQ is NOT defined !!"
#endif
 
//#define F_CPU	CPU_FREQ
//#define F_CPU   (16000000UL)
#endif
*/


/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var " = "  VALUE(var)







/*
 * Forward declaration for all functions with attribute
 * 'always_inline' enforces GCC to inline the code (even
 * if it would be better not to do so from optimization
 * perspective).
 * Without this attribute GCC is free to implement
 * inline code or not (using the keyword 'inline'
 * alone is not sufficient).
 *
 */
static __inline__ void _NOP1( void) __attribute__((always_inline));
static __inline__ void _NOP2( void) __attribute__((always_inline));
static __inline__ void _NOP3( void) __attribute__((always_inline));
static __inline__ void _NOP4( void) __attribute__((always_inline));
static __inline__ void _NOP5( void) __attribute__((always_inline));
static __inline__ void _NOP6( void) __attribute__((always_inline));
static __inline__ void _NOP7( void) __attribute__((always_inline));
static __inline__ void _NOP8( void) __attribute__((always_inline));
static __inline__ void _NOP9( void) __attribute__((always_inline));
static __inline__ void _NOP10(void) __attribute__((always_inline));
static __inline__ void _NOP11(void) __attribute__((always_inline));
static __inline__ void _NOP12(void) __attribute__((always_inline));

static __inline__ void _delay_loop_3(  uint32_t) __attribute__((always_inline));
static __inline__ void _delay_loop_1_x( uint8_t) __attribute__((always_inline));
static __inline__ void _delay_loop_2_x(uint16_t) __attribute__((always_inline));
static __inline__ void _delay_loop_3_x(uint32_t) __attribute__((always_inline));

static __inline__ void _delay_cycles(const double) __attribute__((always_inline));


/*
 * _ N O P x ( void )
 *
 * Code sized optimized NOPs - not using any registers
 *
 * These NOPs will be used for very short delays where
 * it is more code efficient than executing loops.
 *
 */
static __inline__ void _NOP1 (void) { __asm__ volatile ( "nop    " "\n\t" ); }
static __inline__ void _NOP2 (void) { __asm__ volatile ( "rjmp 1f" "\n\t"  "1:" "\n\t" ); }
static __inline__ void _NOP3 (void) { __asm__ volatile ( "lpm    " "\n\t" ); }
static __inline__ void _NOP4 (void) { _NOP3(); _NOP1(); }
static __inline__ void _NOP5 (void) { _NOP3(); _NOP2(); }
static __inline__ void _NOP6 (void) { _NOP3(); _NOP3(); }
static __inline__ void _NOP7 (void) { _NOP3(); _NOP3(); _NOP1(); }
static __inline__ void _NOP8 (void) { _NOP3(); _NOP3(); _NOP2(); }
static __inline__ void _NOP9 (void) { _NOP3(); _NOP3(); _NOP3(); }
static __inline__ void _NOP10(void) { _NOP3(); _NOP3(); _NOP3(); _NOP1(); }
static __inline__ void _NOP11(void) { _NOP3(); _NOP3(); _NOP3(); _NOP2(); }
static __inline__ void _NOP12(void) { _NOP3(); _NOP3(); _NOP3(); _NOP3(); }



/*
 *  _ d e l a y _ l o o p _ 3( uint32_t __count )
 *
 * This delay loop is not used in the code below: It is
 * a supplement to the _delay_loop_1() and _delay_loop_2()
 * within standard WinAVR <arv/delay.h> giving a wider
 * (32 bit) delay range.
 *
 */
static __inline__ void
_delay_loop_3( uint32_t __count )
{
    __asm__ volatile (
        "1: sbiw %A0,1" "\n\t"
        "sbc %C0,__zero_reg__" "\n\t"
        "sbc %D0,__zero_reg__" "\n\t"
        "brne 1b"
        : "=w" (__count)
        : "0" (__count)
    );
}


/*
 *  _ d e l a y _ l o o p _ 1 _ x( uint8_t __count )
 *  _ d e l a y _ l o o p _ 2 _ x( uint16_t  __count )
 *  _ d e l a y _ l o o p _ 4 _ x( uint32_t __count )
 *
 *  These delay loops always have exactly 4(8) cycles per loop.
 *  They use a 8/16/32 bit register counter respectively.
 *
 */
static __inline__ void      /* exactly 4 cycles/loop, max 2**8 loops */
_delay_loop_1_x( uint8_t __n )
{                                               /* cycles per loop      */
    __asm__ volatile (                          /* __n..one        zero */
        "1: dec  %0"   "\n\t"                   /*    1             1   */
        "   breq 2f"   "\n\t"                   /*    1             2   */
        "2: brne 1b"   "\n\t"                   /*    2             1   */
        : "=r" (__n)                            /*  -----         ----- */
        : "0" (__n)                             /*    4             4   */
    );
}

static __inline__ void      /* exactly 4 cycles/loop, max 2**16 loops */
_delay_loop_2_x( uint16_t __n )
{                                               /* cycles per loop      */
    __asm__ volatile (                          /* __n..one        zero */
        "1: sbiw %0,1"   "\n\t"                 /*    2             2   */
        "   brne 1b  "   "\n\t"                 /*    2             1   */
        "   nop      "   "\n\t"                 /*                  1   */
        : "=w" (__n)                            /*  -----         ----- */
        : "0" (__n)                             /*    4             4   */
    );
}

static __inline__ void      /* exactly 8 cycles/loop, max 2**32 loops */
_delay_loop_3_x( uint32_t __n )
{                                               /* cycles per loop      */
    __asm__ volatile (                          /* __n..one        zero */
        "1: sbiw %A0,1           "  "\n\t"      /*    2             2   */
        "   sbc  %C0,__zero_reg__"  "\n\t"      /*    1             1   */
        "   sbc  %D0,__zero_reg__"  "\n\t"      /*    1             1   */
        "   nop                  "  "\n\t"      /*    1             1   */
        "   breq 2f              "  "\n\t"      /*    1             2   */
        "2: brne 1b              "  "\n\t"      /*    2             1   */
        : "=w" (__n)                            /*  -----         ----- */
        : "0" (__n)                             /*    8             8   */
    );
}


/*
 *
 *  _ d e l a y _ c y c l e s (double __ticks_d)
 *
 *  Perform an accurate delay of a given number of processor cycles.
 *
 *  All the floating point arithmetic will be handled by the
 *  GCC Preprocessor and no floating point code will be generated.
 *  Allthough the parameter __ticks_d is of type 'double' this
 *  function can be called with any constant integer value, too.
 *  GCC will handle the casting appropriately.
 *
 *  With an 8 MHz clock e.g., delays ranging from 125 nanoseconds
 *  up to (2**32-1) * 125ns ~= 536,87 seconds are feasible.
 *
 */
static __inline__ void
_delay_cycles(const double __ticks_d)
{
    uint32_t __ticks = (uint32_t)(__ticks_d);
    uint32_t __padding;
    uint32_t __loops;

    /*
     * check for rounding/truncation
     * that truncated fractional cycle count down to 0
     *
     * If you ask for more than 0 but less than 1
     * you get 1 cycle.
     */

    if(__ticks_d && !__ticks)
        __ticks = 1;

    /*
     * Special optimization for very
     * small delays - not using any register.
     */
    if( __ticks <= 12 )  {              /* this can be done with 4 opcodes      */
        __padding = __ticks;

    /* create a single byte counter */
    } else if( __ticks <= 0x400 )  {
        __ticks -= 1;                   /* caller needs 1 cycle to init counter */
        __loops = __ticks / 4;
        __padding = __ticks % 4;
        if( __loops != 0 )
            _delay_loop_1_x( (uint8_t)__loops );

    /* create a two byte counter */
    } else if( __ticks <= 0x40001 )  {
        __ticks -= 2;                   /* caller needs 2 cycles to init counter */
        __loops = __ticks / 4;
        __padding = __ticks % 4;
        if( __loops != 0 )
            _delay_loop_2_x( (uint16_t)__loops );

    /* create a four byte counter */
    } else  {
        __ticks -= 4;                   /* caller needs 4 cycles to init counter */
        __loops = __ticks / 8;
        __padding = __ticks % 8;
        if( __loops != 0 )
            _delay_loop_3_x( (uint32_t)__loops );
    }

    if( __padding ==  1 )  _NOP1();
    if( __padding ==  2 )  _NOP2();
    if( __padding ==  3 )  _NOP3();
    if( __padding ==  4 )  _NOP4();
    if( __padding ==  5 )  _NOP5();
    if( __padding ==  6 )  _NOP6();
    if( __padding ==  7 )  _NOP7();
    if( __padding ==  8 )  _NOP8();
    if( __padding ==  9 )  _NOP9();
    if( __padding == 10 ) _NOP10();
    if( __padding == 11 ) _NOP11();
    if( __padding == 12 ) _NOP12();
}


/*
 *   _ d e l a y _ n s (double __ns)
 *   _ d e l a y _ u s (double __us)
 *
 *   Perform a very exact delay with a resolution as accurate as a
 *   single CPU clock (the macro F_CPU is supposed to be defined to a
 *   constant defining the CPU clock frequency in Hertz).
 *
 */
#define LCD_DELAY_NS(__ns)     _delay_cycles( (double)(CPU_FREQ)*((double)__ns)/1.0e9 + 0.5 )

#endif /* DRV_LCD_KS0108_H */
