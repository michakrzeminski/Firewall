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
        //for (;;) {
            tcp::socket sock(io_service);
        for(;;) {
            a.accept(sock);
            std::thread(&Server::session, this, std::move(sock)).detach();
        }
    }
    catch (std::exception& e) {
        std::cout << "S: Exception: " << e.what() << std::endl;
    }
}

void Server::session(tcp::socket sock) {
    for (;;) {
            Protocol protocol;
            char data[MAX_LENGTH];
            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(data), error);
            //size_t length = boost::asio::read(sock, boost::asio::buffer(data, MAX_LENGTH));
            if (length > 0) {
                try {
                    std::string archive_data(&data[0], length);
                    std::istringstream archive_stream(archive_data);
                    boost::archive::text_iarchive archive(archive_stream);
                    archive >> protocol;
    	            std::cout<<"S: data: "<<archive_data<<std::endl;
                }
                catch (std::exception& e) {
                    // Unable to decode data.
                    boost::system::error_code error(boost::asio::error::invalid_argument);
                    std::cout << "S: Exception: "<<e.what() << std::endl;
                    return;
                }

                if (protocol.header == HELLO) {
                    std::cout << "S: Received: HELLO" <<std::endl;
                    std::cout << "Client: " << counter << " Endpoint: " << sock.remote_endpoint() << std::endl;
                    clients.insert(std::pair<int, boost::asio::ip::tcp::endpoint>(counter, sock.remote_endpoint()));
                    client_rules[counter]= std::vector<std::string>();
                    //sending back HELLO
                    Protocol hello(HELLO);
                    hello.id = counter;
                    ++counter;
                    send(&sock, hello);
                }
		else if(protocol.header == RULELIST) {
		    std::cout<<"S: Received: RULELIST"<<std::endl;
		    for(auto t : protocol.list)
                std::cout << t << std::endl;

            if (client_rules.find(protocol.id) != client_rules.end()) {
                client_rules[protocol.id] = std::vector<std::string>(protocol.list);
            }

            //for (auto k : client_rules) {
                //std::cout << std::to_string(k.first) << " " << k.second.front() << " " <<k.second.back() << std::endl;
            //}
            Protocol ok(OK);
            ok.id = protocol.id;
		    send(&sock, ok);
		}
		else if(protocol.header == CHECK) {
            std::cout << "S: Received: CHECK: ";
            //Tins::IP &ip = protocol.packet;
            std::cout<<"Packet: "<<protocol.packet_prot<<" "<<protocol.packet_src<<" "<<protocol.packet_dst<<std::endl;
            if (analyzePacket(protocol.packet_prot,protocol.packet_src,protocol.packet_dst)) {
                Protocol toSend(ADD);
                toSend.id = protocol.id;
                toSend.rule = generateRule(protocol.packet_prot,protocol.packet_src,protocol.packet_dst);
                //adding rule to client_rules
                auto iter = client_rules.find(protocol.id);
                if (iter != client_rules.end()) {
                    iter->second.push_back(toSend.rule);
                }
                send(&sock, toSend);
            }
		}
		else if(protocol.header == ADDED) {
           	    std::cout << "S: Received: ADDED" << std::endl;
		}
		else if(protocol.header == DELED) {
            	    std::cout << "S: Received: DELED" << std::endl;
		}
                else
                    std::cout<<"S: not supported"<<std::endl;
            }
            if (error == boost::asio::error::eof) {
                std::cout<<"Connection closed cleanly by peer"<<std::endl;
                break; // Connection closed cleanly by peer.
            }
            else if (error) {
                std::cout<<"err"<<std::endl;
                throw boost::system::system_error(error); // Some other error.
            }
            //echoing to client
            //boost::asio::write(sock, boost::asio::buffer(data, length));
        }
}

void Server::send(tcp::socket* sock, Protocol toSend) {
    try {
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << toSend;
        auto outbound_data_ = archive_stream.str();
	    std::cout<<"S: Sending: "<< toSend.header << " " << outbound_data_ <<" "<<outbound_data_.length()<<std::endl;
        boost::asio::write(*sock, boost::asio::buffer(outbound_data_, outbound_data_.length()));
    }
    catch (...) {
        std::cout << "S: Exception: " << "\n";
    }
}

bool Server::analyzePacket(int prot, std::string src, std::string dst) {
    //TODO
    return true;
}

std::string Server::generateRule(int prot, std::string src, std::string dst) {
    Rule rule;
    rule.chain = "INPUT";
    rule.target = "ACCEPT";
    rule.protocol = (uint8_t) prot;
    rule.src = src;
    rule.dst = dst;
    //TODO more
    return rule.toString();
}
