#ifndef SWITCH_H
#define SWITCH_H
#include <thread>

class Switch {
public:
    static Switch* getInstance();
    void init();
private:
    Switch();
    static Switch* instance;
};
#endif
