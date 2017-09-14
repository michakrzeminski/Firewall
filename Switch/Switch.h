#ifndef SWITCH_H
#define SWITCH_H
#include "Client.h"
#include <thread>
#include <tins/tins.h>
#include <deque>
#include <vector>

struct PacketMap{
    Tins::IP &ip;
    Decision dec;
};

class Switch {
public:
    static Switch* getInstance();
    void init();
    void sniff();
    void bufferHandling();
private:
    Switch();
    static Switch* instance;
    Tins::Sniffer* sniffer;
    bool callback(Tins::PDU &pdu);
    bool analyzePacket(const Tins::IP &ip);
    std::deque<Tins::IP> packetBuffer;
    std::vector<PacketMap> packetMap;
};
#endif
