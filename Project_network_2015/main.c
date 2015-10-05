//
//  main.c
//  Project_network_2015
//
//  Created by Geoffroy Zoetardt on 5/10/15.
//  Copyright © 2015 Geoffroy Zoetardt. All rights reserved.
//

#include "main.h"
#include "packet_interface.h"

int main(){
    pkt_t* pkt = pkt_new();
    pkt_set_type(pkt, PTYPE_ACK);
    char buf [sizeof(pkt_t*)];
    pkt_status_code test = pkt_encode(pkt, buf, (size_t*)sizeof(pkt_t*));
    printf("hello\n");
}