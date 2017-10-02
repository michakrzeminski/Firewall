#ifndef SWITCH_H
#define SWITCH_H
#include "Client.h"
#include "PacketDecision.h"
#include <chrono>
#include <tuple>
#include <thread>
#include <deque>
#include <vector>

class Switch {
public:
    static Switch* getInstance();
    void init();
    void sniff();
    void bufferHandling();
    void fillinPacketmap();
    int convertProtocol(std::string str);
private:
    Switch();
    static Switch* instance;
    Client* client = Client::getInstance();
    Tins::Sniffer* sniffer;
    bool callback(Tins::PDU &pdu);
    bool analyzePacket(const Tins::IP &ip);
    std::deque<std::tuple<int,std::string,std::string>> packetBuffer;
    std::vector<PacketDecision> packetMap;
};
#endif
