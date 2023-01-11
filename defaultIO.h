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

class SocketIO : public DefaultIO{
    int socketID;
public:
    SocketIO(int clientid);

    virtual string read();

    virtual void write(string text);
};