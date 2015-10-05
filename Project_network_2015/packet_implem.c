//
//  packet_implem.c
//
//  Created by Geoffroy Zoetardt on 1/10/15.
//  Copyright © 2015 Geoffroy Zoetardt. All rights reserved.
//

#include "packet_interface.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>


/*
 * The following tructure represents a packet sent by the Sender program.
 * In this structure :
 *     - type represents the type of packet sent (ack or data),
 *     - window the size of the receiving window,
 *     - seqNum the sequence number of the packet sent,
 *     - length the total length of the payload,
 *     - crc is the crc computed for this packet,
 *     - payload contains the data (total of 512 bytes) and must be empty for ack packets.
 */
struct __attribute__((__packed__)) pkt {
    /* Your code will be inserted here */
    ptypes_t type;
    uint8_t window;
    uint8_t seqnum;
    uint16_t length;
    uint32_t crc;
    const char* payload[512];
};

/* The following function allocates bytes for à packet structure and returns
 * a pointer to the allocated memory or retuns NULL
 */
pkt_t* pkt_new()
{
    pkt_t* packet;
    size_t size = sizeof(pkt_t);
    packet = (pkt_t*) malloc(size);
    
    if (packet == NULL) {
        fprintf(stderr,"Impossible allocation \n");
        return NULL;
    }
    
    return packet;
    
}

void pkt_del(pkt_t *pkt)
{
    free(pkt->payload);
    free(pkt);
}

/*
 * this function decode data that has been received and create a new pkt_t from it.
 * This functions performs the following checks:
 * - Validates the CRC32 of the received data against the one present at the
 *   end of the data stream
 * - Validates the type of the packet
 * - Validates the length of the packet
*/
pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
    if(pkt->crc != crc32(0L,(const Bytef*) data,sizeof(pkt_t))) {
        return E_CRC;
    }
    else if (pkt->type != PTYPE_DATA || pkt->type != PTYPE_ACK || pkt->type != PTYPE_NACK) {
        return E_TYPE;
    }
    else if (!(0 <= pkt->length) || !(pkt->length <= 512)) {
        return E_LENGTH;
    }
    else {
        return PKT_OK;
    }
}

/*
 * Convert a struct pkt into a set of bytes ready to be sent over the wires,
 * including the CRC32 of the header & payload of the packet
 *
 * @pkt: The struct that contains the info about the packet to send
 * @buf: A buffer to store the resulting set of bytes
 * @len: The number of bytes that can be written in buf.
 * @len-POST: The number of bytes written in the buffer by the function.
 * @return: A status code indicating the success or E_NOMEM if the buffer is
 * 		too small
 * 
 */

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
    uint8_t pkt_bytes [sizeof(pkt_t)];
    memcpy(pkt_bytes, pkt, sizeof(pkt_t));
    uLong crc = crc32(0L, Z_NULL, 0);
    pkt_set_crc(pkt, crc32(crc, pkt_bytes , pkt->length));
    if (sizeof(pkt_t) > *len){
        return E_NOMEM;
    }
    *buf = *pkt_bytes;
    *len = sizeof(pkt_t);
    return PKT_OK;
}

ptypes_t pkt_get_type  (const pkt_t* pkt)
{
    return pkt->type;
}

uint8_t  pkt_get_window(const pkt_t* pkt)
{
    return pkt->window;
}

uint8_t  pkt_get_seqnum(const pkt_t* pkt)
{
    return pkt->seqnum;
}

uint16_t pkt_get_length(const pkt_t* pkt)
{
    return pkt->length;
}

uint32_t pkt_get_crc   (const pkt_t* pkt)
{
    return pkt->crc;
}

const char* pkt_get_payload(const pkt_t*pkt)
{
    return pkt->payload;
}


pkt_status_code pkt_set_type(pkt_t *pkt, const ptypes_t type)
{
    /* Your code will be inserted here */
    if (type != PTYPE_ACK || type != PTYPE_DATA || type != PTYPE_NACK) {
        return E_TYPE;
    }
    pkt->type = type;
    return PKT_OK;
}

pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window)
{
    /* Your code will be inserted here */
    if (window<0 && window>=MAX_WINDOW_SIZE){
        return E_WINDOW;
    }
    pkt->window = window;
    return PKT_OK;
}

pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum)
{
    if (seqnum<0 && seqnum>=255){
        return E_SEQNUM;
    }
    pkt->seqnum = seqnum;
    return PKT_OK;
}

pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length)
{
    if (length<0 && length>=512){
        return E_LENGTH;
    }
    pkt->length = length;
    return PKT_OK;
}

pkt_status_code pkt_set_crc(pkt_t *pkt, const uint32_t crc)
{
    pkt->crc = crc;
    return PKT_OK;
}

pkt_status_code pkt_set_payload(pkt_t *pkt,
                                const char *data,
                                const uint16_t length)
{
    if (length % 4 != 0){
        return E_PADDING;
    }

    return PKT_OK;
}