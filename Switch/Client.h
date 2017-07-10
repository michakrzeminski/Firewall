#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client {
public:
    static Client* getInstance();
    bool init();
    void send(const char* message);
    void read(); //in diffrent thread loop to receive everything
private:
    Client();
    static Client* instance;
    boost::asio::io_service io_service;
    tcp::socket s;
    tcp::resolver resolver;
};
#endif