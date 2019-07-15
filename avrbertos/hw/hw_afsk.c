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
 * Copyright 2010 Develer S.r.l. (http://www.develer.com/)
 * All Rights Reserved.
 * -->
 *
 * \brief AFSK modem hardware-specific definitions.
 *
 *
 * \author Francesco Sacchi <batt@develer.com>
 */


#include "hw_afsk.h"
//#include "mobilinkd_error.h"
#include "cfg/cfg_arch.h"

#include <cfg/macros.h>
#include <cfg/compiler.h>


#define LOG_LEVEL   AFSK_LOG_LEVEL
#define LOG_FORMAT  AFSK_LOG_FORMAT
#include <cfg/log.h>


#include <net/afsk.h>
#include <cpu/irq.h>


#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>

#include <drv/buttons.h>

/**
 * Takes a normalized ADC (-512/+511) value and computes the attenuated
 * value, clamps the input within [-128/+127] and returns the result.
 */
 /*
INLINE int8_t input_attenuation(Afsk* af, int16_t adc)
{
    int16_t result = (adc >> af->input_volume_gain);

    if (result < -128) result = -128;
    else if (result > 127) result = 127;

    return result;
}
*/

/*
 * Here we are using only one modem. If you need to receive
 * from multiple modems, you need to define an array of contexts.
 */
static Afsk *ctx;


/**
 * Initialize the ADC.  The ADC runs at its slowest rate (125KHz) to
 * ensure best resolution possible.
 *
 * - The AVCC (3.3V) voltage reference is used.
 * - The prescaler is set to 125KHz, and set to free-running mode, providing
 *   almost exactly 9600 conversions per second.
 * - The signal source is set to the given ADC channel.
 * - The ADC channel is set to input with digital input disabled.
 * - The ADC interrupt is enabled.
 *
 * @param ch
 * @param _ctx
 */
void hw_afsk_adcInit(int ch, Afsk *_ctx)
{
	ctx = _ctx;
    //ASSERT(ch <= 5);
	ASSERT(ch <= 7);

	AFSK_STROBE_INIT();
	AFSK_STROBE_OFF();

    /* Set reference to AVCC (3.3V) and select ADC channel. */
    ADMUX = BV(REFS0) | ch;                     // ATmega128 has no 1.1 Volt from internal
    //ADMUX = BV(REFS0) | BV(REFS1) | ch;       // Set reference to 2.56 Volt from internal

    DDRF &= ~BV(ch);
    PORTF &= ~BV(ch);

    DIDR0 |= BV(ch);                        // Disable digital input pins on ADC input
    
    // Set prescaler to 128 so we have a 125KHz clock source.
    // This provides almost exactly 9600 conversions a second.
    ADCSRA = (BV(ADPS2) | BV(ADPS1) | BV(ADPS0));

    // Put the ADC into free-running mode.
    ADCSRB &= ~(BV(ADTS2) | BV(ADTS1) | BV(ADTS0));

    // Set signal source to free running, start the ADC, start
    // conversion and enable interrupt.
    //ADCSRA |= (BV(ADATE) | BV(ADEN) | BV(ADSC) | BV(ADIE));

    //ADCSRA |= (BV(ADFR) | BV(ADEN) | BV(ADSC) | BV(ADIE));  // ATmega128 has no ADATE

    //ADCSRA |= (BV(ADEN) | BV(ADSC) | BV(ADIE));
    ADCSRA = 0;

    LOG_INFO("ADC init.. ok\n");
}


void hw_afsk_adc_stop(void)  { ADCSRA  = 0; }

void hw_afsk_adc_start(void) { ADCSRA |= (BV(ADATE) | BV(ADEN) | BV(ADSC) | BV(ADIE)); }



bool hw_afsk_dac_isr = false;

/*
void hw_afsk_dacInit(int ch, struct Afsk *_ctx)
{
    AFSK_PORT_INIT;
}
*/



/**
 * The ADC interrupt.  This is called 9600 times a second while input
 * capture is enabled.  The data is pulled from the ADC, attenuated,
 * and the attenuated value is placed on the ADC FIFO.  This will then
 * be processed by the bottom-half handler outside the interrupt
 * context.
 *
 * If the ADC FIFO overflows, the error is recorded and the TNC is reset.
 */

DECLARE_ISR(ADC_vect)
{

    //TIFR1 = BV(ICF1);
    afsk_adc_isr(ctx, ((int16_t)((ADC) >> 2) - 128));

/*
            __asm__ __volatile__ (
            "nop" "\n\t");
*/

    // Dirty hack to get encoder running inside a ISR to be fast enough...
    //encoder_read_isr();

/*
    if (hw_afsk_dac_isr)
        PORTC = afsk_dac_isr(ctx) & 0xF0;
    else
        PORTC = 128;
*/

    //PORTB ^= (1 << PB7);
}




#if CONFIG_AFSK_PWM_TX == 1

DECLARE_ISR(TIMER0_OVF_vect)
{
    OCR0 = afsk_dac_isr(ctx);             // uses timer 0 on port D bit 5
}

#else


DECLARE_ISR(TIMER2_COMPA_vect)
{
    //AFSK_STROBE_ON();
    // TCNT2 = DAC_TIMER_VALUE;
    PORTC = ( (PORTC & 0x0F) | (afsk_dac_isr(ctx) & 0xF0) );
    //PORTC = ( (PORTC & 0x0F) | (afsk_dac_test(ctx) & 0xF0) );
    //AFSK_STROBE_OFF();
}
#endif






#if 0
void aprs_send_packet(const char *_buffer, size_t length)
{

    // STOP ADC
    ADCSRA = 0;


    aprs_send_packet_raw( _buffer, length);



    // Enable ADC to receive APRS messages
    ADCSRA |= (BV(ADATE) | BV(ADEN) | BV(ADSC) | BV(ADIE));

}
#endif

