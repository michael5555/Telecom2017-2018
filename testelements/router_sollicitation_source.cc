#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>

#include "mobile_ip_packets.h"
#include "router_sollicitation_source.hh"


CLICK_DECLS
RouterSollicitationSource::RouterSollicitationSource(): _sequence(0)
{}

RouterSollicitationSource::~RouterSollicitationSource()
{}

int RouterSollicitationSource::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
            .read_mp("SRC", _srcIP)
            .read_mp("DST", _dstIP)
            .complete() < 0) return -1;
	
	Timer *timer = new Timer(this);
	timer->initialize(this);
	timer->schedule_after_msec(1000);
	return 0;
}

Packet* RouterSollicitationSource::make_packet(){
	int headroom = sizeof(click_ether);

    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) 
    + sizeof(icmp_router_sollicitation) , 0);

    if (!q)
		return 0;
    memset(q->data(), '\0', sizeof(click_ip) + sizeof(icmp_router_sollicitation));

	click_ip *iph = (click_ip *)q->data();
	
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) /*+ 1*/; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_ICMP; /* icmp */
    iph->ip_ttl = 1;
    iph->ip_src = _srcIP;
    iph->ip_dst = _dstIP;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
	
	icmp_router_sollicitation *icmph = (icmp_router_sollicitation *)(iph + 1);
	
	icmph->type = ICMP_ROUTERSOLICIT;
	icmph->code = 0;
	icmph->checksum = 0;
	icmph->checksum = click_in_cksum((const unsigned char *)icmph, sizeof(icmp_router_sollicitation));

	_sequence++; 
		
	q->set_dst_ip_anno(_dstIP);
	
	return q;
}

void
RouterSollicitationSource::run_timer(Timer *timer)
{
    if (Packet *q = make_packet()) {
 	   output(0).push(q);
        click_chatter("send router sollicitation\n");
 	   timer->reschedule_after_msec(1000);
    }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AgentAdvertisementSource)
