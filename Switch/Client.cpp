#include "Client.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

Client* Client::instance = nullptr;

Client* Client::getInstance() {
    if (!instance) {
        instance = new Client;
    }
    return instance;
}

Client::Client() : s(io_service), resolver(io_service) {
    std::cout << "creating tcp client" << std::endl;
}

bool Client::init() {
    try {
        boost::asio::connect(s, resolver.resolve({ HOST, PORT }));
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
        return false;
    }
    return true;
}

void Client::send(Protocol toSend) {
    try {
        //size_t message_length = std::strlen(message);
        //boost::asio::write(s, boost::asio::buffer(message, message_length));
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << toSend;
        auto outbound_data_ = archive_stream.str();
        boost::asio::write(s, boost::asio::buffer(outbound_data_, outbound_data_.length()));
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}

void Client::read() {
    try {
        for (;;) {
            char data[MAX_LENGTH];
            //size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, MAX_LENGTH));
            boost::system::error_code error;
            size_t length = s.read_some(boost::asio::buffer(data), error);
            if (length > 0) {
                std::cout << "Wiadomosc: " << data << std::endl;
                Protocol received;
                try {
                    std::string archive_data(&data[0], length);
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
                    archive >> received;
                }
                catch (std::exception& e) {
                    // Unable to decode data.
                    boost::system::error_code error(boost::asio::error::invalid_argument);
                    return;
                }
                //TODO w zaleznosci co odebralismy wykonujemy...
            }

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
