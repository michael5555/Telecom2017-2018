#ifndef CLICK_ICMPECHOSORTER_HH
#define CLICK_ICMPECHOSORTER_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include "mainfobase.hh"


CLICK_DECLS

class MAICMPEchoSorter : public Element { 
	public:
		MAICMPEchoSorter();
		~MAICMPEchoSorter();
		
		const char *class_name() const	{ return "MAICMPEchoSorter"; }
		const char *port_count() const	{ return "1/2"; }
		const char *processing() const	{ return PUSH; }
		int configure(Vector<String>&, ErrorHandler*);

        void push(int, Packet*);
		Packet* encap(Packet *p);
		
	private:
		MAInfoBase* MABase;
		uint32_t _sequence;

};

CLICK_ENDDECLS
#endif