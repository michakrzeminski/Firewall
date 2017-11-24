#include "UI.h"
#include "Server.h"
#include <windows.h>

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

    printColor("username:", RED);
    getline(std::cin, username);
    printColor("password",RED);

    //hiding input
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    getline(std::cin, password);

    //enabling input
    SetConsoleMode(hStdin, mode);

    if (username == USERNAME && password == PASSWORD) {
        printColor("Welcome " + USERNAME, GREEN);
        std::cout<<"   ___            _             _ _           \n  / __\\___  _ __ | |_ _ __ ___ | | | ___ _ __ \n / /  / _ \\| '_ \\| __| '__/ _ \\| | |/ _ \\ '__|\n/ /__| (_) | | | | |_| | | (_) | | |  __/ |\n\\____/\\___/|_| |_|\\__|_|  \\___/|_|_|\\___|_|"<<std::endl<<std::flush;
        return true;
    }
    else
        return false;
}

void UI::menu() {
    int option = 255;
    auto server = Server::getInstance();
    while (option != 0) {
        printColor("Managed network", YELLOW);
        for (auto s : server->getClients()) {
            std::cout << "Switch " + s.first << " " << s.second << std::endl;
            //TODO wyœwietlenie zawartoœci tych switchów czyli ich uzytkownikow
        }
        
        printColor("Command syntax: protocol switch_no src_addr dst_addr", YELLOW);
        printColor("Example: 17 0 192.168.42.1-192.168.42.100 *", AQUA);
        std::cout << "to exit control panel enter 0" << std::endl;
        std::string command;
        getline(std::cin, command);
        std::vector<std::string> splitted = split(command, ' ');
        try {
            option = std::atoi(splitted.front().c_str());
            if (splitted.size() >= 2) {
                if (splitted[1] == "*") {
                    //std::cout << "Applying action to all switches" << std::endl;
                    //wiec do wszystkich tabel client_rules dodac ze accept protocol
                    for (auto s : server->getClients()) {
                        //server->insertClientRule(s.first, server->generateRule(option,"*","*"));
                        server->insertAdminRule(option, s.first, "*","*");
                    }
                }
                else {
                    if (splitted.size() == 2) {
                        //std::cout << "Applying action to switch" << splitted[1] << std::endl;
                        int switch_no = std::atoi(splitted[1].c_str());
                        auto iter = server->getClients().find(switch_no);
                        if (iter != server->getClients().end()) {
                            //server->insertClientRule(switch_no, server->generateRule(option, "*", "*"));
                            server->insertAdminRule(option, switch_no, "*", "*");
                        }
                    }
                    else if (splitted.size() == 4) {
                        int switch_no = std::atoi(splitted[1].c_str());
                        //std::cout << "Applying action to switch" << splitted[1] << " for " << splitted[2] << " " << splitted[3] << std::endl;
                        auto iter = server->getClients().find(switch_no);
                        if (iter != server->getClients().end()) {
                            //server->insertClientRule(switch_no, server->generateRule(option, splitted[2], splitted[3]));
                            server->insertAdminRule(option, switch_no, splitted[2], splitted[3]);
                        }
                    }
                    else if (splitted.size() == 5 && std::atoi(splitted[4].c_str()) == 1) {
                        int switch_no = std::atoi(splitted[1].c_str());
                        //std::cout << "Applying action to switch" << splitted[1] << " for " << splitted[2] << " " << splitted[3] << std::endl;
                        auto iter = server->getClients().find(switch_no);
                        if (iter != server->getClients().end()) {
                            //server->insertClientRule(switch_no, server->generateRule(option, splitted[2], splitted[3]));
                            server->insertAdminRule(option, switch_no, splitted[2], splitted[3]);
                            server->insertAdminRule(option, switch_no, splitted[3], splitted[2]);
                        }
                    }
                }
            }
            printAdminRules();
        }
        catch (...) {
            printColor("Wrong entry", RED);
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

void UI::printColor(std::string string, COLOR col) {
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, col);
    std::cout << string << std::endl;
    SetConsoleTextAttribute(hConsole, WHITE);
}

void UI::printAdminRules() {
    std::cout << std::endl;
    printColor("Admin rules: ", YELLOW);
    for (auto a : Server::getInstance()->getAdminRules()) {
        std::string temp = "";
        for (auto b : a) {
            if (typeid(int) == b.type())
                temp += std::to_string(boost::any_cast<int>(b)) + " ";
            else
                temp += boost::any_cast<std::string>(b) + " ";
        }
        std::cout << temp << std::endl;
    }
}