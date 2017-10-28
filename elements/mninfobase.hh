#ifndef CLICK_MNINFOBASE_HH
#define CLICK_MNINFOBASE_HH
#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/vector.hh>

CLICK_DECLS

class MNInfoBase : public Element { 
	public:
		MNInfoBase();
		~MNInfoBase();

		const char *class_name() const	{ return "MNInfoBase"; }
		int configure(Vector<String>&, ErrorHandler*);
		void* cast(const char* n);

		IPAddress getMyAddress() {return MyAddress;}

		bool getHomeStatus() {return homestatus;}
		void setHomeStatus(bool);

		IPAddress getHomeRouter() {return HomeRouter;}
		void setHomeRouter(IPAddress);

		IPAddress getCurrentRouter() {return CurrentRouter;}
		void setCurrentRouter(IPAddress);

		int getCurrentLifeTime() {return CurrentLifeTime;}
		int setCurrentLifeTime(int); 

		void add_handlers();

	private:
		IPAddress MyAddress;
		bool homestatus;

		IPAddress HomeRouter;
		IPAddress CurrentRouter;

		int CurrentLifeTime;		
};

CLICK_ENDDECLS
#endif