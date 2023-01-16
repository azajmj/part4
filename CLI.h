
#pragma once
#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    // array of commands.
    vector<Command*> commands;
    DefaultIO* dio;
    CommonData* commonData;
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};


