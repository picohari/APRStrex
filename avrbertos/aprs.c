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
 * Copyright 2005 Develer S.r.l. (http://www.develer.com/)
 * All Rights Reserved.
 * -->
 *
 * \brief APRS Messaging (impl.).
 *
 * \author Harald W. Leschner <dk6yf@h9l.net>
 */

#include "aprs.h"
#include "station.h"

#include <stdio.h>
#include <stdlib.h>	    //malloc()
#include <string.h>     // strlen()
#include <ctype.h>

#include <cfg/macros.h> // BV()
#include <cfg/debug.h>

// Define logging setting (for cfg/log.h module).
#define LOG_LEVEL         3
#define LOG_FORMAT     		0
#include <cfg/log.h>



#include <drv/timer.h> // timer_delay()
#include <struct/list.h> // LIST_EMPTY()
#include <mware/formatwr.h> // _formatted_write()


#include <net/afsk.h>
#include <net/ax25.h>




extern void aprs_msg_callback(struct AX25Msg *msg);


uint8_t aprs_status = 0;
uint8_t beacon_val = 15;
uint8_t ssid_val = QTH_MYSSID;
uint8_t icon_val = 5;



Afsk modem;
AX25Ctx ax25;

AX25Call src;
AX25Call dst;
AX25Call path1;
AX25Call path2;

AX25Call path[4];

char CALL[7] = "NOCALL";
int CALL_SSID = 0;

char DST[7] = "APZBRT";
int DST_SSID = 0;

char PATH1[7] = "WIDE1";
int PATH1_SSID = 1;

char PATH2[7] = "WIDE2";
int PATH2_SSID = 2;


// For output packet message building (C++)
/*
typedef struct _msgbuf MSGBuffer;
struct _msgbuf{
  uint8_t length = 0;
  uint8_t data[APRS_PAYLOAD_LEN] = {0};
};
*/




/* SENDING Message assembly fields */
char message_recip[7];				// Message RECIPIENT
int8_t message_recip_ssid = -1;		// .. and his SSID
int16_t message_recip_mid = 0;		// .. and his message ID to be replied

int16_t message_seq = 0;
char lastMessage[67];				// Last message (whole packet) sent over AX25, used for retry message sending
size_t lastMessageLen;
bool message_autoAck = false;



/* RECEIVE Message storage */
APRSmessage pftmessage;
//APRSmessage pftanyone;



void aprs_setCallsign(const char *call, int ssid)
{
    memset(CALL, 0, 7);
    int i = 0;
    while (i < 6 && call[i] != 0) {
        CALL[i] = call[i];
        i++;
    }
    CALL_SSID = ssid;
}

void aprs_setDestination(char *call, int ssid)
{
    memset(DST, 0, 7);
    int i = 0;
    while (i < 6 && call[i] != 0) {
        DST[i] = call[i];
        i++;
    }
    DST_SSID = ssid;
}

void aprs_setPath1(char *call, int ssid)
{
    memset(PATH1, 0, 7);
    int i = 0;
    while (i < 6 && call[i] != 0) {
        PATH1[i] = call[i];
        i++;
    }
    PATH1_SSID = ssid;
}

void aprs_setPath2(char *call, int ssid)
{
    memset(PATH2, 0, 7);
    int i = 0;
    while (i < 6 && call[i] != 0) {
        PATH2[i] = call[i];
        i++;
    }
    PATH2_SSID = ssid;
}

void aprs_setMessageDestination(char *call, int ssid) {
    memset(message_recip, 0, 7);
    int i = 0;
    while (i < 6 && call[i] != 0) {
        message_recip[i] = call[i];
        i++;
    }
    message_recip_ssid = ssid;
}



/* PACKET RECEIVE ROUTINE */

void aprs_poll(void)
{
    ax25_poll(&ax25);
}

















/* PACKET SEND ROUTINE */
//void aprs_send_packet(void *_buffer, size_t length)
void aprs_send_packet(const char *_buffer, size_t length)
{

    uint8_t *buffer = (uint8_t *)_buffer;

    memcpy(dst.call, DST, 6);
    dst.ssid = DST_SSID;

    memcpy(src.call, CALL, 6);
    src.ssid = CALL_SSID;

    memcpy(path1.call, PATH1, 6);
    path1.ssid = PATH1_SSID;

    memcpy(path2.call, PATH2, 6);
    path2.ssid = PATH2_SSID;

    path[0] = dst;
    path[1] = src;
    path[2] = path1;
    path[3] = path2;

    #if CPU_AVR
    ax25_sendVia(&ax25, path, countof(path), buffer, length);
    #else
    LOG_INFO("  SRC:[%6s-%d]   DST:[%6s-%d]\n", path[1].call, path[1].ssid, path[0].call, path[0].ssid );
    LOG_INFO("PATH1:[%6s-%d] PATH2:[%6s-%d]\n", path[2].call, path[2].ssid, path[3].call, path[3].ssid );
    LOG_INFO(" SEND: %s\n", buffer);
    LOG_INFO("  LEN: %d\n", length);
    #endif

}






/* MESSAGES */

/*
An APRS message is a text string with a specified addressee. The addressee
is a fixed 9-character field (padded with spaces if necessary) following the
":"
Data Type Identifier. The addressee field is followed by another
":"
then the text of the message.

The message text may be up to 67 characters long, and may contain any
printable ASCII characters except "|" or "~" or "{"

A message may also have an optional message identifier, which is appended
to the message text. The message identifier consists of the character
"{"
followed by a message number (up to 5 alphanumeric characters, no spaces)
to identify the message.

Messages without a message identifier are not to be acknowledged.

Messages with a message identifier are intended to be acknowledged by the
addressee. The sending station will repeatedly send the message until it
receives an acknowledgement, or it is canceled, or it times out.
*/

void aprs_send_message(void *_buffer, size_t length)
{
    if (length > 67) length = 67;
    size_t payloadLength = 11+length+4;                 // also max. 67+11+4 = 82 Zeichen (plus null ?)

    /* Set a outPacket pointer to point at the output array */
    MSGBuffer packet;
    char *ptr = packet.data;

    //free(packet);
    memset(packet.data, 0, APRS_PAYLOAD_LEN);
    packet.length = 0;

    packet.data[0] = ':';

    int callSize = 6;
    int count = 0;

    while (callSize--) {
        if (message_recip[count] != 0) {
            packet.data[1+count] = message_recip[count];
            count++;
        }
    }

    if (message_recip_ssid != -1) {
        packet.data[1+count] = '-'; count++;
        if (message_recip_ssid < 10) {
            packet.data[1+count] = message_recip_ssid+48; count++;
        } else {
            packet.data[1+count] = 49; count++;
            packet.data[1+count] = message_recip_ssid-10+48; count++;
        }
    }

    while (count < 9) {
        packet.data[1+count] = ' '; count++;
    }

    packet.data[1+count] = ':';
    ptr += 11;

    if (length > 0) {
        uint8_t *buffer = (uint8_t *)_buffer;
        memcpy(ptr, buffer, length);
        memcpy(lastMessage, buffer, length);
        lastMessageLen = length;
    }

    message_seq++;

    if (message_seq > 999) message_seq = 0;

    packet.data[11+length] = '{';
    int n = message_seq % 10;
    int d = ((message_seq % 100) - n)/10;
    int h = (message_seq - d - n) / 100;

    packet.data[12+length] = h+48;
    packet.data[13+length] = d+48;
    packet.data[14+length] = n+48;

    //LOG_INFO("%s\n", packet.data);
    aprs_send_packet(packet.data, payloadLength);

}

void aprs_message_retry(void)
{
    message_seq--;
    aprs_send_message(lastMessage, lastMessageLen);
}


void aprs_send_message_ack(void)
{
	aprs_setMessageDestination(pftmessage.src.call, pftmessage.src.ssid);

    size_t payloadLength = 19;      // The ACK messages have a fixed maximum length defined by protocol reference

    /* Set a outPacket pointer to point at the output array */
    MSGBuffer packet;
    char *ptr = packet.data;

    //free(packet);
    memset(packet.data, 0, APRS_PAYLOAD_LEN);
    packet.length = 0;

    packet.data[0] = ':';

    int callSize = 6;
    int count = 0;

    while (callSize--) {
        if (message_recip[count] != 0) {
            packet.data[1+count] = message_recip[count];
            count++;
        }
    }

    if (message_recip_ssid != -1) {
        packet.data[1+count] = '-'; count++;
        if (message_recip_ssid < 10) {
            packet.data[1+count] = message_recip_ssid+48; count++;
        } else {
            packet.data[1+count] = 49; count++;
            packet.data[1+count] = message_recip_ssid-10+48; count++;
        }
    }

    while (count < 9) {
        packet.data[1+count] = ' '; count++;
    }

    packet.data[1+count] = ':';
    packet.data[2+count] = 'a';
    packet.data[3+count] = 'c';
    packet.data[4+count] = 'k';
    ptr += 14;

    char messid[5];
    sprintf(messid, "%05d", pftmessage.mid);
	memcpy(ptr, messid, 5);

    //LOG_INFO("%s\n", messid);
    //LOG_INFO("%s\n", packet.data);
    aprs_send_packet(packet.data, payloadLength);


}





static void aprs_parse_pkt_MESSAGE(struct APRSmessage *msg)
{
	// Get DST and SRC callsigns (attention: Here the DST (addresse of message) callsign is in the payload text)
    memset(pftmessage.dst.call, 0, 6);
    pftmessage.dst.ssid = 0;

    // The message was sent from this callsign
    memset(pftmessage.src.call, 0, 6);
    memcpy(&pftmessage.src.call, msg->src.call, 6);
    pftmessage.src.ssid = msg->src.ssid;
    // Must be true to reply a ACK packet of this message
    pftmessage.ack = false;

    int i = 1;
    char temp[5] = {0};

    // Minimum callsign length
    pftmessage.dst.call[0] = msg->buf[i++]; // 1 i now 2
    pftmessage.dst.call[1] = msg->buf[i++]; // 2 i now 3
    pftmessage.dst.call[2] = msg->buf[i++]; // 3 i now 4

    while (i < 10 )	// We parse all 9 bytes of the addressee field for a callsign with ssid
    {
    	if (isalnum(msg->buf[i]))
    	   	pftmessage.dst.call[i-1] = (char)(msg->buf[i]);
    	else if (msg->buf[i] == '-')
    	{
    		i++;				// skip to next char after '-' (we expect a ssid)
			if (isdigit(msg->buf[i]))
				temp[0] = (char)(msg->buf[i]);
			if (isdigit(msg->buf[i+1]))
				temp[1] = (char)(msg->buf[i+1]);
			pftmessage.dst.ssid = atoi(temp);
    	}
        i++;
    }

    //LOG_INFO("C: %.6s S: %d\n", pftmessage.dst.call, pftmessage.dst.ssid );

	// Copy the message payload (text) to the local buffer
	memset(&pftmessage.buf, 0, APRS_PAYLOAD_LEN);
	memcpy(&pftmessage.buf, msg->buf+11, msg->len-11);     // FIXME: Den ACK-Teil hier weglassen


    // Check if message is for ...
    if( (strncmp(pftmessage.dst.call, QTH_MYCALL, QTH_MYCALL_LEN) == 0 ) && (pftmessage.dst.ssid == QTH_MYSSID) |   // For us personally ;)
        (strncmp(pftmessage.dst.call, "BLN", 3) == 0 ) |     // General bulletins
        (strncmp(pftmessage.dst.call, "NWS", 3) == 0 ) )     // Weather reports

    {

		//LOG_INFO("RX-MS:[%.6s-%d]: %s\n", pftmessage.src.call, pftmessage.src.ssid, pftmessage.buf);

		// Check for a message ID at the end of buffer (acknowledgement expected)
		char *pch = strstr(pftmessage.buf, "{" );
		i = 1;
		memset(&temp, 0, 5);

		// Parse message ID number
		if (pch)
		{
			while ( i < 6 )
			{
				if (isdigit((pch[i])))
					temp[i-1] = pch[i];
				i++;
			}
			pftmessage.mid = atoi(temp);
			pftmessage.ack = true;
            //LOG_INFO("ID[%05d]\n", pftmessage.mid);
			//LOG_INFO("%s\n", temp);
		}
	}


	// If message is for us and the sender has requested a acknowledgement message, then reply it
	if (pftmessage.ack == true)
		aprs_send_message_ack();


	/**
	// Inform the user about the received message
	*/
}



















void aprs_parse_packet(struct APRSmessage *msg)
{
	switch(msg->buf[0])
	{

		case APRS_DATA_TYPE_POSITION :
			//LOG_INFO("P");
			break;
		case APRS_DATA_TYPE_MESSAGE :
			//LOG_INFO("M:");
			aprs_parse_pkt_MESSAGE(msg);
			break;
		case APRS_DATA_TYPE_OBJECT :
			//LOG_INFO("O");
			break;
		case APRS_DATA_TYPE_QTH	:
			//LOG_INFO("Q");
			break;
		case APRS_DATA_TYPE_STATUS :
			//LOG_INFO("S");
			break;
		case APRS_DATA_TYPE_ITEM :
			//LOG_INFO("I");
			break;
		case APRS_DATA_TYPE_QUERY :
			//LOG_INFO("?");
			break;
		case APRS_DATA_TYPE_TELEMETRY :
			//LOG_INFO("T");
			break;
		case APRS_DATA_TYPE_USERTYPE :
			//LOG_INFO("U");
			break;
		case APRS_DATA_TYPE_WEATHER :
			//LOG_INFO("W");
			break;
		case APRS_DATA_TYPE_TEST :
			//LOG_INFO("-");
			break;
		case APRS_DATA_TYPE_POSWTSNMESS :
			//LOG_INFO("P1");
			break;
		case APRS_DATA_TYPE_POSNTSNMESS :
			//LOG_INFO("P2");
			break;
		case APRS_DATA_TYPE_POSNTSWMESS :
			//LOG_INFO("P3");
			break;
	}

}



void aprs_init(void)
{

    aprs_status |= (1 << APRS_ENABLE);

	// Initialize AFSK Modem and AX25
    #if CPU_AVR
	afsk_init(&modem, 0 ,0);
    #endif

	ax25_init(&ax25, &modem.fd, aprs_msg_callback);
	//ax25_init(&ax25, &modem.fd, aprs_parse_packet);

}
