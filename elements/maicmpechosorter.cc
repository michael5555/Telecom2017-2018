#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>
#include "maicmpechosorter.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>
#include "mainfobase.hh"


CLICK_DECLS

MAICMPEchoSorter::MAICMPEchoSorter() : MABase(0) {}

MAICMPEchoSorter::~MAICMPEchoSorter() {}

int MAICMPEchoSorter::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MABASE", cpkM+cpkP, cpElementCast, "MAInfoBase", &MABase, cpEnd) < 0) return -1;
    
    if (MABase == 0) return errh->error("Wrong argument, should be an MAInfoBase element.");    
    return 0;
}

void MAICMPEchoSorter::push(int, Packet* p) {

    click_ip *iph = (click_ip *)p->data();
    if (iph->ip_p == IP_PROTO_ICMP) {
        click_icmp_sequenced* icmph = (click_icmp_sequenced*)(iph + 1);
        if (icmph->icmp_type == ICMP_ECHO || icmph->icmp_type == ICMP_ECHOREPLY) {
            localnodeinfo localinfo = MABase->getLocalNode();
            if(localinfo.careofaddress != MABase->getMyPublicAddress() && localinfo.home_address != IPAddress()){
                output(1).push(p);
                return;
            }
            else {
                output(0).push(p);
                return;
            }
        }
        else {
            output(0).push(p);
        }
    }
    else {
        output(0).push(p);
        return;
    }

}



CLICK_ENDDECLS

EXPORT_ELEMENT(MAICMPEchoSorter)
