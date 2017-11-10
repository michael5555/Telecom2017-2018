#include <click/config.h>
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include "noderequestgenerator.hh"
#include "mobile_ip_packets.h"
#include <clicknet/ip.h>
#include <clicknet/udp.h>

#include <clicknet/ether.h>
#include <click/timer.hh>
#include <clicknet/icmp.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

CLICK_DECLS

NodeRequestGenerator::NodeRequestGenerator() : MNBase(0), _sequence(0) {}
NodeRequestGenerator::~NodeRequestGenerator() {}

int NodeRequestGenerator::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MNBASE", cpkM+cpkP, cpElementCast,"MNInfoBase", &MNBase, cpEnd) < 0) return -1;
    
    if (MNBase == 0) return errh->error("Wrong  argument, should be a MNInfoBase element.");
    return 0;
}

int NodeRequestGenerator::sendRequest(IPAddress dst){
    if (Packet *q = make_packet(dst)) {
 	    output(0).push(q);
        click_chatter("Mobile Node -- sent Mobile IP Registration Request\n");
    }
    return 0;
}

Packet* NodeRequestGenerator::make_packet(IPAddress destination) {
    int headroom = sizeof(click_ether);
    
    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) 
    + sizeof(click_udp) + sizeof(mobile_ip_registration_request), 0);

    if (!q)
        return 0;
    memset(q->data(), '\0',+ sizeof(click_ip) 
    + sizeof(click_udp) + sizeof(mobile_ip_registration_request));

   click_ip *iph = (click_ip *)q->data();
    
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) ; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_UDP; 
    iph->ip_ttl = 64;
    iph->ip_src = MNBase->getMyAddress();
    iph->ip_dst = destination;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
    
    click_udp *udph = (click_udp *)(iph + 1);
    
    udph->uh_sport = htons(56026);
    udph->uh_dport = htons(434);
    uint16_t len = q->length() - sizeof(click_ip);
    udph->uh_ulen = htons(len);
    udph->uh_sum = click_in_cksum((unsigned char *)udph, len);


    mobile_ip_registration_request *mipr = (mobile_ip_registration_request*)(udph + 1);

    mipr->type = 1;
    mipr->flags = 0x00;
    mipr->lifetime = htons(60);
    mipr->home_address = MNBase->getMyAddress();
    mipr->home_agent = MNBase->getHomeAgentPublic();
    mipr->care_of_address = MNBase->getCareOfAddress();
    mipr->id = htonq(0);

    _sequence++; 
    
    q->set_dst_ip_anno(destination);
    
    return q;
   
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NodeRequestGenerator)
