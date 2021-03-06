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
//Client::Client() : s(io_service, tcp::endpoint(tcp::v4(), 39000)), resolver(io_service) {
    std::cout << "C: Creating tcp client" << std::endl;
}

bool Client::init() {
    try {
        //boost::asio::connect(s, resolver.resolve({ HOST, PORT }));
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(HOST), PORT);
        //boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("192.168.1.3"), 8888);
        s.connect(endpoint);
    }
    catch (std::exception& e) {
        std::cout << "C: Exception: " << e.what() << "\n";
        return false;
    }
    return true;
}

void Client::send(Protocol toSend) {
    try {
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << toSend;
        auto outbound_data_ = archive_stream.str();
        std::cout<<"C: Sending: "<< toSend.header << " " << outbound_data_<<std::endl;
        mutex_.lock();
        boost::asio::write(s, boost::asio::buffer(outbound_data_, outbound_data_.length()));
        mutex_.unlock();
    }
    catch (std::exception& e) {
        std::cout << "C: Exception: " << e.what() << "\n";
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
		    std::cout<<"C: Rec"<<data<<std::endl;
                    std::string archive_data(&data[0], length);
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
		    archive >> received;
		    //std::cout<<"C: Protocol: "<<received.id<<" "<<received.header<<" "<<received.rule<<std::endl;
                }
                catch (std::exception& e) {
                    // Unable to decode data.
		    //std::cout<<"C: smth goes wrong"<<std::endl;
                    boost::system::error_code error(boost::asio::error::invalid_argument);
                    std::cout<<"C: Exception: "<<e.what()<<std::endl;
		    return;
                }

		if(received.header == HELLO) {
		    std::cout<<"C: Received: HELLO"<<std::endl;
		    name = received.id;
                    Protocol rulelist(RULELIST);
                    rulelist.id = name;
		    rulelist.list = PythonAdapter::getInstance()->getRules();
		    //for(auto t : rulelist.list)
			//std::cout<< t <<std::endl;
		    //std::cout<<"C:"<<std::endl;

		    send(rulelist);
		}
		else if(received.header == OK) {
		    std::cout<<"C: Received: OK"<<std::endl;
		}
		else if(received.header == ERR) {
		    std::cout<<"C: Received: ERR"<<std::endl;
		    //TODO retrasmisja
		}
		else if(received.header == ADD) {
		    std::cout<< "C: Received: ADD "<<received.rule<<std::endl;
		    if(PythonAdapter::getInstance()->addRule(received.rule))
		        received.header = ADDED;
		    else
			received.header = ERR;
		    //send(received); //sending the same message id but empty only with status
		}
		else if(received.header == DEL) {
		    std::cout<<"C: Received DEL "<<received.rule<<std::endl;
		    if(PythonAdapter::getInstance()->deleteRule(received.rule))
			received.header = DELED;
		    else
			received.header = ERR;
		    //send(received); //sending the same message id but empty only status
		}
		else {
		    std::cout<<"C: not supported"<<std::endl;
		}
            }

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
        }
    }
    catch (std::exception& e) {
        std::cout << "C: Exception: " << e.what() << "\n";
    }
}
