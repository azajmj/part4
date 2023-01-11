#pragma once
#include <string>
#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
using namespace std;
class DefaultIO {
public:
    DefaultIO() {}
    virtual ~DefaultIO() {}
    virtual string read() { return ""; }
    virtual void write(string text) {};
    virtual void readAndFile(string name) {};
};

class StandardIO: public DefaultIO {
public:
    virtual string read();
    virtual void write(string text);
};

// class socketIO : public DefaultIO{
//     int clientID;
// public:
//     socketIO(int clientid){
//         this->clientID = clientid;
//     }

//     virtual string read(){
//         string str = "";
//         char ch = 0;
//         ::read(clientID, &ch, sizeof(char));
//         while (ch != '\n') {
//             str += ch;
//             ::read(clientID, &ch, sizeof(char));
//         }
//         return str;

//     }

//     virtual void write(string text){
//         ::write(clientID, text.c_str(), text.length());
//     }
// };