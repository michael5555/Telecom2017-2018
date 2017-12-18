
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
    if (cp_va_kparse(conf, this, errh, "MNBASE", cpkM+cpkP, cpElementCast, "MNInfoBase", &MNBase,
    "RGEN", cpkM+cpkP, cpElementCast, "NodeRequestGenerator", &RGen,
    "RT", cpkM+cpkP, cpElementCast, "LinearIPLookup", &rt,cpEnd) < 0) return -1;
    
    if (MNBase == 0) return errh->error("Wrong argument, should be an MNInfoBase element.");
    if (RGen == 0) return errh->error("Wrong argument, should be an NodeRequestGenerator element.");
    if (rt == 0) return errh->error("Wrong argument, should be an LinearIPLookup element.");

    
    return 0;
}

void MNAdvertisementHandler::push(int, Packet* p) {
    this->handleAdvertisement(p);
}

void MNAdvertisementHandler::handleAdvertisement(Packet* p) {

    click_ip *iph = (click_ip *)p->data();
    icmp_router_advertisement* icmph = (icmp_router_advertisement*)(iph + 1);
    

    if(iph->ip_p != IP_PROTO_ICMP) {
        output(0).push(p);
        return;
    } 
    else {
        const int type = icmph->type;
        if (type != ICMP_ROUTERADVERT) {
            output(0).push(p);
            return;
        }
    }
    router_address_preference_level *rapl = (router_address_preference_level*)(icmph + 1);
    MNBase->setCurrentRouter(rapl->router_address);


    agent_advertisement_extension *aa_ext = (agent_advertisement_extension*)(rapl + 1);
    MNBase->setCurrentLifeTime(aa_ext->registration_lifetime);
    
    IPAddress *coa = (IPAddress*)(aa_ext + 1);
    if(*coa != MNBase->getHomeAgentPublic()){
        MNBase->setCareOfAddress(*coa);
    }

    if(MNBase->getCurrentRouter() == MNBase->getHomeAgentPublic() || MNBase->getCurrentRouter() == MNBase->getHomeAgentPrivate())
    {
        MNBase->setHomeStatus(true);
        click_chatter("Mobile Node -- I am home.\n");
        RGen->sendRequest(MNBase->getHomeAgentPrivate());
    }
    else {

        MNBase->setHomeStatus(false);
        click_chatter("Mobile Node --  I am away.\n");
        RGen->sendRequest(iph->ip_src);
    }
    
}


CLICK_ENDDECLS

EXPORT_ELEMENT(MNAdvertisementHandler)
