

#define GPS 		1
#define ONEWIRE 	1



#include <verstag.h>
#include <buildrev.h>

#include <cfg/macros.h>
#include <cfg/debug.h>

// Define logging setting (for cfg/log.h module).
#define LOG_LEVEL         	3
#define LOG_FORMAT     		0
#include <cfg/log.h>

#include <stdio.h>
#include <stdlib.h>	//malloc()
#include <alloca.h>
#include <string.h>
#include <math.h>


#if CPU_AVR
	#include <avr/eeprom.h>
	#include <avr/io.h>
	#include <cpu/avr/drv/ram_avr.h>
	#include <cpu/avr/drv/eeprom_avr.h>
	#include <io/kfile.h>

	//#define F_CPU 	CPU_FREQ
	//#include <util/delay.h>
#else
	#include <emul/emul.h>
	#include <emul/kfile_posix.h>

#endif



#include <cpu/irq.h>
#include <drv/timer.h>
#include <mware/event.h>

#include <drv/ser.h>
#include <kern/proc.h>



#if CPU_AVR
	#include <drv/lcd_ks0108.h>
	#include <gfx/gfx_p.h> // BM_PLOT()
	#include <gui/micromenu.h>
	#include <drv/buttons.h>
#if ONEWIRE
	#include <drv/onewire.h>
	#include <mware/fixedpoint.h>
#endif
	#include "sensors.h"
#else
	#include <drv/lcd_gfx_qt.h>
	#include <gfx/gfx.h>
	#include <gfx/win.h>
	#include <gui/menu_pro.h>
	#include <kern/proc.h>
	#include <kern/sem.h>
#endif

#include <drv/kbd.h>
#include <gfx/font.h>
#include <gfx/text.h>
#include <icons/logo.h>



//#include <drv/buttons.h>


#include <net/afsk.h>
#include <net/ax25.h>

#include "menu.h"
#include "station.h"
#include "aprs.h"

#if GPS
	#include <net/nmea.h>
	#include "gps.h"
#endif

#if !CPU_AVR
	#include <struct/kfile_mem.h>
	static KFileMem mem;

	static uint8_t nmea_test[] =
	{
	"$GPGLL,5046.31711,N,00604.61726,E,130211.00,A,A*6D\r\n"
	"$GPZDA,130211.00,30,01,2016,00,00*61\r\n"
	"$GPRMC,130212.00,A,5046.31784,N,00604.61731,E,1.310,212.55,300116,,,A*64\r\n"
	"$GPVTG,212.55,T,,M,1.310,N,2.427,K,A*3C\r\n"
	"$GPGGA,130212.00,5046.31784,N,00604.61731,E,1,03,5.72,197.1,M,47.6,M,,*59\r\n"
	"$GPGSA,A,2,07,30,09,,,,,,,,,,5.80,5.72,1.00*02\r\n"
	"$GPGSV,4,1,13,07,67,080,35,30,66,178,25,05,56,292,10,09,35,086,40*77\r\n"
	"$GPGSV,4,2,13,02,26,235,,16,06,021,,06,06,198,,16,06,021,*7F\r\n"
	"$GPGSV,4,3,13,23,03,089,,28,05,158,,20,09,312,,13,23,268,*78\r\n"
	"$GPGSV,4,4,13,13,23,268,*44\r\n"
	"$GPGLL,5046.31784,N,00604.61731,E,130212.00,A,A*64\r\n"
	"$GPZDA,130212.00,30,01,2016,00,00*62\r\n"
	"$GPRMC,130213.00,A,5046.31925,N,00604.61742,E,2.503,2.43,300116,,,A*67\r\n"
	"$GPVTG,2.43,T,,M,2.503,N,4.639,K,A*34\r\n"
	"$GPGGA,130213.00,5046.31925,N,00604.61742,E,1,03,5.71,197.1,M,47.6,M,,*5A\r\n"
	"$GPGSA,A,2,07,30,09,,,,,,,,,,5.79,5.71,1.00*07\r\n"
	"$GPGSV,4,1,13,07,67,080,35,30,66,178,25,05,56,292,,09,35,086,40*76\r\n"
	"$GPGSV,4,2,13,02,26,235,,16,06,021,,06,06,198,,16,06,021,*7F\r\n"
	"$GPGSV,4,3,13,23,03,089,,28,05,158,,20,09,312,,13,23,268,*78\r\n"
	"$GPGSV,4,4,13,13,23,268,*44\r\n"

	/*
	"$GPRMC,162254.00,A,3723.02837,N,12159.39853,W,0.820,188.36,110706,,,A*74\r\n"
	"$GPVTG,188.36,T,,M,0.820,N,1.519,K,A*3F\r\n"
	"$GPGGA,162254.00,3723.02837,N,12159.39853,W,1,03,2.36,525.6,M,-25.6,M,,*65\r\n"
	"$GPGSA,A,2,25,01,22,,,,,,,,,,2.56,2.36,1.00*02\r\n"
	"$GPGSV,4,1,14,25,15,175,30,14,80,041,,19,38,259,14,01,52,223,18*76\r\n"
	"$GPGSV,4,2,14,18,16,079,,11,19,312,,14,80,041,,21,04,135,25*7D\r\n"
	"$GPGSV,4,3,14,15,27,134,18,03,25,222,,22,51,057,16,09,07,036,*79\r\n"
	"$GPGSV,4,4,14,07,01,181,,15,25,135,*76\r\n"
	"$GPGLL,3723.02837,N,12159.39853,W,162254.00,A,A*7C\r\n"
	"$GPZDA,162254.00,11,07,2006,00,00*63\r\n"
	*/
	};

	static uint8_t aprs_test[] =
	{
	";OV-G01   *111111z5045.19N/00608.75E/145.550MHz OV G01 Aachen aprs_parse_packet():315:INFO: Oen OV Abend 4. Freitag 20.00h\n" //125 incl. NULL
	"}PD1AEF-10>APRX28,TCPIP,DL7ATR-2*::PD1AEF-10:UNIT.Rx Erlang,Tx Erlang,count/10m,count/10m,count/10m\n"
	";ON0LGE-R *062152z5037.47N/00541.99Er439.212MHz C074 -760 FM/YSF\n"
	":DK6YF-23 :test@example.com Hari QRV 144.525MHz Aachen\n"
	};
#endif


//static Timer test_timer;


#if CONFIG_KERN_HEAP
	#define ser_stack NULL
#else
#if CONFIG_KERN
	#define PROC_STACK_SIZE	KERN_MINSTACKSIZE * 2
	static PROC_DEFINE_STACK(ser_stack, PROC_STACK_SIZE);
#endif
#endif



/* Graphic LCD bitmap */
#if CPU_AVR
uint8_t raster[RAST_SIZE(LCD_WIDTH, LCD_HEIGHT)];
#endif
Bitmap lcd_bitmap;

extern const Font font_helvB10;
//extern const Font font_helvR8;
extern const Font font_mina;
extern const Font font_gohu;
extern const Font font_hp5;


#if GPS
extern nmeap_context_t nmea;
extern NmeaGga gga;
extern NmeaRmc rmc;
extern NmeaVtg vtg;
static ticks_t	last_gpsfix = 0;

struct tm gps_time;
#endif

#if ONEWIRE

#endif

/* Serial Ports for Debug */
//static Serial ser_port;



//static AX25Call path[] = AX25_PATH(AX25_CALL("apzbrt", 0), AX25_CALL("DK6YF", 9), AX25_CALL("wide1", 1), AX25_CALL("wide2", 2));
#define APRS_MSG 	">"APP_NAME " " VERS_TAG" - BeRTOS & AVR via BOSCH KF163 QRV - op " QTH_MYCALL

static ticks_t last_aprspos = 0;


/**
 * Refresh the GUI.
 */
void schedule(void)
{
	#if CPU_AVR
	lcd_ks0108_blitBitmap(&lcd_bitmap);
	#else
	lcd_gfx_qt_blitBitmap(&lcd_bitmap);
	emul_idle();
	#endif
}

static void lcd_clear(void)
{
	gfx_bitmapClear(&lcd_bitmap);
}



// You always need to include this function. It will
// get called by the aprs module every time a packet is
// received, so you can process incoming packets.
//
// If you are only interested in receiving, you should
// just leave this function empty.
//
// IMPORTANT! This function is called from within an
// interrupt. That means that you should only do things
// here that are FAST. Don't print out info directly
// from this function, instead set a flag and print it
// from your main loop, like this:

//AX25Msg incomingPacket;
//uint8_t *packetData;

bool gotPacket = false;
APRSmessage incPkt;				// Hier wäre nur Datenstruktur für Message...

void aprs_msg_callback(struct AX25Msg *msg)
{

	// If we already have a packet waiting to be
	// processed, we must drop the new one.
	if (!gotPacket) {
		// Set flag to indicate we got a packet
		gotPacket = true;

		#if CPU_AVR
		// The memory referenced as *msg is volatile
		// and we need to copy all the data to a
		// local variable for later processing.

		// !! Remember this function is called from a ISR!! Do not perform expensive time here !!

		// We do copy all relevant information of our received packet to parse it later on

		//ax25_print(&ser_port.fd, msg);	// Only debugging

		// Source of APRS Message (SRC-CALL)
		memcpy(&incPkt.src.call, msg->src.call, sizeof(msg->src.call));
		incPkt.src.ssid = msg->src.ssid;

		// Payload of APRS Message
		memset(incPkt.buf, 0, APRS_PAYLOAD_LEN);	// Empty the buffer first..
		memcpy(&incPkt.buf, msg->info, msg->len);	// Copy buffer content
		incPkt.len = msg->len;						// Store packet length

		#endif

	}

}


// Here's a function to process incoming packets
// Remember to call this function often, so you
// won't miss any packets due to one already
// waiting to be processed
static void aprs_process_packet(void)
{
	if (gotPacket) {
		gotPacket = false;

		#if CPU_AVR
		LOG_INFO("%.6s-%d:\n", incPkt.src.call, incPkt.src.ssid);
		LOG_INFO("%s\n", incPkt.buf);
		//LOG_INFO("%d\n", incPkt.len);

		/**
		// Print packet data on serial port
		kfile_printf(&ser_port.fd, "\n\nSRC[%.6s-%d], DST[%.6s-%d]\r\n", incomingPacket.src.call, incomingPacket.src.ssid, incomingPacket.dst.call, incomingPacket.dst.ssid);

		for (int i = 0; i < incomingPacket.rpt_cnt; i++)
			kfile_printf(&ser_port.fd, "via: [%.6s-%d]\r\n", incomingPacket.rpt_lst[i].call, incomingPacket.rpt_lst[i].ssid);

		kfile_printf(&ser_port.fd, "DATA: %.*s\r\n", incomingPacket.len, incomingPacket.info);
		**/

		// Parse packet for internal use (All data is parsed and retrieved here!)
		aprs_parse_packet(&incPkt);

		// Remeber to free/delete memory for our buffer !!
		// We do this inside the callback, because I want to keep original packet data for display
		//free(packetData);
		lcd_clear();

		// You can print out the amount of free
		// RAM to check you don't have any memory
		// leaks
		// Serial.print(F("Free RAM: ")); Serial.println(freeMemory());
		#else

		// Fill demo content...

		// Source of APRS Message (SRC-CALL)
		memcpy(&incPkt.src.call, "AB0CDE", 6);
		incPkt.src.ssid = 15;

		// Payload of APRS Message
		memcpy(&incPkt.buf,":DK6YF-9  :test@example.com Hari QRV 144.525MHz Aachen{0003", 59);
		incPkt.len = 59;

		aprs_parse_packet(&incPkt);

		#endif
  	}


}


//char *positionMessage[APRS_PAYLOAD_LEN];


MSGBuffer positionPacket;

char *ptr = positionPacket.data;

static void aprs_process_tracker(void)
{
	aprs_status |= (1 << APRS_SENDING);

	uint8_t payloadLength = 0;

	memset(positionPacket.data, 0, APRS_PAYLOAD_LEN);
	positionPacket.length = 0;


	//char buf[16];

	/* Position */
	int8_t deg;
	long min;
	long sec;

	/* Time */
	//gmtime_r(&rmc.time, &tm);
	//tm.tm_hour += 1;			// Aus irgendeinem Grund wird bei der UTC Zeit eine Stunde zu wenig angezeigt, GPS liefert korrekte UTC !!

	#if CPU_AVR
	/* Temperature */
	int16_t temp = 0;
	char temp_str[7];
	temp = aprs_get_temperature(&romcodeExtTemp);
	// Convert value to string
	itoa_fixedpoint(temp, 2, temp_str);
	#endif



	//42,351853 is stored as 42351853
	deg = ABS((long)gga.latitude) / 1000000L;						// returns 42
	min = (ABS((long)gga.latitude) % 1000000L) * 60 / 1000000L;		// returns 351853 * 60 / 1.000.000 = 21
	sec = (ABS((long)gga.latitude) % 1000000L) - (min * 1000000L / 60);		// 351853 - 350000 = 1853, 1853 * 3600 = 6670800
	sec = (((sec * 3600) / 1000L) / 600);		// auf hundertstel Sekunden umgerechnet


	// APRS Data Type Identifier
	payloadLength += sprintf(ptr + payloadLength, "%c",
			APRS_DATA_TYPE_POSNTSWMESS);

	// Latitude
	payloadLength += sprintf(ptr + payloadLength, "%02d%02ld.%ld%c",
			deg, min, sec, (long)gga.latitude >= 0 ? 'N' : 'S' );


	//42,351853 is stored as 42351853
	deg = ABS((long)gga.longitude) / 1000000L;							// returns 42
	min = (ABS((long)gga.longitude) % 1000000L) * 60 / 1000000L;		// returns 351853 * 60 / 1.000.000 = 21
	sec = (ABS((long)gga.longitude) % 1000000L) - (min * 1000000L / 60);		// 351853 - 350000 = 1853, 1853 * 3600 = 6670800
	sec = (((sec * 3600) / 1000L) / 600);		// auf hundertstel Sekunden umgerechnet


	// List of symbols separator
	payloadLength += sprintf(ptr + payloadLength, "%c",
			APRS_ICON_LIST1);

	// Longitude
	payloadLength += sprintf(ptr + payloadLength, "%03d%02ld.%ld%c",
			deg, min, sec, (long)gga.longitude >= 0 ? 'E' : 'W' );

	// Symbol number from previously selected list
	payloadLength += sprintf(ptr + payloadLength, "%c",
			APRS_ICON_1_CAR);

	// Course [000-359]
	payloadLength += sprintf(ptr + payloadLength, "%03d/",
			rmc.course);

	// Speed [knots]	TODO: Convert to knots (is in km/h)
	payloadLength += sprintf(ptr + payloadLength, "%03d/",
			rmc.speed);

	// Altitude [ft]	TODO: Convert to foot (is in meter)
	payloadLength += sprintf(ptr + payloadLength, "A=%06ld",
			gga.altitude);


	#if CPU_AVR
	// Message [text]
	//payloadLength += sprintf(ptr + payloadLength, "Henry QRV 145,550Mhz T=%sC", temp_str );
	payloadLength += sprintf(ptr + payloadLength, " T=%sC", temp_str );



	//LOG_INFO("%s\n", positionPacket.data);
	//LOG_INFO("%d\n", payloadLength);

	//INFO: =5046.33N/00604.61E>207/000/A=000206Hari QRV 145,550Mhz        |<- STOP!
	//INFO: =5046.33N/00604.61E>207/000/A=000206Hari QRV 145,550Mhz im W201|<- STOP!
	//INFO: 63


	// TODO: Dirty update of statusbar. Toggle of aprs_status in hw_afsk.h of project...
	statusbar_display();
	schedule();

	positionPacket.length = payloadLength;
	aprs_send_packet(positionPacket.data, positionPacket.length);
	#endif

/*
	strftime(buf, sizeof(buf),
			"TS: %d%H%Mz", &tm);
	text_xyprintf(&lcd_bitmap, 5, 32, TEXT_FILL, "%s", buf);

	text_xyprintf(&lcd_bitmap,  70, 12, TEXT_FILL, "ALT: %06ld", gga.altitude);

*/

}







void nmea_callback(nmeap_context_t *context, void *data, void *user_data)
{
	(void)context;
	(void)data;
	(void)user_data;

	if (gga.quality > 0)
		last_gpsfix = timer_clock();

}



#if 0
static void nmea_process_data(void)
{
/**
	struct tm tm;

	if (!rmc.time)
	{
		gfx_setFont(&lcd_bitmap, &font_helvB10);
		text_xyprintf(&lcd_bitmap, 0, 0,
			TEXT_CENTER | TEXT_FILL, "No GPS data");
	}
	else
	{
		gmtime_r(&rmc.time, &tm);

	}

	schedule();
**/
}
#endif



static void init(void)
{
	/* CPU and other onboard hardware to be configured first */
	IRQ_ENABLE;
	kdbg_init();

	// AVR and CPU initialization
	timer_init();


	#if CPU_AVR
	// Initialize serial port
    //ser_init(&ser_port, SER_UART1);
    //ser_setbaudrate(&ser_port, 9600UL);
    #else
	// Initialize serial port
    //ser_init(&gps_port, SER_UART0);
    //ser_setbaudrate(&gps_port, 9600UL);
	proc_init();
    #endif
	kbd_init();

    LOG_INFO("%s\r\n", APRS_MSG);


	/* Peripheral hardware and external components */

	// Initialize Display
	#if CPU_AVR
	lcd_ks0108_init();
	gfx_bitmapInit(&lcd_bitmap, raster, LCD_WIDTH, LCD_HEIGHT);
	lcd_clear();
	#else
	lcd_gfx_qt_init(&lcd_bitmap);
	#endif


	// Initialize Menu System and input devices
	#if CPU_AVR			// no progmem!!
	menu_init(); 		// ..we move this to main before loop for splash screen purpose ;)

	encoder_init();		// Done with keyboard init
	#endif


	/* Startup routine of Operating System */
	//#if CPU_AVR
	aprs_init();
	aprs_setCallsign(QTH_MYCALL, QTH_MYSSID);
	//#endif


	#if GPS
	gps_init();
	#if !CPU_AVR
	kfilemem_init(&mem, nmea_test, sizeof(nmea_test));
	LOG_INFO("Init test buffer..done.\n");
	#endif
	#endif


	#if CPU_AVR
	onewire_init();
	sensors_init();
	//DDRD |= (1 << PD7);
	#endif

	LOG_INFO("Initialization complete!\r\n");

	lcd_clear();
	schedule();



#define TEST 	0

#if TEST

	//gfx_setFont(&lcd_bitmap, &font_mina);

	/*
	text_xyprintf(&lcd_bitmap, 0, 30, 0,     "BeRTOS on ATmega128 - APRS");
	LOG_INFO("%d\n", text_vwidthf(&lcd_bitmap, "BeRTOS on ATmega128 - APRS", 0));

	text_xyprintf(&lcd_bitmap, 0, 40, 0,     "Speed: 12.5 km/h   Alt: 168m");
	LOG_INFO("%d\n", text_vwidthf(&lcd_bitmap, "Speed: 12.5 km/h   Alt: 168m", 0));

	text_xyprintf(&lcd_bitmap, 0, 50, 0,     "(UTC) 14:55:43  24.01.2016");
	LOG_INFO("%d\n", text_vwidthf(&lcd_bitmap, "(UTC) 14:55:43  24.01.2016", 0));
	*/

	/*
	gfx_setFont(&lcd_bitmap, &font_helvB10);
	text_xyprintf(&lcd_bitmap, 4, 0, 0,      "Lat: N 50\xb0 46' 29.9532\"");
	LOG_INFO("%d\n", text_vwidthf(&lcd_bitmap, "Lat: N 50\xb0 46' 29.9532\"", 0));

	text_xyprintf(&lcd_bitmap, 2, 13, 0,       "Lon: E 6\xb0 04' 37.1674\"");
	LOG_INFO("%d\n", text_vwidthf(&lcd_bitmap, "Lon: E 6\xb0 04' 37.1674\"", 0));
	*/

	//uint8_t styles = 0;

	//styles = text_style(&lcd_bitmap, STYLEF_INVERT, STYLEF_MASK);
	//LOG_INFO("S: %x %x %x %x\n", styles, STYLEF_INVERT, STYLEF_MASK, lcd_bitmap.styles);

	//text_xyprintf(&lcd_bitmap, 0, 38, TEXT_CENTER, "BeRTOS");

	//gfx_setFont(&lcd_bitmap, &font_mina);

	//text_xyprintf(&lcd_bitmap, 0, 52, TEXT_CENTER, "Ver:0.7b");
#else
	// Other testing...
	gfx_blitImage(&lcd_bitmap,  0, 0, &bertos_logo);
	gfx_setFont(&lcd_bitmap, &font_mina);
	text_xyprintf(&lcd_bitmap,  0, 41, TEXT_CENTER, "BeRTOS | AVR | KF163");
	text_xyprintf(&lcd_bitmap, 20, 52, 0, "Ver:%s by DK6YF", vers_tag);		// STYLES gehen irgendwie nicht ??
#endif

	schedule();




#if CPU_AVR
	timer_delay(250);
	timer_delay(250);
	timer_delay(250);
	timer_delay(250);
	timer_delay(250);
	timer_delay(250);
	timer_delay(250);
	timer_delay(250);
	//gps_purge();

	lcd_clear();
	schedule();
	menu_home();		// Go to inital menu screen
#else

#endif


}


/*
static void test_blink(UNUSED_ARG(iptr_t, arg))
{
	PORTB ^= (1 << PB7);
	//kbd_poll();
	timer_add(&test_timer);
}
*/




#if CPU_AVR
int main(void) {

	DDRB |= (1 << PB7);

#else
int main(int argc, char *argv[]) {
	/* Initialize emulator */
	emul_init(&argc, argv);
#endif


	init();


/*
	event_initSoftint(&test_timer.expire, test_blink, NULL);
	timer_setDelay(&test_timer, ms_to_ticks(250));
	timer_add(&test_timer);
*/



	aprs_send_packet(APRS_MSG, sizeof(APRS_MSG));		// hier wird schon speicher verballert und CPU resettet ???
	//aprs_send_packet((void*)APRS_MSG, sizeof(APRS_MSG));

	#if !CPU_AVR
	// simulate received packet
	gotPacket = true;;
	#endif


	while (1)
	{

		//PORTD ^= (1 << PD7);

		/* Get time */
		ticks_t now = timer_clock();


		/* Show informations on display and do gui stuff */
		menu_process();


		#if CPU_AVR
		/* Retrieve sensor results */
		if (!(aprs_status & BV(APRS_SENDING)))
			ow_periodic();
		#endif


		/* Retrieve last packet received */
		#if CPU_AVR
		aprs_poll();
		#endif

		/* Parse any incoming message packet */
		aprs_process_packet();


		/* Retrieve GPS position */
		#if GPS

		#if CPU_AVR
			gps_poll();
		#else
			for (int i = 0; i < 10; i++)
				nmea_poll(&nmea, &mem.fd);
		#endif

		gmtime_r(&rmc.time, &gps_time);


		#if CPU_AVR

		#if 1
		/* Check status of last GPS fix */
		if ( now - last_gpsfix < ms_to_ticks(1000) && (gga.quality > 0) )
		{
			gps_status |= (1 << GPS_VALID_FIX);
		}
		#if CPU_AVR // on simulator we always have valid GPS data ;)
		else
		{
			gps_status &= ~(1 << GPS_VALID_FIX);
		}
		#endif

		//nmea_process_data();



		/* Send own position packet info if valid GPS in interval */
		//if ((gps_time.tm_sec % beacon_val))
		//{
		//	if ( now - last_aprspos > ms_to_ticks(1000 * beacon_val) && (gps_status & BV(GPS_VALID_FIX)) )
			if ( now - last_aprspos > ms_to_ticks(1000 * beacon_val) )
			{
				if (aprs_status & BV(APRS_ENABLE))
					aprs_process_tracker();
				last_aprspos = now;
			}
		//}
		#endif /* 1 */
		#endif /* CPU_AVR */

		#endif /* GPS */






		#if CPU_AVR
		cpu_relax();
		#else
		emul_idle();
		#endif



		#if TEST
		//aprs_position_info_display();
		//schedule();
		#endif


	}

	return 0;
}




/*

Interessante Ideen:

- Außentemperatur
- Bordspannung





*/
