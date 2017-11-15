#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>
#include <click/vector.hh>
#include "mainfobase.hh"
#include <iostream>

CLICK_DECLS

MAInfoBase::MAInfoBase() : MyNode(0) {}

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

void MAInfoBase::setLocalNode(IPAddress ha, IPAddress coa, int lt){

    MyNode->home_address = ha;
    MyNode->careofaddress = coa;
    MyNode->lifetime = lt;

}



CLICK_ENDDECLS
EXPORT_ELEMENT(MAInfoBase)
