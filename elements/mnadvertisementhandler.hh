#ifndef CLICK_MNADVERTISEMENTHANDLER_HH
#define CLICK_MNADVERTISEMENTHANDLER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mninfobase.hh"

CLICK_DECLS

class MNAdvertisementHandler : public Element { 
	public:
		MNAdvertisementHandler();
		~MNAdvertisementHandler();
		
		const char *class_name() const	{ return "MNAdvertisementHandler"; }
		const char *port_count() const	{ return "1/1"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

        void push(int, Packet*);
		
	private:
		void handleAdvertisement(Packet* p);
		MNInfoBase* MNBase;

};

CLICK_ENDDECLS
#endif