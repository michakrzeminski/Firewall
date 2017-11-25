#include "Server.h"
#include "Rule.h"
#include "UI.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

Server* Server::instance = nullptr;

Server* Server::getInstance()
{
    static Server instance;
    return &instance;
}

Server::Server() {
    std::cout << "creating server" << std::endl;
    //init();
}

void Server::init() {
    try
    {
        boost::asio::io_service io_service;

        tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), std::atoi(PORT)));
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
            try {
                size_t length = sock.read_some(boost::asio::buffer(data), error);
                //size_t length = boost::asio::read(sock, boost::asio::buffer(data, MAX_LENGTH));
                if (length > 0) {
                    try {
                        std::string archive_data(&data[0], length);
                        std::istringstream archive_stream(archive_data);
                        boost::archive::text_iarchive archive(archive_stream);
                        archive >> protocol;
    	                //std::cout<<"S: data: "<<archive_data<<std::endl;
                    }
                    catch (std::exception& e) {
                        // Unable to decode data.
                        boost::system::error_code error(boost::asio::error::invalid_argument);
                        std::cout << "S: Exception: "<<e.what() << std::endl;
                        return;
                    }

                    if (protocol.header == HELLO) {
                        UI::getInstance()->printColor("S: Received: HELLO", YELLOW);
                        std::cout << "New Client: " << counter << std::endl;// " Endpoint: " << sock.remote_endpoint() << std::endl;
                        clients.insert(std::pair<int, boost::asio::ip::tcp::endpoint>(counter, sock.remote_endpoint()));
                        client_rules[counter]= std::vector<std::string>();
                        //sending back HELLO
                        Protocol hello(HELLO);
                        hello.id = counter;
                        ++counter;
                        send(&sock, hello);
                    }
		    else if(protocol.header == RULELIST) {
                UI::getInstance()->printColor("S: Received: RULELIST",YELLOW);
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
                //Tins::IP &ip = protocol.packet;
                UI::getInstance()->printColor("S: Received: CHECK Packet: " + std::to_string(protocol.packet_prot) + " " + protocol.packet_src + " " + protocol.packet_dst, YELLOW);
                if (analyzePacket(protocol.packet_prot,protocol.id,protocol.packet_src,protocol.packet_dst)) {
                    if (protocol.chain == 'I') {
                        //Input rule
                        Protocol toSend(ADD);
                        toSend.id = protocol.id;

                        toSend.rule = generateRule("INPUT", protocol.packet_prot, protocol.packet_src, "");
                        auto iter = client_rules.find(protocol.id);
                        if (iter != client_rules.end()) {
                            //adding rule to client_rules
                            iter->second.push_back(toSend.rule);
                        }
                        send(&sock, toSend);
                        //Output rule
                        toSend.rule = generateRule("OUTPUT", protocol.packet_prot, "", protocol.packet_dst);
                        iter = client_rules.find(protocol.id);
                        if (iter != client_rules.end()) {
                            //adding rule to client_rules
                            iter->second.push_back(toSend.rule);
                        }
                        send(&sock, toSend);
                    }
                    if (protocol.chain == 'O') {
                        //Output rule
                        Protocol toSend(ADD);
                        toSend.id = protocol.id;

                        toSend.rule = generateRule("OUTPUT", protocol.packet_prot, "",protocol.packet_dst);
                        auto iter = client_rules.find(protocol.id);
                        if (iter != client_rules.end()) {
                            //adding rule to client_rules
                            iter->second.push_back(toSend.rule);
                        }
                        send(&sock, toSend);
                        //Input rule
                        toSend.rule = generateRule("INPUT", protocol.packet_prot, protocol.packet_src, "");
                        iter = client_rules.find(protocol.id);
                        if (iter != client_rules.end()) {
                            //adding rule to client_rules
                            iter->second.push_back(toSend.rule);
                        }
                        send(&sock, toSend);
                    }
                    else if (protocol.chain == 'F') {
                        //Forward rules
                        Protocol toSend(ADD);
                        toSend.id = protocol.id;

                        //forward in rule
                        toSend.rule = generateRule("FORWARD", protocol.packet_prot, protocol.packet_src, protocol.packet_dst);
                        auto iter = client_rules.find(protocol.id);
                        if (iter != client_rules.end()) {
                            //adding rule to client_rules
                            iter->second.push_back(toSend.rule);
                        }
                        send(&sock, toSend);

                        //forward out rule
                        toSend.rule = generateRule("FORWARD", protocol.packet_prot, protocol.packet_dst, protocol.packet_src);
                        if (iter != client_rules.end()) {
                            //adding rule to client_rules
                            iter->second.push_back(toSend.rule);
                        }
                        send(&sock, toSend);
                    }
                }
		    }
		    else if(protocol.header == ADDED) {
                UI::getInstance()->printColor("S: Received: ADDED", YELLOW);
		    }
		    else if(protocol.header == DELED) {
                UI::getInstance()->printColor("S: Received: DELED", YELLOW);
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
        }
        catch (...) {
            std::cout << "S: Exception" << std::endl;
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
        UI::getInstance()->printColor("S: Sending: " + std::to_string(toSend.header) + " " + outbound_data_, MAGENTA);
        boost::asio::write(*sock, boost::asio::buffer(outbound_data_, outbound_data_.length()));
    }
    catch (...) {
        std::cout << "S: Exception: " << "\n";
    }
}

bool Server::analyzePacket(int prot, int switch_no, std::string src, std::string dst) {
    //std::cout << "Analyzing packet ..." <<std::to_string(prot)<<" "<< std::to_string(switch_no)<<" "<<src<<" "<<dst<<std::endl;
    for (auto rule_iter : admin_rules) {
        if (rule_iter.size() == 4) {
            //std::cout << boost::any_cast<int>(rule_iter[0]) << " " << boost::any_cast<int>(rule_iter[1]) << " " << boost::any_cast<std::string>(rule_iter[2]) << " " << boost::any_cast<std::string>(rule_iter[3]) << std::endl;
            if ((boost::any_cast<int>(rule_iter[0]) == prot || boost::any_cast<int>(rule_iter[0]) == 255) && (boost::any_cast<int>(rule_iter[1]) == switch_no)
                && checkAddressInRange(boost::any_cast<std::string>(rule_iter[2]), src) && checkAddressInRange(boost::any_cast<std::string>(rule_iter[3]), dst)) {
                UI::getInstance()->printColor("analysis OK", GREEN);
                return true;
            }
            else {
                //std::cout << "DEBUG NOT OK" << std::endl;
                //przechodzimy do kolejnej reguły
            }
        }
    }
    UI::getInstance()->printColor("analysis false", RED);
    return false;
}

std::string Server::generateRule(std::string chain, int prot, std::string src, std::string dst) {
    Rule rule;
    rule.target = "ACCEPT_LOG";
    rule.protocol = (uint8_t) prot;
    if (chain == "INPUT") {
        rule.chain = chain;
        rule.src = src;
    }
    else if (chain == "OUTPUT") {
        rule.chain = chain;
        rule.dst = dst;
    }
    else if (chain == "FORWARD") {
        rule.chain = chain;
        rule.src = src;
        rule.dst = dst;
    }
    //std::cout << "Generated rule: " << rule.toString() << std::endl;
    UI::getInstance()->printColor("Generated rule: " + rule.toString(), GREEN);
    return rule.toString();
}

std::map<int, boost::asio::ip::tcp::endpoint> Server::getClients() {
    return clients;
}

void Server::insertClientRule(int id, std::string rule) {
    auto iter = client_rules.find(id);
    if (iter != client_rules.end()) {
        iter->second.push_back(rule);
    }
}

void Server::insertAdminRule(int prot, int switch_no, std::string src, std::string dst) {
    std::vector<boost::any> temp;
    temp.push_back(prot);
    temp.push_back(switch_no);
    temp.push_back(src);
    temp.push_back(dst);
    admin_rules.push_back(temp);
    //std::cout << "Inserted admin rule" << std::endl;
}

bool Server::checkAddressInRange(std::string range, std::string address) {
    //std::cout << "checking " << range << " " << address << std::endl;
    auto range_split = UI::getInstance()->split(range, '-');//eg. 192.168.1.100-192.168.1.200

    if (range_split.size() > 1) {
        //std::cout << "DEBUG1" << std::endl;
        auto address1 = UI::getInstance()->split(range_split[0], '.');
        auto address2 = UI::getInstance()->split(range_split[1], '.');
        auto address_checked = UI::getInstance()->split(address, '.');
        if (address1.size() == 4 && address2.size() == 4) {
            //std::cout << "DEBUG2" << std::endl;
            //std::cout << address1[0] << "==" << address_checked[0] << "==" << address2[0] << std::endl;
            //std::cout << address1[1] << "==" << address_checked[1] << "==" << address2[1] << std::endl;
            //std::cout << address1[2] << "==" << address_checked[2] << "==" << address2[2] << std::endl;
            //std::cout << address1[3] << "<" << address_checked[3] << "<" << address2[3] << std::endl;
            if (atoi(address1[0].c_str())== atoi(address_checked[0].c_str()) && atoi(address_checked[0].c_str()) == atoi(address2[0].c_str())
                && atoi(address1[1].c_str()) == atoi(address_checked[1].c_str()) && atoi(address_checked[1].c_str()) == atoi(address2[1].c_str())
                && atoi(address1[2].c_str()) == atoi(address_checked[2].c_str()) && atoi(address_checked[2].c_str()) == atoi(address2[2].c_str())
                && atoi(address1[3].c_str()) < atoi(address_checked[3].c_str()) && atoi(address_checked[3].c_str()) < atoi(address2[3].c_str())) {
                //std::cout << "in range " << range << std::endl;
                return true;
            }
        }
    }
    else {
        if (range == address || range == "*") {
            //std::cout << "OK" << std::endl;
            return true;
        }
        //std::cout << "DEBUG3" << std::endl;
    }
    return false;
}

std::vector<std::vector<boost::any>> Server::getAdminRules() {
    return admin_rules;
}