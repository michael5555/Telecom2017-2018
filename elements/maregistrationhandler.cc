#include <click/config.h>
#include <click/confparse.hh>
#include <click/args.hh>

#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/udp.h>

#include <clicknet/ether.h>
#include <clicknet/icmp.h>
#include <click/timer.hh>
#include "maregistrationhandler.hh"
#include "mobile_ip_packets.h"
#include <iostream>
#include <math.h>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <string>       
#include <iostream>     
#include <sstream>      


CLICK_DECLS

MARegistrationHandler::MARegistrationHandler() : MABase(0) {}

MARegistrationHandler::~MARegistrationHandler() {}

int MARegistrationHandler::configure(Vector<String>& conf, ErrorHandler* errh) {
    if (cp_va_kparse(conf, this, errh, "MABASE", cpkM+cpkP, cpElementCast, "MAInfoBase", &MABase,
    "REPLYGEN", cpkM+cpkP, cpElementCast, "MAReplyGenerator", &ReplyGen,cpEnd) < 0) return -1;
    
    if (MABase == 0) return errh->error("Wrong argument, should be an MAInfoBase element.");
    if (ReplyGen == 0) return errh->error("Wrong argument, should be an MAReplyGenerator element.");

    return 0;
}

void MARegistrationHandler::push(int, Packet* p) {
    this->handleRegistration(p);
}

void MARegistrationHandler::handleRegistration(Packet* p) {

    WritablePacket * q = p->uniqueify();
    click_ip *iph = (click_ip *)q->data();
    if(iph->ip_p != IP_PROTO_UDP){
        output(0).push(q);
        return;
    }
    click_udp* udph = (click_udp *)(iph + 1);
    mobile_ip_registration_request* mipr = (mobile_ip_registration_request*)(udph + 1);

    if (mipr->type == REGISTRATION_REQUEST){
        if(mipr->home_agent != MABase->getMyPublicAddress() && mipr->home_agent != MABase->getMyPrivateAddress()) {
            int code = checkRegConditionsForeign(q);
            if(code == 0){
                this->relayRequest(mipr,iph,q);
            }
            else {
                ReplyGen->sendDenial(mipr->id1,mipr->id2,code,iph->ip_dst,iph->ip_src);
                click_chatter("Mobile Agent -- Denied Registration Request.%s  code: %d \n", MABase->getMyPublicAddress().unparse().c_str(),code);

            }
        }
        if(mipr->home_agent == MABase->getMyPublicAddress() || mipr->home_agent == MABase->getMyPrivateAddress()) {

            int code = checkRegConditionsHome(q);
            if(code == 0){


                if( iph->ip_src == mipr->home_address) {
                    MABase->setLocalNode(mipr->home_address,mipr->home_agent,mipr->lifetime);
                }
                else {
                    MABase->setLocalNode(mipr->home_address,mipr->care_of_address,mipr->lifetime);
                }
                ReplyGen->sendReply(mipr->id1,mipr->id2,ntohs(mipr->lifetime));
            }
            else {
                ReplyGen->sendDenial(mipr->id1,mipr->id2,code,iph->ip_dst,iph->ip_src);
                click_chatter("Mobile Agent -- Denied Registration Request. %s code: %d\n",MABase->getMyPublicAddress().unparse().c_str(),code);

            }

        }
    }
    else if(mipr->type == REGISTRATION_REPLY){
        mobile_ip_registration_reply* mipreply = (mobile_ip_registration_reply*)(udph + 1);
        if(mipr->home_agent != MABase->getMyPublicAddress() && mipr->home_agent != MABase->getMyPrivateAddress()) {
            this->relayReply(mipreply,iph,q);
        }
    }
}

void MARegistrationHandler::relayRequest(mobile_ip_registration_request* mipr,click_ip* iph, Packet* q){
    iph->ip_src = mipr->care_of_address;
    iph->ip_dst = mipr->home_agent;
    output(1).push(q);

}

void MARegistrationHandler::relayReply(mobile_ip_registration_reply* mipreply,click_ip* iph, Packet* q){
    iph->ip_src = MABase->getMyPrivateAddress();
    iph->ip_dst = IPAddress(mipreply->home_address);
    output(2).push(q);

}

int MARegistrationHandler::checkRegConditionsForeign(Packet* p) {

    click_ip *iph = (click_ip *)p->data();
    click_udp* udph = (click_udp *)(iph + 1);
    mobile_ip_registration_request* mipr = (mobile_ip_registration_request*)(udph + 1);

    if (ntohs(mipr->lifetime) > 90) {

        return 69;
    }

    int flags = mipr->flags;
    std::stringstream ss;
    ss << flags;
    std::string str = ss.str();

    std::bitset<8> bs(str);

    if(bs.test(2)){

        return 72;
    }
    if(bs.test(3)){

        return 72;
    }
    if(bs.test(4)){

        return 72;
    }

    if(bs.test(5)){

        return 70;
    }
    if(bs.test(7)){

        return 70;
    }

    return 0;
}

int MARegistrationHandler::checkRegConditionsHome(Packet* p) {

    click_ip *iph = (click_ip *)p->data();
    click_udp* udph = (click_udp *)(iph + 1);
    mobile_ip_registration_request* mipr = (mobile_ip_registration_request*)(udph + 1);

    int flags = mipr->flags;
    std::stringstream ss;
    ss << flags;
    std::string str = ss.str();

    std::bitset<8> bs(str);

    if(bs.test(0)){

        return 136;
    }

    return 0;
}


CLICK_ENDDECLS

EXPORT_ELEMENT(MARegistrationHandler)
