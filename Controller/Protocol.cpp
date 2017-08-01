#include "Protocol.h"

int Protocol::counter = 0;

Protocol::Protocol() : id(++counter) {
}

Protocol::Protocol(ProtocolHeader ph) {
    id = ++counter;
    header = ph;
}

Protocol::Protocol(ProtocolHeader ph,std::vector<std::string> l, std::string pl) {
    id = ++counter;
    list = l;
    header = ph;
    payload = pl;
}
