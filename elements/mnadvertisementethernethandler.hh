#ifndef CLICK_MNADVERTISEMENTETHERNETHANDLER_HH
#define CLICK_MNADVERTISEMENTETHERNETHANDLER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mninfobase.hh"
#include "noderequestgenerator.hh"


CLICK_DECLS

class MNAdvertisementEthernetHandler : public Element { 
	public:
		MNAdvertisementEthernetHandler();
		~MNAdvertisementEthernetHandler();
		
		const char *class_name() const	{ return "MNAdvertisementEthernetHandler"; }
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