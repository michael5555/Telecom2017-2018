#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include "myicmppingsource.hh"
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>

CLICK_DECLS
MyICMPPingSource::MyICMPPingSource(): _sequence(0), _ttl(1), _incTTL(false)
{}

MyICMPPingSource::~MyICMPPingSource()
{}

int MyICMPPingSource::configure(Vector<String> &conf, ErrorHandler *errh) {
    if (Args(conf, this, errh)
            .read_mp("SRC", _srcIP)
            .read_mp("DST", _dstIP)
            .read_p("INCR_TTL", _incTTL)
            .complete() < 0) return -1;
	
	Timer *timer = new Timer(this);
	timer->initialize(this);
	timer->schedule_after_msec(1000);
	return 0;
}

Packet* MyICMPPingSource::make_packet(){
	int headroom = sizeof(click_ether);
    WritablePacket *q = Packet::make(headroom, 0, sizeof(click_ip) + sizeof(struct click_icmp_echo), 0);
    if (!q)
		return 0;
    memset(q->data(), '\0', sizeof(click_ip) + sizeof(struct click_icmp_echo));

	click_ip *iph = (click_ip *)q->data();
	
    iph->ip_v = 4;
    iph->ip_hl = sizeof(click_ip) >> 2;
    iph->ip_len = htons(q->length());
    uint16_t ip_id = ((_sequence) % 0xFFFF) + 1; // ensure ip_id != 0
    iph->ip_id = htons(ip_id);
    iph->ip_p = IP_PROTO_ICMP; /* icmp */
    iph->ip_ttl = _incTTL?_ttl++:200;
    iph->ip_src = _srcIP;
    iph->ip_dst = _dstIP;
    iph->ip_sum = click_in_cksum((unsigned char *)iph, sizeof(click_ip));
	
	click_icmp_echo *icmph = (click_icmp_echo *)(iph + 1);
	
	icmph->icmp_type = ICMP_ECHO;
	icmph->icmp_code = 0;
	icmph->icmp_cksum = 0;
	icmph->icmp_identifier = 0;
	icmph->icmp_sequence = htons(_sequence);
	
	_sequence++;
	
	icmph->icmp_cksum = click_in_cksum((const unsigned char *)icmph, sizeof(click_icmp_echo));
	
	q->set_dst_ip_anno(_dstIP);
	
	return q;
}

void
MyICMPPingSource::run_timer(Timer *timer)
{
    if (Packet *q = make_packet()) {
 	   output(0).push(q);
 	   timer->reschedule_after_msec(1000);
    }
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MyICMPPingSource)
