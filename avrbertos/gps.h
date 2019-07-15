
#ifndef GPS_H
#define GPS_H


#include <cfg/macros.h>
#include <cfg/debug.h>


#include "cfg/cfg_nmea.h"

#include <net/nmeap/inc/nmeap.h>
#include <net/nmea.h>


#define GPS_ENABLE        0
#define GPS_VALID_FIX     1



extern nmeap_context_t nmea;
extern NmeaGga gga;
extern NmeaRmc rmc;
extern NmeaVtg vtg;


extern uint8_t gps_status;



void nmea_callback(nmeap_context_t *context, void *data, void *user_data);



void gps_init(void);
void gps_poll(void);
void gps_purge(void);


#endif
