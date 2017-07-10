#include "Switch.h"
#include "Client.h"

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
    Client * client = Client::getInstance();
    client->init();
    std::thread(&Client::read, client).detach();
    client->send("hello");
}