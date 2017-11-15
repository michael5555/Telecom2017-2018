#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/udp.h>

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
    if (cp_va_kparse(conf, this, errh, "MABASE", cpkM+cpkP, cpElementCast, "MAInfoBase", &MABase,
    "REPLYGEN", cpkM+cpkP, cpElementCast, "MAReplyGenerator", &ReplyGen,cpEnd) < 0) return -1;
    
    if (MABase == 0) return errh->error("Wrong argument, should be an MAInfoBase element.");
    if (ReplyGen == 0) return errh->error("Wrong argument, should be an MAReplyGenerator element.");

    return 0;
}

void MARequestHandler::push(int, Packet* p) {
    this->handleRequest(p);
}

void MARequestHandler::handleRequest(Packet* p) {

    WritablePacket * q = p->uniqueify();
    click_ip *iph = (click_ip *)q->data();
    if(iph->ip_p != IP_PROTO_UDP){
        output(0).push(q);
        return;
    }
    click_udp* udph = (click_udp *)(iph + 1);
    mobile_ip_registration_request* mipr = (mobile_ip_registration_request*)(udph + 1);

    if (mipr->type == REGISTRATION_REQUEST){
        click_chatter("Mobile Agent -- Recieved Registration Request. %s\n",MABase->getMyPublicAddress().unparse().c_str());
        if(mipr->home_agent != MABase->getMyPublicAddress() && mipr->home_agent != MABase->getMyPrivateAddress()) {
            this->relayRequest(mipr,iph,q);
        }
        if(mipr->home_agent == MABase->getMyPublicAddress() || mipr->home_agent == MABase->getMyPrivateAddress()) {
            MABase->setLocalNode(mipr->home_address,mipr->care_of_address,mipr->lifetime);
            ReplyGen->sendReply();
        }
    }
    else if(mipr->type == REGISTRATION_REPLY){
        click_chatter("Mobile Agent -- Recieved Registration Request. %s\n",MABase->getMyPublicAddress().unparse().c_str());
    }
}

void MARequestHandler::relayRequest(mobile_ip_registration_request* mipr,click_ip* iph, Packet* q){
    iph->ip_src = mipr->care_of_address;
    iph->ip_dst = mipr->home_agent;
    output(1).push(q);
    click_chatter("Mobile Agent -- relayed Registration Request.\n",MABase->getMyPublicAddress().unparse().c_str());


}

CLICK_ENDDECLS

EXPORT_ELEMENT(MARequestHandler)
