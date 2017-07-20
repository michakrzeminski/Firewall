#include "Switch.h"
#include "Client.h"
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
}
