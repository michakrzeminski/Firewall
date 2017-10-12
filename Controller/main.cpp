#include "Server.h"
#include "UI.h"
#include <cstdlib>

int main(int argc, char* argv[]) {
    auto ui = UI::getInstance();
    auto serv = Server::getInstance();
    std::thread t1(&UI::init, ui);
    std::thread t2(&Server::init, serv);

    t1.join();
    t2.join();
    system("pause");
    return 0;
}