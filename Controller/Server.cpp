#include "Server.h"
#include "Const.h"

Server* Server::instance = nullptr;

Server* Server::getInstance() {
    if (!instance) {
        instance = new Server;
    }
    return instance;
}

Server::Server() {
    std::cout << "creating server" << std::endl;
    init();
}

void Server::init() {
    try
    {
        boost::asio::io_service io_service;

        tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), std::atoi(PORT)));
        for (;;)
        {
            tcp::socket sock(io_service);
            a.accept(sock);
            std::thread(&Server::session, this, std::move(sock)).detach();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void Server::session(tcp::socket sock)
{
    try
    {
        for (;;)
        {
            char data[MAX_LENGTH];

            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(data), error);
            //size_t length = boost::asio::read(sock, boost::asio::buffer(data, MAX_LENGTH));
            if (length > 0) {
                std::cout << data << std::endl;
                if (strncmp(data, "hello", 5) == 0) {
                    std::cout << "Client: " << counter << " Endpoint: " << sock.remote_endpoint() << std::endl;
                    clients.insert(std::pair<int, boost::asio::ip::tcp::endpoint>(++counter, sock.remote_endpoint()));
                }
            }

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            //echoing to client
            boost::asio::write(sock, boost::asio::buffer(data, length));
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception in thread: " << e.what() << "\n";
    }
}