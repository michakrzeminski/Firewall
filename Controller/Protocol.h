#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include "Const.h"

class Protocol {
public:
    Protocol();
    Protocol(ProtocolHeader ph);
    Protocol(ProtocolHeader ph, std::string pl);
    int id;
    ProtocolHeader header;
    std::string payload;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & id;
	ar & header;
	ar & payload;
    }
    static int counter;
};
#endif
