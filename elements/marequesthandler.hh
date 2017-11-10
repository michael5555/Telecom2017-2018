#ifndef CLICK_MAREQUESTHANDLER_HH
#define CLICK_MAREQUESTHANDLER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mainfobase.hh"


CLICK_DECLS

class MARequestHandler : public Element { 
	public:
		MARequestHandler();
		~MARequestHandler();
		
		const char *class_name() const	{ return "MARequestHandler"; }
		const char *port_count() const	{ return "1/0"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

        void push(int, Packet*);
		
	private:
		void handleRequest(Packet* p);
		MAInfoBase* MABase;

};

CLICK_ENDDECLS
#endif