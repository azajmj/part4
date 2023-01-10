//
// Created by Lenovo on 10/01/2023.
//

#ifndef COMMAND_PATTERN_CLI_H
#define COMMAND_PATTERN_CLI_H

#include <string.h>
//#include KNN..
#include "commands.h"

using namespace std;

class CLI {
    // array of commands.
    vector<Command*> commands;
    // need to make a field of the KNN, probably..
    DefaultIO* dio;
    // you can add data members
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};


#endif //COMMAND_PATTERN_CLI_H
