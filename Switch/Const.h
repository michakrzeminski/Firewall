#ifndef CONST_H
#define CONST_H

enum ProtocolHeader {HELLO, RULELIST, OK, ERR, CHECK, ADD, ADDED, DEL, DELED };
enum Decision {ACCEPT, DROP, WAITING, NONE};
const std::string SWITCH_IP = "192.168.1.5";
const int NUM_PACKETS = 10;
const int MAX_LENGTH = 4096;
#endif
