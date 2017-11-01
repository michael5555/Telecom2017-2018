#include <click/config.h>
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include "nodesollicitationgenerator.hh"
#include "mobile_ip_packets.h"
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <clicknet/icmp.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

CLICK_DECLS

NodeSollicitationGenerator::NodeSollicitationGenerator() : MNBase(0), _sequence(0) {}
NodeSollicitationGenerator::~NodeSollicitationGenerator() {}

int NodeSollicitationGenerator::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MNBASE", cpkM+cpkP, cpElementCast,"MNInfoBase", &MNBase, cpEnd) < 0) return -1;
    
    if (MNBase == 0) return errh->error("Wrong  argument, should be a MNInfoBase element.");
    return 0;
}

int NodeSollicitationGenerator::sendSollicitation(const String& conf, Element* e, void* thunk, ErrorHandler* errh){
    NodeSollicitationGenerator* nsg =  (NodeSollicitationGenerator*) e;
    if (Packet *q = nsg->make_packet()) {
 	    nsg->output(0).push(q);
        click_chatter("sent router sollicitation with broadcast destination\n");
    }
    return 0;
}
  
void NodeSollicitationGenerator::add_handlers() {
    add_write_handler("sendSollicitation", &sendSollicitation, (void*)0);
}

Packet* NodeSollicitationGenerator::make_packet() {
    int headroom = sizeof(click_ether);
    
    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) 
    + sizeof(icmp_router_sollicitation) , 0);

    if (!q)
        return 0;
    memset(q->data(), '\0', sizeof(click_ip) + sizeof(icmp_router_sollicitation));

    click_ip *iph = (click_ip *)q->data();
    
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) /*+ 1*/; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_ICMP; /* icmp */
    iph->ip_ttl = 1;
    iph->ip_src = MNBase->getMyAddress();
    iph->ip_dst = IPAddress("255.255.255.255");
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
    
    icmp_router_sollicitation *icmph = (icmp_router_sollicitation *)(iph + 1);
    
    icmph->type = ICMP_ROUTERSOLICIT;
    icmph->code = 0;
    icmph->checksum = 0;
    icmph->checksum = click_in_cksum((const unsigned char *)icmph, sizeof(icmp_router_sollicitation));

    _sequence++; 
        
    q->set_dst_ip_anno(IPAddress("255.255.255.255"));
        
    
    return q;
   
}

CLICK_ENDDECLS
EXPORT_ELEMENT(NodeSollicitationGenerator)