#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/binary_object.hpp>
#include <tins/tins.h>
#include "Const.h"

class Protocol {
public:
    Protocol();
    Protocol(ProtocolHeader ph);
    Protocol(ProtocolHeader ph, std::vector<std::string> l, std::string rl);
    int id = 0;
    ProtocolHeader header;
    std::vector<std::string> list;
    std::string rule;
    //Tins::IP packet;
    char chain = '\0';
    int packet_prot = 0;
    std::string packet_src;
    std::string packet_dst;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
	ar & id;
	ar & header;
	ar & list;
	ar & rule;
	//ar & boost::serialization::make_nvp("packet", boost::serialization::make_binary_object(&packet, sizeof(packet)));
        ar & chain;
        ar & packet_prot;
        ar & packet_src;
        ar & packet_dst;
    }
    static int counter;
};
#endif
