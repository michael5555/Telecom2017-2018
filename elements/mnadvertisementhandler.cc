
#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>
#include "mninfobase.hh"
#include "mnadvertisementhandler.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>

CLICK_DECLS

MNAdvertisementHandler::MNAdvertisementHandler() : MNBase(0) {}

MNAdvertisementHandler::~MNAdvertisementHandler() {}

int MNAdvertisementHandler::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MNBASE", cpkM+cpkP, cpElementCast, "MNInfoBase", &MNBase, cpEnd) < 0) return -1;
    
    if (MNBase == 0) return errh->error("Wrong argument, should be an MNInfoBase element.");

    return 0;
}

void MNAdvertisementHandler::push(int, Packet* p) {
    this->handleAdvertisement(p);
}

void MNAdvertisementHandler::handleAdvertisement(Packet* p) {
    icmp_router_advertisement* icmph = (icmp_router_advertisement*)(p->data() + p->ip_header_length());

    const int type = icmph->type;
    if (type != ICMP_ROUTERADVERT) {
        return;
    }

    router_address_preference_level *rapl = (router_address_preference_level*)(icmph + 1);
    MNBase->setCurrentRouter(rapl->router_address);

    agent_advertisement_extension *aa_ext = (agent_advertisement_extension*)(rapl + 1);
    MNBase->setCurrentLifeTime(aa_ext->registration_lifetime);
    
    IPAddress *coa = (IPAddress*)(aa_ext + 1);
    MNBase->setCareOfAddress(*coa);

    if(MNBase->getCurrentRouter() == MNBase->getHomeAgentPublic() || MNBase->getCurrentRouter() == MNBase->getHomeAgentPrivate())
    {
        MNBase->setHomeStatus(true);
        click_chatter("Mobile Node -- I am home.\n");
        
    }
    else {

        MNBase->setHomeStatus(false);
        click_chatter("Mobile Node --  I am away.\n");
    }
    
}


CLICK_ENDDECLS

EXPORT_ELEMENT(MNAdvertisementHandler)
