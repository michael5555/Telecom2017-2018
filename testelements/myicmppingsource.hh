#ifndef CLICK_MYICMPPINGSOURCE_HH
#define CLICK_MYICMPPINGSOURCE_HH
#include <click/element.hh>
#include <click/ipaddress.hh>
CLICK_DECLS

class MyICMPPingSource : public Element { 
	public:
		MyICMPPingSource();
		~MyICMPPingSource();
		
		const char *class_name() const	{ return "MyICMPPingSource"; }
		const char *port_count() const	{ return "0/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
    	void run_timer(Timer *);

	private:		
    	Packet* make_packet();		
	
		IPAddress _srcIP;
		IPAddress _dstIP;
		uint32_t _sequence;
		uint32_t _ttl;
		bool _incTTL;
};

CLICK_ENDDECLS
#endif
