#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>

#include "../elements/mobile_ip_packets.hh"

#include "agent_advertisement_source.hh"


CLICK_DECLS
AgentAdvertisementSource::AgentAdvertisementSource(): _sequence(0), 
{}

AgentAdvertisementSource::~AgentAdvertisementSource()
{}

int AgentAdvertisementSource::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
            .read_mp("SRC", _srcIP)
            .read_mp("DST", _dstIP)
            .read_mp("RA", RouterAddress)

            .complete() < 0) return -1;
	
	Timer *timer = new Timer(this);
	timer->initialize(this);
	timer->schedule_after_msec(1000);
	return 0;
}

Packet* AgentAdvertisementSource::make_packet(){
	int headroom = sizeof(click_ether);

    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) 
    + sizeof(icmp_router_advertisement) + sizeof(agent_advertisement_extension)
    +  sizeof(router_address_preference_level) + sizeof(IPAddress) , 0);

    if (!q)
		return 0;
    memset(q->data(), '\0', sizeof(click_ip) + sizeof(icmp_router_advertisement)
    + sizeof(agent_advertisement_extension));

	click_ip *iph = (click_ip *)q->data();
	
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) + 1; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_ICMP; /* icmp */
    iph->ip_ttl = 1;
    iph->ip_src = _srcIP;
    iph->ip_dst = _dstIP;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
	
	icmp_router_advertisement *icmph = (icmp_router_advertisement *)(iph + 1);
	
	icmph->type = ICMP_ROUTERADVERT;
	icmph->code = 0;
	icmph->checksum = 0;
	icmph->num_addrs = 1;
	icmph->addr_entry_size = 2;
	icmph->lifetime = 45;

    router_address_preference_level *rapl = (router_address_preference_level*)(icmph + 1);

    rapl->router_address = RouterAddress;
    rapl->preference_level = 1;

    agent_advertisement_extension *aa_ext = (agent_advertisement_extension*)(rapl + 1);

    aa_ext->type = 16;
    aa_ext->length = 6 + 4 * 1;
    aa_ext->sequence_number = htons(_sequence);
    aa_ext->registration_lifetime = 30;

    aa_ext->R = 1;
    aa_ext->B = 0;
    aa_ext->H = 1;
    aa_ext->F = 1;
    aa_ext->M = 0;
    aa_ext->G = 0;
    aa_ext->r = 0;
    aa_ext->T = 0;
    aa_ext->U = 0;
    aa_ext->X = 0;
    aa_ext->I = 0;

    aa_ext->reserved = 0;

    IPAddress *coa = (IPAddress*)(aa_ext + 1);
     *coa = RouterAddress;

	_sequence++;
	
	icmph->icmp_cksum = click_in_cksum((const unsigned char *)icmph, sizeof(icmp_router_advertisement));
	
	q->set_dst_ip_anno(_dstIP);
	
	return q;
}

void
AgentAdvertisementSource::run_timer(Timer *timer)
{
    if (Packet *q = make_packet()) {
 	   output(0).push(q);
 	   timer->reschedule_after_msec(1000);
    }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AgentAdvertisementSource)