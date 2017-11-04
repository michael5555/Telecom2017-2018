#ifndef CLICK_REGISTRATIONREPLYSOURCE_HH
#define CLICK_REGISTRATIONREPLYSOURCE_HH

#include <click/element.hh>
#include <click/ipaddress.hh>

CLICK_DECLS

class RegistrationReplySource : public Element { 
	public:
		RegistrationReplySource();
		~RegistrationReplySource();
		
		const char *class_name() const	{ return "RegistrationReplySource"; }
		const char *port_count() const	{ return "0/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
    	void run_timer(Timer *);

	private:		
    	Packet* make_packet();		
	
		IPAddress _srcIP;
        IPAddress _dstIP;
        IPAddress HomeAgent;
        
		uint32_t _sequence;
};

CLICK_ENDDECLS
#endif