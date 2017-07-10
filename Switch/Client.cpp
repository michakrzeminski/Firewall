#include "Client.h"
#include "Const.h"

Client* Client::instance = nullptr;

Client* Client::getInstance() {
    if (!instance) {
        instance = new Client;
    }
    return instance;
}

Client::Client() : s(io_service), resolver(io_service){
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

void Client::send(const char* message) {
    try {
        size_t message_length = std::strlen(message);
        boost::asio::write(s, boost::asio::buffer(message, message_length));
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}

void Client::read() {
    try {
        for (;;)
        {
            char reply[MAX_LENGTH];
            //size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, MAX_LENGTH));
            boost::system::error_code error;
            size_t reply_length = s.read_some(boost::asio::buffer(reply), error);
            if (reply_length > 0) {
                std::cout << "Reply is: " << reply << std::endl;
            }

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
