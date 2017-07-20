#include "Server.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "Protocol.h"

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
        for (;;) {
            tcp::socket sock(io_service);
            a.accept(sock);
            std::thread(&Server::session, this, std::move(sock)).detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void Server::session(tcp::socket sock) {
    try {
        for (;;) {
            Protocol protocol;
            char data[MAX_LENGTH];

            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(data), error);
            //size_t length = boost::asio::read(sock, boost::asio::buffer(data, MAX_LENGTH));
            if (length > 0) {
                std::cout << "Wiadomosc: " << data << std::endl;
                //przyszla jakas wiadomosc
                //TO TEST--------------
                try {
                    std::string archive_data(&data[0], length);
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
                    archive >> protocol;
                }
                catch (std::exception& e) {
                    // Unable to decode data.
                    boost::system::error_code error(boost::asio::error::invalid_argument);
                    return;
                }
                //--------------

                if (protocol.header == HELLO) {
                    std::cout << "Client: " << counter << " Endpoint: " << sock.remote_endpoint() << std::endl;
                    clients.insert(std::pair<int, boost::asio::ip::tcp::endpoint>(++counter, sock.remote_endpoint()));
                    //odsy³amy HELLO
                    //TOTEST-------------
                    try {
                        Protocol toSend(HELLO);
                        std::ostringstream archive_stream;
                        boost::archive::text_oarchive archive(archive_stream);
                        archive << toSend;
                        auto outbound_data_ = archive_stream.str();

                        //boost::asio::async_write(sock, buffers, handler);
                        boost::asio::write(sock, boost::asio::buffer(outbound_data_, outbound_data_.length()));
                    }
                    catch (std::exception& e) {
                        std::cout << "Exception: " << e.what() << "\n";
                    }
                    //-----------------
                }
            }

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            //echoing to client
            //boost::asio::write(sock, boost::asio::buffer(data, length));
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception in thread: " << e.what() << "\n";
    }
}