#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/vector.hh>
#include "hainfobase.hh"
#include <iostream>

CLICK_DECLS

HAInfoBase::HAInfoBase() {}

HAInfoBase::~HAInfoBase() {}

void* HAInfoBase::cast(const char* n) {
    if (strcmp(n, "HAInfoBase") == 0)
        return (HAInfoBase*)this;
    return 0;
}


int HAInfoBase::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (Args(conf, this, errh)
            .read("PUBLIC", MyPublicAddress)
            .read("PRIVATE", MyPrivateAddress)
            .complete() < 0) return -1;
            
    return 0;
}

void HAInfoBase::add_handlers() {}



CLICK_ENDDECLS
EXPORT_ELEMENT(FAInfoBase)