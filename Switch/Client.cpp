#include "Client.h"
#include "PythonAdapter.h"
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
	std::cout<<"C: Sending..."<<std::endl;
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
	    Protocol received;
            char data[MAX_LENGTH];
            //size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, MAX_LENGTH));
            boost::system::error_code error;
            size_t length = s.read_some(boost::asio::buffer(data), error);
            if (length > 0) {
                try {
                    std::string archive_data(&data[0], length);
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
		    archive >> received;
		    std::cout<<"C: Protocol: "<<received.id<<" "<<received.header<<" "<<received.payload<<std::endl;
                }
                catch (std::exception& e) {
                    // Unable to decode data.
		    std::cout<<"smth goes wrong"<<std::endl;
                    boost::system::error_code error(boost::asio::error::invalid_argument);
                    std::cout<<e.what()<<std::endl;
		    return;
                }

		if(received.header == HELLO) {
		    std::cout<<"C: Received HELLO"<<std::endl;
		    Protocol rulelist;
		    rulelist.header = RULELIST;
		    //TODO payload
		    send(rulelist);
		}
		else if(received.header == OK) {
		    std::cout<<"C: Received OK";
		}
		else if(received.header == ERR) {
		    std::cout<<"C: Received ERR"<<std::endl;
		    //TODO retrasmisja
		}
		else if(received.header == ADD) {
		    std::cout<< "C: Received ADD"<<std::endl;
		    Protocol status;
		    Rule rule; //TODO parsing received Rule
		    if(PythonAdapter::getInstance()->addRule(rule))
		        status.header = ADDED;
		    else
			status.header = ERR;
		    send(status);
		}
		else if(received.header == DEL) {
		    std::cout<<"C: Received DEL"<<std::endl;
		    Protocol status;
		    Rule rule; //TODO parsing received rule
		    if(PythonAdapter::getInstance()->deleteRule(rule))
			status.header = DELED;
		    else
			status.header = ERR;
		    send(status);
		}
		else {
		    std::cout<<"not supported"<<std::endl;
		}
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
