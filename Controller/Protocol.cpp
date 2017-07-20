#include "Protocol.h"

int Protocol::counter = 0;

Protocol::Protocol() : id(++counter) {
}

Protocol::Protocol(ProtocolHeader ph) {
    id = ++counter;
    header = ph;
}

Protocol::Protocol(ProtocolHeader ph, std::string pl) {
    id = ++counter;
    header = ph;
    payload = pl;
}

template<class Archive>
void Protocol::serialize(Archive & ar, const unsigned int version) {
    ar & id;
    ar & header;
    ar & payload;
}