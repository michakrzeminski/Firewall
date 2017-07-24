#ifndef SWITCH_H
#define SWITCH_H
#include "Client.h"
#include <thread>
#include <tins/tins.h>

class Switch {
public:
    static Switch* getInstance();
    void init();
    void sniff();
private:
    Switch();
    static Switch* instance;
    Tins::Sniffer* sniffer;
    bool callback(Tins::PDU &pdu);
};
#endif
