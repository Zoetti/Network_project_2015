//
//  packet_implem.c
//
//  Created by Geoffroy Zoetardt on 1/10/15.
//  Copyright © 2015 Geoffroy Zoetardt. All rights reserved.
//

#include "packet_interface.h"

/* Extra #includes */
/* Your code will be inserted here */

struct __attribute__((__packed__)) pkt {
    /* Your code will be inserted here */
    ptypes_t type;
    uint8_t window;
    uint8_t seqnum;
    uint16_t length;
    uint32_t crc;
    const char* payload[512];
};

/* Extra code */
/* Your code will be inserted here */

pkt_t* pkt_new()
{
    /* Your code will be inserted here */
    if (/*syst out of mem*/) {
        return NULL;
    }
    
}

void pkt_del(pkt_t *pkt)
{
    free()
}

pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
    /* Your code will be inserted here */
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
    /* Your code will be inserted here */
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
    /* Your code will be inserted here */
}

pkt_status_code pkt_set_payload(pkt_t *pkt,
                                const char *data,
                                const uint16_t length)
{
    if (length % 4 != 0){
        return E_PADDING;
    }
        
}