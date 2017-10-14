#ifndef PACKETDECISION_H
#define PACKETDECISION_H

#include <chrono>
#include <tins/tins.h>

class PacketDecision {
    public:
    PacketDecision() {};
    Tins::IP ip;
    std::chrono::milliseconds timestamp;
};
#endif
