#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/vector.hh>
#include "Mainfobase.hh"
#include <iostream>

CLICK_DECLS

MAInfoBase::MAInfoBase() {}

MAInfoBase::~MAInfoBase() {}

void* MAInfoBase::cast(const char* n) {
    if (strcmp(n, "MAInfoBase") == 0)
        return (MAInfoBase*)this;
    return 0;
}


int MAInfoBase::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (Args(conf, this, errh)
            .read("PUBLIC", MyPublicAddress)
            .read("PRIVATE", MyPrivateAddress)
            .complete() < 0) return -1;
            
    return 0;
}

void MAInfoBase::add_handlers() {}



CLICK_ENDDECLS
EXPORT_ELEMENT(MAInfoBase)