#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <mutex>
#include <boost/asio.hpp>
#include "Protocol.h"

using boost::asio::ip::tcp;

class Client {
public:
    static Client* getInstance();
    bool init();
    void send(Protocol msg);
    void read(); //in diffrent thread loop to receive everything
private:
    Client();
    static Client* instance;
    boost::asio::io_service io_service;
    tcp::socket s;
    tcp::resolver resolver;
    const char* HOST = "192.168.1.3";
    const char* PORT = "8888";
    std::mutex mutex_;
};
#endif
