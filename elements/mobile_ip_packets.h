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

    uint8_t R : 1;
    uint8_t B : 1;
    uint8_t H : 1;
    uint8_t F : 1;
    uint8_t M : 1;
    uint8_t G : 1;
    uint8_t r : 1;
    uint8_t T : 1;
    uint8_t U : 1;
    uint8_t X : 1;
    uint8_t I : 1;

    uint8_t reserved : 5;
};

#endif
