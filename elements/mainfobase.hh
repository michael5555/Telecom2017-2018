#ifndef CLICK_HAINFOBASE_HH
#define CLICK_HAINFOBASE_HH

#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/vector.hh>

CLICK_DECLS

class MAInfoBase : public Element { 
	public:
		MAInfoBase();
		~MAInfoBase();

		const char *class_name() const	{ return "MAInfoBase"; }
		int configure(Vector<String>&, ErrorHandler*);
		void* cast(const char* n);

		IPAddress getMyPublicAddress() {return MyPublicAddress;}
		IPAddress getMyPrivateAddress() {return MyPrivateAddress;}

		void add_handlers();

	private:
		IPAddress MyPublicAddress;
		IPAddress MyPrivateAddress;

		
};

CLICK_ENDDECLS
#endif