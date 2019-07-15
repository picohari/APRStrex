
#include <cfg/debug.h>

#include <verstag.h>
#include <buildrev.h>

// Define logging setting (for cfg/log.h module).
#define LOG_LEVEL         	3
#define LOG_FORMAT     		0
#include <cfg/log.h>

#include <stdio.h>
#include <stdlib.h>	//malloc()
#include <alloca.h>
#include <string.h>
#include <time.h>

#include <cpu/attr.h>     /* CPU_HARVARD */
#include <cfg/compiler.h>


#define MICROMENU 	0

#if CPU_HARVARD
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <avr/pgmspace.h>
	#include <util/atomic.h>
	#include <cpu/avr/drv/ram_avr.h>
	#include <drv/lcd_ks0108.h>
	#include <gfx/gfx_p.h> // BM_PLOT()
	#include <io/kfile.h>
	#include <cpu/irq.h>
	#include <drv/onewire.h>
	#include <mware/fixedpoint.h>
#else
	#include <emul/emul.h>
	#include <emul/kfile_posix.h>
	#include <drv/lcd_gfx_qt.h>
	#include <gfx/gfx.h>
	#include <gfx/win.h>

#endif

#if CPU_HARVARD
#include <avr/pgmspace.h> /* strncpy_P() */
#endif

#if defined(CONFIG_LOCALE) && (CONFIG_LOCALE == 1)
#include "msg.h"
#else
#define PTRMSG(x) ((const char *)x)
#endif

#include <kern/irq.h>
#include <kern/proc.h>
#include <kern/sem.h>

#include <drv/timer.h>
#include <drv/ser.h>

#include <gfx/font.h>
#include <gfx/text.h>
#include <icons/logo.h>

#if MICROMENU
	#include <gui/micromenu.h>
#else
	#include <gui/menu_pro.h>
#endif

#include <drv/kbd.h>
#include <drv/buttons.h>

#include <net/afsk.h>
#include <net/ax25.h>

#include <net/nmea.h>
#include "gps.h"
#include "time.h"

#include "menu.h"
#include "station.h"
#include "aprs.h"
#include "sensors.h"




//FILE lcd = FDEV_SETUP_STREAM (hd44780_put, NULL, _FDEV_SETUP_WRITE);

/* Graphic LCD bitmap */
//extern uint8_t raster[RAST_SIZE(LCD_WIDTH, LCD_HEIGHT)];

extern Bitmap lcd_bitmap;

/* Fonts */
extern const Font font_helvB10;
//extern const Font font_helvR8;
extern const Font font_mina;
extern const Font font_gohu;
extern const Font font_hp5;


extern APRSmessage incPkt;
extern APRSmessage pftmessage;
//extern APRSmessage pftanyone;


/* Holds the current menu title */
char menutitle[24];

/* Holds the current temperature */
char temp_str[] = {' ', '0', '0', '.', '0', '0', 0};


/* GPS receiver status */
static const char *gps_fix[] =
{
	"?",			// Invalid
	"GPS",
	"DGPS",
	"PPS",
	"RTK",
	"FRTK",			// float-RTK
	"EST",			// estimated
	"MAN",			// manual
	"SIM",			// simulated
};




#if MICROMENU

/* DEFAULT or initial write function for menu-system.
 * Required only at startup for the first init of the menu.
 */
static void generic_write(const char* str) {

	memset(menutitle, 0, sizeof(menutitle));
	#if CPU_HARVARD
	strcpy_P(menutitle, str);
	#else
	strcpy(menutitle, str);
	#endif
}



#if CPU_HARVARD

/* Display the selected menu item */
//static void menu_navigate(menu_item_t* const new_menu, const Bitmap *bm, bool update)
#if CPU_HARVARD
static void menu_navigate(menu_item_t* const new_menu, bool update)
#else
void menu_navigate(menu_item_t* const new_menu, bool update)
#endif
{
	// Retrieve pointers from micromenu
	lcdm_navigate(new_menu);

	// Clear the display bitmap
	if (update)
		gfx_bitmapClear(&lcd_bitmap);


	#if 1
	// call generic write or whatever was defined by lcdm_set_default_write_callback()
	lcdm_write_function(lcdm_get_current_menu()->text);
	// Print current menu title on display at position...
	//gfx_setFont(&lcd_bitmap, &font_hp5);
	//text_xyprintf(&lcd_bitmap, x, y, TEXT_NORMAL, menutitle );
	#endif


	// Render the content (REMEBER: this is a pointer function to the selected display_func from micromenu)
	display_func();

	// Blit the content of bitmap on LCD screen
	#if CPU_HARVARD
	lcd_ks0108_blitBitmap(&lcd_bitmap);
	#else
	lcd_gfx_qt_blitBitmap(&lcd_bitmap);
	#endif
}


/* NAVIGATION functions assigned to the buttons */
//static void nav_next (void) { /*hd44780_clear();*/ lcdm_navigate(MENU_NEXT); }
static void nav_next (void) { /*hd44780_clear();*/ menu_navigate(MENU_NEXT, 1); }
static void nav_prev (void) { /*hd44780_clear();*/ menu_navigate(MENU_PREV, 1); }
static void nav_down (void) { /*hd44780_clear();*/ lcdm_navigate(MENU_CHILD);   }
/**
void nav_up   (void) { lcdm_navigate(MENU_PARENT); }
void nav_null (void) { lcdm_navigate(current_menu_item); }
void set_para (void) { lcdm_navigate(MENU_CHILD); }
**/

#endif /* CPU_HARVARD */

static void nav_next (void)  { /*hd44780_clear();*/  }
static void nav_prev (void)  { /*hd44780_clear();*/  }
static void nav_enter (void) { /*hd44780_clear();*/  }

#endif /* MICROMENU */

























#if 1

#if CPU_HARVARD
static void demo1_menu_info_display(void)
#else
void demo1_menu_info_display(void)
#endif
{
	#if !CPU_HARVARD // Only for simulator
	gfx_setFont(&lcd_bitmap, &font_hp5);
	//text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, 0, TEXT_NORMAL, "[1]:");
	//menu_xytitle(0,0);
	//gfx_line(&lcd_bitmap, 0, 10, 128, 10 );

	gfx_setFont(&lcd_bitmap, &font_mina);


	#endif
}

#if CPU_HARVARD
static void demo2_menu_info_display(void)
#else
void demo2_menu_info_display(void)
#endif
{
	#if !CPU_HARVARD // Only for simulator
	gfx_setFont(&lcd_bitmap, &font_hp5);
	//text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, 0, TEXT_NORMAL, "[2]:");
	//menu_xytitle(0,0);
	//gfx_line(&lcd_bitmap, 0, 10, 128, 10 );

	gfx_setFont(&lcd_bitmap, &font_mina);



	#endif
}

#if CPU_HARVARD
static void demo3_menu_info_display(void)
#else
void demo3_menu_info_display(void)
#endif
{
	#if !CPU_HARVARD // Only for simulator
	gfx_setFont(&lcd_bitmap, &font_hp5);
	//text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, 0, TEXT_NORMAL, "[3]:");
	//menu_xytitle(0,0);
	//gfx_line(&lcd_bitmap, 0, 10, 128, 10 );

	gfx_setFont(&lcd_bitmap, &font_mina);



	#endif
}

#if CPU_HARVARD
static void demo4_menu_info_display(void)
#else
void demo4_menu_info_display(void)
#endif
{
	#if !CPU_HARVARD // Only for simulator
	gfx_setFont(&lcd_bitmap, &font_hp5);
	//text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, 0, TEXT_NORMAL, "[4]:");
	//menu_xytitle(0,0);
	//gfx_line(&lcd_bitmap, 0, 10, 128, 10 );

	gfx_setFont(&lcd_bitmap, &font_mina);



	#endif
}

#endif



#if CPU_HARVARD
void statusbar_display(void)
#else
void statusbar_display(void)
#endif
{

	//TODO: Falls diese Funktion am Ende der Screens steht, wird das Menu
	//		nicht ganz vollständig am Ende angezeigt.. Der Cursor verschwindet
	//		quasi unterhalb des Bildschirms...

	gfx_setFont(&lcd_bitmap, &font_hp5);


	// APRS sending active symbol
	if (aprs_status & BV(APRS_SENDING))
	{
		// Got a fix
		text_xyprintf(&lcd_bitmap, 108, -3, TEXT_NORMAL, "%c", '\xA4' );
	} else {
		text_xyprintf(&lcd_bitmap, 108, -3, TEXT_NORMAL, "g" );
	}

	// APRS tracking active symbol
	if (aprs_status & BV(APRS_ENABLE))
	{
		// Got a fix
		text_xyprintf(&lcd_bitmap, 115, -3, TEXT_NORMAL, "%c", '\xA2' );
	} else {
		text_xyprintf(&lcd_bitmap, 115, -3, TEXT_NORMAL, "g" );
	}

	// GPS fix is ok symbol
	if (gps_status & BV(GPS_VALID_FIX))
	{
		// Got a fix
		text_xyprintf(&lcd_bitmap, 122, -3, TEXT_NORMAL, "%c", '\xA3' );
	} else {
		text_xyprintf(&lcd_bitmap, 122, -3, TEXT_NORMAL, "g" );
	}


}




#if CPU_HARVARD
static void aprs_basis_info_display(void)
#else
void aprs_basis_info_display(void)
#endif
{

	gfx_setFont(&lcd_bitmap, &font_hp5);
	text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, -3, TEXT_NORMAL, "[ SYS ]:" );
	gfx_line(&lcd_bitmap, 0, 6, 128, 6 );

	statusbar_display();


	/* Temperature */
	#if CPU_AVR	
	int16_t temperature = 0;
	//char temp_str[] = {' ', '0', '0', '.', '0', '0', 0};
	
	if (!(aprs_status & BV(APRS_SENDING)))
	{
		temperature = aprs_get_temperature(&romcodeExtTemp);

		// Convert value to string
		//memset(temp_str, 0, sizeof(temp_str));
		itoa_fixedpoint(temperature, 2, temp_str);
	}

	#else /* Simulator */
	char temp_str[] = {'+', '2', '1', '.', '7', '4', 0};
	#endif
	gfx_setFont(&lcd_bitmap, &font_helvB10);
	text_xyprintf(&lcd_bitmap,   2, 54, TEXT_FILL, "%s%c", temp_str, '\xA1' );

	/* TIME */
	struct tm tm;
	char buf[16];
	gmtime_r(&rmc.time, &tm);
	strftime(buf, sizeof(buf),
			"%T", &tm);
	text_xyprintf(&lcd_bitmap, 85, 54, TEXT_FILL, "%s", buf);

	/* SPEED */
	text_xyprintf(&lcd_bitmap, 1, 12, TEXT_FILL,
			"%4d", vtg.km_speed);
	gfx_setFont(&lcd_bitmap, &font_gohu);
	text_xyprintf(&lcd_bitmap, 27, 13, TEXT_FILL,
			"km/h");

	/* ALTITUDE */
	gfx_setFont(&lcd_bitmap, &font_helvB10);
	text_xyprintf(&lcd_bitmap, 53, 12, TEXT_FILL,
			"%4ld", (long)gga.altitude);
	gfx_setFont(&lcd_bitmap, &font_gohu);
	text_xyprintf(&lcd_bitmap, 78, 13, TEXT_FILL,
			"m");

	/* COURSE */
	gfx_setFont(&lcd_bitmap, &font_helvB10);
	text_xyprintf(&lcd_bitmap,  88, 12, TEXT_FILL, "%03d%c", rmc.course, '\xb0');

	/* GPS INFO */
	gfx_setFont(&lcd_bitmap, &font_hp5);
	text_xyprintf(&lcd_bitmap, 115, 7, TEXT_NORMAL,
			"%2d", gga.satellites);

	if (gga.quality < countof(gps_fix))
		text_xyprintf(&lcd_bitmap, 113, 15, TEXT_FILL,
				"%s", gps_fix[gga.quality]);
	else
		text_xyprintf(&lcd_bitmap, 113, 15, TEXT_FILL,
				"%d", gga.quality);



	/* POSITION */
	int8_t deg;
	long min;
	long sec;
	gfx_setFont(&lcd_bitmap, &font_mina);

	/* LAT */
	deg = ABS((long)gga.latitude) / 1000000L;
	min = (ABS((long)gga.latitude) % 1000000L) * 60 / 1000000L;
	sec = (ABS((long)gga.latitude) % 1000000L) - (min * 1000000L / 60);
	sec *= 3600;
	text_xyprintf(&lcd_bitmap, 2, 26, TEXT_FILL, "%c", (long)gga.latitude >= 0 ? 'N' : 'S'); 
	text_xyprintf(&lcd_bitmap, 15, 26, TEXT_FILL,
			"%2d\xb0 %02ld\' %ld.%ld\"",
			deg, min, sec / 1000000L, (sec % 1000000L) / 100 );

	/* LON */
	deg = ABS((long)gga.longitude) / 1000000L;
	min = (ABS((long)gga.longitude) % 1000000L) * 60 / 1000000L;
	sec = (ABS((long)gga.longitude) % 1000000L) - (min * 1000000L / 60);
	sec *= 3600;
	text_xyprintf(&lcd_bitmap, 2, 36, TEXT_FILL, "%c", (long)gga.longitude >= 0 ? 'E' : 'W'); 
	text_xyprintf(&lcd_bitmap, 15, 36, TEXT_FILL,
			"%2d\xb0 %02ld\' %ld.%ld\"",
			deg, min, sec / 1000000L, (sec % 1000000L) / 100 );





}


#if CPU_HARVARD
static void aprs_message_info_display(void)
#else
void aprs_message_info_display(void)
#endif
{

	gfx_setFont(&lcd_bitmap, &font_hp5);
	text_clearLine(&lcd_bitmap, 0);
	if (pftmessage.src.call[0] == 0) pftmessage.src.call[0] = '?';
	if (pftmessage.dst.call[0] == 0) pftmessage.dst.call[0] = '?';
	
	text_xyprintf(&lcd_bitmap, 0, -3, TEXT_NORMAL, "[ MSG ]: %.6s-%d > %.6s-%d", pftmessage.src.call, pftmessage.src.ssid, pftmessage.dst.call, pftmessage.dst.ssid );
	gfx_line(&lcd_bitmap, 0, 6, 128, 6 );

	statusbar_display();	// too long here...

	gfx_setFont(&lcd_bitmap, &font_mina);

	//text_xyprintf(&lcd_bitmap, 0, 12, 0, "%s", pftmessage.buf);
	text_xywrap(&lcd_bitmap, 0, 12, pftmessage.buf, 128 );

}


#if CPU_HARVARD
static void aprs_packet_info_display(void)
#else
void aprs_packet_info_display(void)
#endif
{

	gfx_setFont(&lcd_bitmap, &font_hp5);
	text_clearLine(&lcd_bitmap, 0);
	if (incPkt.src.call[0] == 0) incPkt.src.call[0] = '?';
	text_xyprintf(&lcd_bitmap, 0, -3, TEXT_NORMAL, "[ PKT ]: %.6s-%d", incPkt.src.call, incPkt.src.ssid );
	gfx_line(&lcd_bitmap, 0, 6, 128, 6 );

	statusbar_display();

	gfx_setFont(&lcd_bitmap, &font_mina);

	//text_xyprintf(&lcd_bitmap, 0, 12, 0, "%s", incPkt.buf);
	text_xywrap(&lcd_bitmap, 0, 12, incPkt.buf, 128 );

}


#if CPU_HARVARD
static void aprs_position_info_display(void)
#else
void aprs_position_info_display(void)
#endif
{
	struct tm tm;
	char buf[16];

	int8_t deg;
	long min;
	long sec;


	gfx_setFont(&lcd_bitmap, &font_hp5);
	text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, -3, TEXT_NORMAL, "[ APRS ]:" );
	gfx_line(&lcd_bitmap, 0, 6, 128, 6 );

	statusbar_display();

	gmtime_r(&rmc.time, &tm);
	//tm.tm_hour += 1;			// Aus irgendeinem Grund wird bei der UTC Zeit eine Stunde zu wenig angezeigt, GPS liefert korrekte UTC !!

	gfx_setFont(&lcd_bitmap, &font_mina);

	//42,351853 is stored as 42351853
	deg = ABS((long)gga.latitude) / 1000000L;						// returns 42
	min = (ABS((long)gga.latitude) % 1000000L) * 60 / 1000000L;		// returns 351853 * 60 / 1.000.000 = 21
	sec = (ABS((long)gga.latitude) % 1000000L) - (min * 1000000L / 60);		// 351853 - 350000 = 1853, 1853 * 3600 = 6670800
	sec = (((sec * 3600) / 1000L) / 600);		// auf hundertstel Sekunden umgerechnet


	text_xyprintf(&lcd_bitmap, 0, 12, TEXT_FILL,
			"LAT: %02d%02ld.%ld%c",
			deg, min, sec,
			(long)gga.latitude >= 0 ? 'N' : 'S' );

	//42,351853 is stored as 42351853
	deg = ABS((long)gga.longitude) / 1000000L;						// returns 42
	min = (ABS((long)gga.longitude) % 1000000L) * 60 / 1000000L;		// returns 351853 * 60 / 1.000.000 = 21
	sec = (ABS((long)gga.longitude) % 1000000L) - (min * 1000000L / 60);		// 351853 - 350000 = 1853, 1853 * 3600 = 6670800
	sec = (((sec * 3600) / 1000L) / 600);		// auf hundertstel Sekunden umgerechnet

	text_xyprintf(&lcd_bitmap, 1, 22, TEXT_FILL,
			"LON: %03d%02ld.%ld%c",
			deg, min, sec,
			(long)gga.longitude >= 0 ? 'E' : 'W' );

	strftime(buf, sizeof(buf),
			"TS: %d%H%Mz", &tm);
	text_xyprintf(&lcd_bitmap, 5, 32, TEXT_FILL, "%s", buf);

	text_xyprintf(&lcd_bitmap,  70, 12, TEXT_FILL, "ALT: %06ld", gga.altitude);
	text_xyprintf(&lcd_bitmap,  71, 22, TEXT_FILL, "CSE: %03d", rmc.course);
	text_xyprintf(&lcd_bitmap,  71, 32, TEXT_FILL, "SPD: %03d", rmc.speed);

	//lcd_gfx_qt_blitBitmap(&lcd_bitmap);
}

#if CPU_HARVARD
static void gps_basis_info_display(void)
#else
void gps_basis_info_display(void)
#endif
{
	int8_t deg;
	long min;
	long sec;


	gfx_setFont(&lcd_bitmap, &font_hp5);
	//text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, -3, TEXT_NORMAL, "[ GPS ]:" );
	gfx_line(&lcd_bitmap, 0, 6, 128, 6 );

	statusbar_display();


	/* LAT */

	//42,351853 is stored as 42351853
	deg = ABS((long)gga.latitude) / 1000000L;						// returns 42
	min = (ABS((long)gga.latitude) % 1000000L) * 60 / 1000000L;		// returns 351853 * 60 / 1.000.000 = 21
	sec = (ABS((long)gga.latitude) % 1000000L) - (min * 1000000L / 60);		// 351853 - 350000 = 1853, 1853 * 3600 = 6670800
	sec *= 3600;

	// ° is 0x8f gohu
	// ° is 0xb0 helvB10
	gfx_setFont(&lcd_bitmap, &font_helvB10);
	text_xyprintf(&lcd_bitmap,  4, 12, TEXT_FILL, "Lat:");

	text_xyprintf(&lcd_bitmap, 26, 12, TEXT_FILL, "%c", (long)gga.latitude >= 0 ? 'N' : 'S'); 
	text_xyprintf(&lcd_bitmap, 39, 12, TEXT_FILL,
			"%2d\xb0 %02ld\' %ld.%ld\"",
			deg, min, sec / 1000000L, (sec % 1000000L) / 100 );

	/* LON */

	deg = ABS((long)gga.longitude) / 1000000L;
	min = (ABS((long)gga.longitude) % 1000000L) * 60 / 1000000L;
	sec = (ABS((long)gga.longitude) % 1000000L) - (min * 1000000L / 60);
	sec *= 3600;

	// ° is 0x8f gohu
	// ° is 0xb0 helvB10
	//gfx_setFont(&lcd_bitmap, &font_gohu);
	text_xyprintf(&lcd_bitmap,  2, 25, TEXT_FILL, "Lon:");
	
	text_xyprintf(&lcd_bitmap, 26, 25, TEXT_FILL, "%c", (long)gga.longitude >= 0 ? 'E' : 'W'); 
	text_xyprintf(&lcd_bitmap, 39, 25, TEXT_FILL,
			"%2d\xb0 %02ld\' %ld.%ld\"",
			deg, min, sec / 1000000L, (sec % 1000000L) / 100 );


	//vtg.km_speed = 1254;
	text_xyprintf(&lcd_bitmap, 1, 37, TEXT_FILL,
			"%4d", vtg.km_speed);

	gfx_setFont(&lcd_bitmap, &font_gohu);
	text_xyprintf(&lcd_bitmap, 27, 38, TEXT_FILL,
			"km/h");


	//gga.altitude = 1539;
	text_xyprintf(&lcd_bitmap, 54, 38, TEXT_FILL,
			"%4ldm", (long)gga.altitude);


	//gga.satellites = 36;
	text_xyprintf(&lcd_bitmap, 87, 38, TEXT_FILL,
			"%2d", gga.satellites);

	//gga.quality = 5;
	if (gga.quality < countof(gps_fix))
		text_xyprintf(&lcd_bitmap, 101, 38, TEXT_FILL,
				"%s", gps_fix[gga.quality]);
	else
		text_xyprintf(&lcd_bitmap, 101, 38, TEXT_FILL,
				"%d", gga.quality);

	//lcd_gfx_qt_blitBitmap(&lcd_bitmap);





}



static void show_version(Bitmap *bm)
{
	gfx_setFont(&lcd_bitmap, &font_hp5);
	text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, -3, TEXT_NORMAL, "[ SYS ]: VERSION INFO" );
	gfx_line(&lcd_bitmap, 0, 6, 128, 6 );

	gfx_setFont(bm, &font_mina);
	text_xyprintf(bm,  2, 10, 0, "Version: %s", vers_tag);
	text_xyprintf(bm, 14, 20, 0, "Build: %d %s", vers_build_nr, APP_OS);
	text_xyprintf(bm, 16, 30, 0, "Host: %s", vers_host);

}

static void show_about(Bitmap *bm)
{
	gfx_blitImage(bm,  0, 0, &bertos_logo);
	gfx_setFont(bm, &font_mina);
	text_xyprintf(bm,  0, 41, TEXT_CENTER, "BeRTOS | AVR | KF163");
	text_xyprintf(bm,  0, 51, TEXT_CENTER, "APRS Tracker by DK6YF" );

}











static void show_edit_value(iptr_t value)
{
	gfx_setFont(&lcd_bitmap, &font_hp5);
	text_clearLine(&lcd_bitmap, 0);
	text_xyprintf(&lcd_bitmap, 0, -3, TEXT_NORMAL, "[ SET ]: VALUE" );
	gfx_line(&lcd_bitmap, 0, 6, 128, 6 );

	gfx_setFont(&lcd_bitmap, &font_mina);

	text_xyprintf(&lcd_bitmap,  2, 10, 0, "%s: %d", PTRMSG(current_menu_item->label)+4, *(uint8_t*)value );		// STYLES gehen irgendwie nicht ??

	// Falls im Menu der Eintrag (iptr_t)param steht, muss es heissen:
	// uint8_t param = *(uint8_t *)&value;	// Okay, scheint zu gehen!


}


uint8_t sensors_flags = 255;

static void sensors_toggle_active(iptr_t value)
{
	sensors_flags ^= ( 1 << *(uint8_t*)&value);
	//LOG_INFO(BINPATTERN "\n", BYTETOBIN(sensors_flags) );
}


static void aprs_status_toggle(iptr_t value)
{
	aprs_status ^= ( 1 << *(uint8_t*)&value);
	//LOG_INFO(BINPATTERN "\n", BYTETOBIN(sensors_flags) );
}

static void gps_status_toggle(iptr_t value)
{
	gps_status ^= ( 1 << *(uint8_t*)&value);
	//LOG_INFO(BINPATTERN "\n", BYTETOBIN(sensors_flags) );
}








#if CPU_HARVARD
static void test_display(void)
#else
void test_display(void)
#endif
{
	gfx_setFont(&lcd_bitmap, &font_mina);

	char last_character = 0x20;
	uint8_t last_width = 0;

	uint8_t curr_position = 2;

	keymask_t key, old_rpt_mask;

	old_rpt_mask = kbd_setRepeatMask(K_OK);

	for(;;)
	{

		key = kbd_peek();
		cpu_relax();


		if (key & K_RIGHT)
		{
			last_character++;
		}

		else if (key & K_LEFT)
		{
			last_character--;

		}


		if (key & K_REPEAT)
			last_character = 0x48;
		
		last_width = text_widthf(&lcd_bitmap, "%c", last_character);

		if (key & K_OK)
		{
			curr_position += last_width;
		}


		// Stay in ASCII boundaries
		if (last_character < 0x20) last_character = 0x7E;
		if (last_character > 0x7E) last_character = 0x20;

		text_xyprintf(&lcd_bitmap, curr_position, 10, TEXT_FILL, "%c", last_character);

	}




}







#if !MICROMENU

/*
Titel kann auch leer sein...
Hook führt die Funktion nur 1x aus
*/




static struct MenuItem sensor_items[] =
{
	{ (const_iptr_t)"Enable TEMP OIL",   	 	MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)TEMP_OIL },
	{ (const_iptr_t)"Enable VOLT BATTERY", 	 	MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)VOLT_BATTERY },
	{ (const_iptr_t)"Enable RPM ENGINE",   	 	MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)RPM_ENGINE },
	{ (const_iptr_t)"Enable PRESS BAROMETRIC",  MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)PRESS_BAROMETRIC },
	{ (const_iptr_t)"Enable LEVEL FUELTANK", 	MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)LEVEL_FUELTANK },
	{ (const_iptr_t)"Enable SPEED VEHICLE",	 	MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)SPEED_VEHICLE },
	{ (const_iptr_t)"Enable AMPS CHARGER",	 	MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)AMPS_CHARGER },
	{ (const_iptr_t)"Enable POWER TX",	 		MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)sensors_toggle_active,  (iptr_t)POWER_TX },
	{ (const_iptr_t)"Return", 					MIF_DISABLED, NULL,  (iptr_t)0 },
	{ (const_iptr_t)0, 0, NULL, (iptr_t)0 }

};
static struct Menu sensor_menu = { sensor_items, "[ SYS ]: MENU",  MF_STICKY | 0, &lcd_bitmap, 0,
#if CPU_AVR
	lcd_ks0108_blitBitmap };
#else
	lcd_gfx_qt_blitBitmap };
#endif





static struct MenuItem sys_items[] =
{
	{ (const_iptr_t)"Active SENSORS",			0, 								(MenuHook)menu_handle,  			(iptr_t)&sensor_menu },
	{ (const_iptr_t)"Active MODULES",   		0, 								(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Show VERSION",   			MIF_TOGGLE | MIF_SHOWSCREEN, 	(MenuHook)show_version, 			(iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"ABOUT ...",   				MIF_TOGGLE | MIF_SHOWSCREEN, 	(MenuHook)show_about,  				(iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Return", 					MIF_DISABLED, NULL,  (iptr_t)0 },
	{ (const_iptr_t)0, 0, NULL, (iptr_t)0 }
};
static struct Menu sys_menu = { sys_items, "[ SYS ]: MENU",  MF_STICKY | 0, &lcd_bitmap, 0,
#if CPU_AVR
	lcd_ks0108_blitBitmap };
#else
	lcd_gfx_qt_blitBitmap };
#endif


static struct MenuItem msg_items[] =
{
	{ (const_iptr_t)"Set MESSAGE FILTER",		0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set MESSAGE NOTIFY",   	0, 		(MenuHook)demo2_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set ALPHA",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set BRAVO",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set CHARLIE",  			0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set DELTA",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set ECHO",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set FOXTROTT",  			0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set GOLF",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set INDIA",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set JULIET",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set LIMA",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set MIKE",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set NOVEMBER",  			0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Return", 					MIF_DISABLED, NULL,  (iptr_t)0 },
	{ (const_iptr_t)0, 0, NULL, (iptr_t)0 }
};
static struct Menu msg_menu = { msg_items, "[ PKT ]: MENU",  MF_STICKY | 0, &lcd_bitmap, 0,
#if CPU_AVR
	lcd_ks0108_blitBitmap };
#else
	lcd_gfx_qt_blitBitmap };
#endif


static struct MenuItem aprs_items[] =
{
	{ (const_iptr_t)"Enable BEACON TX",   	 	MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)aprs_status_toggle,  		(iptr_t)APRS_ENABLE },
	{ (const_iptr_t)"Set BEACON INTERVAL",  	MIF_TOGGLE | MIF_SHOWSCREEN | MIF_EDITABLE, (MenuHook)show_edit_value,  		(iptr_t)&beacon_val },
	{ (const_iptr_t)"Set COMMENT TEXT",   	 	0, 											(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set SSID", 				MIF_TOGGLE | MIF_SHOWSCREEN | MIF_EDITABLE, (MenuHook)show_edit_value,  		(iptr_t)&ssid_val },
	{ (const_iptr_t)"Set MAP ICON",  			MIF_TOGGLE | MIF_SHOWSCREEN | MIF_EDITABLE,	(MenuHook)show_edit_value,  		(iptr_t)&icon_val },
	{ (const_iptr_t)"Set PATH",  				0, 		(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set APRS DESTINATION", 	0, 											(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Return", 					MIF_DISABLED, NULL,  (iptr_t)0 },
	{ (const_iptr_t)0, 0, NULL, (iptr_t)0 }
};
static struct Menu aprs_menu = { aprs_items, "[ APRS ]: MENU",  MF_STICKY | 0, &lcd_bitmap, 0,
#if CPU_AVR
	lcd_ks0108_blitBitmap };
#else
	lcd_gfx_qt_blitBitmap };
#endif


static struct MenuItem gps_items[] =
{
	{ (const_iptr_t)"Enable GPS",				MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)gps_status_toggle,  		(iptr_t)GPS_ENABLE },
	{ (const_iptr_t)"Use GPS TIME",				MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set UNITS SI/US",			MIF_CHECKIT | MIF_TOGGLE | MIF_CHECKED, 	(MenuHook)demo1_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Set TIMEZONE",   	 		0, 											(MenuHook)demo2_menu_info_display,  (iptr_t)&lcd_bitmap },
	{ (const_iptr_t)"Return", 					MIF_DISABLED, NULL,  (iptr_t)0 },
	{ (const_iptr_t)0, 0, NULL, (iptr_t)0 }
};
static struct Menu gps_menu = { gps_items, "[ GPS ]: MENU",  MF_STICKY | 0, &lcd_bitmap, 0,
#if CPU_AVR
	lcd_ks0108_blitBitmap };
#else
	lcd_gfx_qt_blitBitmap };
#endif


static struct MenuItem info_items[] =
{
//	{ (const_iptr_t)test_display,     			MIF_RENDERHOOK, (MenuHook)menu_handle,  (iptr_t)&gps_menu },
	{ (const_iptr_t)aprs_basis_info_display,    MIF_RENDERHOOK, (MenuHook)menu_handle,  (iptr_t)&sys_menu },
	{ (const_iptr_t)gps_basis_info_display,     MIF_RENDERHOOK, (MenuHook)menu_handle,  (iptr_t)&gps_menu },
	{ (const_iptr_t)aprs_position_info_display, MIF_RENDERHOOK, (MenuHook)menu_handle,  (iptr_t)&aprs_menu },
	{ (const_iptr_t)aprs_packet_info_display,   MIF_RENDERHOOK, (MenuHook)menu_handle,  (iptr_t)&msg_menu },
	{ (const_iptr_t)aprs_message_info_display,  MIF_RENDERHOOK, (MenuHook)menu_handle,  (iptr_t)&msg_menu },
	{ (const_iptr_t)0, 0, NULL, (iptr_t)0 }
};
static struct Menu info_screens = { info_items, 0,  MF_STICKY | MF_SAVESEL, &lcd_bitmap, 0,
#if CPU_AVR
	lcd_ks0108_blitBitmap };
#else
	lcd_gfx_qt_blitBitmap };
#endif








void menu_home(void)
{

}

/* Process menu activities */
void menu_process(void)
{
	//menu_handle(&info_screens);
	screen_handle(&info_screens);

/*
	LOG_INFO("BECN: %d\n", beacon_val);
	LOG_INFO("SSID: %d\n", ssid_val);
	LOG_INFO("ICON: %d\n", icon_val);
*/

}


void menu_init(void)
{
	//*item = info_screens.items[0];
}











#else

/*
 ## INFO-TOP-LEVEL ##
  
          NAME              NEXT           		PREV				PARENT      CHILD       	LEFT     	RIGHT 		ENTER     DISPLAY                    	TEXT ( MAX TEXT SIZE !! Without \0 !! )
          																																						"APRS: Config Menu Settings Title ON" + \0
*/  
MENU_ITEM(mi_gps_info,   	mi_aprs_info,     	mi_system_info,		NULL_MENU,  NULL_MENU,  	nav_next, 	nav_prev,  nav_down,  gps_basis_info_display,   	"[ GPS ]:");
MENU_ITEM(mi_aprs_info,   	mi_message_info,    mi_gps_info,		NULL_MENU,  mi_aprs_menu,  nav_next, 	nav_prev,  nav_down,  aprs_position_info_display,	"[ APRS ]:");
MENU_ITEM(mi_message_info,  mi_packet_info,     mi_aprs_info,		NULL_MENU,  NULL_MENU,  	nav_next, 	nav_prev,  nav_down,  aprs_message_info_display,   	"[ MSG ]:");
MENU_ITEM(mi_packet_info,   mi_system_info,     mi_message_info,	NULL_MENU,  NULL_MENU,  	nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"[ PKT ]:");
MENU_ITEM(mi_system_info,   mi_gps_info,     	mi_packet_info,		NULL_MENU,  mi_aprs_menu,  	nav_next, 	nav_prev,  nav_down,  aprs_basis_info_display,   	"[ SYS ]:");

MENU_ITEM(mi_aprs_menu,   	mi_aprs_menu_1,     mi_message_info,	mi_aprs_info,  	NULL_MENU,  nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"[ MENU ]: APRS SETTINGS");
MENU_ITEM(mi_aprs_menu_1,   mi_aprs_menu_2,     mi_message_info,	mi_aprs_menu,  	NULL_MENU,  nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"Set beacon interval");
MENU_ITEM(mi_aprs_menu_2,   mi_aprs_menu_3,     mi_message_info,	mi_aprs_menu,  	NULL_MENU,  nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"Set comment text");
MENU_ITEM(mi_aprs_menu_3,   mi_aprs_menu_4,     mi_message_info,	mi_aprs_menu,  	NULL_MENU,  nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"Set SSID");
MENU_ITEM(mi_aprs_menu_4,   mi_aprs_menu_5,     mi_message_info,	mi_aprs_menu,  	NULL_MENU,  nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"Set map icon ");
MENU_ITEM(mi_aprs_menu_5,   mi_aprs_menu_ret,	mi_message_info,	mi_aprs_menu,  	NULL_MENU,  nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"Set another item");
MENU_ITEM(mi_aprs_menu_ret, mi_aprs_menu_1,  	mi_message_info,	mi_aprs_menu,  	NULL_MENU,  nav_next, 	nav_prev,  nav_down,  aprs_packet_info_display,   	"RETURN");
//menu_item_t* ml_aprs_menu_items[5] = { &mi_aprs_menu_1, &mi_aprs_menu_2, &mi_aprs_menu_3, &mi_aprs_menu_4, &mi_aprs_menu_5 };

// For simulator purpose
MENU_ITEM(mi_testapp,   	mi_system_info,     mi_system_info,		NULL_MENU,  NULL_MENU,  nav_next,	nav_prev,  nav_down,  demo1_menu_info_display,		"[ APRS ]: Status idle");

/* Go to home menu */ 
//void menu_home(void) { menu_navigate(&mi_system_info, &lcd_bitmap, 1); }
//void menu_home(void) { menu_navigate(&mi_system_info, 1); }
void menu_home(void) { menu_navigate(&mi_gps_info, 1); }

/* Process menu activities */
void menu_process(void)
{
	// Get input keys first...
	int8_t dir = encoder_read2();

	if (dir > 0) next_func();
	if (dir < 0) prev_func();

/**
	// Render the content (REMEBER: this is a pointer function to the selected display_func from micromenu)
	display_func();

	// Blit the content of bitmap on LCD screen
	#if CPU_HARVARD
	lcd_ks0108_blitBitmap(&lcd_bitmap);
	#else
	lcd_gfx_qt_blitBitmap(&lcd_bitmap);
	#endif
**/

	#if CPU_HARVARD
	menu_navigate((menu_item_t* const)(current_menu_item), 0);
	#else
	menu_navigate(&mi_testapp, 1);
	#endif
}

/* Initialize the menu system*/
void menu_init(void)
{
	// Must be called the first time
	lcdm_set_default_write_callback(generic_write);

	// Set home menu as starting point...
	lcdm_navigate(&mi_system_info);
}

#endif


