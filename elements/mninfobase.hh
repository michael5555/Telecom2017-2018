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
		IPAddress getHomeAgentPublic() {return HomeAgentPublic;}
		IPAddress getHomeAgentPrivate() {return HomeAgentPrivate;}
		
		bool getHomeStatus() {return homestatus;}
		void setHomeStatus(bool);

		IPAddress getCurrentRouter() {return CurrentRouter;}
		void setCurrentRouter(IPAddress);

		IPAddress getCareOfAddress() {return CareOfAddress;}
		void setCareOfAddress(IPAddress);

		int getCurrentLifeTime() {return CurrentLifeTime;}
		int setCurrentLifeTime(int); 

	private:
		IPAddress MyAddress;
		bool homestatus;

		IPAddress HomeAgentPublic;
		IPAddress HomeAgentPrivate;
		
		IPAddress CurrentRouter;
		IPAddress CareOfAddress;
		
		int CurrentLifeTime;		
};

CLICK_ENDDECLS
#endif