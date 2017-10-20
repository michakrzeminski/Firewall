#ifndef CONST_H
#define CONST_H

enum ProtocolHeader { HELLO, RULELIST, OK, ERR, CHECK, ADD, ADDED, DEL, DELED };
const int MAX_LENGTH = 4096;
enum COLOR { BLACK=0, BLUE=1, GREEN=2, AQUA=3, RED=4, MAGENTA=5, YELLOW=6, WHITE=7 };
static std::string USERNAME = "admin";
static std::string PASSWORD = "XyZ123#";
#endif
