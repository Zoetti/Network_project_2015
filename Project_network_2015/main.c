//
//  main.c
//  Project_network_2015
//
//  Created by Geoffroy Zoetardt on 5/10/15.
//  Copyright © 2015 Geoffroy Zoetardt. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "packet_interface.h"

size_t pkt_get_total_length(const pkt_t* pkt);
char* pkt_copy(char *buf, pkt_t* pkt, size_t len);

int main(){
    pkt_t* pkt = pkt_new();
   // printf("%s",pkt);
    //ptypes_t type = PTYPE_ACK;
    pkt_set_type(pkt, PTYPE_ACK);
    
    printf("fin\n");
    //printf("type:%s\n", (char)pkt->type);
    /*uint8_t window = 0;
    pkt_set_window(pkt, window);
    pkt_set_seqnum(pkt, 0);
    pkt_set_length(pkt, 0);
    pkt_set_crc(pkt, 0);*/
    
    char* data ="data test 1      zzzzz        eeerecez";
    pkt_set_payload(pkt, data, strlen(data));
    
    char * buf = (char*)malloc( sizeof(char *)*pkt_get_total_length(pkt));
    size_t size = pkt_get_total_length(pkt);
    
    int v = pkt_encode(pkt, buf, &size);
    printf("out:%d\n", v);
}