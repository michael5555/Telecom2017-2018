#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/vector.hh>
#include "mninfobase.hh"
#include <iostream>

CLICK_DECLS

MNInfoBase::MNInfoBase() : homestatus(true), CurrentLifeTime(-1) {}

MNInfoBase::~MNInfoBase() {}

void* MNInfoBase::cast(const char* n) {
    if (strcmp(n, "MNInfoBase") == 0)
        return (MNInfoBase*)this;
    return 0;
}

int MNInfoBase::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (Args(conf, this, errh)
            .read("MYADDRESS", MyAddress)
            .read("HAPUBLIC", HomeAgentPublic)
            .read("HAPRIVATE", HomeAgentPrivate)
            .complete() < 0) return -1;

    CareOfAddress = HomeAgentPublic;
    CurrentRouter = HomeAgentPrivate;
    return 0;
}

void MNInfoBase::setCurrentRouter(IPAddress addr) {

    CurrentRouter = addr;
}

void MNInfoBase::setCurrentRouterEthernet(uint8_t* addr) {

    CurrentRouterEthernet[0] = addr[0];
    CurrentRouterEthernet[1] = addr[1];
    CurrentRouterEthernet[2] = addr[2];
    CurrentRouterEthernet[3] = addr[3];
    CurrentRouterEthernet[4] = addr[4];
    CurrentRouterEthernet[5] = addr[5];

}

void MNInfoBase::setCareOfAddress(IPAddress addr) {
    
    CareOfAddress = addr;
}

void MNInfoBase::setHomeStatus(bool hs) {

    homestatus = hs;
}

void MNInfoBase::setCurrentLifeTime(int lt) {

    CurrentLifeTime = lt;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MNInfoBase)
