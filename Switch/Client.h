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
    int name;
private:
    Client();
    static Client* instance;
    boost::asio::io_service io_service;
    tcp::socket s;
    tcp::resolver resolver;
    const char* HOST = "127.0.0.1";
    int PORT = 39000;
    std::mutex mutex_;
};
#endif
