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
    char payload[512];
    uint32_t crc;
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
    free(pkt);
}

pkt_t* stock_in_packet(const char* data, const size_t len){
    pkt_t* pkt = pkt_new();
    uint8_t i = 0;
    while (i<len){
        if (i<3){
            char * dst = (char*)malloc(sizeof(ptypes_t));
            strncpy(dst, data, sizeof(ptypes_t));
            pkt_set_type(pkt, (ptypes_t)dst);
            i=i+sizeof(ptypes_t);
            free(dst);
        }
        else if(i>=3 && i<8){
            char * dst = (char*)malloc(sizeof(uint8_t));
            strncpy(dst, data + i, sizeof(uint8_t));
            pkt_set_window(pkt, (uint8_t)atoi(dst));
            i=i+sizeof(uint8_t);
            free(dst);
        }
        else if(i>=8 && i<16){
            char * dst = (char*)malloc(sizeof(uint8_t));
            strncpy(dst, data + i, sizeof(uint8_t));
            pkt_set_seqnum(pkt, (uint8_t)atoi(dst));
            i=i+sizeof(uint8_t);
            free(dst);
        }
        else if(i>=16 && i<32){
            char * dst = (char*)malloc(sizeof(uint16_t));
            strncpy(dst, data + i, sizeof(uint16_t));
            pkt_set_length(pkt, (uint8_t)atoi(dst));
            i=i+sizeof(uint16_t);
            free(dst);
        }
        else if(i>=32 && i<len-4){
            char * dst = (char*)malloc(len-4-i);
            strncpy(dst, data + i, len-4-i);
            pkt_set_payload(pkt, dst,len-4-i);
            i=len-4;
            free(dst);
        }
        else{
            char * dst = (char*)malloc(sizeof(uint32_t));
            strncpy(dst, data + i, sizeof(uint32_t));
            pkt_set_crc(pkt, (uint32_t) atoi(dst));
            i=len;
            free(dst);
        }
    }
    return pkt;
}
/*
 * this function decode data that has been received and create a new pkt_t from it.
 * This functions performs the following checks:
 * - Validates the CRC32 of the received data against the one present at the
 *   end of the data stream
 * - Validates the type of the packet
 * - Validates the length of the packet
 * @data: The set of bytes that constitutes the packet
 * @len: The number of bytes received
 * @pkt: An allocated struct pkt, where the decoded data should be stored.
 *
 * @return: A status code indicating the success or the failure type.
 * 			Unless the error is E_NOHEADER, the packet has at least the
 * 			values of the header found in the data stream.
 */
pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
    pkt = stock_in_packet(data, len);
    //Check the CRC
    if(pkt->crc != crc32(0L,(const Bytef*) data,sizeof(uint32_t))) {
        return E_CRC;
    }
    //Check if the type is right
    else if (pkt->type != PTYPE_DATA && pkt->type != PTYPE_ACK && pkt->type != PTYPE_NACK) {
        return E_TYPE;
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
    if (sizeof(pkt) > *len){
        return E_NOMEM;
    }
    //flux de bytes du payload et du header (sans le CRC)
    char* pkt_bytes = (char*)malloc(sizeof(pkt)-4);
    if (pkt_bytes == NULL){
        fprintf(stderr,"Impossible allocation \n");
        return E_NOMEM;
    }
    memcpy(pkt_bytes, pkt, sizeof(pkt)-4);
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (const Bytef*)pkt_bytes , sizeof(pkt)-4);
    //Buf = concat du flux et du crc calculé
    memcpy(buf, pkt_bytes, sizeof(pkt)-4);
    memcpy(buf, (char*)crc, 4);
    *len = sizeof(buf);
    free(pkt_bytes);
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
    if (type != PTYPE_ACK && type != PTYPE_DATA && type != PTYPE_NACK) {
        return E_TYPE;
    }
    pkt->type = type;
    return PKT_OK;
}

pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window)
{
    pkt->window = window;
    return PKT_OK;
}

pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum)
{
    pkt->seqnum = seqnum;
    return PKT_OK;
}

pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length)
{
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
    uint padding = length % 4;
    char* payload = (char*)malloc(pkt->length+padding);
    memcpy(payload, data, length);
    uint i = 1;
    while (i <= padding){
        memcpy(&payload[length+i], "\0", sizeof(uint));
        i++;
    }
    pkt_set_length(pkt,length+padding);
    memcpy(pkt->payload, payload, pkt->length);
    free(payload);
    return PKT_OK;
}