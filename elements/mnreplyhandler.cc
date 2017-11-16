#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/udp.h>

#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>
#include "mninfobase.hh"
#include "mnreplyhandler.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>

CLICK_DECLS

MNReplyHandler::MNReplyHandler() : MNBase(0) {}

MNReplyHandler::~MNReplyHandler() {}

int MNReplyHandler::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MNBASE", cpkM+cpkP, cpElementCast, "MNInfoBase", &MNBase,cpEnd) < 0) return -1;
    
    if (MNBase == 0) return errh->error("Wrong argument, should be an MNInfoBase element.");
    
    return 0;
}

void MNReplyHandler::push(int, Packet* p) {
    this->handleReply(p);
}

void MNReplyHandler::handleReply(Packet* p) {

    click_ip *iph = (click_ip *)p->data();
    click_udp* udph = (click_udp*)(iph + 1);
    mobile_ip_registration_reply* mipr = (mobile_ip_registration_reply*)(udph + 1);
    if(iph->ip_p == IP_PROTO_UDP){

        if(mipr->type == REGISTRATION_REPLY){
            click_chatter("Mobile Node -- recieved registration reply %s",MNBase->getMyAddress().unparse().c_str());
            return;
        }
        return;
    }
    else {
        output(0).push(p);
    }

}


CLICK_ENDDECLS

EXPORT_ELEMENT(MNReplyHandler)
