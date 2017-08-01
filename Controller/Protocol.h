#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include <vector>
#include <boost/serialization/vector.hpp>
#include "Const.h"

class Protocol {
public:
    Protocol();
    Protocol(ProtocolHeader ph);
    Protocol(ProtocolHeader ph, std::vector<std::string> l, std::string pl);
    int id;
    ProtocolHeader header;
    std::vector<std::string> list;
    std::string payload;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & id;
	ar & header;
	ar & list;
	ar & payload;
    }
    static int counter;
};
#endif
