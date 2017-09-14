#ifndef CONST_H
#define CONST_H

enum ProtocolHeader {HELLO, RULELIST, OK, ERR, CHECK, ADD, ADDED, DEL, DELED };
enum Decision {ACCEPT, DROP, NONE};
const int NUM_PACKETS = 1024;
const int MAX_LENGTH = 1024;
#endif
