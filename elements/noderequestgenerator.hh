#ifndef CLICK_NODEREQUESTGENERATOR_HH
#define CLICK_NODEREQUESTGENERATOR_HH

#include <click/element.hh>
#include <click/ipaddress.hh>
#include <click/vector.hh>
#include <click/hashtable.hh>
#include <click/timer.hh>
#include "mninfobase.hh"
#include "mobile_ip_packets.h"

CLICK_DECLS

class NodeRequestGenerator : public Element {
public:
    NodeRequestGenerator();
    ~NodeRequestGenerator();

    const char* class_name() const { return "NodeRequestGenerator"; }
    const char* port_count() const { return "0/1"; }
    const char* processing() const { return PUSH; }
    int configure(Vector<String>&, ErrorHandler*);
    int sendRequest();
    void setRequestDestination(IPAddress);
    void run_timer(Timer*);



private:

    Packet* make_packet(IPAddress);
    MNInfoBase* MNBase;

    uint32_t _sequence;
    Timer timer;
    IPAddress requestdst;
    
};

CLICK_ENDDECLS
#endif