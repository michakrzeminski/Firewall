#include "Switch.h"
#include "PythonAdapter.h"

Switch* Switch::instance = nullptr;

Switch* Switch::getInstance() {
    if (!instance) {
        instance = new Switch;
    }
    return instance;
}

Switch::Switch() {
    std::cout << "creating switch" << std::endl;
    init();
}

void Switch::init() {
    std::cout << "switch init" <<std::endl;
    PythonAdapter adapter;
    Client * client = Client::getInstance();
    client->init();
    std::thread(&Client::read, client).detach();
    Protocol hello(HELLO);
    client->send(hello);

    sniffer = new Tins::Sniffer("eth0");
    sniffer->sniff_loop(make_sniffer_handler(this, &Switch::callback));
    std::cout<<"Debug"<<std::endl;
}

bool Switch::callback(Tins::PDU &pdu) {
    const Tins::IP &ip = pdu.rfind_pdu<Tins::IP>();
    std::cout<< "from "<<ip.src_addr() <<" to "<<ip.dst_addr()<<std::endl;
    return true;
}
