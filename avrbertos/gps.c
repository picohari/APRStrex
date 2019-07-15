

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
//#include <time.h>


#include <drv/timer.h>
#include <drv/ser.h>
#include <net/nmea.h>

#include "gps.h"



static Serial gps_port;


nmeap_context_t nmea;
NmeaGga gga;
NmeaRmc rmc;
NmeaVtg vtg;



uint8_t gps_status = 0;



extern void nmea_callback(nmeap_context_t *context, void *data, void *user_data);



void gps_init(void)
{
	#if CPU_AVR
		// Init serial port for GPS receiver
    ser_init(&gps_port, SER_UART1);
    ser_setbaudrate(&gps_port, 9600UL);
  #endif

  	// Init NMEA Parser
  	gps_status |= (1 << GPS_ENABLE);
  	nmeap_init(&nmea, NULL);

  	nmeap_addParser(&nmea, "GPGGA", nmea_gpgga, nmea_callback, &gga);
  	nmeap_addParser(&nmea, "GPRMC", nmea_gprmc, nmea_callback, &rmc);
	nmeap_addParser(&nmea, "GPVTG", nmea_gpvtg, nmea_callback, &vtg);

#if 0
	nmeap_addParser(&nmea, "GPGGA", nmea_gpgga, gpgga_callout, &gga);
	nmeap_addParser(&nmea, "GPRMC", nmea_gprmc, gprmc_callout, &rmc);
	nmeap_addParser(&nmea, "GPVTG", nmea_gpvtg, gpvtg_callout, &vtg);
#endif
}


void gps_purge(void)
{
	kfile_clearerr(&gps_port.fd);
}


void gps_poll(void)
{

	if (gps_status & BV(GPS_ENABLE))
		nmea_poll(&nmea, &gps_port.fd);
	else
	{
		memset(&gga, 0, sizeof(gga));
		memset(&rmc, 0, sizeof(rmc));
		memset(&vtg, 0, sizeof(vtg));

	}

	kfile_clearerr(&gps_port.fd);
}
