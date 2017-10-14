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
    void sniffEth();
    void sniffWlan();
    void bufferHandling();
    void fillinPacketmap();
    int convertProtocol(std::string str);
private:
    Switch();
    static Switch* instance;
    Client* client = Client::getInstance();
    Tins::Sniffer* eth_sniffer;
    Tins::Sniffer* wlan_sniffer;
    bool ethCallback(Tins::PDU &pdu);
    bool wlanCallback(Tins::PDU &pdu);
    bool analyzePacket(const Tins::IP &ip);
    std::deque<std::tuple<char,int,std::string,std::string>> packetBuffer;
    std::vector<PacketDecision> packetMap;
};
#endif
