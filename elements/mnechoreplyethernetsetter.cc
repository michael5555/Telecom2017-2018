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
#include "mnechoreplyethernetsetter.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>

CLICK_DECLS

MNEchoReplyEthernetSetter::MNEchoReplyEthernetSetter() : MNBase(0) {}

MNEchoReplyEthernetSetter::~MNEchoReplyEthernetSetter() {}

int MNEchoReplyEthernetSetter::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MNBASE", cpkM+cpkP, cpElementCast, "MNInfoBase", &MNBase,cpEnd) < 0) return -1;
    
    if (MNBase == 0) return errh->error("Wrong argument, should be an MNInfoBase element.");
    
    return 0;
}

void MNEchoReplyEthernetSetter::push(int, Packet* p) {
    this->handleReply(p);
}

void MNEchoReplyEthernetSetter::handleReply(Packet* p) {

    click_ether *ethh = (click_ether *)p->data();
    click_ip *iph = (click_ip *)(ethh + 1);

    if (!MNBase->getHomeStatus() && MNBase->getMyAddress() == iph->ip_src) {

        if( iph->ip_p == IP_PROTO_ICMP) {
            click_icmp_sequenced* icmph = (click_icmp_sequenced*)(iph + 1);
            if(icmph->icmp_type == ICMP_ECHOREPLY) {
                ethh->ether_dhost[0] = MNBase->getCurrentRouterEthernet()[0];
                ethh->ether_dhost[1] = MNBase->getCurrentRouterEthernet()[1];
                ethh->ether_dhost[2] = MNBase->getCurrentRouterEthernet()[2];
                ethh->ether_dhost[3] = MNBase->getCurrentRouterEthernet()[3];
                ethh->ether_dhost[4] = MNBase->getCurrentRouterEthernet()[4];
                ethh->ether_dhost[5] = MNBase->getCurrentRouterEthernet()[5];

            }
        }
    }
    output(0).push(p);
 

}


CLICK_ENDDECLS

EXPORT_ELEMENT(MNEchoReplyEthernetSetter)
