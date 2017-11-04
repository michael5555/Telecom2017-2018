#ifndef CLICK_REGISTRATIONREQUESTSOURCE_HH
#define CLICK_REGISTRATIONREQUESTSOURCE_HH

#include <click/element.hh>
#include <click/ipaddress.hh>

CLICK_DECLS

class RegistrationRequestSource : public Element { 
	public:
		RegistrationRequestSource();
		~RegistrationRequestSource();
		
		const char *class_name() const	{ return "RegistrationRequestSource"; }
		const char *port_count() const	{ return "0/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);
		
    	void run_timer(Timer *);

	private:		
    	Packet* make_packet();		
	
		IPAddress _srcIP;
        IPAddress _dstIP;
        IPAddress HomeAgent;
        IPAddress CareOfAddress;
        
		uint32_t _sequence;
};

CLICK_ENDDECLS
#endif