#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>
#include "marequesthandler.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>

CLICK_DECLS

MARequestHandler::MARequestHandler() : MABase(0) {}

MARequestHandler::~MARequestHandler() {}

int MARequestHandler::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MABASE", cpkM+cpkP, cpElementCast, "MAInfoBase", &MABase, cpEnd) < 0) return -1;
    
    if (MABase == 0) return errh->error("Wrong argument, should be an MAInfoBase element.");
    
    return 0;
}

void MARequestHandler::push(int, Packet* p) {
    this->handleRequest(p);
}

void MARequestHandler::handleRequest(Packet* p) {
    click_ip *iph = (click_ip *)p->data();
    if(iph->ip_p != IP_PROTO_UDP){
        output(0).push(p);
        return;
    }
    click_udp* udph = (click_udp *)(iph + 1);
    mobile_ip_registration_request* mipr = (mobile_ip_registration_request*)(udph + 1);

    if (mipr->type != 1)
        return;

    click_chatter("Mobile Agent -- Recieved Registration Request.\n");



}


CLICK_ENDDECLS

EXPORT_ELEMENT(MARequestHandler)
