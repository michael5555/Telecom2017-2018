#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>
#include "mninfobase.hh"
#include "mnadvertisementethernethandler.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>

CLICK_DECLS

MNAdvertisementEthernetHandler::MNAdvertisementEthernetHandler() : MNBase(0) {}

MNAdvertisementEthernetHandler::~MNAdvertisementEthernetHandler() {}

int MNAdvertisementEthernetHandler::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MNBASE", cpkM+cpkP, cpElementCast, "MNInfoBase", &MNBase, cpEnd) < 0) return -1;
    
    if (MNBase == 0) return errh->error("Wrong argument, should be an MNInfoBase element.");
    
    return 0;
}

void MNAdvertisementEthernetHandler::push(int, Packet* p) {
    this->handleAdvertisement(p);
}

void MNAdvertisementEthernetHandler::handleAdvertisement(Packet* p) {

    click_ether *ethh = (click_ether *)p->data();
    click_ip *iph = (click_ip *)(ethh + 1);
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
    MNBase->setCurrentRouterEthernet(ethh->ether_shost);
    output(0).push(p);


}


CLICK_ENDDECLS

EXPORT_ELEMENT(MNAdvertisementEthernetHandler)
