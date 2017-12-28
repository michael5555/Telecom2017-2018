#include <click/config.h>
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include "mareplygenerator.hh"
#include "mobile_ip_packets.h"
#include <clicknet/ip.h>
#include <clicknet/udp.h>

#include <clicknet/ether.h>
#include <click/timer.hh>
#include <clicknet/icmp.h>
#include <ctime>
#include <chrono>

#include <stdlib.h>
#include <iostream>

CLICK_DECLS

typedef std::chrono::system_clock Clock;


MAReplyGenerator::MAReplyGenerator() : MABase(0), _sequence(0) {}
MAReplyGenerator::~MAReplyGenerator() {}

int MAReplyGenerator::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MABASE", cpkM+cpkP, cpElementCast,"MAInfoBase", &MABase, cpEnd) < 0) return -1;
    
    if (MABase == 0) return errh->error("Wrong argument, should be a MAInfoBase element.");
    return 0;
}

int MAReplyGenerator::sendReply(uint32_t id1,uint32_t id2){

    if (Packet *q = make_packet()) {
        click_ip *iph = (click_ip *)q->data();
        if( iph->ip_src == MABase->getMyPublicAddress())
 	        output(0).push(q);
        else if(iph->ip_src == MABase->getMyPrivateAddress())
            output(1).push(q);
        click_chatter("Mobile Agent -- sent Mobile IP Registration Reply\n");
    }
    return 0;
}

Packet* MAReplyGenerator::make_packet(uint32_t id1,uint32_t id2) {

    int headroom = sizeof(click_ether);
    
    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) 
    + sizeof(click_udp) + sizeof(mobile_ip_registration_reply), 0);

    if (!q)
        return 0;
    memset(q->data(), '\0',+ sizeof(click_ip) 
    + sizeof(click_udp) + sizeof(mobile_ip_registration_reply));

    localnodeinfo nodeinfo = MABase->getLocalNode();
    IPAddress dst = IPAddress();
    IPAddress src = IPAddress();

    if(nodeinfo.careofaddress ==  MABase->getMyPublicAddress()) {
        src = MABase->getMyPrivateAddress();
        dst = nodeinfo.home_address;
    } else {
        src = MABase->getMyPublicAddress();
        dst = nodeinfo.careofaddress;
    }

    click_ip *iph = (click_ip *)q->data();
    
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) ; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_UDP; 
    iph->ip_ttl = 64;
    iph->ip_src = src;
    iph->ip_dst = dst;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
    
    click_udp *udph = (click_udp *)(iph + 1);
    
    udph->uh_sport = htons(434);
    udph->uh_dport = htons(56026);
    uint16_t len = q->length() - sizeof(click_ip);
    udph->uh_ulen = htons(len);
    udph->uh_sum = click_in_cksum((unsigned char *)udph, sizeof(click_udp));

    mobile_ip_registration_reply *mipr = (mobile_ip_registration_reply*)(udph + 1);

    mipr->type = 3;
    mipr->code = 0;
    mipr->lifetime = htons(30);
    mipr->home_address = nodeinfo.home_address.addr();
    mipr->home_agent = MABase->getMyPublicAddress().addr();




    mipr->id1 = id1;
    mipr->id2 = id2;

    _sequence++; 
    
    q->set_dst_ip_anno(dst);

    return q;
   
}

int MAReplyGenerator::denyReply(uint32_t id1,uint32_t id2,uint8_t code, IPAddress src, IPAddress dst) {

int headroom = sizeof(click_ether);
    
    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) 
    + sizeof(click_udp) + sizeof(mobile_ip_registration_reply), 0);

    if (!q)
        return 0;
    memset(q->data(), '\0',+ sizeof(click_ip) 
    + sizeof(click_udp) + sizeof(mobile_ip_registration_reply));

    localnodeinfo nodeinfo = MABase->getLocalNode();

    click_ip *iph = (click_ip *)q->data();
    
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) ; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_UDP; 
    iph->ip_ttl = 64;
    iph->ip_src = src;
    iph->ip_dst = dst;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
    
    click_udp *udph = (click_udp *)(iph + 1);
    
    udph->uh_sport = htons(434);
    udph->uh_dport = htons(56026);
    uint16_t len = q->length() - sizeof(click_ip);
    udph->uh_ulen = htons(len);
    udph->uh_sum = click_in_cksum((unsigned char *)udph, sizeof(click_udp));

    mobile_ip_registration_reply *mipr = (mobile_ip_registration_reply*)(udph + 1);

    mipr->type = 3;
    mipr->code = code;
    mipr->lifetime = htons(30);
    mipr->home_address = nodeinfo.home_address.addr();
    mipr->home_agent = MABase->getMyPublicAddress().addr();




    mipr->id1 = id1;
    mipr->id2 = id2;

    _sequence++; 
    
    q->set_dst_ip_anno(dst);

    return q;


}

CLICK_ENDDECLS
EXPORT_ELEMENT(MAReplyGenerator)
