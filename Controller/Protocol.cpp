#include "Protocol.h"

int Protocol::counter = 0;

Protocol::Protocol(){
}

Protocol::Protocol(ProtocolHeader ph) {
    header = ph;
}

Protocol::Protocol(ProtocolHeader ph,std::vector<std::string> l, std::string rl) {
    list = l;
    header = ph;
    rule = rl;
}
