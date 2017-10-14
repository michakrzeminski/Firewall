#include "UI.h"
#include "Server.h"

UI* UI::instance = nullptr;

UI* UI::getInstance() {
    if (!instance) {
        instance = new UI;
    }
    return instance;
}

UI::UI() {
    std::cout << "----- CONTROL PANEL -----" << std::endl;
}

void UI::init() {
    if (!authentication()) {
        authentication();
    }
    else {
        menu();
    }
}

bool UI::authentication() {
    std::string username, password;
    std::cout << "username:";
    getline(std::cin, username);
    std::cout << "\n password";
    getline(std::cin, password);
    if (username == USERNAME && password == PASSWORD) {
        std::cout << "Welcome " + USERNAME << std::endl;
        return true;
    }
    else
        return false;
}

void UI::menu() {
    int option = 255;
    std::cout << "D1" << option << std::endl;
    auto server = Server::getInstance();
    while (option != 0) {
        std::cout << "Managed network" << std::endl;
        for (auto s : server->getClients()) {
            std::cout << "Switch " + s.first << " " << s.second << std::endl;
            //TODO wyœwietlenie zawartoœci tych switchów czyli ich uzytkownikow
        }
        
        std::cout << "Command syntax: protocol switch_no src_addr dst_addr \t if all - *" << std::endl;
        std::cout << "[0] exit control panel" << std::endl;
        std::string command;
        getline(std::cin, command);
        std::vector<std::string> splitted = split(command, ' ');
        try {
            option = std::atoi(splitted.front().c_str());
            if (splitted.size() >= 2) {
                if (splitted[1] == "*") {
                    std::cout << "Applying action to all switches" << std::endl;
                    //wiec do wszystkich tabel client_rules dodac ze accept protocol
                    for (auto s : server->getClients()) {
                        //server->insertClientRule(s.first, server->generateRule(option,"*","*"));
                        server->insertAdminRule(option, s.first, "*","*");
                    }
                }
                else {
                    if (splitted.size() == 2) {
                        std::cout << "Applying action to switch" << splitted[1] << std::endl;
                        int switch_no = std::atoi(splitted[1].c_str());
                        auto iter = server->getClients().find(switch_no);
                        if (iter != server->getClients().end()) {
                            //server->insertClientRule(switch_no, server->generateRule(option, "*", "*"));
                            server->insertAdminRule(option, switch_no, "*", "*");
                        }
                    }
                    else if (splitted.size() == 4) {
                        int switch_no = std::atoi(splitted[1].c_str());
                        std::cout << "Applying action to switch" << splitted[1] << " for " << splitted[2] << " " << splitted[3] << std::endl;
                        auto iter = server->getClients().find(switch_no);
                        if (iter != server->getClients().end()) {
                            //server->insertClientRule(switch_no, server->generateRule(option, splitted[2], splitted[3]));
                            server->insertAdminRule(option, switch_no, splitted[2], splitted[3]);
                        }
                    }
                }
            }
        }
        catch (...) {
            std::cout << "Wrong entry" << std::endl;
        }
    }
}

std::vector<std::string> UI::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}