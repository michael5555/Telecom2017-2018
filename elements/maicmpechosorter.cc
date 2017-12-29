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

MAICMPEchoSorter::MAICMPEchoSorter() : MABase(0),_sequence(0) {}

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
                Packet* q = encap(p);
                output(1).push(q);
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

Packet* MAICMPEchoSorter::encap(Packet*p) {

    WritablePacket *q = p->push(sizeof(click_ip));
    if (!q) return 0;

    click_ip* iph = (click_ip*)(q->data());

    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    iph->ip_p = 4;
    uint16_t ip_id = ((_sequence) % 0xFFFF);
    iph->ip_id = htons(ip_id);
    iph->ip_ttl = 250;

    iph->ip_src = MABase->getMyPublicAddress();
    iph->ip_dst = MABase->getLocalNode().careofaddress;

    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));

    q->set_dst_ip_anno(iph->ip_dst);
    q->set_ip_header(iph, sizeof(click_ip));

    return q;

}



CLICK_ENDDECLS

EXPORT_ELEMENT(MAICMPEchoSorter)
