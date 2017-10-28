#ifndef CLICK_FAINFOBASE_HH
#define CLICK_FAINFOBASE_HH
#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/vector.hh>

CLICK_DECLS

class FAInfoBase : public Element { 
	public:
		FAInfoBase();
		~FAInfoBase();

		const char *class_name() const	{ return "FAInfoBase"; }
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