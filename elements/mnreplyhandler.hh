#ifndef CLICK_MNREPLYHANDLER_HH
#define CLICK_MNREPLYHANDLER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mninfobase.hh"


CLICK_DECLS

class MNReplyHandler : public Element { 
	public:
		MNReplyHandler();
		~MNReplyHandler();
		
		const char *class_name() const	{ return "MNReplyHandler"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

        void push(int, Packet*);
		
	private:
		void handleReply(Packet* p);
		MNInfoBase* MNBase;

};

CLICK_ENDDECLS
#endif