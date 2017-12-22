#ifndef CLICK_MNADVERTISEMENTHANDLER_HH
#define CLICK_MNADVERTISEMENTHANDLER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <click/iptable.hh>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mninfobase.hh"
#include "noderequestgenerator.hh"

#include "/home/student/click/elements/ip/lineariplookup.hh"
#include "/home/student/click/elements/ip/iproutetable.hh"




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
   		void run_timer(Timer*);
		void discoverHome(Packet*);
		void discoverAway(Packet*);



		
	private:
		void handleAdvertisement(Packet* p);
		void resetAfterAdvertisementExpiry();
		MNInfoBase* MNBase;
		NodeRequestGenerator* RGen;
		LinearIPLookup * rt;
		Timer timer;

};

CLICK_ENDDECLS
#endif