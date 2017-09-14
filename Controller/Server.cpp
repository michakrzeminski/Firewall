#include "Server.h"
#include "Rule.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

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
            if (length > 0) {
                try {
		    //std::cout<<"S: data"<<data<<std::endl;
                    std::string archive_data(&data[0], length);
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
                    archive >> protocol;
		    std::cout<<"Protocol: "<<protocol.id<<" "<<protocol.header<<" "<<protocol.rule<<std::endl;
                }
                catch (std::exception& e) {
                    // Unable to decode data.
                    boost::system::error_code error(boost::asio::error::invalid_argument);
                    std::cout << e.what() << std::endl;
                    return;
                }

                if (protocol.header == HELLO) {
                    std::cout << "Client: " << counter << " Endpoint: " << sock.remote_endpoint() << std::endl;
                    clients.insert(std::pair<int, boost::asio::ip::tcp::endpoint>(++counter, sock.remote_endpoint()));
                    //odsylamy HELLO
                    Protocol hello(HELLO);
                    send(&sock, hello);
                }
		else if(protocol.header == RULELIST) {
		    std::cout<<"S: Received RULELIST"<<std::endl;
		    for(auto t : protocol.list)
			std::cout << t << std::endl;
		    send(&sock, Protocol(OK));
		}
		else if(protocol.header == CHECK) {
		    //TODO analiza pakietu i dezycja
            	    std::cout << "S: Received CHECK" << std::endl;
                    try{Tins::IP &ip = protocol.packet->rfind_pdu<Tins::IP>();
                    std::cout<<"TEST: "<<ip.protocol()<<" "<<ip.src_addr()<<" "<<ip.dst_addr()<<std::endl;
		    if (analyzePacket(ip)) {
                        Protocol toSend(ADD);
                        toSend.rule = generateRule(ip);
                	send(&sock, toSend);
                    }
            	    else {
                        //default policy is DROP so packet will be dropped
                	//send(&sock, Protocol(DEL));
                    }
		    } catch(std::exception &e) {std::cout<<"S: err"<<e.what();}
		}
		else if(protocol.header == ADDED) {
           	    std::cout << "Rule ADDED" << std::endl;
		}
		else if(protocol.header == DELED) {
            	    std::cout << "Rule DELED" << std::endl;
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

void Server::send(tcp::socket* sock, Protocol toSend) {
    try {
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << toSend;
        auto outbound_data_ = archive_stream.str();
	std::cout<<"S: sending"<< outbound_data_ <<std::endl;
        boost::asio::write(*sock, boost::asio::buffer(outbound_data_, outbound_data_.length()));
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}

bool Server::analyzePacket(Tins::IP &ip) {
    //TODO
    return true;
}

std::string Server::generateRule(Tins::IP &ip) {
    Rule rule;
    rule.chain = "?";
    rule.target = "ACCEPT";
    rule.protocol = ip.protocol();
    rule.src = ip.src_addr();
    rule.dst = ip.dst_addr();
    //TODO more
    return rule.toString();
}
