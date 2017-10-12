#ifndef UI_H
#define UI_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
class UI {
public:
    static UI* getInstance();
    bool authentication();
    void menu();
    void init();
private:
    UI();
    std::vector<std::string> split(const std::string &s, char delim);
    static UI* instance;
};
#endif
