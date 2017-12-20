#ifndef CLICK_AGENTADVERTISEMENTGENERATOR_HH
#define CLICK_AGENTADVERTISEMENTGENERATOR_HH

#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/ipaddress.hh>
#include <click/vector.hh>
#include <click/hashtable.hh>
#include "mainfobase.hh"
#include "mobile_ip_packets.h"

CLICK_DECLS

class AgentAdvertisementGenerator : public Element {
public:
    AgentAdvertisementGenerator();
    ~AgentAdvertisementGenerator();

    const char* class_name() const { return "AgentAdvertisementGenerator"; }
    const char* port_count() const { return "0/1"; }
    const char* processing() const { return PUSH; }
    int configure(Vector<String>&, ErrorHandler*);

    static int sendAgentAdvertisement(const String& conf, Element* e, void* thunk, ErrorHandler* errh);
    void sendMultiCastAdvertisement();
    void add_handlers();
    void run_timer(Timer*);


private:

    Packet* make_packet(IPAddress);
    MAInfoBase* MABase;

    uint32_t _sequence;
    Timer timer;
    int interval;
    
};

CLICK_ENDDECLS
#endif