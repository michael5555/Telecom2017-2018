#ifndef CLICK_NODESOLLICITATIONGENERATOR_HH
#define CLICK_NODESOLLICITATIONGENERATOR_HH

#include <click/element.hh>
#include <click/ipaddress.hh>
#include <click/vector.hh>
#include <click/hashtable.hh>
#include <click/timer.hh>
#include "mninfobase.hh"
#include "mobile_ip_packets.h"

CLICK_DECLS

class NodeSollicitationGenerator : public Element {
public:
    NodeSollicitationGenerator();
    ~NodeSollicitationGenerator();

    const char* class_name() const { return "NodeSollicitationGenerator"; }
    const char* port_count() const { return "0/1"; }
    const char* processing() const { return PUSH; }
    int configure(Vector<String>&, ErrorHandler*);

    static int sendSollicitationHandler(const String& conf, Element* e, void* thunk, ErrorHandler* errh);
    int sendSollicitation();
    void add_handlers();
    void run_timer(Timer*);
    void setInterval(unsigned int);


private:

    Packet* make_packet();
    MNInfoBase* MNBase;

    uint32_t _sequence;
    Timer timer;

    unsigned int interval;
    
};

CLICK_ENDDECLS
#endif