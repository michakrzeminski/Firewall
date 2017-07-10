#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server {
public:
    static Server* getInstance();
    void init();
    void session(tcp::socket sock);
private:
    Server();
    static Server* instance;
    tcp::socket* s;
    tcp::resolver* resolver;
    std::map<int, boost::asio::ip::tcp::endpoint> clients;
    int counter = 0;
};
#endif