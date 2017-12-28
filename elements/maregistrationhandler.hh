#ifndef CLICK_MAREGISTRATIONHANDLER_HH
#define CLICK_MAREGISTRATIONHANDLER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mainfobase.hh"
#include "mareplygenerator.hh"



CLICK_DECLS

class MARegistrationHandler : public Element { 
	public:
		MARegistrationHandler();
		~MARegistrationHandler();
		
		const char *class_name() const	{ return "MARegistrationHandler"; }
		const char *port_count() const	{ return "1/3"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

        void push(int, Packet*);
		int checkRegConditionsForeign(Packet*);
		int checkRegConditionsHome(Packet*);

	private:
		void handleRegistration(Packet* p);
		void relayRequest(mobile_ip_registration_request*, click_ip*, Packet*);
		void relayReply(mobile_ip_registration_reply*, click_ip*, Packet*);

		MAInfoBase* MABase;
		MAReplyGenerator* ReplyGen;

};

CLICK_ENDDECLS
#endif