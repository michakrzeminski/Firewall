#ifndef PACKETDECISION_H
#define PACKETDECISION_H

#include <tins/tins.h>

class PacketDecision {
    public:
    PacketDecision() {};
    Tins::IP ip;
    Decision dec;
};
#endif
