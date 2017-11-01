#ifndef CLICK_MASOLLICITATIONHANDLER_HH
#define CLICK_MASOLLICITATIONHANDLER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mainfobase.hh"
#include "agentadvertisementgenerator.hh"


CLICK_DECLS

class MASollicitationHandler : public Element { 
	public:
		MASollicitationHandler();
		~MASollicitationHandler();
		
		const char *class_name() const	{ return "MASollicitationHandler"; }
		const char *port_count() const	{ return "1/0"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

        void push(int, Packet*);
		
	private:
		void handleSollicitation(Packet* p);
		MAInfoBase* MABase;
		AgentAdvertisementGenerator* AAGen;

};

CLICK_ENDDECLS
#endif