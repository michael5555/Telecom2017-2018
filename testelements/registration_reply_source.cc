#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/udp.h>

#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>

#include "mobile_ip_packets.h"
#include "registration_reply_source.hh"


CLICK_DECLS
RegistrationReplySource::RegistrationReplySource(): _sequence(0)
{}

RegistrationReplySource::~RegistrationReplySource()
{}

int RegistrationReplySource::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
            .read_mp("SRC", _srcIP)
            .read_mp("DST", _dstIP)
            .read_mp("HA", HomeAgent)
            .complete() < 0) return -1;
	
	Timer *timer = new Timer(this);
	timer->initialize(this);
	timer->schedule_after_msec(1000);
	return 0;
}

Packet* RegistrationReplySource::make_packet(){
	int headroom = sizeof(click_ether);

    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) 
    + sizeof(click_udp) + sizeof(mobile_ip_registration_reply), 0);

    if (!q)
		return 0;
    memset(q->data(), '\0', sizeof(click_ip) + sizeof(click_udp)
    + sizeof(mobile_ip_registration_reply));

	click_ip *iph = (click_ip *)q->data();
	
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) /*+ 1*/; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_UDP; /* udp */
    iph->ip_ttl = 64;
    iph->ip_src = _srcIP;
    iph->ip_dst = _dstIP;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
	
	click_udp *udph = (click_udp *)(iph + 1);
	
    udph->uh_sport = htons(56026);
    udph->uh_dport = htons(434);
    udph->uh_ulen = htons(28);
    udph->uh_sum = click_in_cksum((unsigned char *)udph, sizeof(click_udp));
    

    mobile_ip_registration_reply *mipr = (mobile_ip_registration_reply*)(udph + 1);

    mipr->type = 3;
    mipr->code = 0;
    mipr->lifetime = htons(30);
    mipr->home_address = _dstIP;
    mipr->home_agent = HomeAgent;
    mipr->id = htonq(0);

	_sequence++; 
	
	q->set_dst_ip_anno(_dstIP);
	
	return q;
}

void
RegistrationReplySource::run_timer(Timer *timer)
{
    if (Packet *q = make_packet()) {
 	   output(0).push(q);
        click_chatter("sent Mobile IP Registration Reply\n");
 	   timer->reschedule_after_msec(1000);
    }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RegistrationReplySource)
