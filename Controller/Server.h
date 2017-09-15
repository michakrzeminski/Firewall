#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "Protocol.h"

using boost::asio::ip::tcp;

class Server {
public:
    static Server* getInstance();
    void init();
    void session(tcp::socket sock);
    void send(tcp::socket* sock, Protocol toSend);
    bool analyzePacket(Tins::IP &ip);
    std::string generateRule(Tins::IP &ip);
    std::string convertProtocol(uint8_t prot);
private:
    Server();
    static Server* instance;
    tcp::socket* s;
    tcp::resolver* resolver;
    std::map<int, boost::asio::ip::tcp::endpoint> clients;
    int counter = 0;
    const char* PORT = "8888";
};
#endif
