#ifndef CLICK_ROUTERSOLLICITATIONSOURCE_HH
#define CLICK_ROUTERSOLLICITATIONSOURCE_HH
#include <click/element.hh>
#include <click/ipaddress.hh>
CLICK_DECLS

class RouterSollicitationSource : public Element { 
	public:
		RouterSollicitationSource();
		~RouterSollicitationSource();
		
		const char *class_name() const	{ return "RouterSollicitationSource"; }
		const char *port_count() const	{ return "0/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
    	void run_timer(Timer *);

	private:		
    	Packet* make_packet();		
	
		IPAddress _srcIP;
		IPAddress _dstIP;

		uint32_t _sequence;
};

CLICK_ENDDECLS
#endif