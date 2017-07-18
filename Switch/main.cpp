#include "Switch.h"
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    Switch::getInstance();
    std::string temp;
    std::getline(std::cin, temp);
    return 0;
}
