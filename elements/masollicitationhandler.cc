#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>
#include "masollicitationhandler.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>

CLICK_DECLS

MASollicitationHandler::MASollicitationHandler() : MABase(0) {}

MASollicitationHandler::~MASollicitationHandler() {}

int MASollicitationHandler::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MABASE", cpkM+cpkP, cpElementCast, "MAInfoBase", &MABase,
    "AAGEN", cpkM+cpkP, cpElementCast, "AgentAdvertisementGenerator", &AAGen, cpEnd) < 0) return -1;
    
    if (MABase == 0) return errh->error("Wrong argument, should be an MAInfoBase element.");
    if (AAGen == 0) return errh->error("Wrong argument, should be an AgentAdvertisementGenerator element.");
    
    return 0;
}

void MASollicitationHandler::push(int, Packet* p) {
    this->handleSollicitation(p);
}

void MASollicitationHandler::handleSollicitation(Packet* p) {
    click_chatter("Mobility Agent -- recieved router sollicitation.\n");
    click_ip *iph = (click_ip *)p->data();
    icmp_router_sollicitation *icmph = (icmp_router_sollicitation *)(iph + 1);

    if (icmph->type != ICMP_ROUTERSOLICIT)
        return;
    IPAddress Advertdst = iph->ip_src;
    AAGen->sendUnicastAdvertisement(Advertdst);

}


CLICK_ENDDECLS

EXPORT_ELEMENT(MASollicitationHandler)
