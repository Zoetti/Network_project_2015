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

size_t pkt_get_total_length(const pkt_t* pkt);

int main(){
    pkt_t* pkt = pkt_new();
   // printf("%s",pkt);
    pkt_set_type(pkt, PTYPE_ACK);
    char* data ="data test 1      zzzzz        eeerecez";
    pkt_set_payload(pkt, data, strlen(data));
    char buf [sizeof(pkt_t*)];
    size_t size = pkt_get_total_length(pkt);
    printf("out:%d\n",pkt_encode(pkt, buf, &size));
}