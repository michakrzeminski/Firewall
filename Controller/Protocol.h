#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include <vector>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/vector.hpp>
#include <tins/tins.h>
#include "Const.h"

class Protocol {
public:
    Protocol();
    Protocol(ProtocolHeader ph);
    Protocol(ProtocolHeader ph, std::vector<std::string> l, std::string pl);
    int id;
    ProtocolHeader header;
    std::vector<std::string> list;
    std::string rule;
    Tins::PDU *packet;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & id;
	ar & header;
	ar & list;
	ar & rule;
	ar & boost::serialization::make_nvp("packet", boost::serialization::make_binary_object(&packet, sizeof(packet))); 
    }
    static int counter;
};
#endif
