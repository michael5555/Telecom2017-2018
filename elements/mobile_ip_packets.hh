#ifndef CLICK_MOBILEIPPACKETS_H
#define CLICK_MOBILEIPPACKETS_H

// mobility agent advertisement extension

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
}

#endif
