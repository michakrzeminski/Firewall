#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include "Protocol.h"

using boost::asio::ip::tcp;

class Server {
public:
    static Server* getInstance();
    void init();
    void session(tcp::socket sock);
    void send(tcp::socket* sock, Protocol toSend);
    bool analyzePacket(int,std::string,std::string);
    std::string generateRule(int,std::string,std::string);
    std::map<int, boost::asio::ip::tcp::endpoint> getClients();
    void insertClientRule(int, std::string);
private:
    Server();
    static Server* instance;
    tcp::socket* s;
    tcp::resolver* resolver;
    std::map<int, boost::asio::ip::tcp::endpoint> clients;
    std::map<int, std::vector<std::string>> client_rules;
    int counter = 0;
    const char* PORT = "8888";
};
#endif
