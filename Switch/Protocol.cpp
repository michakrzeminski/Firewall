#include "Protocol.h"

int Protocol::counter = 0;

Protocol::Protocol() : id(++counter) {
}

Protocol::Protocol(ProtocolHeader ph) {
    id = ++counter;
    header = ph;
}

Protocol::Protocol(ProtocolHeader ph, std::vector<std::string> l, std::string rl) {
    id = ++counter;
    header = ph;
    list = l;
    rule = rl;
}
