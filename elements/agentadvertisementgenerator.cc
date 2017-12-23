#include <click/config.h>
#include <click/args.hh>
#include <click/confparse.hh>
#include <click/error.hh>
#include "agentadvertisementgenerator.hh"
#include "mobile_ip_packets.h"
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <clicknet/icmp.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

CLICK_DECLS

AgentAdvertisementGenerator::AgentAdvertisementGenerator() : MABase(0), _sequence(0), interval(15), timer(this) {}
AgentAdvertisementGenerator::~AgentAdvertisementGenerator() {}

int AgentAdvertisementGenerator::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MABASE", cpkM+cpkP, cpElementCast,"MAInfoBase", &MABase, cpEnd) < 0) return -1;
    
    if (MABase == 0) return errh->error("Wrong  argument, should be a MAInfoBase element.");
    timer.initialize(this);
    timer.schedule_after_msec(0);
    return 0;
}

int AgentAdvertisementGenerator::sendAgentAdvertisement(const String& conf, Element* e, void* thunk, ErrorHandler* errh){
    AgentAdvertisementGenerator* aag =  (AgentAdvertisementGenerator*) e;
    if (Packet *q = aag->make_packet(IPAddress("255.255.255.255"))) {
 	    aag->output(0).push(q);
        click_chatter("Mobile Agent -- sent agent advertisement with broadcast destination\n");
    }
    return 0;
}

void AgentAdvertisementGenerator::sendMultiCastAdvertisement(){
    if (Packet *q = make_packet(IPAddress("255.255.255.255"))) {
 	    output(0).push(q);
        click_chatter("Mobile Agent -- sent agent advertisement with unicast destination %s\n",MABase->getMyPublicAddress().unparse().c_str());
    }
}
void AgentAdvertisementGenerator::run_timer(Timer* t){

    this->sendMultiCastAdvertisement();
    double r = ((double) rand() / (RAND_MAX));
    timer.schedule_after_msec((interval * 1000) - r);
}
  
void AgentAdvertisementGenerator::add_handlers() {
    add_write_handler("sendAdvertisement", &sendAgentAdvertisement, (void*)0);
}

void AgentAdvertisementGenerator::setInterval(unsigned int i) {

    interval = i;
}

Packet* AgentAdvertisementGenerator::make_packet(IPAddress _dst) {
    int headroom = sizeof(click_ether);
    
    int advertisementsize = sizeof(click_ip) 
    + sizeof(icmp_router_advertisement) + sizeof(router_address_preference_level) 
    + sizeof(agent_advertisement_extension)
    + sizeof(IPAddress);
    
    WritablePacket *q = Packet::make(headroom, 0,advertisementsize , 0);

    if (!q)
        return 0;
    memset(q->data(), '\0', advertisementsize);

    click_ip *iph = (click_ip *)q->data();
    
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) /*+ 1*/; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_ICMP; /* icmp */
    iph->ip_ttl = 1;
    iph->ip_src = MABase->getMyPrivateAddress();
    iph->ip_dst = _dst;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
    
    icmp_router_advertisement *icmph = (icmp_router_advertisement *)(iph + 1);
    
    icmph->type = ICMP_ROUTERADVERT;
    icmph->code = 0;
    icmph->checksum = 0;
    icmph->num_addrs = 1;
    icmph->addr_entry_size = 2;
    icmph->lifetime = htons(45);

    router_address_preference_level *rapl = (router_address_preference_level*)(icmph + 1);

    rapl->router_address = MABase->getMyPublicAddress();
    rapl->preference_level = htonl(1);

    agent_advertisement_extension *aa_ext = (agent_advertisement_extension*)(rapl + 1);

    aa_ext->type = 16;
    aa_ext->length = 6 + 4 * 1;
    aa_ext->sequence_number = htons(_sequence);
    aa_ext->registration_lifetime = htons(30);
    aa_ext->flags_and_reserved = htons(0b1011000000000000);

    IPAddress *coa = (IPAddress*)(aa_ext + 1);
    *coa = MABase->getMyPublicAddress();

    icmph->checksum = click_in_cksum((const unsigned char *)icmph, sizeof(icmp_router_advertisement)
    + sizeof(router_address_preference_level) + sizeof(agent_advertisement_extension) + sizeof(IPAddress));

    _sequence++; 
    
    q->set_dst_ip_anno(_dst);
    
    return q;
   
}



CLICK_ENDDECLS
EXPORT_ELEMENT(AgentAdvertisementGenerator)
