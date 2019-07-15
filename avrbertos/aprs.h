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
 * Copyright 2008 Develer S.r.l. (http://www.develer.com/)
 * All Rights Reserved.
 * -->
 *
 * \brief APRS Protocol Definitions
 *
 *
 * \author Harald W. Leschner <dk6yf@h9l.net>
 */

#ifndef APRS_H
#define APRS_H

#include <cfg/compiler.h>
#include <net/afsk.h>
#include <net/ax25.h>



/* APRS Data Type Identifiers */
#define APRS_DATA_TYPE_POSWTSNMESS		0x2F	/*	/		Position with timestamp (no APRS messaging) */
#define APRS_DATA_TYPE_POSITION				0x40	/*	@		Position with timestamp (with APRS messaging) */
#define APRS_DATA_TYPE_POSNTSNMESS		0x21	/*	!		Position without timestamp (no APRS messaging), or Ultimeter 2000 WX Station */
#define APRS_DATA_TYPE_POSNTSWMESS		0x3D	/*	=		Position without timestamp (with APRS messaging) */
#define APRS_DATA_TYPE_TELEMETRY			0x54	/*	T 		Telemetry data */
#define APRS_DATA_TYPE_MESSAGE				0x3A	/*	:		Message */
#define APRS_DATA_TYPE_OBJECT					0x3B	/*	;		Object */
#define APRS_DATA_TYPE_STATUS					0x3E	/*	>		Status */
#define APRS_DATA_TYPE_QTH						0x3C	/*	<		Station Capabilities */
#define APRS_DATA_TYPE_ITEM						0x29	/*	)		Item */
#define APRS_DATA_TYPE_QUERY					0x3F	/*	?		Query */
#define APRS_DATA_TYPE_USERTYPE				0x7B	/*	{		User-Defined APRS packet format */
#define APRS_DATA_TYPE_WEATHER				0x5F	/*	_		Weather Report (without position) */
#define APRS_DATA_TYPE_TEST						0x2C	/*	,		Invalid data or test data */
#define APRS_DATA_TYPE_CMICE					0x1C 	/*	0x1c	Current Mic-E Data (Rev 0 beta) */
//#define APRS_DATA_TYPE_ 			0x1D 	/*	0x1d	Old Mic-E Data (Rev 0 beta) */
//#define APRS_DATA_TYPE_ 			0x27	/*	'		Old Mic-E Data (but Current data for TM-D700) */
//#define APRS_DATA_TYPE_ 			0x60	/*	‘		Current Mic-E Data (not used in TM-D700) */


/* Unused APRS Data Type Identifiers from Documentation */
//#define APRS_DATA_TYPE_ 	/*	$		Raw GPS data or Ultimeter 2000 */
//#define APRS_DATA_TYPE_ 	/*	[		Maidenhead grid locator beacon (obsolete) */
//#define APRS_DATA_TYPE_ 	/*	}		Third-party traffic */
//#define APRS_DATA_TYPE_ 	/*	#		Peet Bros U-II Weather Station */
//#define APRS_DATA_TYPE_ 	/*	*		Peet Bros U-II Weather Station */
//#define APRS_DATA_TYPE_ 	/*	%		Agrelo DFJr / MicroFinder */
//#define APRS_DATA_TYPE_ 	/*	(		[Unused] */
//#define APRS_DATA_TYPE_ 	/*	\		[Unused] */
//#define APRS_DATA_TYPE_ 	/*	"		[Unused] */
//#define APRS_DATA_TYPE_ 	/*	]		[Unused] */
//#define APRS_DATA_TYPE_ 	/*	^		[Unused] */
//#define APRS_DATA_TYPE_ 	/*	-		[Unused] */
//#define APRS_DATA_TYPE_ 	/*	A–S		[Do not use] */
//#define APRS_DATA_TYPE_ 	/*	0–9		[Do not use] */
//#define APRS_DATA_TYPE_ 	/*	a–z		[Do not use] */
//#define APRS_DATA_TYPE_ 	/*	U–Z		[Do not use] */
//#define APRS_DATA_TYPE_ 	/*	|		[Do not use — TNC stream switch character] */
//#define APRS_DATA_TYPE_ 	/*	~		[Do not use — TNC stream switch character] */
//#define APRS_DATA_TYPE_ 	/*	&		[Reserved — Map Feature] */
//#define APRS_DATA_TYPE_ 	/*	+		[Reserved — Shelter data with time] */
//#define APRS_DATA_TYPE_ 	/*	.		[Reserved — Space weather] */


/* APRS SSIDs */
#define APRS_SSID_QTH 		0	/* -0 	Your primary station usually fixed and message capable */
#define APRS_SSID_1 		1	/* -1 	generic additional station, digi, mobile, wx, etc. */
#define APRS_SSID_2 		2	/* -2 	generic additional station, digi, mobile, wx, etc. */
#define APRS_SSID_3 		3	/* -3 	generic additional station, digi, mobile, wx, etc. */
#define APRS_SSID_4 		4	/* -4 	generic additional station, digi, mobile, wx, etc. */
#define APRS_SSID_NET 		5	/* -5 	Other network sources (Dstar, Iphones, Blackberry's etc) */
#define APRS_SSID_6 		6	/* -6 	Special activity, Satellite ops, camping or 6 meters, etc. */
#define APRS_SSID_HT 		7	/* -7 	walkie talkies, HT's or other human portable */
#define APRS_SSID_8 		8	/* -8 	boats, sailboats, RV's or second main mobile */
#define APRS_SSID_CAR 		9	/* -9 	Primary Mobile (usually message capable) */
#define APRS_SSID_10 		10	/* -10 	internet, Igates, echolink, winlink, AVRS, APRN, etc. */
#define APRS_SSID_AIR		11	/* -11 	balloons, aircraft, spacecraft, etc. */
#define APRS_SSID_12 		12	/* -12 	APRStt, DTMF, RFID, devices, one-way trackers*, etc. */
#define APRS_SSID_WEATHER 	13	/* -13 	Weather stations */
#define APRS_SSID_TRUCK		14	/* -14 	Truckers or generally full time drivers */
#define APRS_SSID_15 		15	/* -15 	generic additional station, digi, mobile, wx, etc. */


/* APRS SYMBOLS (TABLE 1) */
#define APRS_ICON_LIST1				0x2F	/*	/		Primary list of symbols */
#define APRS_ICON_1_HOUSE_QTH		0x2D	/*	-		House QTH */
#define APRS_ICON_1_ANTENNA			0x72	/*	r		Antenna */
#define APRS_ICON_1_EMERGENCY		0x6F	/*	o		Emergency operation center */
#define APRS_ICON_1_CAMPGROUND		0x3B	/*	;		Campground */
#define APRS_ICON_1_MOTORCYCLE		0x3C	/*	<		Motorcycle */
#define APRS_ICON_1_CAR				0x3E	/*	>		Car */
#define APRS_ICON_1_RECREATION		0x52	/*	R		Recreational vehicle */
#define APRS_ICON_1_HELICOPTER		0x58	/*	X		Helicopter */
#define APRS_ICON_1_YACHT			0x59	/*	Y		Yacht / Sailboat */
#define APRS_ICON_1_JOGGER			0x5B	/*	[		Jogger */
#define APRS_ICON_1_BICYCLE			0x62	/*	b		Bicycle */
#define APRS_ICON_1_HORSE			0x65	/*	e		Horse */
#define APRS_ICON_1_BALLOON			0x4F	/*	O		Balloon */
#define APRS_ICON_1_AIRPLANE		0x27	/*	'		Small aircraft */

/* APRS SYMBOLS (TABLE 2) */
#define APRS_ICON_LIST2				0x5C	/*	\		Alternate list of symbols */
/* TBD */






#define APRS_PAYLOAD_LEN	128

typedef struct APRSmessage
{
	AX25Call src;  					//< Source adress
	AX25Call dst;  					//< Destination address
	char buf[APRS_PAYLOAD_LEN];		//  Buffer for received payload text
	size_t len;    					//< Payload length
	uint8_t mid;					//< Message ID
	bool ack;						//< Send a ACK back to sender
} APRSmessage;


typedef struct _msgbuf{
  uint8_t length;
  char data[APRS_PAYLOAD_LEN];
} MSGBuffer;



#define APRS_ENABLE			0
#define APRS_SENDING		1



extern uint8_t aprs_status;
extern uint8_t beacon_val;			// Default APRS message interval in minutes
extern uint8_t ssid_val;			// Default SSID value
extern uint8_t icon_val;			// Default ICON


void aprs_setCallsign(const char *call, int ssid);
void aprs_setDestination(char *call, int ssid);
void aprs_setPath1(char *call, int ssid);
void aprs_setPath2(char *call, int ssid);
void aprs_setMessageDestination(char *call, int ssid);

void aprs_poll(void);

//void aprs_send_packet(void *_buffer, size_t length);
extern void aprs_send_packet(const char *_buffer, size_t length);

void aprs_send_message(void *_buffer, size_t length);
void aprs_send_message_ack(void);

void aprs_message_retry(void);

void aprs_msg_callback(struct AX25Msg *msg);

void aprs_parse_packet(struct APRSmessage *msg);


//void aprs_process_tracker(void);


void aprs_init(void);


#endif /* APRS_H */
