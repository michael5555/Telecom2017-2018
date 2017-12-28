#ifndef CLICK_MAREPLYGENERATOR_HH
#define CLICK_MAREPLYGENERATOR_HH

#include <click/element.hh>
#include <click/ipaddress.hh>
#include <click/vector.hh>
#include <click/hashtable.hh>
#include "mainfobase.hh"
#include "mobile_ip_packets.h"

CLICK_DECLS

class MAReplyGenerator : public Element {
public:
    MAReplyGenerator();
    ~MAReplyGenerator();

    const char* class_name() const { return "MAReplyGenerator"; }
    const char* port_count() const { return "0/2"; }
    const char* processing() const { return PUSH; }
    int configure(Vector<String>&, ErrorHandler*);

    int sendReply(uint32_t,uint32_t);
    int denyReply(uint32_t,uint32_t,uint8_t,IPAddress,IPAddress);


private:

    Packet* make_packet(uint32_t,uint32_t);
    MAInfoBase* MABase;

    uint32_t _sequence;
    
};

CLICK_ENDDECLS
#endif