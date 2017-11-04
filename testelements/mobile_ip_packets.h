#ifndef CLICK_MOBILEIPPACKETS_H
#define CLICK_MOBILEIPPACKETS_H

//ICMP Router Advertisement

struct icmp_router_advertisement {

    uint8_t type; // 9
    uint8_t code; // 2
    uint16_t checksum;

    uint8_t num_addrs;
    uint8_t addr_entry_size; // 2
    uint16_t lifetime;

};

struct router_address_preference_level {

    IPAddress router_address;
    int32_t preference_level;
};

// ICMP Router solicitation

struct icmp_router_sollicitation {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;

    uint32_t reserved;

};

// Mobility Agent Advertisement extension
struct agent_advertisement_extension {

    uint8_t type;
    uint8_t length;
    uint16_t sequence_number;
    uint16_t registration_lifetime;
    uint16_t flags_and_reserved;

};

struct mobile_ip_registration_request {

    uint8_t type;
    uint8_t flags;
    uint16_t lifetime;
    IPAddress home_address;    
    IPAddress home_agent;
    IPAddress care_of_address;
    uint64_t id;
    
};

struct mobile_ip_registration_reply {
    
        uint8_t type;
        uint8_t code;
        uint16_t lifetime;
        IPAddress home_address;    
        IPAddress home_agent;
        uint64_t id;
        
};

#endif
